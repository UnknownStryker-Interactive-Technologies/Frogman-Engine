#ifndef _FE_CORE_BLOCK_POOL_HXX_
#define _FE_CORE_BLOCK_POOL_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.h>
#include <FE/pool/private/pool_common.hxx>
#include <FE/fstack.hxx>




BEGIN_NAMESPACE(FE)


namespace internal::pool
{
    template <FE::size InBytes>
    class uninitialized_bytes
    {
        var::byte m_memory[InBytes];
    };

    template<class Alignment>
    class chunk<PoolType::_Block, Alignment>
    {
        FE_STATIC_ASSERT(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    public:
        static constexpr size fixed_block_size_in_bytes = Alignment::size;
        static constexpr count_t page_capacity = 81920;
        static constexpr size page_capacity_in_bytes = fixed_block_size_in_bytes * page_capacity;
        using block_info_type = var::byte*;

    private: // DO NOT MEMZERO THIS ARRAY. IT WILL PUT THE COMPILER INTO AN INFINITE COMPLIATION LOOP.
        alignas(Alignment::size) std::array<var::byte, page_capacity_in_bytes> m_memory;

    public:
        alignas(Alignment::size) FE::fstack<block_info_type, page_capacity> _free_blocks;
        var::byte* const _begin = m_memory.data();
        var::byte* _page_iterator = m_memory.data();
        var::byte* const _end = m_memory.data() + m_memory.size();

        _FE_FORCE_INLINE_ boolean is_out_of_memory() const noexcept
        {
            return (_free_blocks.is_empty() == true) && (_page_iterator >= _end);
        }

#ifdef _ENABLE_ASSERT_
    private:
        var::uint32 m_double_free_tracker[page_capacity];

    public:
        _FE_FORCE_INLINE_ void check_double_allocation(FE::byte* const address_p, FE::uint32 of_type_p) noexcept
        {
            FE::index_t l_idx = (address_p - _begin) / Alignment::size;
            FE_ASSERT(m_double_free_tracker[l_idx] == 0, "Double allocation detected: cannot alloate the same address twice.");
            this->m_double_free_tracker[l_idx] = of_type_p;
        }

        _FE_FORCE_INLINE_ void check_double_free(FE::byte* const address_p, FE::uint32 of_type_p) noexcept
        {
            FE::index_t l_idx = (address_p - _begin) / Alignment::size;
            FE_ASSERT(m_double_free_tracker[l_idx] == of_type_p, "Double free detected: cannot dealloate the same address twice.");
            this->m_double_free_tracker[l_idx] = 0;
        }
#endif
    };
}




template<class Alignment>
class pool<PoolType::_Block, Alignment>
{
    FE_STATIC_ASSERT(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    using chunk_type = internal::pool::chunk<PoolType::_Block, Alignment>;
    using block_info_type = typename chunk_type::block_info_type;
    using pool_type = std::list<chunk_type>;

public:
    using alignment_type = Alignment;

    static constexpr size fixed_block_size_in_bytes = Alignment::size;
    static constexpr count_t page_capacity = chunk_type::page_capacity;
    static constexpr count_t maximum_list_node_count = 3;

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
        FE_NEGATIVE_STATIC_ASSERT((sizeof(U) > fixed_block_size_in_bytes), "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
        FE_STATIC_ASSERT(Alignment::size == fixed_block_size_in_bytes, "Static assertion failed: incorrect Alignment::size detected.");

        for (typename pool_type::iterator iterator = this->m_memory_pool.begin(); iterator != this->m_memory_pool.cend(); ++iterator)
        {
            if (iterator->is_out_of_memory() == true) _FE_UNLIKELY_
            {
                continue;
            }

            void* l_allocation_result;
            if (iterator->_free_blocks.is_empty() == false)
            {
                l_allocation_result = iterator->_free_blocks.pop();
            }
            else
            {
                l_allocation_result = iterator->_page_iterator;
                iterator->_page_iterator += fixed_block_size_in_bytes;
            } 

            if constexpr (FE::is_trivial<U>::value == false)
            {
                new(static_cast<U*>(l_allocation_result)) U();
            }

            FE_ASSERT((reinterpret_cast<FE::uintptr>(l_allocation_result) % Alignment::size) == 0, "FE.pool.block_pool has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", l_allocation_result, &Alignment::size);
            return static_cast<U*>(l_allocation_result);
        }

        create_pages(1);
        return allocate<U>();
    }

    // Incorrect type will cause a critical runtime error.
    template<typename U> 
    void deallocate(U* const pointer_p) noexcept 
    {
        FE_NEGATIVE_STATIC_ASSERT((sizeof(U) > fixed_block_size_in_bytes), "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
		FE_NEGATIVE_ASSERT(pointer_p == nullptr, "Critical Error in FE.pool.block_pool: Unable to deallocate() a nullptr.");
        FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.pool.block_pool: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this block_pool instance.", pointer_p, &Alignment::size);

        block_info_type l_to_be_freed = reinterpret_cast<block_info_type>(pointer_p);

        for (typename pool_type::iterator  iterator = this->m_memory_pool.begin(); iterator != this->m_memory_pool.cend(); ++iterator)
        {
            if ((iterator->_begin <= l_to_be_freed) && (l_to_be_freed < iterator->_end))
            {
                if constexpr (FE::is_trivial<U>::value == false)
                {
                    pointer_p->~U();
                }

                iterator->_free_blocks.push(l_to_be_freed);
                return;
            }
        }
    }

    _FE_FORCE_INLINE_ void create_pages(size chunk_count_p) noexcept
    {
        FE_NEGATIVE_ASSERT(chunk_count_p == 0, "${%s@0}: ${%s@1} was 0", TO_STRING(FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(chunk_count_p));

        for (var::size i = 0; i < chunk_count_p; ++i)
        {
            this->m_memory_pool.emplace_back();
        }

        FE_NEGATIVE_ASSERT(this->m_memory_pool.size() == maximum_list_node_count, "Maximum chunk count of a memory chunk list is limited to ${%lu@0} for some performance reasons.", &maximum_list_node_count);
    }

    boolean shrink_to_fit() noexcept
    {
        typename pool_type::iterator l_list_iterator = this->m_memory_pool.begin();
        typename pool_type::const_iterator l_cend = this->m_memory_pool.cend();

        if (l_list_iterator == l_cend)
        {
            FE_LOG("Unable to shrink_to_fit() an empty pool.");
            return false;
        }


        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            var::size l_unused_element_size = l_list_iterator->_free_blocks.size();

            FE_NEGATIVE_ASSERT((l_list_iterator->_end - l_list_iterator->_begin) != page_capacity, "The chunk range is invalid.");

            if (l_list_iterator->_page_iterator < l_list_iterator->_end)
            {
                l_unused_element_size += (l_list_iterator->_end - l_list_iterator->_page_iterator);
            }

            if (l_unused_element_size == page_capacity)
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

/*
- allocate(): O(1)
- deallocate(): O(1)
*/
template<FE::size FixedBlockSizeInBytes, class Alignment = FE::SIMD_auto_alignment>
using block_pool = pool<PoolType::_Block, FE::align_as<FixedBlockSizeInBytes, Alignment>>;

END_NAMESPACE
#endif