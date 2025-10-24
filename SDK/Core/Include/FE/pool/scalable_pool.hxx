#ifndef _FE_CORE_SCALABLE_POOL_HXX_
#define _FE_CORE_SCALABLE_POOL_HXX_
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
#include <FE/prerequisites.hxx>
#include <FE/algorithm/utility.hxx>
#include <FE/do_once.hxx>
#include <FE/pool/private/pool_common.hxx>
#include <FE/iterator.hxx>

// std::sort()
#include <algorithm>
#include <execution> // std::execution::par
#include <memory_resource> // std::pmr::memory_resource

#include <taskflow.hpp>




BEGIN_NAMESPACE(FE)


namespace internal::pool
{
    class from_low_address
    {
    public:
        _FE_FORCE_INLINE_ bool operator()(const block_info& lhs_p, const block_info& rhs_p) noexcept
		{
			return lhs_p._address < rhs_p._address;
		}
    };

    class less_than
    {
    public:
        _FE_FORCE_INLINE_ bool operator()(const block_info& lhs_p, const block_info& rhs_p) noexcept
        {
            return lhs_p._size_in_bytes < rhs_p._size_in_bytes;
        }
    };

	class greater_than
    {
    public:
        _FE_FORCE_INLINE_ bool operator()(const block_info& lhs_p, const block_info& rhs_p) noexcept
        {
            return lhs_p._size_in_bytes > rhs_p._size_in_bytes;
        }
    };

    _FE_FORCE_INLINE_ bool operator==(const block_info& lhs_p, const block_info& rhs_p) noexcept
    {
        return lhs_p._address == rhs_p._address;
    }

    _FE_FORCE_INLINE_ bool operator!=(const block_info& lhs_p, const block_info& rhs_p) noexcept
    {
        return lhs_p._address != rhs_p._address;
    }
    
    template<class Alignment>
    class chunk<PoolType::_Scalable, Alignment>
    {
        static_assert(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    public:
        constexpr static FE::int32 page_capacity_in_bytes = 64 * FE::one_MiB;
        constexpr static FE::int32 possible_address_count = (page_capacity_in_bytes / Alignment::size);
        constexpr static FE::int32 free_list_capacity = possible_address_count;
		static_assert(possible_address_count > 1, "Static assertion failed: possible_address_count is less than 1.");

        using free_list_type = block_info[free_list_capacity];
        using free_list_iterator = block_info*;
        using free_list_element = block_info;
        
    private: // DO NOT MEMZERO THIS ARRAY. IT WILL PUT THE COMPILER INTO AN INFINITE COMPLIATION LOOP.
        var::byte m_page[page_capacity_in_bytes];
        alignas(FE::CPU_L1_cache_line::size)  block_info m_free_list[free_list_capacity];
        var::int32 m_free_list_size = 0;
        var::boolean m_is_page_heapified = false;

    public:
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

        _FE_FORCE_INLINE_ FE::boolean is_page_heapified() const noexcept { return m_is_page_heapified; }
        _FE_FORCE_INLINE_ void set_page_heapified() noexcept { m_is_page_heapified = true; }

        void add_to_the_free_list(const block_info& block_p) noexcept
        {
            FE_NEGATIVE_ASSERT(m_free_list_size == free_list_capacity, "Assertion Failure: The free list is full.");
            block_info* const l_position = static_cast<block_info*>(m_free_list) + m_free_list_size;

			l_position->_address = block_p._address;
			l_position->_size_in_bytes = block_p._size_in_bytes;

            ++(m_free_list_size);
            
            if (m_is_page_heapified == true)
            {
				std::push_heap(static_cast<free_list_iterator>(m_free_list), static_cast<free_list_iterator>(m_free_list) + m_free_list_size, internal::pool::less_than{});
            }
        }

		// Time complexity: O(2 log n)
		FE::boolean retrieve_from_the_free_list(internal::pool::block_info& out_alloc_result_p, FE::int32 requested_bytes_p) noexcept
		{
            FE_ASSERT((requested_bytes_p % Alignment::size) == 0, "Critical Error in FE.Core.scalable_pool: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &requested_bytes_p, &Alignment::size);
            FE_ASSERT(m_is_page_heapified == true, "Assertion Failure: The page is not binary heapified.");
            
            if (m_free_list_size == 0)
            {
				out_alloc_result_p._address = nullptr;
				out_alloc_result_p._size_in_bytes = out_alloc_result_p._size_in_bytes xor out_alloc_result_p._size_in_bytes;
                return _FE_FAILED_;
            }

			std::pop_heap(static_cast<free_list_iterator>(m_free_list), static_cast<free_list_iterator>(m_free_list) + m_free_list_size, internal::pool::less_than{});
			--(m_free_list_size);
            
            // Try allocation.
            if (m_free_list[m_free_list_size]._size_in_bytes >= requested_bytes_p)
            {
                out_alloc_result_p._address = m_free_list[m_free_list_size]._address; 
                out_alloc_result_p._size_in_bytes = requested_bytes_p;
                FE_ASSERT((reinterpret_cast<FE::uintptr>(out_alloc_result_p._address) % Alignment::size) == 0, "FE.Core.scalable_pool has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", out_alloc_result_p._address, &Alignment::size);
                
                m_free_list[m_free_list_size]._address += requested_bytes_p;
                m_free_list[m_free_list_size]._size_in_bytes -= requested_bytes_p;

                if (m_free_list[m_free_list_size]._size_in_bytes > 0)
                {
                    add_to_the_free_list(m_free_list[m_free_list_size]);
                }
                return _FE_SUCCEEDED_;
            }
            else if (m_free_list[m_free_list_size]._size_in_bytes > 0) // Failed to find a block that fits the requested size.
            {
                add_to_the_free_list(m_free_list[m_free_list_size]);
            }

            out_alloc_result_p._address = nullptr;
            out_alloc_result_p._size_in_bytes = out_alloc_result_p._size_in_bytes xor out_alloc_result_p._size_in_bytes;
            return _FE_FAILED_; // Try iterate to the next page.
		}

        _FE_FORCE_INLINE_ block_info* get_free_list() noexcept { return static_cast<block_info*>(m_free_list); }

		_FE_FORCE_INLINE_ FE::int32 get_free_list_size() const noexcept { return m_free_list_size; }
        _FE_FORCE_INLINE_ void set_free_list_size(FE::int32 size_p) noexcept 
        {
            m_free_list_size = size_p; 
        }

        _FE_FORCE_INLINE_ FE::int32 get_usage_as_percentile() const noexcept 
        {
            return static_cast<FE::int32>(( (FE::float32)_usage_in_bytes / (FE::float32)page_capacity_in_bytes ) * 100.0f);
        }

#ifdef _ENABLE_ASSERT_
    private:
        var::int32 m_double_free_tracker[possible_address_count]{};

    public:
        _FE_FORCE_INLINE_ void check_double_allocation(const block_info& block_info_p) noexcept
        {
			FE::size l_idx = (block_info_p._address - _begin) / Alignment::size;
            FE_ASSERT(m_double_free_tracker[l_idx] == 0, "Double allocation detected: cannot alloate the same address twice.");
			m_double_free_tracker[l_idx] = block_info_p._size_in_bytes;
        }

        _FE_FORCE_INLINE_ void check_double_free(const block_info& block_info_p) noexcept
		{
            FE::size l_idx = (block_info_p._address - _begin) / Alignment::size;
            FE_ASSERT(m_double_free_tracker[l_idx] == block_info_p._size_in_bytes, "Double free detected: cannot dealloate the same address twice.");
			m_double_free_tracker[l_idx] = 0;
		}
#endif
    };
}




template<class Alignment>
class pool<PoolType::_Scalable, Alignment> : public std::pmr::memory_resource
{
    static_assert(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    using chunk_type = internal::pool::chunk<PoolType::_Scalable, Alignment>;
    using free_list_type = typename chunk_type::free_list_type;
    using free_list_iterator = typename chunk_type::free_list_iterator;

public:
	using alignment_type = Alignment;

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
		for (var::int32 i = 0; i < maximum_page_count; ++i)
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

        for (var::int32 i = 0; i < maximum_page_count; ++i)
        {
            m_memory_pool[i] = other_p.m_memory_pool[i];
            other_p.m_memory_pool[i] = nullptr;
        }
        return *this;
    }

	_FE_FORCE_INLINE_ bool operator==(const pool& other_p) const noexcept { return m_memory_pool[0] == other_p.m_memory_pool[0]; }

    pool(const pool&) noexcept = delete;
    pool& operator=(const pool&) noexcept = delete;

    template<typename U>
    U* allocate(FE::int32 size_p = 1) noexcept
    {
        FE_STATIC_ASSERT(std::is_array<U>::value == false, "Static Assertion Failed: The T must not be an array[] type.");

        FE::int32 l_queried_allocation_size_in_bytes = (FE::int32)FE::calculate_aligned_memory_size_in_bytes<U, Alignment>(size_p);
        FE_EXIT_IF(l_queried_allocation_size_in_bytes > page_capacity, FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow, "Fatal Error: Unable to allocate ${%d0} bytes of memmory that exceeds the pool chunk's capacity.", &size_p);
        FE_ASSERT((l_queried_allocation_size_in_bytes % Alignment::size) == 0, "Critical Error in FE.Core.scalable_pool: the requested allocation size '${%d@0}' is not properly aligned by ${%lu@1}.", &l_queried_allocation_size_in_bytes, &Alignment::size);

        for (var::int32 i = 0; i < maximum_page_count; ++i)
        {
            if (m_memory_pool[i] == nullptr) _FE_UNLIKELY_
            {
				m_memory_pool[i] = (chunk_type*)FE_ALIGNED_ALLOC(sizeof(chunk_type), page_capacity);
			    FE_ASSERT(m_memory_pool[i] != nullptr, "Critical Error in FE.Core.scalable_pool: Unable to allocate a new memory page for this pool instance.");
			    new(m_memory_pool[i]) chunk_type();
                ++m_page_count;

				// Swap the new page to the front of the array for faster access.
                std::swap(m_memory_pool[0], m_memory_pool[i]);
                i = 0;
                FE_LOG(FE::log::Severity::_Info, "New memory page has been created for this instance.\nThe instance address: ${%p@0}\nThe number of pages have been allocated for the instance: ${%u32@1}.", this, &m_page_count);
            }

            alignas(16) internal::pool::block_info l_memblock_info{};
           
            if (__try_allocation_from_page(m_memory_pool[i], l_memblock_info, l_queried_allocation_size_in_bytes) == _FE_FAILED_)
            { 
                if (m_page_count == maximum_page_count)
                {
                    return nullptr;
                }
                continue; // It will eventually create a new page if the next pages are not available.
            }

            if constexpr (FE::is_trivial<U>::value == false)
            {
                U* const l_end = reinterpret_cast<U*>(l_memblock_info._address) + size_p;
                for (U* it = reinterpret_cast<U*>(l_memblock_info._address); it != l_end; ++it)
                {
                    new(it) U();
                }
            }

#ifdef _ENABLE_ASSERT_
            m_memory_pool[i]->check_double_allocation(l_memblock_info);
#endif
            FE_ASSERT((reinterpret_cast<FE::uintptr>(l_memblock_info._address) % Alignment::size) == 0, "FE.Core.scalable_pool has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", l_memblock_info._address, &Alignment::size);
			m_memory_pool[i]->_usage_in_bytes += l_memblock_info._size_in_bytes;
            return reinterpret_cast<U*>(l_memblock_info._address);
        }

        FE_LOG(FE::log::Severity::_Warning, "The allocation failed because the pool instance is out of its capacity. A nullptr has been returned.");
        return nullptr;
    }

    // Incorrect type will cause a critical runtime error.
    template <typename T> 
    void deallocate(T* pointer_p, FE::uint64 element_count_p) noexcept 
    {
        static_assert(sizeof(T) <= page_capacity, "Static assertion failed: sizeof(T) must not be greater than page_capacity.");
        FE_NEGATIVE_ASSERT(pointer_p == nullptr, "Critical Error in FE.Core.scalable_pool: Unable to deallocate() a nullptr.");
        FE_NEGATIVE_ASSERT(element_count_p == 0, "${%s@0}: ${%s@1} was 0", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(element_count_p));
        FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.Core.scalable_pool: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this scalable_pool instance.", pointer_p, &Alignment::size);

        alignas(16) internal::pool::block_info l_block_to_free;
        l_block_to_free._address = reinterpret_cast<var::byte*>(pointer_p);
        l_block_to_free._size_in_bytes = (FE::int32)FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(element_count_p);

        // use a pointer arithmetic to find which page the pointer belongs to.
        page_pointer L_page_base = reinterpret_cast<page_pointer>(reinterpret_cast<FE::uintptr>(l_block_to_free._address) - (reinterpret_cast<FE::uintptr>(l_block_to_free._address) % page_capacity));
        FE::int64 l_distance_from_begin = l_block_to_free._address - reinterpret_cast<FE::byte*>(L_page_base);
        if ((l_distance_from_begin < 0) || (l_distance_from_begin > page_capacity))
        {
            FE_LOG(FE::log::Severity::_Warning, "A Warning in FE.Core.scalable_pool: the pointer value '${%p@0}' does not belong to this pool instance. The deallocation has been yielded.", pointer_p);
            return; // The pointer does not belong to this pool instance.
        }

        if constexpr (FE::is_trivial<T>::value == false)
        {
            for (var::uint64 i = 0; i < element_count_p; ++i)
            {
                pointer_p->~T();
                ++pointer_p;
            }
        }

#ifdef _ENABLE_ASSERT_
        L_page_base->check_double_free(l_block_to_free);
#endif
        L_page_base->add_to_the_free_list(l_block_to_free);
        L_page_base->_usage_in_bytes -= l_block_to_free._size_in_bytes;
        FE_ASSERT(L_page_base->_usage_in_bytes >= 0, "Critical Error in FE.Core.scalable_pool: the internal usage counter has gone negative. Memory corruption might have occurred.");
    }

    bool try_trim_a_page() noexcept
    {
        for(page_pointer& page_ptr : m_memory_pool)
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
                std::swap( page_ptr, m_memory_pool[m_page_count] );
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

	_FE_FORCE_INLINE_ FE::int32 get_page_count() const noexcept { return m_page_count; }

protected:
    _FE_FORCE_INLINE_ virtual void* do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p = Alignment::size) noexcept override
    {
        FE_ASSERT(bytes_p <= FE::max_value<FE::int32>);
		return allocate<std::byte>((FE::int32)bytes_p);
    }

    _FE_FORCE_INLINE_ virtual void do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p = Alignment::size) noexcept override
	{
        FE_ASSERT(bytes_p <= FE::max_value<FE::int32>);
		deallocate<std::byte>(static_cast<std::byte*>(ptr_p), (FE::int32)bytes_p);
	}

    _FE_FORCE_INLINE_ virtual bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override
    {
		if (dynamic_cast<const pool*>(&other_p) == nullptr)
		{
			return false;
		}

		return operator==(dynamic_cast<const pool&>(other_p));
    }

private:
    /* Time complexity: 
	Allocation from a stack: O(1)
	Allocation from a heap tree: O(2 log n)
	Defragmentation: O(5n + n log n) [rarely gets called]

	Best: O(1)
	Worst: O(5n + n log n) + O(2 log n)
    */
    static FE::boolean __try_allocation_from_page(page_pointer& page_p, internal::pool::block_info& out_result_p, FE::int32 bytes_p) noexcept
    {
        FE_ASSERT((bytes_p % Alignment::size) == 0, "Critical Error in FE.Core.scalable_pool: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &bytes_p, &Alignment::size);
        if (page_p->is_page_heapified() == true)
        {
			if(page_p->retrieve_from_the_free_list(out_result_p, bytes_p) == _FE_FAILED_)
            {
				// Try defragmenting the page.
				__defragment(page_p);
				return page_p->retrieve_from_the_free_list(out_result_p, bytes_p); // Retry it. Traverse to the nxt page if it fails.
            }
			return _FE_SUCCEEDED_;
        }
        else
        {
			// Try allocation by pushing the stack pointer.
            out_result_p._address = page_p->_page_iterator;
            out_result_p._size_in_bytes = bytes_p;
            page_p->_page_iterator += bytes_p;

            // The requested allocation size overflows the page capacity.
            if (page_p->_page_iterator > page_p->_end)
            {
                page_p->_page_iterator -= bytes_p; // Allocation failed, unwind the stack to cancel the allocation.
                out_result_p._address = nullptr;
                out_result_p._size_in_bytes = out_result_p._size_in_bytes xor out_result_p._size_in_bytes;

                if (page_p->get_free_list_size() > 1) // Is free list defragmentable?
                {
                    // Defragment the page.
                    __defragment(page_p);
                }

                if (page_p->get_free_list_size() > 0) // Isn't the free list empty?
                {
                    return page_p->retrieve_from_the_free_list(out_result_p, bytes_p); // Try allocating from the defragmented free list. Traverse to the nxt page if it fails.
                }
                // If the page is out of capacity and the free list is empty, the pool needs to create a new page.
                return _FE_FAILED_;
            }
        }
        return _FE_SUCCEEDED_;
    }

	// Time complexity: O(5n + n log n).
    static void __defragment(page_pointer& page_p) noexcept
    {
        if (page_p->get_free_list_size() <= 1) _FE_UNLIKELY_
        {
            return;
        }

        std::sort<std::execution::parallel_unsequenced_policy, free_list_iterator, internal::pool::from_low_address>(std::execution::parallel_unsequenced_policy{},
            page_p->get_free_list(),
            page_p->get_free_list() + page_p->get_free_list_size(),
            internal::pool::from_low_address{});

		// Merge the free list.
		free_list_iterator l_iterator = page_p->get_free_list();
		free_list_iterator l_next = l_iterator + 1;
		free_list_iterator l_end = l_iterator + page_p->get_free_list_size();

		FE::int32 l_threads_count = std::thread::hardware_concurrency() >> 1;
		FE::int32 l_jobs_count_per_thread = page_p->get_free_list_size() / l_threads_count;
  
        static_assert(sizeof(internal::pool::block_info)*4 == FE::CPU_L1_cache_line::size, "Static Assertion failed: can't compile due to false sharing issues!");
		if (l_jobs_count_per_thread > 8) // Parallelize the defragmentation if the job count per thread is large enough.
        {
			thread_local static tf::Executor tl_s_executor(l_threads_count);
            thread_local static tf::Taskflow tl_s_taskflow;

			std::atomic<free_list_iterator> l_atomic_partition = page_p->get_free_list();

            FE_DO_ONCE(_DO_ONCE_PER_THREAD_,
                for (var::int32 i = 0; i < l_threads_count; ++i)
                {
                    tl_s_taskflow.emplace(
                        [l_jobs_count_per_thread, &l_atomic_partition]()
                        {
                            free_list_iterator l_range_iterator = l_atomic_partition.fetch_add(l_jobs_count_per_thread);
							free_list_iterator l_next = l_range_iterator + 1;
                            free_list_iterator l_range_end = l_range_iterator + l_jobs_count_per_thread;

                            // Time complexity: O(n)
                            while (l_next != l_range_end)
                            {
                                // Merge the adjacent blocks.
                                if ((l_range_iterator->_address + l_range_iterator->_size_in_bytes) == l_next->_address)
                                {
                                    l_range_iterator->_size_in_bytes += l_next->_size_in_bytes;

                                    // Nullify the block.
                                    l_next->_address = nullptr;
                                    l_next->_size_in_bytes = l_next->_size_in_bytes xor l_next->_size_in_bytes;
                                }
                                // Move to the next block if they are not adjacent.
                                ++l_next;
                            }
                        }
                    );
                }
            );

			tl_s_executor.run(tl_s_taskflow).wait();
			tl_s_taskflow.clear();

			// Migrate null blocks to right-side of the array to exclude them from being iterated.
            l_end = std::stable_partition<std::execution::parallel_unsequenced_policy>(std::execution::parallel_unsequenced_policy{}, page_p->get_free_list(), l_end,
                [](const internal::pool::block_info& block_p)
                {
					return block_p._address != nullptr;
                }
            );
        }


        // Time complexity: O(n)
        while (l_next != l_end)
        {
            // Merge the adjacent blocks.
            if ((l_iterator->_address + l_iterator->_size_in_bytes) == l_next->_address)
            {
                l_iterator->_size_in_bytes += l_next->_size_in_bytes;

                // Nullify the block.
                l_next->_address = nullptr;
                l_next->_size_in_bytes = l_next->_size_in_bytes xor l_next->_size_in_bytes;
                ++l_next; // Look for the next block.
                continue;
            }
            // Move to the next block if they are not adjacent.
            l_iterator = l_next;
            ++l_next;
        }

		// Migrate null blocks to right-side of the array to exclude them from being binary searched.
        /* Time complexity: 
        Best - O(n/2)
		Worst - O(n)
        */                                                
        l_end = std::partition<std::execution::parallel_unsequenced_policy>(std::execution::parallel_unsequenced_policy{}, page_p->get_free_list(), l_end, 
            [](const internal::pool::block_info& block_p)
            {
                return block_p._address != nullptr;
            }
        );
        // Reset it.
        page_p->set_free_list_size( static_cast<FE::int32>( l_end - page_p->get_free_list() ) );

		// Heapify the free list. Time complexity: O(3n)
		std::make_heap(page_p->get_free_list(), l_end, internal::pool::less_than{}); // To do: consider parallelizing the heapification.

		page_p->set_page_heapified(); // Switch the allocation strategy to binary search.
    }
};


/*
- allocate()
 best: O(1)
 average: O(log n)
 worst: O(5n/2 + n log n) ~ O(3n + n log n)

- deallocate()
 best: O(1)
 worst: O(log n)

- __defragment() [this function rarely gets called]
 O(5n/2 + n log n) ~ O(3n + n log n)

 - __retrive_from_free_list()
 O(log n)

 The scalable_pool class template in the Frogman Engine provides a scalable memory pool for efficient allocation and deallocation of memory blocks with a specified alignment_p and page capacity
 ensuring that allocations are properly aligned and managed within a limited number of memory pages.
*/
template<class Alignment = FE::SIMD_auto_alignment>
using scalable_pool = pool<PoolType::_Scalable, Alignment>;


END_NAMESPACE
#endif