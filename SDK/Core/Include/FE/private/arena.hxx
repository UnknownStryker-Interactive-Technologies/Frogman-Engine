#ifndef _FE_CORE_PRIVATE_ARENA_HXX_
#define _FE_CORE_PRIVATE_ARENA_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/private/pool_common.hxx>




BEGIN_NAMESPACE(FE)


namespace internal::super_large::pool
{
    template<class Alignment>
    class FE::internal::pool::chunk<PoolType::_SuperLargeArea, Alignment>
    {
        static_assert(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");
        static_assert(Alignment::size == 32 || Alignment::size == 64);

    public:
        using free_list_iterator = large::block_info*;
        using free_list_element = large::block_info;

        constexpr static FE::int32 page_granularity_in_bytes = FE::one_GiB;
        constexpr static FE::int32 page_size_in_bytes = page_granularity_in_bytes - FE::CPU_L1_cache_line::size; // To avoid using an extra large page
		constexpr static FE::int32 possible_address_count = (page_size_in_bytes / Alignment::size);
        constexpr static FE::int32 integrity_validator_size = (page_size_in_bytes / Alignment::size);

    private:
        var::byte m_page[page_size_in_bytes];
        free_list_element* m_free_list;
        var::int32 m_largest_free_fragment_size_in_bytes;
        var::int32 m_free_list_size;
        var::byte* m_page_end;
        var::boolean m_is_page_heapified;
        var::boolean m_has_free_list_been_updated_since_defragmentation;

        static_assert(sizeof(m_page) < FE::max_value<FE::int32>, "Static assertion failed: sizeof(m_page) is exceeds the maximum allowed size.");

    public:
        var::int32 _usage_in_bytes;
        var::byte* _page_iterator;

    public:
        chunk() noexcept
            : m_free_list(nullptr),
            m_largest_free_fragment_size_in_bytes(0),
            m_free_list_size(0),
            m_is_page_heapified(false),
            m_has_free_list_been_updated_since_defragmentation(false),
            _usage_in_bytes(0)

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            , m_double_free_tracker()
#endif
        {
            m_free_list = (free_list_element*)FE_ALIGNED_ALLOC(possible_address_count * sizeof(free_list_element), FE::CPU_L1_cache_line::size);
            _page_iterator = static_cast<var::byte*>(m_page);
            m_page_end = _page_iterator + page_size_in_bytes;


#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            m_double_free_tracker = (var::int32*)FE_ALIGNED_ALLOC(integrity_validator_size * sizeof(var::int32), FE::CPU_L1_cache_line::size);
            FE_EXIT_IF(m_double_free_tracker == nullptr, FE::ErrorCode::_FatalMemoryError_1XX_AllocationFailure, "Fatal Error: Unable to FE_ALIGNED_ALLOC double-free tracker for a scalable_allocator.");
            std::memset(m_double_free_tracker, 0, integrity_validator_size * sizeof(var::int32));
#endif
        }

        ~chunk() noexcept
        {
            FE_ASSERT(m_free_list != nullptr, "Assertion failed: someone tampered the free list pointer");
            FE_ALIGNED_FREE(m_free_list);


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
        var::int32* m_double_free_tracker;

    public:
        _FE_FORCE_INLINE_ void check_double_allocation(const free_list_element& block_info_p) noexcept
        {
            FE::int32 l_idx = static_cast<FE::int32>((block_info_p._address) / Alignment::size);
            FE_EXIT_IF(m_double_free_tracker[l_idx] != 0, FE::ErrorCode::_FatalMemoryError_1XX_HeapCorruption, "Double allocation detected: cannot allocate the same address twice.");
            m_double_free_tracker[l_idx] = (var::int32)block_info_p._size_in_bytes;
        }

        _FE_FORCE_INLINE_ void check_double_free(const free_list_element& block_info_p) noexcept
        {
            FE::int32 l_idx = static_cast<FE::int32>((block_info_p._address) / Alignment::size);
            FE_EXIT_IF(m_double_free_tracker[l_idx] != block_info_p._size_in_bytes, FE::ErrorCode::_FatalMemoryError_1XX_DoubleFree, "Double free detected: cannot deallocate the same address twice.");
            m_double_free_tracker[l_idx] = 0;
        }
#endif
        var::byte* get_page() noexcept { return m_page; }
        FE::int32 get_page_size() const noexcept { return page_size_in_bytes; }
        var::byte* get_page_end() noexcept { return m_page_end; }

        FE::int32 get_free_list_size() const noexcept { return m_free_list_size; }
        void set_free_list_size(FE::int32 size_p) noexcept { m_free_list_size = size_p; }

        FE::boolean is_page_heapified() const noexcept { return m_is_page_heapified; }
        FE::boolean has_free_list_been_updated_since_defragmentation() const noexcept { return m_has_free_list_been_updated_since_defragmentation; }
        void reset_dirty_flag() noexcept { m_has_free_list_been_updated_since_defragmentation = false; }

        void set_page_heapified() noexcept { m_is_page_heapified = true; }
        void set_page_unheapified() noexcept { m_is_page_heapified = false; }

        FE::int32 get_usage_as_percentile() const noexcept
        {
            return static_cast<FE::int32>(((FE::float32)_usage_in_bytes / (FE::float32)page_size_in_bytes) * 100.0f);
        }

        void _FE_VECTOR_CALL_ add_to_the_free_list(const free_list_element& block_p) noexcept
        {
            FE_NEGATIVE_ASSERT(m_free_list_size == possible_address_count, "Assertion Failure: The free list is full.");
            free_list_element* const l_position = static_cast<free_list_element*>(m_free_list) + m_free_list_size;

            l_position->_address = block_p._address;
            l_position->_size_in_bytes = block_p._size_in_bytes;

            ++(m_free_list_size);

            if (m_is_page_heapified == true)
            {
                std::push_heap(static_cast<free_list_iterator>(m_free_list), static_cast<free_list_iterator>(m_free_list) + m_free_list_size, internal::pool::large::less_than{});
                m_largest_free_fragment_size_in_bytes = m_free_list[0]._size_in_bytes;
            }
            m_has_free_list_been_updated_since_defragmentation = true;
        }

        // Time complexity: O(2 log n)
        FE::boolean _FE_VECTOR_CALL_ retrieve_from_the_free_list(free_list_element& out_alloc_result_p, FE::int32 requested_bytes_p) noexcept
        {
            FE_ASSERT((requested_bytes_p % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &requested_bytes_p, &Alignment::size);

            if (m_is_page_heapified == false)
            {
                return _FE_FAILED_;
            }

            if (m_free_list_size == 0)
            {
                out_alloc_result_p._address = static_cast<var::int32>(-1);
                out_alloc_result_p._size_in_bytes = out_alloc_result_p._size_in_bytes xor out_alloc_result_p._size_in_bytes;
                return _FE_FAILED_;
            }

            FE_ASSERT(m_is_page_heapified == true, "Assertion Failure: The page is not binary heapified.");
            std::pop_heap(static_cast<free_list_iterator>(m_free_list), static_cast<free_list_iterator>(m_free_list) + m_free_list_size, internal::pool::large::less_than{});
            --(m_free_list_size);
            m_largest_free_fragment_size_in_bytes = m_free_list[0]._size_in_bytes;

            // Try allocation.
            if (m_free_list[m_free_list_size]._size_in_bytes >= requested_bytes_p)
            {
                out_alloc_result_p._address = m_free_list[m_free_list_size]._address;
                out_alloc_result_p._size_in_bytes = requested_bytes_p;
                FE_ASSERT((out_alloc_result_p._address % Alignment::size) == 0, "FE.Core.scalable_allocator has failed to allocate an address: the pointer value '${%d@0}' is not properly aligned by ${%lu@1}.", &out_alloc_result_p._address, &Alignment::size);

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

            out_alloc_result_p._address = static_cast<var::int32>(-1);
            out_alloc_result_p._size_in_bytes = out_alloc_result_p._size_in_bytes xor out_alloc_result_p._size_in_bytes;
            return _FE_FAILED_; // Try iterate to the next page.
        }

        _FE_FORCE_INLINE_ free_list_element* get_free_list() noexcept { return static_cast<free_list_element*>(m_free_list); }
    };
}


namespace super_large
{
    template<class Alignment>
    class pool<PoolType::_SuperLargeArea, Alignment>
    {
        static_assert(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

        using chunk_type = internal::pool::chunk<PoolType::_SuperLargeArea, Alignment>;
        static_assert(sizeof(chunk_type) <= FE::one_GiB, "Static assertion failed: chunk must fit within a single 1 GiB-sized memory page.");

        using free_list_iterator = typename chunk_type::free_list_iterator;
        using free_list_element = typename chunk_type::free_list_element;

    public:
        constexpr static FE::int32 page_capacity = chunk_type::page_size_in_bytes;
        using alignment_type = Alignment;

    private:
        chunk_type* m_page;
		var::boolean m_did_1GiB_page_allocation_fail;

    public:
        pool() noexcept
            :   m_page(),
                m_did_1GiB_page_allocation_fail(false)
        {
            FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, FE::internal::pool::__enable_large_pages(););

            MEM_EXTENDED_PARAMETER l_param = {};
            l_param.Type = MemExtendedParameterAttributeFlags;
            l_param.ULong64 = MEM_EXTENDED_PARAMETER_NONPAGED_HUGE;

            m_page = (chunk_type*)VirtualAlloc2(GetCurrentProcess(),
                                                nullptr,
                                                FE::one_GiB,
                                                MEM_RESERVE | MEM_COMMIT | MEM_LARGE_PAGES,
                                                PAGE_READWRITE,
                                                &l_param, 
                                                sizeof(l_param) / sizeof(MEM_EXTENDED_PARAMETER)
            );

            if (m_page == nullptr)
            {
                _FE_MAYBE_UNUSED_ DWORD l_errcode = GetLastError();
                FE_LOG(FE::log::Severity::_Warning, "1GiB page allocation failed due to ${%d@0}.", &l_errcode);
                m_did_1GiB_page_allocation_fail = true;
                return;
            }

            new(m_page) chunk_type(); // Placement new to construct the chunk in the allocated memory.

            FE_LOG(FE::log::Severity::_Info, "1GiB page allocation was successful");
        }

        ~pool() noexcept
        {
			if (m_page != nullptr)
			{
                m_page->~chunk_type(); // Explicitly call the destructor for the chunk.

				VirtualFree(m_page, 0, MEM_RELEASE);
			}
        }

        pool(pool&& other_p) noexcept
            :   m_page(other_p.m_page),
                m_did_1GiB_page_allocation_fail(other_p.m_did_1GiB_page_allocation_fail)    
        {
			other_p.m_page = nullptr;
        }

        pool& operator=(pool&& other_p) noexcept
        {
            if (this == &other_p) // prevent self-move-assignment
            {
                return *this;
            }

			if (m_page != nullptr)
			{
				m_page->~chunk_type(); // Explicitly call the destructor for the chunk.
				VirtualFree(m_page, 0, MEM_RELEASE);
			}

            m_page = other_p.m_page;
            other_p.m_page = nullptr;

			m_did_1GiB_page_allocation_fail = other_p.m_did_1GiB_page_allocation_fail;
            return *this;
        }

        bool operator==(const pool& other_p) const noexcept
        {
            if (FE::polymorphic_cast<const pool*>(&other_p) == nullptr)
            {
                return false;
            }

            auto l_this = dynamic_cast<const pool*>(this);
            FE_ASSERT(l_this != nullptr, "Assertion failed: dynamic_cast from 'this' to const pool* has returned a nullptr. This should never happen.");
            return l_this == &other_p;
        }

        pool(const pool&) noexcept = delete;
        pool& operator=(const pool&) noexcept = delete;




        template<typename U>
        U* _FE_VECTOR_CALL_ allocate(FE::size size_p) noexcept
        {
            static_assert(FE::is_trivial_v<U> == true, "Static Assertion Failed: The T must be a trivial type.");
            static_assert(std::is_array_v<U> == false, "Static Assertion Failed: The T must not be an array[] type.");
            
            FE::int32 l_queried_allocation_in_bytes = (FE::int32)FE::calculate_aligned_memory_size_in_bytes<U, Alignment>(size_p);
            FE_ASSERT(l_queried_allocation_in_bytes > 2*FE::one_MiB, "Critical Error in FE.Core.scalable_allocator: the requested allocation size is too small.");
            FE_ASSERT((l_queried_allocation_in_bytes % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the requested allocation size '${%d@0}' is not properly aligned by ${%lu@1}.", &l_queried_allocation_in_bytes, &Alignment::size);
            FE_ASSERT((FE::calculate_aligned_memory_size_in_bytes<U, Alignment>(size_p)) <= page_capacity, "Fatal Error: Unable to allocate ${%d0} bytes of memory that exceeds the pool chunk's capacity.", &size_p);


            if (m_did_1GiB_page_allocation_fail == true)
            {
                return nullptr;
            }


            free_list_element l_memblock_info = {};
            var::boolean l_was_allocation_successful = __try_allocation_from_page(m_page, l_memblock_info, l_queried_allocation_in_bytes);

            if (l_was_allocation_successful == false)
            {
                return nullptr;
            }

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            m_page->check_double_allocation(l_memblock_info);
#endif
            FE_ASSERT((reinterpret_cast<FE::uintptr>(l_memblock_info._address + m_page->get_page()) % Alignment::size) == 0, "FE.Core.scalable_allocator has failed to allocate an address: the pointer value '${%d@0}' is not properly aligned by ${%lu@1}.", &l_memblock_info._address, &Alignment::size);
            m_page->_usage_in_bytes += l_memblock_info._size_in_bytes;

            return reinterpret_cast<U*>(l_memblock_info._address + m_page->get_page());
        }

        // Incorrect type will cause a critical runtime error.
        template <typename T>
        bool _FE_VECTOR_CALL_ deallocate(T* pointer_p, FE::size count_p) noexcept
        {
			static_assert(FE::is_trivial_v<T> == true, "Static Assertion Failed: The T must be a trivial type.");

            FE_ASSERT(pointer_p != nullptr, "Critical Error in FE.Core.scalable_allocator: Unable to deallocate() a nullptr.");
            FE_ASSERT(count_p > 0, "${%s@0}: ${%s@1} was 0", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(count_p));
            FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this scalable_allocator instance.", pointer_p, &Alignment::size);


            if (m_did_1GiB_page_allocation_fail == true)
            {
                return false;
            }


            if (((m_page->get_page() <= (var::byte*)pointer_p) && ((var::byte*)pointer_p < m_page->get_page() + m_page->get_page_size())) == false)
            {
                return false;
            }


            free_list_element l_block_to_free;
            l_block_to_free._address = static_cast<var::int32>((var::byte*)pointer_p - m_page->get_page());
            l_block_to_free._size_in_bytes = (FE::int32)FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(count_p);
            FE_ASSERT((FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(count_p)) <= page_capacity, "Critical Error in FE.Core.scalable_allocator: the pointer '${%p@0}' does not belong to this scalable_allocator instance located at '${%p@1}'.", pointer_p, this);


#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
			m_page->check_double_free(l_block_to_free);
#endif
            m_page->add_to_the_free_list(l_block_to_free);
            m_page->_usage_in_bytes -= l_block_to_free._size_in_bytes;
            FE_ASSERT(m_page->_usage_in_bytes >= 0, "Critical Error in FE.Core.scalable_allocator: the internal usage counter has gone negative. Memory corruption might have occurred.");
            
            if (m_page->_usage_in_bytes == 0) _FE_UNLIKELY_
            {
                m_page->set_free_list_size(0);
                m_page->set_page_unheapified();
                m_page->_page_iterator = m_page->get_page();
                m_page->reset_dirty_flag();
            }
            return true;
        }


    private:
        /* Time complexity:
        Allocation from a stack: O(1)
        Allocation from a heap tree: O(2 log n)
        Defragmentation: O(5n + n log n) [rarely gets called]

        Best: O(1)
        Worst: O(5n + n log n) + O(2 log n)
        */
        static FE::boolean _FE_VECTOR_CALL_ __try_allocation_from_page(chunk_type* page_p, free_list_element& out_result_p, FE::int32 bytes_p) noexcept
        {
            FE_ASSERT((bytes_p % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &bytes_p, &Alignment::size);
            if (page_p->is_page_heapified() == true)
            {
                if (page_p->retrieve_from_the_free_list(out_result_p, bytes_p) == _FE_FAILED_)
                {
                    if (page_p->has_free_list_been_updated_since_defragmentation() == false)
                    {
                        return _FE_FAILED_;
                    }
                    // Try defragmenting the page.
                    __defragment(page_p);
                    FE::boolean l_was_successful = page_p->retrieve_from_the_free_list(out_result_p, bytes_p); // Retry it. Traverse to the nxt page if it fails.
                    FE_ASSERT(out_result_p._size_in_bytes <= page_capacity, "Critical Error in FE.Core.scalable_allocator: value overflow suspected.");
                    return l_was_successful;
                }
                FE_ASSERT(out_result_p._size_in_bytes <= page_capacity, "Critical Error in FE.Core.scalable_allocator: value overflow suspected.");
                return _FE_SUCCEEDED_;
            }
            else
            {
                // Try allocation by pushing the stack pointer.
                out_result_p._address = static_cast<var::int32>(page_p->_page_iterator - page_p->get_page());
                out_result_p._size_in_bytes = bytes_p;
                page_p->_page_iterator += bytes_p;

                // The requested allocation size overflows the page capacity.
                if (page_p->_page_iterator > page_p->get_page_end())
                {
                    page_p->_page_iterator -= bytes_p; // Allocation failed, unwind the stack to cancel the allocation.
                    out_result_p._address = static_cast<var::int32>(-1);
                    out_result_p._size_in_bytes = out_result_p._size_in_bytes xor out_result_p._size_in_bytes;

                    free_list_element l_block_to_free;
                    l_block_to_free._address = static_cast<var::int32>(page_p->_page_iterator - page_p->get_page());
                    l_block_to_free._size_in_bytes = static_cast<var::int32>(page_p->get_page_end() - page_p->_page_iterator);

                    if (l_block_to_free._size_in_bytes > 0)
                    {
                        page_p->add_to_the_free_list(l_block_to_free);
                    }

                    __defragment(page_p); // Defragment the page.

                    if ((page_p->get_free_list_size() > 0) &&
                        (page_p->is_page_heapified() == true)) // Is the free list not empty?
                    {
                        //FE_ASSERT(page_p->is_page_heapified() == true, "Assertion Failure: The page must be heapified after defragmentation.");

                        FE::boolean l_was_successful = page_p->retrieve_from_the_free_list(out_result_p, bytes_p); // Try allocating from the defragmented free list. Traverse to the next page if it fails.
                        FE_ASSERT(out_result_p._size_in_bytes <= page_capacity, "Critical Error in FE.Core.scalable_allocator: value overflow suspected.");
                        return l_was_successful;
                    }
                    // If the page is out of capacity and the free list is empty, the pool needs to create a new page.
                    return _FE_FAILED_;
                }
            }
            FE_ASSERT(out_result_p._size_in_bytes <= page_capacity, "Critical Error in FE.Core.scalable_allocator: value overflow suspected.");
            return _FE_SUCCEEDED_;
        }

        // Try reducing the pointer offset data to int32 to save memory, since the page size is no larger than 2MiB.
        static void _FE_VECTOR_CALL_ __defragment(chunk_type* page_p) noexcept
        {
            if (page_p->get_free_list_size() < 1) _FE_UNLIKELY_
            {
                page_p->reset_dirty_flag(); // Reset the dirty flag.
                return;
            }

                std::sort<free_list_iterator, internal::pool::large::from_low_address>(
                    page_p->get_free_list(),
                    page_p->get_free_list() + page_p->get_free_list_size(),
                    internal::pool::large::from_low_address{});
            // Merge the free list.
            free_list_iterator l_iterator = page_p->get_free_list();
            free_list_iterator l_next = l_iterator + 1;
            free_list_iterator l_end = l_iterator + page_p->get_free_list_size();

            // Time complexity: O(n)
            while (l_next != l_end)
            {
                // Merge the adjacent blocks.
                if ((l_iterator->_address + l_iterator->_size_in_bytes) == l_next->_address)
                {
                    l_iterator->_size_in_bytes += l_next->_size_in_bytes;

                    // Nullify the block.
                    l_next->_address = static_cast<var::int32>(-1);
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
            l_end = std::partition(page_p->get_free_list(), l_end,
                [](const free_list_element& block_p)
                {
                    return block_p._size_in_bytes != 0;
                }
            );
            // Reset it.
            page_p->set_free_list_size(static_cast<FE::int32>(l_end - page_p->get_free_list()));

            // Heapify the free list. Time complexity: O(3n)
            std::make_heap(page_p->get_free_list(), l_end, internal::pool::large::less_than{});

            page_p->set_page_heapified(); // Switch the allocation strategy to binary search.
            page_p->reset_dirty_flag(); // Reset the dirty flag.
        }
    };
}

END_NAMESPACE
#endif