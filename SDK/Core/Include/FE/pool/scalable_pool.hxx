#ifndef _FE_CORE_SCALABLE_POOL_HXX_
#define _FE_CORE_SCALABLE_POOL_HXX_
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
#include <FE/algorithm/utility.hxx>
#include <FE/pool/private/pool_common.hxx>
#include <FE/iterator.hxx>
#include <FE/memory.hpp>

#ifdef _FE_ON_X86_64_
    #ifdef _SSE2_
        #include <emmintrin.h>
    #else
        #error "SSE2 is required for the x86-64 version of Frogman Engine's memory pool."
    #endif
#endif

// std::sort()
#include <algorithm>

// std::execution::par
#include <execution>

// std::unique_ptr
#include <memory>

#include <memory_resource>

// std thread for parallel sorting.
#include <thread>




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
    
    template<PoolPageCapacity PageCapacity, class Alignment>
    class chunk<PoolType::_Scalable, PageCapacity, Alignment>
    {
        FE_STATIC_ASSERT(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    public:
		static FE::size page_capacity_in_bytes = static_cast<FE::size>(PageCapacity);
        static FE::size possible_address_count = (page_capacity_in_bytes / Alignment::size);
        static FE::size free_list_capacity = possible_address_count;

        using free_list_type = block_info[free_list_capacity];
        using free_list_iterator = block_info*;
        using free_list_element = block_info;
        
    private: // DO NOT MEMZERO THIS ARRAY. IT WILL PUT THE COMPILER INTO AN INFINITE COMPLIATION LOOP.
        alignas(Alignment::size) std::array<var::byte, page_capacity_in_bytes> m_memory;

    public:
        alignas(16) block_info _free_list[free_list_capacity];

    private:
		var::boolean m_is_page_heapified = false;
        var::int64 m_free_list_size = 0;

    public:
        var::byte* const _begin = m_memory.data();
        var::byte* _page_iterator = m_memory.data();
        var::byte* const _end = m_memory.data() + m_memory.size();

    public:
        chunk() noexcept = default;
        ~chunk() noexcept = default;

    public:
        _FE_FORCE_INLINE_ FE::boolean is_page_heapified() const noexcept { return this->m_is_page_heapified; }
        _FE_FORCE_INLINE_ void set_page_heapified() noexcept { this->m_is_page_heapified = true; }

        void add_to_the_free_list(const block_info& block_p) noexcept
        {
            FE_NEGATIVE_ASSERT(this->m_free_list_size == free_list_capacity, "Assertion Failure: The free list is full.");
            block_info* const l_position = static_cast<block_info*>(_free_list) + this->m_free_list_size;
#if defined(_FE_ON_X86_64_) && defined(_SSE2_)
            _mm_store_si128( reinterpret_cast<__m128i* const>(l_position), _mm_load_si128( reinterpret_cast<const __m128i*>(&block_p) ) );
#endif
            ++(this->m_free_list_size);
            
            if (this->m_is_page_heapified == true)
            {
				std::push_heap(static_cast<free_list_iterator>(_free_list), static_cast<free_list_iterator>(_free_list) + this->m_free_list_size, internal::pool::less_than{});
            }
        }

		FE::boolean retrieve_from_the_free_list(internal::pool::block_info& out_alloc_result_p, FE::size requested_bytes_p) noexcept
		{
            FE_ASSERT((requested_bytes_p % Alignment::size) == 0, "Critical Error in FE.pool.scalable_pool: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &requested_bytes_p, &Alignment::size);
            FE_ASSERT(this->m_is_page_heapified == true, "Assertion Failure: The page is not binary heapified.");
            
            if (this->m_free_list_size == 0)
            {
                out_alloc_result_p._address = nullptr;
                out_alloc_result_p._size_in_bytes = 0;
                return _FE_FAILED_;
            }

			std::pop_heap(static_cast<free_list_iterator>(_free_list), static_cast<free_list_iterator>(_free_list) + this->m_free_list_size, internal::pool::less_than{});
			--(this->m_free_list_size);
            
            // Try allocation.
            if (static_cast<FE::size>(_free_list[this->m_free_list_size]._size_in_bytes) >= requested_bytes_p)
            {
                out_alloc_result_p._address = _free_list[this->m_free_list_size]._address; 
                out_alloc_result_p._size_in_bytes = requested_bytes_p;
                FE_ASSERT((reinterpret_cast<FE::uintptr>(out_alloc_result_p._address) % Alignment::size) == 0, "FE.pool.scalable_pool has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", out_alloc_result_p._address, &Alignment::size);
                
                _free_list[this->m_free_list_size]._address += requested_bytes_p;
                _free_list[this->m_free_list_size]._size_in_bytes -= requested_bytes_p;

                if (_free_list[this->m_free_list_size]._size_in_bytes > 0)
                {
                    add_to_the_free_list(_free_list[this->m_free_list_size]);
                }
                return _FE_SUCCEEDED_;
            }
            else if (_free_list[this->m_free_list_size]._size_in_bytes > 0) // Failed to find a block that fits the requested size.
            {
                add_to_the_free_list(_free_list[this->m_free_list_size]);
            }

            out_alloc_result_p._address = nullptr;
            out_alloc_result_p._size_in_bytes = 0;
            return _FE_FAILED_; // Try iterate to the next page.
		}

		_FE_FORCE_INLINE_ FE::int64 get_free_list_size() const noexcept { return this->m_free_list_size; }
        _FE_FORCE_INLINE_ void set_free_list_size(FE::int64 size_p) noexcept { this->m_free_list_size = size_p; }

#ifdef _ENABLE_ASSERT_
    private:
        var::int64 m_double_free_tracker[possible_address_count]{};

    public:
        _FE_FORCE_INLINE_ void check_double_allocation(const block_info& block_info_p) noexcept
        {
			FE::uint64 l_idx = (block_info_p._address - _begin) / Alignment::size;
            FE_ASSERT(m_double_free_tracker[l_idx] == 0, "Double allocation detected: cannot alloate the same address twice.");
			this->m_double_free_tracker[l_idx] = static_cast<FE::int64>(block_info_p._size_in_bytes);
        }

        _FE_FORCE_INLINE_ void check_double_free(const block_info& block_info_p) noexcept
		{
            FE::uint64 l_idx = (block_info_p._address - _begin) / Alignment::size;
            FE_ASSERT(static_cast<FE::int64>(m_double_free_tracker[l_idx]) == block_info_p._size_in_bytes, "Double free detected: cannot dealloate the same address twice.");
			this->m_double_free_tracker[l_idx] = 0;
		}
#endif
    };
}




template<PoolPageCapacity PageCapacity, class Alignment>
class pool<PoolType::_Scalable, PageCapacity, Alignment> : public std::pmr::memory_resource
{
    FE_STATIC_ASSERT(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    using chunk_type = internal::pool::chunk<PoolType::_Scalable, PageCapacity, Alignment>;
    using free_list_type = typename chunk_type::free_list_type;
    using free_list_iterator = typename chunk_type::free_list_iterator;

public:
	using alignment_type = Alignment;

    constexpr static FE::size page_capacity = chunk_type::page_capacity_in_bytes;
    constexpr static FE::size possible_address_count = (page_capacity / Alignment::size);
    constexpr static FE::size free_list_capacity = chunk_type::free_list_capacity;

    constexpr static FE::size maximum_page_count = 6;

	constexpr static FE::size auto_defragmentation_denominator = 2;
    constexpr static FE::size auto_defragmentation_point = possible_address_count / auto_defragmentation_denominator;

private:
    using page_pointer = std::shared_ptr<chunk_type>;

    page_pointer m_memory_pool[maximum_page_count];
    std::pmr::memory_resource* m_upstream_resource;
    var::uint32 m_page_count;

public:
    pool() noexcept 
		: m_memory_pool{}, m_upstream_resource(std::pmr::get_default_resource()), m_page_count() {}
    pool(std::pmr::memory_resource* const upstream_resource_p) noexcept 
        : m_memory_pool{}, m_upstream_resource(upstream_resource_p), m_page_count() 
    {
        FE_ASSERT(upstream_resource_p != nullptr, "Assertion failed: the upstream_resource_p must not be a nullptr.");
    }

    virtual ~pool() noexcept override = default;

    _FE_FORCE_INLINE_ pool(pool&& other_p) noexcept
        : m_upstream_resource(other_p.m_upstream_resource), m_page_count(other_p.m_page_count)
	{
		for (var::size i = 0; i < maximum_page_count; ++i)
		{
			this->m_memory_pool[i] = std::move(other_p.m_memory_pool[i]);
		}
        other_p.m_page_count = 0;
	}

    _FE_FORCE_INLINE_ pool& operator=(pool&& other_p) noexcept
    {
        this->m_upstream_resource = other_p.m_upstream_resource;
        this->m_page_count = other_p.m_page_count;
        other_p.m_page_count = 0;

        for (var::size i = 0; i < maximum_page_count; ++i)
        {
            this->m_memory_pool[i] = std::move(other_p.m_memory_pool[i]);
        }
        this->m_page_count = other_p.m_page_count;
        return *this;
    }

	_FE_FORCE_INLINE_ bool operator==(const pool& other_p) const noexcept { return this->m_memory_pool[0].get() == other_p.m_memory_pool[0].get(); }

    pool(const pool&) noexcept = delete;
    pool& operator=(const pool&) noexcept = delete;

    template<typename U>
    U* allocate(FE::uint64 size_p = 1) noexcept
    {
        FE_STATIC_ASSERT(std::is_array<U>::value == false, "Static Assertion Failed: The T must not be an array[] type.");

        FE::size l_queried_allocation_size_in_bytes = FE::calculate_aligned_memory_size_in_bytes<U, Alignment>(size_p);
        FE_EXIT(l_queried_allocation_size_in_bytes > page_capacity, FE::ErrorCode::_FatalMemoryError_1XX_BufferOverflow, "Fatal Error: Unable to allocate ${%lu@0} bytes of memmory that exceeds the pool chunk's capacity.", &size_p);
        FE_ASSERT((l_queried_allocation_size_in_bytes % Alignment::size) == 0, "Critical Error in FE.pool.scalable_pool: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &l_queried_allocation_size_in_bytes, &Alignment::size);

        for (var::size i = 0; i < maximum_page_count; ++i)
        {
            if (this->m_memory_pool[i] == nullptr) _FE_UNLIKELY_
            {
                this->m_memory_pool[i] = std::allocate_shared<chunk_type, std::pmr::polymorphic_allocator<chunk_type>>( (m_upstream_resource == nullptr) ? std::pmr::polymorphic_allocator<chunk_type>() : m_upstream_resource );
                ++this->m_page_count;

				// Swap the new page to the front of the array for faster access.
                std::swap(this->m_memory_pool[0], this->m_memory_pool[i]);
                i = 0;
            }

            internal::pool::block_info l_memblock_info{};
           
            if (__try_allocation_from_page(this->m_memory_pool[i], l_memblock_info, l_queried_allocation_size_in_bytes) == _FE_FAILED_)
            { 
                if (this->m_page_count == maximum_page_count)
                {
                    return nullptr;
                }
                FE_LOG("New memory page has been created for this scalable_pool instance.\nThe instance address: ${%p@0}\nThe number of pages have been allocated for the instance: ${%u32@1}.", this, &m_page_count);
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
            this->m_memory_pool[i]->check_double_allocation(l_memblock_info);
#endif
            FE_ASSERT((reinterpret_cast<FE::uintptr>(l_memblock_info._address) % Alignment::size) == 0, "FE.pool.scalable_pool has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", l_memblock_info._address, &Alignment::size);
            return reinterpret_cast<U*>(l_memblock_info._address);
        }
        
        FE_LOG("Frogman Engine scalable pool warning: the allocation failed because the pool instance is out of its capacity. A nullptr has been returned.");
        return nullptr;
    }

    // Incorrect type will cause a critical runtime error.
    template <typename T> 
    void deallocate(T* pointer_p, FE::uint64 element_count_p) noexcept 
    {
        FE_NEGATIVE_ASSERT(pointer_p == nullptr, "Critical Error in FE.pool.scalable_pool: Unable to deallocate() a nullptr.");
        FE_NEGATIVE_ASSERT(element_count_p == 0, "${%s@0}: ${%s@1} was 0", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(element_count_p));
        FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.pool.scalable_pool: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this scalable_pool instance.", pointer_p, &Alignment::size);
        
        alignas(16) internal::pool::block_info l_block_to_free;
        l_block_to_free._address = reinterpret_cast<var::byte*>(pointer_p);
        l_block_to_free._size_in_bytes = FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(element_count_p);
        
        for (page_pointer& page_ptr : m_memory_pool)
        {
            if (page_ptr == nullptr) _FE_UNLIKELY_
            {
                continue;
            }

            if ((page_ptr->_begin <= l_block_to_free._address) && (l_block_to_free._address < page_ptr->_end))
            {
                if constexpr (FE::is_trivial<T>::value == false)
                {
                    for (var::uint64 i = 0; i < element_count_p; ++i)
                    {
                        pointer_p->~T();
                        ++pointer_p;
                    }
                }
#ifdef _ENABLE_ASSERT_
                page_ptr->check_double_free(l_block_to_free);
#endif
                page_ptr->add_to_the_free_list(l_block_to_free);

                //if constexpr (PageCapacity > PoolPageCapacity::_16KB)
                //{
                //    if (page_ptr->get_free_list_size() >= auto_defragmentation_point)
                //    {
                //        __defragment(page_ptr);
                //    }
                //}
                return;
            }
        }

        FE_EXIT(true, FE::ErrorCode::_FatalMemoryError_1XX_FalseDeallocation, "Critical Error in FE.pool.block_pool: the pointer value '${%p@0}' does not belong to this block_pool instance.", l_block_to_free._address);
    }
    
    _FE_FORCE_INLINE_ void create_pages(size chunk_count_p) noexcept
    {
        FE_ASSERT(this->m_page_count < maximum_page_count, "The pool instance is out of its page table capacity: unable to create new pages for the pool instance.");

        for (; this->m_page_count < chunk_count_p; ++m_page_count)
        {
            this->m_memory_pool[m_page_count] = std::allocate_shared<chunk_type, std::pmr::polymorphic_allocator<chunk_type>>((m_upstream_resource == nullptr) ? std::pmr::polymorphic_allocator<chunk_type>() : m_upstream_resource);
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

            var::size l_unused_memory_size_in_bytes = 0;
            for (auto block : page_ptr->_free_list)
            {
                l_unused_memory_size_in_bytes += block.second;
            }

            if (page_ptr->_page_iterator < page_ptr->_end)
            {
                l_unused_memory_size_in_bytes += page_ptr->_end - page_ptr->_page_iterator;
            }

            if (l_unused_memory_size_in_bytes == page_capacity)
            {
				page_ptr.reset();
            }
        }
    }

	_FE_FORCE_INLINE_ FE::size get_page_count() const noexcept { return this->m_page_count; }

protected:
    inline virtual void* do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p = Alignment::size) noexcept override
    {
		return this->allocate<std::byte>(bytes_p);
    }

	inline virtual void do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p = Alignment::size) noexcept override
	{
		this->deallocate<std::byte>(static_cast<std::byte*>(ptr_p), bytes_p);
	}

    inline virtual bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override
    {
		if (dynamic_cast<const pool*>(&other_p) == nullptr)
		{
			return false;
		}

		return this->operator==(dynamic_cast<const pool&>(other_p));
    }

private:
    /* Time complexity: 
	Best - O(1)
    */
    static FE::boolean __try_allocation_from_page(page_pointer& page_p, internal::pool::block_info& out_result_p, FE::size bytes_p) noexcept
    {
        FE_ASSERT((bytes_p % Alignment::size) == 0, "Critical Error in FE.pool.scalable_pool: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &bytes_p, &Alignment::size);
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
				out_result_p._size_in_bytes = 0;

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

	// Time complexity: O(3n + n log n).
    static void __defragment(page_pointer& page_p) noexcept
    {
		if (2 > page_p->get_free_list_size()) // Nah, it's not worth it.
        {
			return;
        }

        std::sort<std::execution::parallel_unsequenced_policy, free_list_iterator, internal::pool::from_low_address>(std::execution::parallel_unsequenced_policy{},
            static_cast<free_list_iterator>(page_p->_free_list),
            static_cast<free_list_iterator>(page_p->_free_list) + page_p->get_free_list_size(),
            internal::pool::from_low_address{});

		// Merge the free list.
		free_list_iterator l_iterator = static_cast<free_list_iterator>(page_p->_free_list);
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
                l_next->_address = nullptr;
                l_next->_size_in_bytes = 0;
				++l_next; // Look for the next block.
            }
            else
            {
				// Move to the next block if they are not adjacent.
                l_iterator = l_next;
            }
        }

		// Migrate null blocks to right-side of the array to exclude them from being binary searched.
        /* Time complexity: 
        Best - O(n/2)
		Worst - O(n)
        */
        auto l_binary_searchable_range = algorithm::utility::exclude<algorithm::utility::IsolationVector::_Right, free_list_iterator>(static_cast<free_list_iterator>(page_p->_free_list),
                                                                                                                                     l_end, internal::pool::block_info{ nullptr, 0 });
        // Reset it.
        page_p->set_free_list_size(l_binary_searchable_range._second - l_binary_searchable_range._first);

		// Heapify the free list. Time complexity: O(n)
		std::make_heap(l_binary_searchable_range._first, l_binary_searchable_range._second, internal::pool::less_than{}); // To do: consider parallelizing the heapification.

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
template<PoolPageCapacity PageCapacity, class Alignment = FE::SIMD_auto_alignment>
using scalable_pool = pool<PoolType::_Scalable, PageCapacity, Alignment>;


END_NAMESPACE
#endif