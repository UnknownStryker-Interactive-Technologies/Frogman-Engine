#ifndef _FE_CORE_BLOCK_POOL_HXX_
#define _FE_CORE_BLOCK_POOL_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

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
        var::byte m_page[InBytes];
    };

    template<class Alignment>
    class chunk<PoolType::_Block, Alignment>
    {
    public:
        constexpr static FE::size fixed_block_size_in_bytes = Alignment::size;
        constexpr static FE::size page_capacity_in_bytes = 16 * FE::one_MiB;//FE::system_large_page_size;
        constexpr static FE::size possible_address_count = page_capacity_in_bytes / fixed_block_size_in_bytes;
        static_assert(possible_address_count > 1, "Static assertion failed: possible_address_count is less than 1.");

        using block_pointer = var::byte*;

    private: // DO NOT MEMZERO THIS ARRAY. IT WILL PUT THE COMPILER INTO AN INFINITE COMPLIATION LOOP.
        var::byte m_page[page_capacity_in_bytes];

    public:
        alignas(FE::CPU_L1_cache_line::size)  FE::fstack<block_pointer, possible_address_count> _free_blocks;
        var::byte* const _begin = static_cast<var::byte*>(m_page);
        var::byte* _page_iterator = static_cast<var::byte*>(m_page);
        var::byte* const _end = static_cast<var::byte*>(m_page) + sizeof(m_page);
        var::int32 _usage_in_bytes = 0;

    public:
        chunk() noexcept
        {
            FE_LOG_IF(reinterpret_cast<FE::uintptr>(this) % FE::CPU_L1_cache_line::size != 0, FE::log::Severity::_Warning, "Warning: a memory pool page created on a memory address not aligned to the CPU cache line size.");
            FE_ASSERT((FE::byte*)this == _begin, "Assertion failed: modifying the memory layout breaks the assumption used in deallocate for efficiency.");
        }
		~chunk() noexcept = default;

        _FE_FORCE_INLINE_ boolean is_out_of_memory() const noexcept
        {
            return (_free_blocks.is_empty() == true) && (_page_iterator >= _end);
        }

        _FE_FORCE_INLINE_ FE::int32 get_usage_as_percentile() const noexcept
        {
            return static_cast<FE::int32>(((FE::float32)_usage_in_bytes / (FE::float32)page_capacity_in_bytes) * 100.0f);
        }

#ifdef _ENABLE_ASSERT_
    private:
        var::uint32 m_double_free_tracker[possible_address_count];

    public:
        void check_double_allocation(FE::byte* const address_p, FE::uint32 of_type_p) noexcept
        {
            FE::uint64 l_idx = (address_p - _begin) / Alignment::size;
            FE_ASSERT(m_double_free_tracker[l_idx] == 0, "Double allocation detected: cannot alloate the same address twice.");
            m_double_free_tracker[l_idx] = of_type_p;
        }

        void check_double_free(FE::byte* const address_p, FE::uint32 of_type_p) noexcept
        {
            FE::uint64 l_idx = (address_p - _begin) / Alignment::size;
            FE_ASSERT(m_double_free_tracker[l_idx] == of_type_p, "Double free detected: cannot dealloate the same address twice.");
            m_double_free_tracker[l_idx] = 0;
        }
#endif
    };
}




template<class Alignment>
class pool<PoolType::_Block, Alignment> : public std::pmr::memory_resource
{
    using chunk_type = internal::pool::chunk<PoolType::_Block, Alignment>;
    using block_pointer = typename chunk_type::block_pointer;

public:
    using alignment_type = Alignment;

    constexpr static FE::int32 fixed_block_size_in_bytes = Alignment::size;
    static_assert(FE::is_power_of_two(fixed_block_size_in_bytes) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    constexpr static FE::int32 page_capacity = chunk_type::page_capacity_in_bytes;
    constexpr static FE::int32 possible_address_count = chunk_type::possible_address_count;
    constexpr static FE::int32 maximum_page_count = (4ull * (FE::uint64)FE::one_GiB) / page_capacity;
    static_assert(maximum_page_count > 0, "Static assertion failed: maximum_page_count is 0.");

private:
    using page_pointer = chunk_type*;

    page_pointer m_memory_pool[maximum_page_count];
    var::int32 m_page_count;

public:
    pool() noexcept
		: m_memory_pool{}, m_page_count() 
    {
    }

    virtual ~pool() noexcept override
    {
        for (page_pointer& page_ptr : m_memory_pool)
        {
            if (page_ptr != nullptr)
            {
                page_ptr->~chunk_type();
				FE_ALIGNED_FREE(page_ptr);
            }
        }
    }

    pool(pool&& other_p) noexcept
		: m_page_count(other_p.m_page_count)
	{
		for (var::size i = 0; i < maximum_page_count; ++i)
		{
            m_memory_pool[i] = other_p.m_memory_pool[i];
            other_p.m_memory_pool[i] = nullptr;
		}
        other_p.m_page_count = 0;
	}

    pool& operator=(pool&& other_p) noexcept
    {
        m_page_count = other_p.m_page_count;
        other_p.m_page_count = 0;

        for (var::size i = 0; i < maximum_page_count; ++i)
        {
            m_memory_pool[i] = other_p.m_memory_pool[i];
            other_p.m_memory_pool[i] = nullptr;
        }
        return *this;
    }

    _FE_FORCE_INLINE_ bool operator==(const pool& other_p) const noexcept { return m_memory_pool[0] == other_p.m_memory_pool[0]; }

    pool(const pool&) noexcept = delete;
    pool& operator=(const pool&) noexcept = delete;

protected:
    inline virtual void* do_allocate(_FE_MAYBE_UNUSED_ std::size_t bytes_p = 0, _FE_MAYBE_UNUSED_ std::size_t alignment_p = Alignment::size) noexcept override
    {
        FE_ASSERT(bytes_p <= fixed_block_size_in_bytes, "Assertion failed: the allocation failed because the requested size, ${%lu@0} is greater than the fixed block size, ${%lu@1}.", &bytes_p, &fixed_block_size_in_bytes);
        return allocate<std::byte>();
    }

    inline virtual void do_deallocate(_FE_MAYBE_UNUSED_ void* ptr_p, _FE_MAYBE_UNUSED_ std::size_t bytes_p = 0, _FE_MAYBE_UNUSED_ std::size_t alignment_p = Alignment::size) noexcept override
    {
        FE_ASSERT(bytes_p <= fixed_block_size_in_bytes, "Assertion failed: the allocation failed because the requested size is greater than the fixed block size. A nullptr has been returned.");
        deallocate<std::byte>(static_cast<std::byte*>(ptr_p));
    }

    inline virtual bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override
    {
        if (dynamic_cast<const pool*>(&other_p) == nullptr)
        {
            return false;
        }

        return operator==(dynamic_cast<const pool&>(other_p));
    }

public:
    template<typename U>
    U* allocate() noexcept
    {
        static_assert(sizeof(U) <= fixed_block_size_in_bytes, "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
        static_assert(Alignment::size == fixed_block_size_in_bytes, "Static assertion failed: incorrect Alignment::size detected.");

        for (var::size i = 0; i < maximum_page_count; ++i)
        {
            if (m_memory_pool[i] == nullptr) _FE_UNLIKELY_
            {
				m_memory_pool[i] = (chunk_type*)FE_ALIGNED_ALLOC(sizeof(chunk_type), page_capacity);
			    FE_ASSERT(m_memory_pool[i] != nullptr, "Critical Error in FE.Core.block_pool: Unable to allocate a new memory page for this pool instance.");
                new(m_memory_pool[i]) chunk_type();
                ++m_page_count;

                // Swap the new page to the front of the array for faster access.
                std::swap(m_memory_pool[0], m_memory_pool[i]);
                i = 0;
                FE_LOG(FE::log::Severity::_Info, "New memory page has been created for this instance.\nThe instance address: ${%p@0}\nThe number of pages have been allocated for the instance: ${%u32@1}.", this, &m_page_count);
            }

            if (m_memory_pool[i]->is_out_of_memory() == true) _FE_UNLIKELY_
            {
                continue;
            }

            void* l_allocation_result;
            if (m_memory_pool[i]->_free_blocks.is_empty() == false)
            {
                l_allocation_result = m_memory_pool[i]->_free_blocks.pop();
            }
            else
            {
                l_allocation_result = m_memory_pool[i]->_page_iterator;
                m_memory_pool[i]->_page_iterator += fixed_block_size_in_bytes;
            }

            if constexpr (FE::is_trivial<U>::value == false)
            {
                new(static_cast<U*>(l_allocation_result)) U();
            }

            FE_ASSERT((reinterpret_cast<FE::uintptr>(l_allocation_result) % Alignment::size) == 0, "FE.Core.block_pool has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", l_allocation_result, &Alignment::size);
			m_memory_pool[i]->_usage_in_bytes += fixed_block_size_in_bytes;
            return static_cast<U*>(l_allocation_result);
        }

        FE_LOG(FE::log::Severity::_Warning, "The allocation failed because the pool instance is out of its capacity. A nullptr has been returned.");
        return nullptr;
    }

    // Incorrect type will cause a critical runtime error.
    template<typename U> 
    bool deallocate(U* const pointer_p) noexcept 
    {
        static_assert(sizeof(U) <= fixed_block_size_in_bytes, "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
		FE_NEGATIVE_ASSERT(pointer_p == nullptr, "Critical Error in FE.Core.block_pool: Unable to deallocate() a nullptr.");
        FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.Core.block_pool: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this block_pool instance.", pointer_p, &Alignment::size);
        block_pointer l_to_be_freed = reinterpret_cast<block_pointer>(pointer_p);

        // use a pointer arithmetic to find which page the pointer belongs to.
        page_pointer L_page_base = reinterpret_cast<page_pointer>( reinterpret_cast<FE::uintptr>(l_to_be_freed) - (reinterpret_cast<FE::uintptr>(l_to_be_freed) % page_capacity) );
        FE::int64 l_distance_from_begin = l_to_be_freed - reinterpret_cast<FE::byte*>(L_page_base);
        if ( (l_distance_from_begin < 0) || (l_distance_from_begin > page_capacity) )
        {
			FE_LOG(FE::log::Severity::_Warning, "A Warning in FE.Core.block_pool: the pointer value '${%p@0}' does not belong to this pool instance. The deallocation has been yielded.", pointer_p);
			return false; // The pointer does not belong to this pool instance.
        }

        if constexpr (FE::is_trivial<U>::value == false)
        {
            pointer_p->~U();
        }

        L_page_base->_free_blocks.push(l_to_be_freed);
        L_page_base->_usage_in_bytes -= fixed_block_size_in_bytes;
        
        FE_ASSERT(L_page_base->_usage_in_bytes >= 0, "Critical Error in FE.Core.block_pool: the internal usage counter has gone negative. Memory corruption might have occurred.");
        return true;
    }

    bool try_trim_a_page() noexcept
    {
        for (page_pointer& page_ptr : m_memory_pool)
        {
            if (page_ptr == nullptr) _FE_UNLIKELY_
            {
                continue;
            }

            FE_ASSERT(page_ptr->_usage_in_bytes >= 0, "Critical Error in FE.Core.scalable_pool: the internal usage counter has gone negative. Memory corruption might have occurred.");

            if (page_ptr->_usage_in_bytes == 0)
            {
                page_ptr->~chunk_type();
                FE_ALIGNED_FREE(page_ptr);
                page_ptr = nullptr;
                std::swap(page_ptr, m_memory_pool[m_page_count]);
                --m_page_count;
                return true;
            }
        }
        return false;
    }

    void try_trim_all_pages() noexcept
    {
        for (page_pointer& page_ptr : m_memory_pool)
        {
            if (page_ptr == nullptr) _FE_UNLIKELY_
            {
                continue;
            }

            FE_ASSERT(page_ptr->_usage_in_bytes >= 0, "Critical Error in FE.Core.scalable_pool: the internal usage counter has gone negative. Memory corruption might have occurred.");

            if (page_ptr->_usage_in_bytes == 0)
            {
                page_ptr->~chunk_type();
                FE_ALIGNED_FREE(page_ptr);
                page_ptr = nullptr;
                std::swap(page_ptr, m_memory_pool[m_page_count]);
                --m_page_count;
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
template<FE::size FixedBlockSizeInBytes, class Alignment = FE::SIMD_auto_alignment>
using block_pool = pool<PoolType::_Block, FE::align_as<FixedBlockSizeInBytes, Alignment>>;


END_NAMESPACE
#endif