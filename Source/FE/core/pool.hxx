#ifndef _FE_CORE_POOL_HXX_
#define _FE_CORE_POOL_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/block_pool_allocator.hxx>
#include <FE/core/private/pool_common.hxx>
#include <FE/core/adjacency_graph.hxx>




BEGIN_NAMESPACE(FE)


namespace internal::pool
{

    template<size PageCapacity, class Alignment>
    struct chunk<POOL_TYPE::_DYNAMIC, PageCapacity, Alignment>
    {
        constexpr static count_t recycler_capacity = ((PageCapacity / Alignment::size) / 2) + 1; // The possible fragment count would be ((PageCapacity / Alignment::size) / 2) + 1 because adjacent fragments gets immediately merged during deallocate() operation.
        using recycler_type = FE::adjacency_graph<var::byte*, var::size>;
        using recycler_iterator = typename recycler_type::iterator;
        
    private:
        alignas(FE::SIMD_auto_alignment::size) std::array<var::byte, PageCapacity> m_memory;
        /*
         std::pair's first contains the address of the memory block.
         std::pair's second contains the size of the memory block.
        */

    public:
        recycler_type _free_blocks;
        var::byte* const _begin = m_memory.data();
        var::byte* _page_iterator = _begin;
        var::byte* const _end = _begin + m_memory.size();

        _FORCE_INLINE_ boolean is_full() const noexcept
        {
            return (_free_blocks.is_empty() == true) && (_page_iterator >= _end);
        }

        _FORCE_INLINE_ boolean has_reached_the_end(size size_in_bytes_to_allocate_p) const noexcept
        {
            return (_page_iterator + size_in_bytes_to_allocate_p) >= _end;
        }

        _FORCE_INLINE_ boolean has_to_merge() const noexcept
        {
            constexpr static size merging_point = (PageCapacity / 4) * 3; // Trigger at 75%
            return _page_iterator > (_begin + merging_point);
        }
    };
}




// static declaration of FE.generic_pool is not supported.
template<size PageCapacity, class Alignment, class Allocator>
class pool<POOL_TYPE::_DYNAMIC, PageCapacity, Alignment, Allocator>
{
public:
    using chunk_type = internal::pool::chunk<POOL_TYPE::_DYNAMIC, PageCapacity, Alignment>;
    using recycler_type = typename chunk_type::recycler_type;
    using recycler_iterator = typename chunk_type::recycler_iterator;

	using pool_type = std::list<chunk_type, Allocator>;

    constexpr static size page_capacity = PageCapacity;
    constexpr static count_t possible_address_count = (PageCapacity / Alignment::size);
    constexpr static count_t recycler_capacity = chunk_type::recycler_capacity;
    constexpr static count_t maximum_list_node_count = 10;

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
3. Something definitely went wrong, if anything else rather than 0 is found within the allocation target's bitset, or if the nearest bits are inspected as 1.
It is hard to tell which corrupted memory, but very sure to say that there was an illegal memory access operation.
-- This feature is enabled if _DEBUG_ is defined. --
*/
    template<typename U>
    U* allocate(count_t size_p = 1) noexcept
    {
        FE_STATIC_ASSERT((Alignment::size % 2) != 0, "Static Assertion Failed: The Alignment::size must be an even number.");
        FE_STATIC_ASSERT(std::is_array<U>::value == true, "Static Assertion Failed: The T must not be an array[] type.");
        FE_STATIC_ASSERT(std::is_const<U>::value == true, "Static Assertion Failed: The T must not be a const type.");
        FE_ASSERT(size_p == 0, "${%s@0}: ${%s@1} was 0", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(size_p));
        FE_EXIT(size_p >= PageCapacity, FE::ERROR_CODE::_FATAL_MEMORY_ERROR_OUT_OF_CAPACITY, "Fatal Error: Unable to allocate ${%lu@0} bytes of memmory that exceeds the pool chunk's capacity [which is ${%lu@1} bytes].", &size_p, &FE::buffer<var::size>::set_and_get(PageCapacity));

        size l_queried_allocation_size_in_bytes = FE::calculate_aligned_memory_size_in_bytes<U, Alignment>(size_p);

        for (typename pool_type::iterator iterator = this->m_memory_pool.begin(); iterator != this->m_memory_pool.cend(); ++iterator)
        {
            if (iterator->is_full() == false)
            {
                internal::pool::block_info l_memblock_info;
                l_memblock_info._address = nullptr;
                l_memblock_info._size_in_bytes = _FE_NOT_FOUND_;

                if (iterator->has_reached_the_end(l_queried_allocation_size_in_bytes) == true)
                {
                    if (iterator->_free_blocks.size() > 0)
                    { // address not mapped to object when std::set::insert() split memory.
                        __recycle<U>(l_memblock_info, iterator->_free_blocks, l_queried_allocation_size_in_bytes);
                    }

                    if (l_memblock_info._size_in_bytes == _FE_NOT_FOUND_)
                    {
                        create_pages(1);
                        continue;
                    }
                }

                l_memblock_info._address = iterator->_page_iterator;
                iterator->_page_iterator += l_queried_allocation_size_in_bytes;

                if constexpr (FE::is_trivial<U>::value == false)
                {
                    for (var::index_t i = 0; i < size_p; ++i)
                    {
                        new(reinterpret_cast<U*>(l_memblock_info._address)) U();
                        l_memblock_info._address = l_memblock_info._address + sizeof(U);
                    }
                }

                return reinterpret_cast<U*>(l_memblock_info._address);
            }
            else
            {
                create_pages(1);
                continue;
            }
        }

        create_pages(1);
        return allocate<U>(size_p);
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
        typename pool_type::iterator  l_list_iterator = this->m_memory_pool.begin();
        typename pool_type::const_iterator l_cend = this->m_memory_pool.cend();
      
        FE_ASSERT(l_list_iterator == l_cend, "Unable to shrink_to_fit() an empty pool.");

        if (l_list_iterator == l_cend)
        {
            return false; // this is to ignore the operation if it is running in a release mode.
        }

        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            var::size l_unused_memory_size_in_bytes = 0;
            for (auto block : l_list_iterator->_free_blocks)
            {
                l_unused_memory_size_in_bytes += block.second;
            }

            if (l_list_iterator->_page_iterator < l_list_iterator->_end)
            {
                l_unused_memory_size_in_bytes += l_list_iterator->_end - l_list_iterator->_page_iterator;
            }

            if (l_unused_memory_size_in_bytes == PageCapacity)
            {
                this->m_memory_pool.erase(l_list_iterator);

                if (this->m_memory_pool.size() == 0)
                {
                    break;
                }

                l_list_iterator = this->m_memory_pool.begin();
            }
        }

        return true;
    }

    template <typename T> // Incorrect non-trivial T type will cause a critical runtime error.
    void deallocate(T* pointer_p, count_t element_count_p) noexcept 
    {
        typename pool_type::iterator l_list_iterator = this->m_memory_pool.begin();
        FE_ASSERT(l_list_iterator == this->m_memory_pool.cend(), "Unable to request deallocate() to an empty pool.");

        for (; l_list_iterator != this->m_memory_pool.cend(); ++l_list_iterator)
        {
            if ((l_list_iterator->_begin <= reinterpret_cast<var::byte*>(pointer_p)) && (reinterpret_cast<var::byte*>(pointer_p) < l_list_iterator->_end))
            {
                if constexpr (FE::is_trivial<T>::value == false)
                {
                    for (var::count_t i = 0; i < element_count_p; ++i)
                    {
                        pointer_p->~T();
                        ++pointer_p;
                    }
                }

                auto l_dealloc_result = l_list_iterator->_free_blocks.insert(reinterpret_cast<var::byte*>(pointer_p), FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(element_count_p));
                FE_EXIT(l_dealloc_result == l_list_iterator->_free_blocks.end(), ERROR_CODE::_FATAL_MEMORY_ERROR_DOUBLE_FREE, "Frogman Engine Memory Pool Debug Information: double free detected.");

                if (l_list_iterator->has_to_merge() == true)
                {
                    __merge(l_dealloc_result, l_list_iterator->_free_blocks);
                }
                return;
            }
        }
    }

private:
    static void __merge(recycler_iterator in_out_recently_deleted_p, recycler_type& in_out_free_block_list_p) noexcept
    {
        var::byte* l_merged_address = in_out_recently_deleted_p->get_key();
        var::size l_merged_size = in_out_recently_deleted_p->_value;

        auto l_adjacent = in_out_recently_deleted_p;
        ++l_adjacent;

        // merge upper part
        for(; l_adjacent != in_out_free_block_list_p.end();)
        {
            if(l_merged_address + l_merged_size == l_adjacent->get_key())
            {
                l_merged_size += l_adjacent->_value;

                auto l_previous = l_adjacent;
                ++l_adjacent;
                in_out_free_block_list_p.erase(l_previous); 
                continue;
            }
            else
            {
                break;
            }
        }

        l_adjacent = in_out_recently_deleted_p;
        --l_adjacent;
        auto l_rend = --(in_out_free_block_list_p.begin());
        // merge lower part
        for(; l_adjacent != l_rend;) 
        {
            if( l_adjacent->get_key() + l_adjacent->_value == l_merged_address)
            {
                l_merged_size += l_adjacent->_value;
                l_merged_address = l_adjacent->get_key();
                auto l_previous = l_adjacent;
                --l_adjacent; 
                in_out_free_block_list_p.erase(l_previous); 
                continue;
            }
            else
            {
                break;
            }
        }

        in_out_free_block_list_p.insert(l_merged_address, l_merged_size); 
    }

    template <typename T>
    static void __recycle(internal::pool::block_info& out_memblock_info_p, typename chunk_type::recycler_type& in_out_free_blocks_p, size queried_allocation_size_in_bytes_p) noexcept
    {
        FE_ASSERT(in_out_free_blocks_p.is_empty() == true, "Assertion Failure: Cannot recycle from an empty bin.");

        for (auto iterator = in_out_free_blocks_p.begin(); iterator != in_out_free_blocks_p.end(); ++iterator)
        {
            if (iterator->_value >= queried_allocation_size_in_bytes_p)
            {
                out_memblock_info_p._address = iterator->get_key();
                out_memblock_info_p._size_in_bytes = queried_allocation_size_in_bytes_p;

                size l_leftover_size = iterator->_value - queried_allocation_size_in_bytes_p;
                if (l_leftover_size > 0)
                {
                    _MAYBE_UNUSED_ auto l_split_result = in_out_free_blocks_p.insert((out_memblock_info_p._address + queried_allocation_size_in_bytes_p), l_leftover_size);
                    FE_EXIT(l_split_result == in_out_free_blocks_p.end(), ERROR_CODE::_FATAL_MEMORY_ERROR_DOUBLE_FREE, "Frogman Engine Memory Pool Debug Information: double free detected.");
                }
                in_out_free_blocks_p.erase(iterator);

                return;
            }
        }
    }
};


template<size PageCapacity = 8 KB, class Alignment = FE::SIMD_auto_alignment, class Allocator = FE::aligned_allocator<internal::pool::chunk<POOL_TYPE::_DYNAMIC, PageCapacity, Alignment>>>
using scalable_pool = pool<POOL_TYPE::_DYNAMIC, PageCapacity, Alignment, Allocator>;


END_NAMESPACE
#endif