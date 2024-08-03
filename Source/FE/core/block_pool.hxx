#ifndef _FE_CORE_BLOCK_POOL_HXX_
#define _FE_CORE_BLOCK_POOL_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/private/pool_common.hxx>
#include <FE/core/stack.hxx>




BEGIN_NAMESPACE(FE)


namespace internal::pool
{
    template <size InBytes>
    class uninitialized_bytes
    {
        var::byte m_memory[InBytes];
    };

    template<count_t PageCapacity, class Alignment>
    struct chunk<POOL_TYPE::_STATIC, PageCapacity, Alignment>
    {
        static constexpr size fixed_block_size_in_bytes = Alignment::size;
        static constexpr count_t page_capacity = PageCapacity;
        static constexpr size page_capacity_in_bytes = fixed_block_size_in_bytes * page_capacity;
        using block_info_type = var::byte*;

    private:
    #ifdef _DEBUG_
        alignas(FE::SIMD_auto_alignment::size) std::array<var::byte, page_capacity_in_bytes> m_memory = { 0 };
    #else
        alignas(FE::SIMD_auto_alignment::size) std::array<var::byte, page_capacity_in_bytes> m_memory;
    #endif

    public:
        FE::fstack<block_info_type, PageCapacity> _free_blocks;
        var::byte* const _begin = reinterpret_cast<var::byte* const>(m_memory.data());
        var::byte* _page_iterator = _begin;
        var::byte* const _end = _begin + page_capacity_in_bytes;

        _FORCE_INLINE_ boolean is_full() const noexcept
        {
            return (_free_blocks.is_empty() == true) && (_page_iterator >= _end);
        }
    };
}




template<size PageCapacity, class Alignment, class Allocator>
class pool<POOL_TYPE::_STATIC, PageCapacity, Alignment, Allocator>
{
public:
    using chunk_type = internal::pool::chunk<POOL_TYPE::_STATIC, PageCapacity, Alignment>;
    FE_STATIC_ASSERT((std::is_same<chunk_type, typename Allocator::value_type>::value == false), "Static Assertion Failed: The chunk_type has to be equivalent to Allocator::value_type.");
    using block_info_type = typename chunk_type::block_info_type;
    using pool_type = std::list<chunk_type, Allocator>;

    static constexpr size fixed_block_size_in_bytes = Alignment::size;
    static constexpr count_t page_capacity = PageCapacity;
    static constexpr count_t maximum_list_node_count = 10;

private:
    pool_type m_memory_pool;

public:
    pool() noexcept = default;
    ~pool() noexcept = default;

    pool(const pool&) noexcept = delete;
    pool(pool&& rvalue_p) noexcept : m_memory_pool( std::move(rvalue_p.m_memory_pool) ) {}

    pool& operator=(const pool&) noexcept = delete;
    pool& operator=(pool&&) noexcept = delete;

/* - Memory pool corruption detector - 
1. unused bits are always 0.
2. bits are set to 0 during the deallocate() routine.
3. if anything else rather than 0 is found within the allocation target's bitset, then something went wrong.
It is hard to tell which corrupted memory, but very sure to say that there was an illegal memory access operation.
-- This feature is enabled if _DEBUG_ is defined. --
*/

    template<typename U>
    U* allocate() noexcept
    {
        FE_STATIC_ASSERT((sizeof(U) > fixed_block_size_in_bytes), "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
        typename pool_type::iterator l_list_iterator = this->m_memory_pool.begin();

    #ifdef _DEBUG_
        static FE::byte s_clean_bits[sizeof(U)] { 0 };
    #endif

        for (; l_list_iterator != this->m_memory_pool.cend(); ++l_list_iterator)
        {
            if (l_list_iterator->is_full() == false)
            {
                U* l_allocation_result;
                if (l_list_iterator->_free_blocks.is_empty() == true)
                {
                    l_allocation_result = reinterpret_cast<U*>(l_list_iterator->_page_iterator);
                    l_list_iterator->_page_iterator += fixed_block_size_in_bytes;
                }
                else
                {
                    l_allocation_result = reinterpret_cast<U*>(l_list_iterator->_free_blocks.pop());
                }

            #ifdef _DEBUG_
                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wdynamic-class-memaccess" // This is to avoid -Werror causing build failure and to suppress the -Wdynamic-class-memaccess.
                /*
                clang says: 
                /workspace/Frogman-Engine-Lab/Source/CMakeLists/../FE/core/block_pool.hxx:113:41: error: first operand of this 'memcmp' call is a pointer to dynamic class 'c_style_task<void (const std::vector<int> &), FE::arguments<const std::vector<int>>>'; vtable pointer will be compared [-Werror,-Wdynamic-class-memaccess]
                FE_ASSERT(( std::memcmp(l_allocation_result, s_clean_bits, sizeof(U)) != 0 ), "Frogman Engine Block Memory Pool Debug Information: Detected memory corruption!");

                However, the bits of *l_allocation_result must be to 0.
                */
                FE_ASSERT(( std::memcmp(l_allocation_result, s_clean_bits, sizeof(U)) != 0 ), "Frogman Engine Block Memory Pool Debug Information: Detected memory corruption!");
                #pragma clang diagnostic pop
            #endif

                if constexpr (FE::is_trivial<U>::value == false)
                {
                    new(l_allocation_result) U();
                }

                return l_allocation_result;
            }
        }

        create_pages(1);
        return allocate<U>();
    }

    template<typename U>  // Incorrect non-trivial T type will cause a critical runtime error.
    void deallocate(U* const pointer_p) noexcept 
    {
        FE_STATIC_ASSERT((sizeof(U) > fixed_block_size_in_bytes), "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
        typename pool_type::iterator  l_list_iterator = this->m_memory_pool.begin();
        FE_ASSERT(l_list_iterator == this->m_memory_pool.cend(), "Unable to request deallocate() to an empty pool.");

        block_info_type l_to_be_freed = reinterpret_cast<block_info_type>(pointer_p);

        for (; l_list_iterator != this->m_memory_pool.cend(); ++l_list_iterator)
        {
            if ((l_list_iterator->_begin <= l_to_be_freed) && (l_to_be_freed < l_list_iterator->_end))
            {
                if constexpr (FE::is_trivial<U>::value == false)
                {
                    pointer_p->~U();
                }

            #ifdef _DEBUG_
                std::memset(l_to_be_freed, null, sizeof(U));
            #endif
                l_list_iterator->_free_blocks.push(l_to_be_freed);
                return;
            }
        }
    }

    _FORCE_INLINE_ void create_pages(size chunk_count_p) noexcept
    {
        FE_ASSERT(chunk_count_p == 0, "${%s@0}: ${%s@1} was 0", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(chunk_count_p));

        for (var::size i = 0; i < chunk_count_p; ++i)
        {
            this->m_memory_pool.emplace_back();
        }

        FE_ASSERT(this->m_memory_pool.size() == maximum_list_node_count, "Maximum chunk count of a memory chunk list is limited to ${%lu@0} for some performance reasons.", &maximum_list_node_count);
    }

    boolean shrink_to_fit() noexcept
    {
        typename pool_type::iterator l_list_iterator = this->m_memory_pool.begin();
        typename pool_type::const_iterator l_cend = this->m_memory_pool.cend();
        FE_ASSERT(l_list_iterator == l_cend, "Unable to shrink_to_fit() an empty pool.");

        if (l_list_iterator == l_cend)
        {
            return false;
        }


        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            var::size l_unused_element_size = l_list_iterator->_free_blocks.size();

            FE_ASSERT((l_list_iterator->_end - l_list_iterator->_begin) != PageCapacity, "The chunk range is invalid.");

            if (l_list_iterator->_page_iterator < l_list_iterator->_end)
            {
                l_unused_element_size += (l_list_iterator->_end - l_list_iterator->_page_iterator);
            }

            if (l_unused_element_size == PageCapacity)
            {
                this->m_memory_pool.erase(l_list_iterator);

                if (this->m_memory_pool.empty() == true)
                {
                    break;
                }

                l_list_iterator = this->m_memory_pool.begin();
            }
        }

        return true;
    }
};


template<size FixedBlockSizeInBytes = FE::SIMD_auto_alignment::size, count_t PageCapacity = 128, class Allocator = FE::aligned_allocator<internal::pool::chunk<POOL_TYPE::_STATIC, PageCapacity, FE::align_custom_bytes<FixedBlockSizeInBytes>>>>
using block_pool = pool<POOL_TYPE::_STATIC, PageCapacity, FE::align_custom_bytes<FixedBlockSizeInBytes>, Allocator>;


END_NAMESPACE
#endif