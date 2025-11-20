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
#include <FE/pool/private/pool_common.hxx>
#include <FE/iterator.hxx>




BEGIN_NAMESPACE(FE)


namespace internal::pool
{
    template<class Alignment>
    class chunk<PoolType::_Scalable, Alignment>
    {
        static_assert(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    public:
        using free_list_iterator = block_info*;
        using free_list_element = block_info;

        constexpr static FE::uint32 page_size_in_bytes = 4 * FE::one_KiB;
        constexpr static FE::uint32 possible_address_count = page_size_in_bytes / Alignment::size;
        
    private:
        var::byte m_page[page_size_in_bytes];
        block_info m_free_list[possible_address_count];
        var::byte* m_page_end;
        var::uint32 m_free_list_size;
        var::boolean m_is_page_heapified;

    public:
        var::uint32 _usage_in_bytes;
        var::byte* _page_iterator;


    public:
        chunk() noexcept
            :   m_free_list_size(0),
                m_is_page_heapified(false),
                _usage_in_bytes(0)

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            ,   m_double_free_tracker()
#endif
        {
            _page_iterator = static_cast<var::byte*>(m_page);
            m_page_end = _page_iterator + page_size_in_bytes;

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            m_double_free_tracker = (var::uint32*)FE_ALIGNED_ALLOC(possible_address_count * sizeof(var::uint32), FE::CPU_L1_cache_line::size);
			FE_EXIT_IF(m_double_free_tracker == nullptr, FE::ErrorCode::_FatalMemoryError_1XX_AllocationFailure, "Fatal Error: Unable to FE_ALIGNED_ALLOC double-free tracker for a scalable_pool.");
			std::memset(m_double_free_tracker, 0, possible_address_count * sizeof(var::uint32));
#endif
        }

        ~chunk() noexcept
        {
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            FE_ASSERT(m_double_free_tracker != nullptr, "Assertion failed: someone tampered the double-free tracker pointer");
			FE_ALIGNED_FREE(m_double_free_tracker);
#endif
        }

        chunk(const chunk&) = delete;
		chunk& operator=(const chunk&) = delete;
        chunk(chunk&&) = delete;
		chunk& operator=(chunk&&) = delete;
        
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
    private:
        var::uint32* m_double_free_tracker;

    public:
        _FE_FORCE_INLINE_ void check_double_allocation(const block_info& block_info_p) noexcept
        {
            FE::uint32 l_idx = static_cast<FE::uint32>((block_info_p._address - m_page) / Alignment::size);
            FE_EXIT_IF(m_double_free_tracker[l_idx] != 0, FE::ErrorCode::_FatalMemoryError_1XX_HeapCorruption, "Double allocation detected: cannot allocate the same address twice.");
            m_double_free_tracker[l_idx] = (var::uint32)block_info_p._size_in_bytes;
        }

        _FE_FORCE_INLINE_ void check_double_free(const block_info& block_info_p) noexcept
        {
            FE::uint32 l_idx = static_cast<FE::uint32>((block_info_p._address - m_page) / Alignment::size);
            FE_EXIT_IF(m_double_free_tracker[l_idx] != block_info_p._size_in_bytes, FE::ErrorCode::_FatalMemoryError_1XX_DoubleFree, "Double free detected: cannot deallocate the same address twice.");
            m_double_free_tracker[l_idx] = 0;
        }
#endif
		var::byte* get_page() noexcept { return m_page; }
		FE::uint32 get_page_size() const noexcept { return page_size_in_bytes; }
        var::byte* get_page_end() noexcept { return m_page_end; }

        _FE_FORCE_INLINE_ FE::uint32 get_free_list_size() const noexcept { return m_free_list_size; }
        _FE_FORCE_INLINE_ void set_free_list_size(FE::uint32 size_p) noexcept
        {
            m_free_list_size = size_p;
        }

        _FE_FORCE_INLINE_ FE::boolean is_page_heapified() const noexcept { return m_is_page_heapified; }
        _FE_FORCE_INLINE_ void set_page_heapified() noexcept { m_is_page_heapified = true; }

        _FE_FORCE_INLINE_ FE::uint32 get_usage_as_percentile() const noexcept
        {
            return static_cast<FE::uint32>(((FE::float32)_usage_in_bytes / (FE::float32)page_size_in_bytes) * 100.0f);
        }

        void _FE_VECTOR_CALL_ add_to_the_free_list(const block_info& block_p) noexcept
        {
            FE_NEGATIVE_ASSERT(m_free_list_size == possible_address_count, "Assertion Failure: The free list is full.");
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
		FE::boolean _FE_VECTOR_CALL_ retrieve_from_the_free_list(internal::pool::block_info& out_alloc_result_p, FE::uint32 requested_bytes_p) noexcept
		{
            FE_ASSERT((requested_bytes_p % Alignment::size) == 0, "Critical Error in FE.Core.scalable_pool: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &requested_bytes_p, &Alignment::size);

            if (m_is_page_heapified == false)
            {
                return _FE_FAILED_;
            }

            if (m_free_list_size == 0)
            {
                out_alloc_result_p._address = nullptr;
                out_alloc_result_p._size_in_bytes = out_alloc_result_p._size_in_bytes xor out_alloc_result_p._size_in_bytes;
                return _FE_FAILED_;
            }

            FE_ASSERT(m_is_page_heapified == true, "Assertion Failure: The page is not binary heapified.");
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
    };
}




template<class Alignment>
class pool<PoolType::_Scalable, Alignment> : public std::pmr::memory_resource
{
    static_assert(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    using chunk_type = internal::pool::chunk<PoolType::_Scalable, Alignment>;
    using free_list_iterator = typename chunk_type::free_list_iterator;

public:
	using alignment_type = Alignment;
    
private:
    using pool_type = FE::list<chunk_type, FE::page_aligned_allocator<chunk_type>>;
	using page_pointer = typename pool_type::iterator;

    pool_type m_pages;

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
    using page_validation_table = absl::flat_hash_set<chunk_type*,
        FE::hash<chunk_type*>, std::equal_to<chunk_type*>,
        FE::cache_aligned_allocator<chunk_type*>>;
    page_validation_table m_page_validation_table;
#endif

public:
    pool() noexcept 
	    :   m_pages()
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
		,   m_page_validation_table()
#endif
    {
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        m_page_validation_table.reserve(512);
#endif
    }

    virtual ~pool() noexcept override = default;

    pool(pool&& other_p) noexcept
        :   m_pages( std::move(other_p.m_pages) )
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
		,   m_page_validation_table( std::move(other_p.m_page_validation_table) )
#endif
	{}

    pool& operator=(pool&& other_p) noexcept
    {
		if (this == &other_p) // prevent self-move-assignment
        {
            return *this;
		}

        m_pages = std::move(other_p.m_pages);
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        m_page_validation_table = std::move(other_p.m_page_validation_table);
#endif
        return *this;
    }

	_FE_FORCE_INLINE_ bool operator==(const pool&) const noexcept { return false; }

    pool(const pool&) noexcept = delete;
    pool& operator=(const pool&) noexcept = delete;

    template<typename U>
    U* _FE_VECTOR_CALL_ allocate(FE::size size_p) noexcept
    {
        static_assert(std::is_array_v<U> == false, "Static Assertion Failed: The T must not be an array[] type.");
        FE::uint32 l_queried_allocation_size_in_bytes = (FE::uint32)FE::calculate_aligned_memory_size_in_bytes<U, Alignment>(size_p);
        FE_ASSERT((l_queried_allocation_size_in_bytes % Alignment::size) == 0, "Critical Error in FE.Core.scalable_pool: the requested allocation size '${%d@0}' is not properly aligned by ${%lu@1}.", &l_queried_allocation_size_in_bytes, &Alignment::size);

        for (page_pointer page = m_pages.begin(); page != m_pages.end(); ++page)
        {
            FE_EXIT_IF(l_queried_allocation_size_in_bytes > page->get_page_size(), FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow, "Fatal Error: Unable to allocate ${%d0} bytes of memmory that exceeds the pool chunk's capacity.", &size_p);
            
            alignas(16) internal::pool::block_info l_memblock_info{};
            if (__try_allocation_from_page(page, l_memblock_info, l_queried_allocation_size_in_bytes) == _FE_FAILED_)
            { 
                continue; // It will eventually create a new page if the next pages are not available.
            }

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            page->check_double_allocation(l_memblock_info);
#endif
            if constexpr (FE::is_trivial_v<U> == false)
            {
                U* const l_end = reinterpret_cast<U*>(l_memblock_info._address) + size_p;
                for (U* it = reinterpret_cast<U*>(l_memblock_info._address); it != l_end; ++it)
                {
                    new(it) U();
                }
            }

            FE_ASSERT((reinterpret_cast<FE::uintptr>(l_memblock_info._address) % Alignment::size) == 0, "FE.Core.scalable_pool has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", l_memblock_info._address, &Alignment::size);
            page->_usage_in_bytes += (var::uint32)l_memblock_info._size_in_bytes;
            return reinterpret_cast<U*>(l_memblock_info._address);
        }

        FE_LOG(FE::log::Severity::_Warning, "The initial attempts to allocating memory failed. Creating a new page.");
        /* this sort the list in the new-to-old order; the newest page is always at the front of the list.
        * This sorting strategy relies on the assumption that recently used pages are more likely to have free blocks available.
        
        * 0. // swap_extremes()
        * 1. (page 1) // emplace_front()
        *
        * 2. (page 1) // swap_extremes()
        * 3. (page 2) (page 1) // emplace_front()
        *
        * 4. (page 1) (page 2) // swap_extremes()
        * 5. (page 3) (page 1) (page 2) // emplace_front()
        *
        * 6. (page 2) (page 1) (page 3) // swap_extremes()
        * 7. (page 4) (page 2) (page 1) (page 3) // emplace_front()
        *
        * 8. (page 3) (page 2) (page 1) (page 4) // swap_extremes()
        * 9. (page 5) (page 3) (page 2) (page 1) (page 4) // emplace_front()
        */
        m_pages.swap_extremes(); 
		m_pages.emplace_front(); 
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
		m_page_validation_table.insert(&m_pages.front());
#endif
        return allocate<U>(size_p);
    }

    // Incorrect type will cause a critical runtime error.
    template <typename T> 
    void _FE_VECTOR_CALL_ deallocate(T* pointer_p, FE::size count_p) noexcept
    {
        FE_ASSERT(pointer_p != nullptr, "Critical Error in FE.Core.scalable_pool: Unable to deallocate() a nullptr.");
        FE_ASSERT(count_p > 0, "${%s@0}: ${%s@1} was 0", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(count_p));
        FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.Core.scalable_pool: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this scalable_pool instance.", pointer_p, &Alignment::size);

        alignas(16) internal::pool::block_info l_block_to_free;
        l_block_to_free._address = reinterpret_cast<var::byte*>(pointer_p);
        l_block_to_free._size_in_bytes = (FE::uint32)FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(count_p);
        FE_ASSERT(l_block_to_free._size_in_bytes <= FE::system_page_size, "Critical Error in FE.Core.scalable_pool: the pointer '${%p@0}' does not belong to this scalable_pool instance located at '${%p@1}'.", pointer_p, this);

        // use a pointer arithmetic to find which page the pointer belongs to.
        chunk_type* l_page_base = reinterpret_cast<chunk_type*>( (FE::uintptr)l_block_to_free._address - ( ((FE::uintptr)(l_block_to_free._address)) % FE::system_page_size ) );

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        FE_EXIT_IF(m_page_validation_table.find(l_page_base) == m_page_validation_table.end(), ErrorCode::_FatalMemoryError_1XX_FalseDeallocation, "Critical Error in FE.Core.scalable_pool: the pointer '${%p@0}' does not belong to this scalable_pool instance located at '${%p@1}'.", pointer_p, this);
        l_page_base->check_double_free(l_block_to_free);
#endif
        if constexpr (FE::is_trivial<T>::value == false)
        {
            for (var::uint64 i = 0; i < count_p; ++i)
            {
                pointer_p->~T();
                ++pointer_p;
            }
        }
        l_page_base->add_to_the_free_list(l_block_to_free);
        l_page_base->_usage_in_bytes -= (FE::uint32)l_block_to_free._size_in_bytes;
        FE_ASSERT(l_page_base->_usage_in_bytes >= 0, "Critical Error in FE.Core.scalable_pool: the internal usage counter has gone negative. Memory corruption might have occurred.");
    }

    bool try_trim_a_page() noexcept
    {
        for (typename pool_type::iterator page = m_pages.begin(); page != m_pages.end(); ++page)
        {
            if (page->_usage_in_bytes == 0)
            {
				m_pages->erase(page);
                return true;
			}
		}
        return false;
    }

    void try_trim_all_pages() noexcept
    {
        for (typename pool_type::iterator page = m_pages.begin(); page != m_pages.end();)
        {
            if (page->_usage_in_bytes == 0)
            {
                page = m_pages->erase(page);
                continue;
            }
            ++page;
        }
    }

	_FE_FORCE_INLINE_ FE::size get_page_count() const noexcept { return m_pages.size(); }

protected:
    _FE_FORCE_INLINE_ virtual void* do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p = Alignment::size) noexcept override
    {
        FE_ASSERT(bytes_p <= FE::max_value<FE::uint32>);
		return allocate<std::byte>(bytes_p);
    }

    _FE_FORCE_INLINE_ virtual void do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p = Alignment::size) noexcept override
	{
        FE_ASSERT(bytes_p <= FE::max_value<FE::uint32>);
		deallocate<std::byte>(static_cast<std::byte*>(ptr_p), bytes_p);
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
    static FE::boolean _FE_VECTOR_CALL_ __try_allocation_from_page(page_pointer page_p, internal::pool::block_info& out_result_p, FE::uint32 bytes_p) noexcept
    {
        FE_ASSERT((bytes_p % Alignment::size) == 0, "Critical Error in FE.Core.scalable_pool: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &bytes_p, &Alignment::size);
        if (page_p->is_page_heapified() == true)
        {
			if(page_p->retrieve_from_the_free_list(out_result_p, bytes_p) == _FE_FAILED_)
            {
				// Try defragmenting the page.
				__defragment(page_p);
                FE::boolean l_was_successful = page_p->retrieve_from_the_free_list(out_result_p, bytes_p); // Retry it. Traverse to the nxt page if it fails.
                FE_ASSERT(out_result_p._size_in_bytes <= FE::system_page_size, "Critical Error in FE.Core.scalable_pool: value overflow suspected.");
                return l_was_successful;
            }
            FE_ASSERT(out_result_p._size_in_bytes <= FE::system_page_size, "Critical Error in FE.Core.scalable_pool: value overflow suspected.");
			return _FE_SUCCEEDED_;
        }
        else
        {
			// Try allocation by pushing the stack pointer.
            out_result_p._address = page_p->_page_iterator;
            out_result_p._size_in_bytes = bytes_p;
            page_p->_page_iterator += bytes_p;

            // The requested allocation size overflows the page capacity.
            if (page_p->_page_iterator > page_p->get_page_end())
            {
                page_p->_page_iterator -= bytes_p; // Allocation failed, unwind the stack to cancel the allocation.
                out_result_p._address = nullptr;
                out_result_p._size_in_bytes = out_result_p._size_in_bytes xor out_result_p._size_in_bytes;

                if (page_p->get_free_list_size() > 1) // Can the free list defragmented?
                {
                    __defragment(page_p); // Defragment the page.
                }

                if ((page_p->get_free_list_size() > 0) &&
                    (page_p->is_page_heapified() == true)) // Is the free list not empty?
                {
					//FE_ASSERT(page_p->is_page_heapified() == true, "Assertion Failure: The page must be heapified after defragmentation.");
                 
                    FE::boolean l_was_successful = page_p->retrieve_from_the_free_list(out_result_p, bytes_p); // Try allocating from the defragmented free list. Traverse to the next page if it fails.
                    FE_ASSERT(out_result_p._size_in_bytes <= FE::system_page_size, "Critical Error in FE.Core.scalable_pool: value overflow suspected.");
                    return l_was_successful;
                }
                // If the page is out of capacity and the free list is empty, the pool needs to create a new page.
                return _FE_FAILED_;
            }
        }
        FE_ASSERT(out_result_p._size_in_bytes <= FE::system_page_size, "Critical Error in FE.Core.scalable_pool: value overflow suspected.");
        return _FE_SUCCEEDED_;
    }

	// Time complexity: O(5n + n log n).
    static void _FE_VECTOR_CALL_ __defragment(page_pointer page_p) noexcept
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

		FE::uint32 l_threads_count = std::thread::hardware_concurrency() >> 1;
		FE::uint32 l_jobs_count_per_thread = page_p->get_free_list_size() / l_threads_count;
  
        static_assert(sizeof(internal::pool::block_info)*4 == FE::CPU_L1_cache_line::size, "Static Assertion failed: can't compile due to false sharing issues!");
		if (l_jobs_count_per_thread > 8) // Parallelize the defragmentation if the job count per thread is large enough.
        {
			thread_local static tf::Executor tl_s_executor(l_threads_count);
            thread_local static tf::Taskflow tl_s_taskflow;

			std::atomic<free_list_iterator> l_atomic_partition = page_p->get_free_list();

            FE_DO_ONCE(_DO_ONCE_PER_THREAD_,
                for (var::uint32 i = 0; i < l_threads_count; ++i)
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
        page_p->set_free_list_size( static_cast<FE::uint32>( l_end - page_p->get_free_list() ) );

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