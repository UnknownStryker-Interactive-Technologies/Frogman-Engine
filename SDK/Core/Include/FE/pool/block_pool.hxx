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

    template<PoolPageCapacity PageCapacity, class Alignment>
    class chunk<PoolType::_Block, PageCapacity, Alignment>
    {
        FE_STATIC_ASSERT(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    public:
        static constexpr FE::size fixed_block_size_in_bytes = Alignment::size;
        static constexpr FE::size possible_address_count = static_cast<FE::size>(PageCapacity) / fixed_block_size_in_bytes;
        static constexpr FE::size page_capacity_in_bytes = static_cast<FE::size>(PageCapacity);
        using block_info_type = var::byte*;

    private: // DO NOT MEMZERO THIS ARRAY. IT WILL PUT THE COMPILER INTO AN INFINITE COMPLIATION LOOP.
        alignas(Alignment::size) std::array<var::byte, page_capacity_in_bytes> m_memory;

    public:
        alignas(Alignment::size) FE::fstack<block_info_type, possible_address_count> _free_blocks;
        var::byte* const _begin = m_memory.data();
        var::byte* _page_iterator = m_memory.data();
        var::byte* const _end = m_memory.data() + m_memory.size();

        _FE_FORCE_INLINE_ boolean is_out_of_memory() const noexcept
        {
            return (_free_blocks.is_empty() == true) && (_page_iterator >= _end);
        }

#ifdef _ENABLE_ASSERT_
    private:
        var::uint32 m_double_free_tracker[possible_address_count];

    public:
        _FE_FORCE_INLINE_ void check_double_allocation(FE::byte* const address_p, FE::uint32 of_type_p) noexcept
        {
            FE::uint64 l_idx = (address_p - _begin) / Alignment::size;
            FE_ASSERT(m_double_free_tracker[l_idx] == 0, "Double allocation detected: cannot alloate the same address twice.");
            this->m_double_free_tracker[l_idx] = of_type_p;
        }

        _FE_FORCE_INLINE_ void check_double_free(FE::byte* const address_p, FE::uint32 of_type_p) noexcept
        {
            FE::uint64 l_idx = (address_p - _begin) / Alignment::size;
            FE_ASSERT(m_double_free_tracker[l_idx] == of_type_p, "Double free detected: cannot dealloate the same address twice.");
            this->m_double_free_tracker[l_idx] = 0;
        }
#endif
    };
}




template<PoolPageCapacity PageCapacity, class Alignment>
class pool<PoolType::_Block, PageCapacity, Alignment>
{
    FE_STATIC_ASSERT(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    using chunk_type = internal::pool::chunk<PoolType::_Block, PageCapacity, Alignment>;
    using block_info_type = typename chunk_type::block_info_type;

public:
    using alignment_type = Alignment;

    static constexpr size fixed_block_size_in_bytes = Alignment::size;
    static constexpr uint64 possible_address_count = chunk_type::possible_address_count;
    static constexpr uint64 maximum_page_count = 7;

private:
    using page_pointer = std::unique_ptr<chunk_type>;

    page_pointer m_memory_pool[maximum_page_count];
    var::uint32 m_page_count;

public:
    pool() noexcept : m_memory_pool{}, m_page_count() {};
    ~pool() noexcept = default;

    pool(pool&& other_p) noexcept
    {
		for (var::size i = 0; i < maximum_page_count; ++i)
		{
			this->m_memory_pool[i] = std::move( other_p.m_memory_pool[i] );
		}
    }

    _FE_FORCE_INLINE_ pool& operator=(pool&& other_p) noexcept
    {
        for (var::size i = 0; i < maximum_page_count; ++i)
        {
            this->m_memory_pool[i] = std::move( other_p.m_memory_pool[i] );
        }
        return *this;
    }

    _FE_FORCE_INLINE_ bool operator==(const pool& other_p) const noexcept { return this->m_memory_pool[0].get() == other_p.m_memory_pool[0].get(); }

    pool(const pool&) noexcept = delete;
    pool& operator=(const pool&) noexcept = delete;

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
        FE_STATIC_ASSERT((sizeof(U) <= fixed_block_size_in_bytes), "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
        FE_STATIC_ASSERT(Alignment::size == fixed_block_size_in_bytes, "Static assertion failed: incorrect Alignment::size detected.");

        for (page_pointer& page_ptr : m_memory_pool)
        {
			if (page_ptr.get() == nullptr) _FE_UNLIKELY_
            {
				page_ptr = std::make_unique<chunk_type>();
                ++this->m_page_count;
            }

            if (page_ptr->is_out_of_memory() == true) _FE_UNLIKELY_
            {
                continue;
            }

            void* l_allocation_result;
            if (page_ptr->_free_blocks.is_empty() == false)
            {
                l_allocation_result = page_ptr->_free_blocks.pop();
            }
            else
            {
                l_allocation_result = page_ptr->_page_iterator;
                page_ptr->_page_iterator += fixed_block_size_in_bytes;
            }

            if constexpr (FE::is_trivial<U>::value == false)
            {
                new(static_cast<U*>(l_allocation_result)) U();
            }

            FE_ASSERT((reinterpret_cast<FE::uintptr>(l_allocation_result) % Alignment::size) == 0, "FE.pool.block_pool has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", l_allocation_result, &Alignment::size);
            return static_cast<U*>(l_allocation_result);
        }

        FE_LOG("Frogman Engine block pool warning: the allocation failed because the pool instance is out of its capacity. A nullptr has been returned.");
        return nullptr;
    }

    // Incorrect type will cause a critical runtime error.
    template<typename U> 
    void deallocate(U* const pointer_p) noexcept 
    {
        FE_STATIC_ASSERT((sizeof(U) <= fixed_block_size_in_bytes), "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
		FE_NEGATIVE_ASSERT(pointer_p == nullptr, "Critical Error in FE.pool.block_pool: Unable to deallocate() a nullptr.");
        FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.pool.block_pool: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this block_pool instance.", pointer_p, &Alignment::size);

        block_info_type l_to_be_freed = reinterpret_cast<block_info_type>(pointer_p);

        for (page_pointer& page_ptr : m_memory_pool)
        {
            if (page_ptr == nullptr) _FE_UNLIKELY_
            {
                continue;
            }

            if ((page_ptr->_begin <= l_to_be_freed) && (l_to_be_freed < page_ptr->_end))
            {
                if constexpr (FE::is_trivial<U>::value == false)
                {
                    pointer_p->~U();
                }

                page_ptr->_free_blocks.push(l_to_be_freed);
                return;
            }
        }

		FE_EXIT(true, FE::ErrorCode::_FatalMemoryError_1XX_FalseDeallocation, "Critical Error in FE.pool.block_pool: the pointer value '${%p@0}' does not belong to this block_pool instance.", l_to_be_freed);
    }

    _FE_FORCE_INLINE_ void create_pages(FE::size chunk_count_p) noexcept
    {
        FE_ASSERT(this->m_page_count < maximum_page_count, "The pool instance is out of its page table capacity: unable to create new pages for the pool instance.");

        for (; this->m_page_count < chunk_count_p; ++m_page_count)
        {
            this->m_memory_pool[m_page_count] = std::make_unique<chunk_type>();
        }
    }

    void shrink_to_fit() noexcept
    {
        for (page_pointer& page_ptr : m_memory_pool)
        {
            if (page_ptr == nullptr)
            {
                continue;
            }

            var::size l_unused_element_size = (page_ptr->_free_blocks.size() * fixed_block_size_in_bytes);

            if (page_ptr->_page_iterator < page_ptr->_end)
            {
                l_unused_element_size += (page_ptr->_end - page_ptr->_page_iterator);
            }

            if (l_unused_element_size == possible_address_count * fixed_block_size_in_bytes)
            {
                page_ptr.reset();
            }
        }
    }
};


/*
- allocate(): O(1)
- deallocate(): O(1)

The FE::block_pool class template implements a memory pool allocator for managing fixed-size blocks of memory with a specified alignment and page capacity
ensuring efficient allocation and deallocation while providing mechanisms for free-list corruption detection.
*/
template<PoolPageCapacity PageCapacity, FE::size FixedBlockSizeInBytes, class Alignment = FE::SIMD_auto_alignment>
using block_pool = pool<PoolType::_Block, PageCapacity, FE::align_as<FixedBlockSizeInBytes, Alignment>>;


END_NAMESPACE
#endif