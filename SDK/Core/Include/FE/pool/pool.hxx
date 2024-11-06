#ifndef _FE_CORE_POOL_HXX_
#define _FE_CORE_POOL_HXX_
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
#include <FE/iterator.hxx>
#include <FE/memory.hxx>

#ifdef _FE_ON_X86_64_
    #ifdef _SSE2_
        #include <emmintrin.h>
    #else
        #error "SSE2 is required for the x86-64 version of Frogman Engine's memory pool."
    #endif
#endif

#if defined(_ENABLE_ASSERT_) || defined(_ENABLE_NEGATIVE_ASSERT_)
#include <robin_hood.h> // a hash map for checking double free.
#endif

// std::sort()
#include <algorithm>

// std thread for parallel sorting.
#include <thread>




BEGIN_NAMESPACE(FE)


namespace internal::pool
{
    class from_low_address
    {
    public:
        bool operator()(const block_info& lhs_p, const block_info& rhs_p) noexcept
		{
			return lhs_p._address < rhs_p._address;
		}
    };

    class less_than
    {
    public:
        bool operator()(const block_info& lhs_p, const block_info& rhs_p) noexcept
        {
            return lhs_p._size_in_bytes < rhs_p._size_in_bytes;
        }
    };

	class greater_than
    {
    public:
        bool operator()(const block_info& lhs_p, const block_info& rhs_p) noexcept
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
    
    template<FE::size PageCapacity, class Alignment>
    class chunk<POOL_TYPE::_SCALABLE, PageCapacity, Alignment>
    {
        FE_STATIC_ASSERT(PageCapacity >= 32, "Static Assertion Failure: The PageCapacity is too small.");
        FE_STATIC_ASSERT((PageCapacity % Alignment::size) == 0, "Static Assertion Failure: The PageCapacity must be a multiple of Alignment::size.");
        FE_STATIC_ASSERT(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    public:
        constexpr static count_t possible_address_count = (PageCapacity / Alignment::size);
        constexpr static count_t free_list_capacity = possible_address_count;

        using free_list_type = block_info[free_list_capacity];
        using free_list_iterator = block_info*;
        using free_list_element = block_info;
        
    private:
        alignas(Alignment::size) std::array<var::byte, PageCapacity> m_memory{ 0 };
        /*
         std::pair's first contains the address of the memory block.
         std::pair's second contains the size of the memory block.
        */
    public:
        alignas(16) free_list_type _free_list{ };

    private:
		var::boolean m_is_page_binary_searchable = false;
        var::int64 m_free_list_size = 0;

    public:
        var::byte* const _begin = m_memory.data();
        var::byte* _page_iterator = m_memory.data();
        var::byte* const _end = m_memory.data() + m_memory.size();

    public:

        _FE_FORCE_INLINE_ FE::boolean is_page_binary_searchable() const noexcept { return this->m_is_page_binary_searchable; }
        _FE_FORCE_INLINE_ void set_page_binary_searchable() noexcept { this->m_is_page_binary_searchable = true; }

        _FE_FORCE_INLINE_ void add_to_the_free_list(const block_info& block_p) noexcept
        {
            FE_NEGATIVE_ASSERT(this->m_free_list_size == free_list_capacity, "Assertion Failure: The free list is full.");
            block_info* const l_position = static_cast<block_info*>(_free_list) + this->m_free_list_size;
#if defined(_FE_ON_X86_64_) && defined(_SSE2_)
            _mm_store_si128( reinterpret_cast<__m128i* const>(l_position), _mm_load_si128( reinterpret_cast<const __m128i*>(&block_p) ) );
#endif
            ++(this->m_free_list_size);
            
            if (this->m_is_page_binary_searchable == true)
            {
				std::push_heap(static_cast<free_list_iterator>(_free_list), static_cast<free_list_iterator>(_free_list) + this->m_free_list_size, internal::pool::less_than{});
            }
        }

		FE::boolean retrieve_from_the_free_list(internal::pool::block_info& out_alloc_result_p, FE::size requested_bytes_p) noexcept
		{
            FE_ASSERT((requested_bytes_p % Alignment::size) == 0, "Critical Error in FE.pool.scalable_pool: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &requested_bytes_p, &Alignment::size);
            FE_ASSERT(this->m_is_page_binary_searchable == true, "Assertion Failure: The page is not binary searchable.");
            
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
    };
}




template<FE::size PageCapacity, class Alignment, class Allocator>
class pool<POOL_TYPE::_SCALABLE, PageCapacity, Alignment, Allocator>
{
    FE_STATIC_ASSERT(PageCapacity >= 32, "Static Assertion Failure: The PageCapacity is too small.");
    FE_STATIC_ASSERT((PageCapacity % Alignment::size) == 0, "Static Assertion Failure: The PageCapacity must be a multiple of Alignment::size.");
    FE_STATIC_ASSERT(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    using chunk_type = internal::pool::chunk<POOL_TYPE::_SCALABLE, PageCapacity, Alignment>;
    FE_NEGATIVE_STATIC_ASSERT((std::is_same<chunk_type, typename Allocator::value_type>::value == false), "Static Assertion Failed: The chunk_type has to be equivalent to Allocator::value_type.");
    
    using free_list_type = typename chunk_type::free_list_type;
    using free_list_iterator = typename chunk_type::free_list_iterator;
    using pool_type = std::list<chunk_type, Allocator>;
	using pool_iterator = typename pool_type::iterator;

public:
	using alignment_type = Alignment;

    constexpr static size page_capacity = PageCapacity;
    constexpr static count_t possible_address_count = (PageCapacity / Alignment::size);
    constexpr static count_t free_list_capacity = chunk_type::free_list_capacity;
    constexpr static count_t maximum_list_node_count = 3;

private:
    pool_type m_memory_pool;

public:
    pool() noexcept = default;
    ~pool() noexcept = default;

    pool(const pool&) noexcept = delete;
    pool(pool&& rvalue_p) noexcept : m_memory_pool( std::move(rvalue_p.m_memory_pool) ) {}

    pool& operator=(const pool&) noexcept = delete;
    pool& operator=(pool&&) noexcept = delete;

    template<typename U>
    U* allocate(FE::count_t size_p = 1) noexcept
    {
        FE_NEGATIVE_STATIC_ASSERT((Alignment::size % 2) != 0, "Static Assertion Failed: The Alignment::size must be an even number.");
        FE_STATIC_ASSERT(std::is_array<U>::value == false, "Static Assertion Failed: The T must not be an array[] type.");

        FE::size l_queried_allocation_size_in_bytes = FE::calculate_aligned_memory_size_in_bytes<U, Alignment>(size_p);
        FE_EXIT(l_queried_allocation_size_in_bytes > PageCapacity, FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_BUFFER_OVERFLOW, "Fatal Error: Unable to allocate ${%lu@0} bytes of memmory that exceeds the pool chunk's capacity.", &size_p);
        FE_ASSERT((l_queried_allocation_size_in_bytes % Alignment::size) == 0, "Critical Error in FE.pool.scalable_pool: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &l_queried_allocation_size_in_bytes, &Alignment::size);

        for (typename pool_type::iterator iterator = this->m_memory_pool.begin(); iterator != this->m_memory_pool.cend(); ++iterator)
        {
            internal::pool::block_info l_memblock_info{};
           
            if (__try_allocation_from_page(iterator, l_memblock_info, l_queried_allocation_size_in_bytes) == _FE_FAILED_)
            { 
#ifdef _ENABLE_LOG_
				//::int32 l_page_count = this->m_memory_pool.size();
#endif 
                //FE_LOG("New memory page has been created for this scalable_pool instance.\nThe instance address: ${%p@0}\nThe number of pages have been allocated for the instance: ${%d@1}.", this, &l_page_count);
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

            FE_ASSERT((reinterpret_cast<FE::uintptr>(l_memblock_info._address) % Alignment::size) == 0, "FE.pool.scalable_pool has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", l_memblock_info._address, &Alignment::size);
            return reinterpret_cast<U*>(l_memblock_info._address);
        }
        //FE_LOG("Warning: failed to allocate memory from this FE.pool.scalable_pool instance located at '${%p@0}'.", this);
		create_pages(1); // Failed to find a page that fits the requested size; allocate a new page.
        return allocate<U>(size_p); // Retry the allocation.
    }

    // Incorrect type will cause a critical runtime error.
    template <typename T> 
    void deallocate(T* pointer_p, FE::count_t element_count_p) noexcept 
    {
        FE_NEGATIVE_ASSERT(pointer_p == nullptr, "Critical Error in FE.pool.scalable_pool: Unable to deallocate() a nullptr.");
        FE_NEGATIVE_ASSERT(element_count_p == 0, "${%s@0}: ${%s@1} was 0", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(element_count_p));
        FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.pool.scalable_pool: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this scalable_pool instance.", pointer_p, &Alignment::size);
        
        alignas(16) internal::pool::block_info l_block_to_free;
        l_block_to_free._address = reinterpret_cast<var::byte*>(pointer_p);
        l_block_to_free._size_in_bytes = FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(element_count_p);
        
        for (typename pool_type::iterator iterator = this->m_memory_pool.begin(); iterator != this->m_memory_pool.cend(); ++iterator)
        {
            if ((iterator->_begin <= reinterpret_cast<var::byte*>(pointer_p)) && (reinterpret_cast<var::byte*>(pointer_p) < iterator->_end))
            {
                if constexpr (FE::is_trivial<T>::value == false)
                {
                    for (var::count_t i = 0; i < element_count_p; ++i)
                    {
                        pointer_p->~T();
                        ++pointer_p;
                    }
                }

                iterator->add_to_the_free_list(l_block_to_free);
                return;
            }
        }
    }

    _FE_FORCE_INLINE_ void create_pages(size chunk_count_p) noexcept
    {
        FE_NEGATIVE_ASSERT(chunk_count_p == 0, "${%s@0}: ${%s@1} was 0", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_INVALID_SIZE), TO_STRING(chunk_count_p));

        for (var::size i = 0; i < chunk_count_p; ++i)
        {
            this->m_memory_pool.emplace_back();

            FE_ASSERT((reinterpret_cast<FE::uintptr>(this->m_memory_pool.back()._begin) % Alignment::size) == 0, "Critical Error in FE.pool.scalable_pool: the memory page address '${%p@0}' is not aligned by ${%lu@1}.", this->m_memory_pool.back()._begin, &Alignment::size);
        }

        FE_NEGATIVE_ASSERT(this->m_memory_pool.size() == maximum_list_node_count, "Maximum chunk count of a memory chunk list is limited to ${%lu@0} for some performance reasons.", &maximum_list_node_count);
    }

    FE::boolean shrink_to_fit() noexcept
    {
        typename pool_type::iterator  l_list_iterator = this->m_memory_pool.begin();
        typename pool_type::const_iterator l_cend = this->m_memory_pool.cend();

        if (l_list_iterator == l_cend)
        {
            FE_LOG("Unable to shrink_to_fit() an empty pool.");
            return false;
        }

        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            var::size l_unused_memory_size_in_bytes = 0;
            for (auto block : l_list_iterator->_free_list)
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

private:
    /* Time complexity: 
	Best - O(1)
    */
    static FE::boolean __try_allocation_from_page(pool_iterator page_p, internal::pool::block_info& out_result_p, FE::size bytes_p) noexcept
    {
        FE_ASSERT((bytes_p % Alignment::size) == 0, "Critical Error in FE.pool.scalable_pool: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &bytes_p, &Alignment::size);
        if (page_p->is_page_binary_searchable() == true)
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
    static void __defragment(pool_iterator page_p) noexcept
    {
        if (2 > page_p->get_free_list_size())
        {
			return;
        }

		// Sort the array to merge the adjacent blocks. Time complexity: O(n log n)
        if (page_p->get_free_list_size() > 512)
        {
            index_t l_mid = page_p->get_free_list_size() >> 1;
            std::jthread l_thread = std::jthread(
                [=]()
                {
                    std::sort<free_list_iterator, internal::pool::from_low_address>(
                        static_cast<free_list_iterator>(page_p->_free_list) + l_mid,
                        static_cast<free_list_iterator>(page_p->_free_list) + page_p->get_free_list_size(),
                        internal::pool::from_low_address{});
                }
            );

            std::sort<free_list_iterator, internal::pool::from_low_address>(
                static_cast<free_list_iterator>(page_p->_free_list),
                static_cast<free_list_iterator>(page_p->_free_list) + l_mid,
                internal::pool::from_low_address{});
        }

        std::sort<free_list_iterator, internal::pool::from_low_address>(
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
        auto l_binary_searchable_range = algorithm::utility::exclusion_sort<algorithm::utility::EXCLUSION_SORT_MODE::_PUSH_TO_RIGHT, free_list_iterator>(static_cast<free_list_iterator>(page_p->_free_list),
                                                                                                                                     l_end, internal::pool::block_info{ nullptr, 0 });
        // Reset it.
        page_p->set_free_list_size(l_binary_searchable_range._second - l_binary_searchable_range._first);

		// Heapify the free list. Time complexity: O(n)
		std::make_heap(l_binary_searchable_range._first, l_binary_searchable_range._second, internal::pool::less_than{});

		page_p->set_page_binary_searchable(); // Switch the allocation strategy to binary search.
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
*/
template<FE::size PageCapacity = 8192, class Alignment = FE::SIMD_auto_alignment, class Allocator = FE::aligned_allocator<internal::pool::chunk<POOL_TYPE::_SCALABLE, PageCapacity, Alignment>, Alignment>>
using scalable_pool = pool<POOL_TYPE::_SCALABLE, PageCapacity, Alignment, Allocator>;


END_NAMESPACE
#endif