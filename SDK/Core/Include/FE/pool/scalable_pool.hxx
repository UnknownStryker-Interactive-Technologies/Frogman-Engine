#ifndef _FE_CORE_SCALABLE_POOL_HXX_
#define _FE_CORE_SCALABLE_POOL_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
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
        var::uint32 _remaining_capacity_in_bytes;
        var::byte* _page_iterator;

    public:
        chunk() noexcept
            :   m_free_list_size(0),
                m_is_page_heapified(false),
                _usage_in_bytes(0),
                _remaining_capacity_in_bytes(page_size_in_bytes)

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            ,   m_double_free_tracker()
#endif
        {
            _page_iterator = static_cast<var::byte*>(m_page);
            m_page_end = _page_iterator + page_size_in_bytes;

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            m_double_free_tracker = (var::uint32*)FE_ALIGNED_ALLOC(possible_address_count * sizeof(var::uint32), FE::CPU_L1_cache_line::size);
			FE_EXIT_IF(m_double_free_tracker == nullptr, FE::ErrorCode::_FatalMemoryError_1XX_AllocationFailure, "Fatal Error: Unable to FE_ALIGNED_ALLOC double-free tracker for a scalable_allocator.");
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
            FE_ASSERT((requested_bytes_p % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &requested_bytes_p, &Alignment::size);

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
                FE_ASSERT((reinterpret_cast<FE::uintptr>(out_alloc_result_p._address) % Alignment::size) == 0, "FE.Core.scalable_allocator has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", out_alloc_result_p._address, &Alignment::size);
                
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

    _FE_FORCE_INLINE_ PageListClass __select_page_list_by_capacity(FE::uint32 allocation_request_in_bytes_p) noexcept
    {
        FE_ASSERT(allocation_request_in_bytes_p <= FE::system_page_size);
        constexpr FE::uint32 l_divider = 256; // 256 bytes
        FE::uint32 l_quotient = allocation_request_in_bytes_p / l_divider;
        /*
        * 
		* l_quotient == 0 if 0~255 bytes are requested, fits within the pages with 6.25% capacity remaining.
		* l_quotient == 1 if 256~511 bytes are requested
		* l_quotient == 2 if 512~767 bytes are requested, 
		* l_quotient == 3 if 768~1023 bytes are requested
		* l_quotient == 4 if 1024~1279 bytes are requested, 
		* l_quotient == 5 if 1280~1535 bytes are requested
		* l_quotient == 6 if 1536~1791 bytes are requested
		* l_quotient == 7 if 1792~2047 bytes are requested
		* l_quotient == 8 if 2048~2303 bytes are requested, 
		* l_quotient == 9 if 2304~2559 bytes are requested
		* l_quotient == 10 if 2560~2815 bytes are requested
		* l_quotient == 11 if 2816~3071 bytes are requested
		* l_quotient == 12 if 3072~3327 bytes are requested
		* l_quotient == 13 if 3328~3583 bytes are requested
		* l_quotient == 14 if 3584~3839 bytes are requested
		* l_quotient == 15 if 3840~4095 bytes are requested
		* l_quotient == 16 if 4096 bytes are requested, 
        */

        if (allocation_request_in_bytes_p == 0)
        {
			PageListClass::_Unavailable;
        }

        switch (l_quotient)
        {
        case 0:
			return PageListClass::_6_25_PercentRemaining;


        case 1:
			return PageListClass::_12_5_Percent;


		case 2:
            _FE_FALLTHROUGH_;
		case 3:
			return PageListClass::_25_Percent;
        

        case 4:
		    _FE_FALLTHROUGH_;
		case 5:
            _FE_FALLTHROUGH_;
        case 6:
			_FE_FALLTHROUGH_;
		case 7:
			return PageListClass::_50_Percent;


		case 8:
			_FE_FALLTHROUGH_;
		case 9:
			_FE_FALLTHROUGH_;
		case 10:
			_FE_FALLTHROUGH_;
		case 11:
            return PageListClass::_75_Percent;


		case 12:
			_FE_FALLTHROUGH_;
		case 13:
			_FE_FALLTHROUGH_;
		case 14:
			_FE_FALLTHROUGH_;
		case 15:
            _FE_FALLTHROUGH_;
		case 16:
			return PageListClass::_100_Percent;

            _FE_NODEFAULT_;
        }
    }
}




template<class Alignment>
class pool<PoolType::_Scalable, Alignment>
{
    static_assert(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    using chunk_type = internal::pool::chunk<PoolType::_Scalable, Alignment>;
    using free_list_iterator = typename chunk_type::free_list_iterator;

public:
    constexpr static FE::uint32 page_capacity = chunk_type::page_size_in_bytes;
	using alignment_type = Alignment;
    
private:
    using pool_type = FE::list<chunk_type, FE::page_aligned_allocator<chunk_type>>;
	using page_iterator = typename pool_type::iterator;

	pool_type m_unavailable_pages;       // 
	pool_type m_pages_with_6_25_capacity;// ~256B
	pool_type m_pages_with_12_5_capacity;// ~512B
    pool_type m_pages_with_25_capacity;  // ~1KiB
	pool_type m_pages_with_50_capacity;  // ~2KiB
    pool_type m_pages_with_75_capacity;  // ~3KiB
    pool_type m_pages_with_100_capacity; // ~4KiB

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
    using page_validation_table = absl::flat_hash_set<chunk_type*,
        typename absl::lts_20260107::container_internal::FlatHashSetPolicy<chunk_type*>::DefaultHash, std::equal_to<chunk_type*>,
        FE::cache_aligned_allocator<chunk_type*>>;
    page_validation_table m_page_validation_table;
#endif

public:
    pool() noexcept
        :   m_unavailable_pages(),
            m_pages_with_6_25_capacity(),
		    m_pages_with_12_5_capacity(),
		    m_pages_with_25_capacity(),
		    m_pages_with_50_capacity(),
            m_pages_with_75_capacity(),
            m_pages_with_100_capacity()
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
		,   m_page_validation_table()
#endif
    {
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        m_page_validation_table.reserve(512);
#endif
        m_pages_with_100_capacity.emplace_front();
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        m_page_validation_table.insert(&m_pages_with_100_capacity.front());
#endif
    }

     ~pool() noexcept  = default;

    pool(pool&& other_p) noexcept
        :   m_unavailable_pages( std::move(other_p.m_unavailable_pages) ),
            m_pages_with_6_25_capacity( std::move(other_p.m_pages_with_6_25_capacity) ),
            m_pages_with_12_5_capacity( std::move(other_p.m_pages_with_12_5_capacity) ),
            m_pages_with_25_capacity( std::move(other_p.m_pages_with_25_capacity) ),
            m_pages_with_50_capacity( std::move(other_p.m_pages_with_50_capacity) ),
            m_pages_with_75_capacity( std::move(other_p.m_pages_with_75_capacity) ),
            m_pages_with_100_capacity( std::move(other_p.m_pages_with_100_capacity) )
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

        m_unavailable_pages = std::move(other_p.m_unavailable_pages);
        m_pages_with_6_25_capacity = std::move(other_p.m_pages_with_6_25_capacity);
        m_pages_with_12_5_capacity = std::move(other_p.m_pages_with_12_5_capacity);
        m_pages_with_25_capacity = std::move(other_p.m_pages_with_25_capacity);
        m_pages_with_50_capacity = std::move(other_p.m_pages_with_50_capacity);
        m_pages_with_75_capacity = std::move(other_p.m_pages_with_75_capacity);
        m_pages_with_100_capacity = std::move(other_p.m_pages_with_100_capacity);
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
        FE::uint32 l_queried_allocation_in_bytes = (FE::uint32)FE::calculate_aligned_memory_size_in_bytes<U, Alignment>(size_p);
        FE_ASSERT((l_queried_allocation_in_bytes % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the requested allocation size '${%d@0}' is not properly aligned by ${%lu@1}.", &l_queried_allocation_in_bytes, &Alignment::size);
        FE_ASSERT(l_queried_allocation_in_bytes <= page_capacity, "Fatal Error: Unable to allocate ${%d0} bytes of memory that exceeds the pool chunk's capacity.", &size_p);
        FE_ASSERT(m_pages_with_100_capacity.end() == nullptr, "Assertion failed: FE::list::end must return an iterator equivalent to a null pointer.");

		typename pool_type::iterator l_page_list_iterator;
        internal::pool::PageListClass l_required_page_class = internal::pool::__select_page_list_by_capacity(l_queried_allocation_in_bytes);
        do
        {
            switch (l_required_page_class)
            {
            case internal::pool::PageListClass::_6_25_PercentRemaining:
                l_page_list_iterator = m_pages_with_6_25_capacity.begin();
                if (l_page_list_iterator == m_pages_with_6_25_capacity.end())
                {
                    l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                    continue;
                }
                break;

            case internal::pool::PageListClass::_12_5_Percent:
                l_page_list_iterator = m_pages_with_12_5_capacity.begin();
                if (l_page_list_iterator == m_pages_with_12_5_capacity.end())
                {
                    l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                    continue;
                }
                break;

            case internal::pool::PageListClass::_25_Percent:
                l_page_list_iterator = m_pages_with_25_capacity.begin();
                if (l_page_list_iterator == m_pages_with_25_capacity.end())
                {
                    l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                    continue;
                }
                break;

            case internal::pool::PageListClass::_50_Percent:
                l_page_list_iterator = m_pages_with_50_capacity.begin();
                if (l_page_list_iterator == m_pages_with_50_capacity.end())
                {
                    l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                    continue;
                }
                break;

            case internal::pool::PageListClass::_75_Percent:
                l_page_list_iterator = m_pages_with_75_capacity.begin();
                if (l_page_list_iterator == m_pages_with_75_capacity.end())
                {
                    l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                    continue;
                }
                break;

            case internal::pool::PageListClass::_100_Percent:
                l_page_list_iterator = m_pages_with_100_capacity.begin();
                if (l_page_list_iterator == m_pages_with_100_capacity.end())
                {
                    m_pages_with_100_capacity.emplace_front();
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
                    m_page_validation_table.insert(&m_pages_with_100_capacity.front());
#endif
                    l_page_list_iterator = m_pages_with_100_capacity.begin();
                }
                break;

            case internal::pool::PageListClass::_Unavailable:
                return nullptr;

                _FE_NODEFAULT_;
            }

            if (l_page_list_iterator->_remaining_capacity_in_bytes < l_queried_allocation_in_bytes)
            {
                if (l_required_page_class != internal::pool::PageListClass::_100_Percent)
                {
                    l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                    l_page_list_iterator = m_pages_with_100_capacity.end();
                    continue;
                }

                m_pages_with_100_capacity.emplace_front();
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
                m_page_validation_table.insert(&m_pages_with_100_capacity.front());
#endif
                l_page_list_iterator = m_pages_with_100_capacity.begin();
                break;
            }
        } 
        while (l_page_list_iterator == m_pages_with_100_capacity.end());


        alignas(16) internal::pool::block_info l_memblock_info{};
        var::boolean l_was_allocation_successful = __try_allocation_from_page(l_page_list_iterator, l_memblock_info, l_queried_allocation_in_bytes);
        while (l_was_allocation_successful == false)
        {
            ++l_page_list_iterator;
            if (l_page_list_iterator == m_pages_with_100_capacity.end())
            {
                m_pages_with_100_capacity.emplace_front();
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
                m_page_validation_table.insert(&m_pages_with_100_capacity.front());
#endif
                l_page_list_iterator = m_pages_with_100_capacity.begin();
            }
            l_was_allocation_successful = __try_allocation_from_page(l_page_list_iterator, l_memblock_info, l_queried_allocation_in_bytes);
        }


#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        l_page_list_iterator->check_double_allocation(l_memblock_info);
#endif
        if constexpr (FE::is_trivial_v<U> == false)
        {
            U* const l_end = reinterpret_cast<U*>(l_memblock_info._address) + size_p;
            for (U* it = reinterpret_cast<U*>(l_memblock_info._address); it != l_end; ++it)
            {
                new(it) U();
            }
        }

        FE_ASSERT((reinterpret_cast<FE::uintptr>(l_memblock_info._address) % Alignment::size) == 0, "FE.Core.scalable_allocator has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", l_memblock_info._address, &Alignment::size);
        l_page_list_iterator->_usage_in_bytes += l_memblock_info._size_in_bytes;


		pool_type* l_previous_page_list = nullptr;
        internal::pool::PageListClass l_previous_class = internal::pool::__select_page_list_by_capacity(l_page_list_iterator->_remaining_capacity_in_bytes);
        switch (l_previous_class) // class before allocation
        {
        case internal::pool::PageListClass::_6_25_PercentRemaining:
			l_previous_page_list = &m_pages_with_6_25_capacity;
            break;

        case internal::pool::PageListClass::_12_5_Percent:
			l_previous_page_list = &m_pages_with_12_5_capacity;
            break;

        case internal::pool::PageListClass::_25_Percent:
			l_previous_page_list = &m_pages_with_25_capacity;
            break;

        case internal::pool::PageListClass::_50_Percent:
			l_previous_page_list = &m_pages_with_50_capacity;
            break;

        case internal::pool::PageListClass::_75_Percent:
			l_previous_page_list = &m_pages_with_75_capacity;
            break;

		case internal::pool::PageListClass::_100_Percent:
            l_previous_page_list = &m_pages_with_100_capacity;
			break;

		_FE_NODEFAULT_;
        }
		FE_ASSERT(l_previous_page_list != nullptr, "Assertion Failure: The previous page list cannot be null.");

        l_page_list_iterator->_remaining_capacity_in_bytes -= l_memblock_info._size_in_bytes;

        internal::pool::PageListClass l_new_class = internal::pool::__select_page_list_by_capacity(l_page_list_iterator->_remaining_capacity_in_bytes);
		if (l_new_class != l_previous_class) // if the page has changed its class after allocation, move it to the corresponding list.
        {
            switch (l_new_class) // new class after allocation
            {
            case internal::pool::PageListClass::_Unavailable:
                m_unavailable_pages.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_unavailable_pages.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                break;

            case internal::pool::PageListClass::_6_25_PercentRemaining:
                m_pages_with_6_25_capacity.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_6_25_capacity.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                break;

            case internal::pool::PageListClass::_12_5_Percent:
                m_pages_with_12_5_capacity.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_12_5_capacity.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                break;

            case internal::pool::PageListClass::_25_Percent:
                m_pages_with_25_capacity.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_25_capacity.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                break;

            case internal::pool::PageListClass::_50_Percent:
                m_pages_with_50_capacity.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_50_capacity.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                break;

            case internal::pool::PageListClass::_75_Percent:
                m_pages_with_75_capacity.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_75_capacity.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                break;

            default:
                break;
            }
        }
        
        return reinterpret_cast<U*>(l_memblock_info._address);
    }

    // Incorrect type will cause a critical runtime error.
    template <typename T> 
    void _FE_VECTOR_CALL_ deallocate(T* pointer_p, FE::size count_p) noexcept
    {
        FE_ASSERT(pointer_p != nullptr, "Critical Error in FE.Core.scalable_allocator: Unable to deallocate() a nullptr.");
        FE_ASSERT(count_p > 0, "${%s@0}: ${%s@1} was 0", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(count_p));
        FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this scalable_allocator instance.", pointer_p, &Alignment::size);

        alignas(16) internal::pool::block_info l_block_to_free;
        l_block_to_free._address = reinterpret_cast<var::byte*>(pointer_p);
        l_block_to_free._size_in_bytes = (FE::uint32)FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(count_p);
        FE_ASSERT(l_block_to_free._size_in_bytes <= page_capacity, "Critical Error in FE.Core.scalable_allocator: the pointer '${%p@0}' does not belong to this scalable_allocator instance located at '${%p@1}'.", pointer_p, this);

        // use a pointer arithmetic to find which page the pointer belongs to.
        chunk_type* l_page_base = reinterpret_cast<chunk_type*>( (FE::uintptr)l_block_to_free._address - ( ((FE::uintptr)(l_block_to_free._address)) % page_capacity) );

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        FE_EXIT_IF(m_page_validation_table.find(l_page_base) == m_page_validation_table.end(), ErrorCode::_FatalMemoryError_1XX_FalseDeallocation, "Critical Error in FE.Core.scalable_allocator: the pointer '${%p@0}' does not belong to this scalable_allocator instance located at '${%p@1}'.", pointer_p, this);
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
        l_page_base->_usage_in_bytes -= l_block_to_free._size_in_bytes;
        FE_ASSERT(l_page_base->_usage_in_bytes >= 0, "Critical Error in FE.Core.scalable_allocator: the internal usage counter has gone negative. Memory corruption might have occurred.");


        pool_type* l_previous_page_list = nullptr;
        internal::pool::PageListClass l_previous_class = internal::pool::__select_page_list_by_capacity(l_page_base->_remaining_capacity_in_bytes);
        switch (l_previous_class) // class before deallocation
        {
        case internal::pool::PageListClass::_Unavailable:
            l_previous_page_list = &m_unavailable_pages;
            break;

        case internal::pool::PageListClass::_6_25_PercentRemaining:
            l_previous_page_list = &m_pages_with_6_25_capacity;
            break;

        case internal::pool::PageListClass::_12_5_Percent:
            l_previous_page_list = &m_pages_with_12_5_capacity;
            break;

        case internal::pool::PageListClass::_25_Percent:
            l_previous_page_list = &m_pages_with_25_capacity;
            break;

        case internal::pool::PageListClass::_50_Percent:
            l_previous_page_list = &m_pages_with_50_capacity;
            break;

        case internal::pool::PageListClass::_75_Percent:
            l_previous_page_list = &m_pages_with_75_capacity;
            break;

		case internal::pool::PageListClass::_100_Percent:
			break;

        _FE_NODEFAULT_;
        }


        // After deallocation, the page might have more remaining capacity and thus change its class. Move it to the corresponding list if it has changed its class.
        l_page_base->_remaining_capacity_in_bytes += l_block_to_free._size_in_bytes;
        typename pool_type::const_iterator l_page_list_iterator = FE::iterator_cast<typename pool_type::const_iterator>((typename pool_type::const_iterator::wrapped_iterator_type::pointer)l_page_base);


        internal::pool::PageListClass l_new_class = internal::pool::__select_page_list_by_capacity(l_page_base->_remaining_capacity_in_bytes);
        if (l_new_class != l_previous_class) // if the page has changed its class after allocation, move it to the corresponding list.
        {
            switch (l_new_class) // new class after deallocation
            {
            case internal::pool::PageListClass::_Unavailable:
                FE_ASSERT(false);
                return;

            case internal::pool::PageListClass::_6_25_PercentRemaining:
                m_pages_with_6_25_capacity.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_6_25_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                return;

            case internal::pool::PageListClass::_12_5_Percent:
                m_pages_with_12_5_capacity.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_12_5_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                return;

            case internal::pool::PageListClass::_25_Percent:
                m_pages_with_25_capacity.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_25_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                return;

            case internal::pool::PageListClass::_50_Percent:
                m_pages_with_50_capacity.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_50_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                return;

            case internal::pool::PageListClass::_75_Percent:
                m_pages_with_75_capacity.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_75_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                return;

            case internal::pool::PageListClass::_100_Percent:
                m_pages_with_100_capacity.splice( FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_100_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                break;

            default:
                return;
            }
        }

    }


    bool try_trim_a_page() noexcept
    {
        for (typename pool_type::iterator page = m_pages_with_100_capacity.begin(); page != m_pages_with_100_capacity.end(); ++page)
        {
            if (page->_usage_in_bytes == 0)
            {
				m_pages_with_100_capacity->erase(page);
                return true;
			}
		}
        return false;
    }

    void try_trim_all_pages() noexcept
    {
        for (typename pool_type::iterator page = m_pages_with_100_capacity.begin(); page != m_pages_with_100_capacity.end();)
        {
            if (page->_usage_in_bytes == 0)
            {
                page = m_pages_with_100_capacity->erase(page);
                continue;
            }
            ++page;
        }
    }

	_FE_FORCE_INLINE_ FE::size get_page_count() const noexcept { return m_pages_with_100_capacity.size(); }

private:
    /* Time complexity: 
	Allocation from a stack: O(1)
	Allocation from a heap tree: O(2 log n)
	Defragmentation: O(5n + n log n) [rarely gets called]

	Best: O(1)
	Worst: O(5n + n log n) + O(2 log n)
    */
    static FE::boolean _FE_VECTOR_CALL_ __try_allocation_from_page(page_iterator page_p, internal::pool::block_info& out_result_p, FE::uint32 bytes_p) noexcept
    {
        FE_ASSERT((bytes_p % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &bytes_p, &Alignment::size);
        if (page_p->is_page_heapified() == true)
        {
			if(page_p->retrieve_from_the_free_list(out_result_p, bytes_p) == _FE_FAILED_)
            {
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

	// Try reducing the pointer offset data to uint32 to save memory, since the page size is no larger than 2MiB.
    static void _FE_VECTOR_CALL_ __defragment(page_iterator page_p) noexcept
    {
        if (page_p->get_free_list_size() <= 1) _FE_UNLIKELY_
        {
            return;
        }
        std::sort<free_list_iterator, internal::pool::from_low_address>(page_p->get_free_list(),
                                                                        page_p->get_free_list() + page_p->get_free_list_size(),
                                                                        internal::pool::from_low_address{}
        );
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
        l_end = std::partition(page_p->get_free_list(), l_end,
            [](const internal::pool::block_info& block_p)
            {
                return block_p._address != nullptr;
            }
        );
        // Reset it.
        page_p->set_free_list_size( static_cast<FE::uint32>( l_end - page_p->get_free_list() ) );

		// Heapify the free list. Time complexity: O(3n)
		std::make_heap(page_p->get_free_list(), l_end, internal::pool::less_than{});

		page_p->set_page_heapified(); // Switch the allocation strategy to binary search.
    }
};


template<class Alignment = FE::SIMD_auto_alignment>
using scalable_allocator = pool<PoolType::_Scalable, Alignment>;




namespace internal::large::pool
{
    template<class Alignment>
    class FE::internal::pool::chunk<PoolType::_ScalableLargePage, Alignment>
    {
        static_assert(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    public:
        using free_list_iterator = block_info*;
        using free_list_element = block_info;

        constexpr static FE::uint32 page_size_in_bytes = 2 * FE::one_MiB;
        constexpr static FE::uint32 possible_address_count = page_size_in_bytes / Alignment::size;

    private:
        var::byte m_page[page_size_in_bytes];
        block_info m_free_list[possible_address_count];
        var::byte* m_page_end;
        var::uint32 m_free_list_size;
        var::boolean m_is_page_heapified;

    public:
        var::uint32 _usage_in_bytes;
        var::uint32 _remaining_capacity_in_bytes;
        var::byte* _page_iterator;

    public:
        chunk() noexcept
            : m_free_list_size(0),
            m_is_page_heapified(false),
            _usage_in_bytes(0),
            _remaining_capacity_in_bytes(page_size_in_bytes)

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            , m_double_free_tracker()
#endif
        {
            _page_iterator = static_cast<var::byte*>(m_page);
            m_page_end = _page_iterator + page_size_in_bytes;

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            m_double_free_tracker = (var::uint32*)FE_ALIGNED_ALLOC(possible_address_count * sizeof(var::uint32), FE::CPU_L1_cache_line::size);
            FE_EXIT_IF(m_double_free_tracker == nullptr, FE::ErrorCode::_FatalMemoryError_1XX_AllocationFailure, "Fatal Error: Unable to FE_ALIGNED_ALLOC double-free tracker for a scalable_allocator.");
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
            FE_ASSERT((requested_bytes_p % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &requested_bytes_p, &Alignment::size);

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
                FE_ASSERT((reinterpret_cast<FE::uintptr>(out_alloc_result_p._address) % Alignment::size) == 0, "FE.Core.scalable_allocator has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", out_alloc_result_p._address, &Alignment::size);

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

    _FE_FORCE_INLINE_ FE::internal::pool::PageListClass __select_page_list_by_capacity(FE::uint32 allocation_request_in_bytes_p) noexcept
    {
        FE_ASSERT(allocation_request_in_bytes_p <= FE::system_large_page_size);
        constexpr FE::uint32 l_divider = 131072; // 131072 bytes because 4096*512 == 2MiB and 256*512 == 131072.
        FE::uint32 l_quotient = allocation_request_in_bytes_p / l_divider;
        /*
        * The exact byte numbers below change if _LARGE_PAGES_ is defined as 1.
        *
        * l_quotient == 0 if 0~255 bytes are requested, fits within the pages with 6.25% capacity remaining.
        * l_quotient == 1 if 256~511 bytes are requested
        * l_quotient == 2 if 512~767 bytes are requested,
        * l_quotient == 3 if 768~1023 bytes are requested
        * l_quotient == 4 if 1024~1279 bytes are requested,
        * l_quotient == 5 if 1280~1535 bytes are requested
        * l_quotient == 6 if 1536~1791 bytes are requested
        * l_quotient == 7 if 1792~2047 bytes are requested
        * l_quotient == 8 if 2048~2303 bytes are requested,
        * l_quotient == 9 if 2304~2559 bytes are requested
        * l_quotient == 10 if 2560~2815 bytes are requested
        * l_quotient == 11 if 2816~3071 bytes are requested
        * l_quotient == 12 if 3072~3327 bytes are requested
        * l_quotient == 13 if 3328~3583 bytes are requested
        * l_quotient == 14 if 3584~3839 bytes are requested
        * l_quotient == 15 if 3840~4095 bytes are requested
        * l_quotient == 16 if 4096 bytes are requested,
        */

        if (allocation_request_in_bytes_p == 0)
        {
            FE::internal::pool::PageListClass::_Unavailable;
        }

        switch (l_quotient)
        {
        case 0:
            return FE::internal::pool::PageListClass::_6_25_PercentRemaining;


        case 1:
            return FE::internal::pool::PageListClass::_12_5_Percent;


        case 2:
            _FE_FALLTHROUGH_;
        case 3:
            return FE::internal::pool::PageListClass::_25_Percent;


        case 4:
            _FE_FALLTHROUGH_;
        case 5:
            _FE_FALLTHROUGH_;
        case 6:
            _FE_FALLTHROUGH_;
        case 7:
            return FE::internal::pool::PageListClass::_50_Percent;


        case 8:
            _FE_FALLTHROUGH_;
        case 9:
            _FE_FALLTHROUGH_;
        case 10:
            _FE_FALLTHROUGH_;
        case 11:
            return FE::internal::pool::PageListClass::_75_Percent;


        case 12:
            _FE_FALLTHROUGH_;
        case 13:
            _FE_FALLTHROUGH_;
        case 14:
            _FE_FALLTHROUGH_;
        case 15:
            _FE_FALLTHROUGH_;
        case 16:
            return FE::internal::pool::PageListClass::_100_Percent;

            _FE_NODEFAULT_;
        }
    }
}


namespace large
{
    template<class Alignment>
    class pool<PoolType::_ScalableLargePage, Alignment>
    {
        static_assert(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

        using chunk_type = internal::pool::chunk<PoolType::_ScalableLargePage, Alignment>;
        using free_list_iterator = typename chunk_type::free_list_iterator;

    public:
        constexpr static FE::uint32 page_capacity = chunk_type::page_size_in_bytes;
        using alignment_type = Alignment;

    private:
        using pool_type = FE::list<chunk_type, FE::page_aligned_allocator<chunk_type>>;
        using page_iterator = typename pool_type::iterator;

        pool_type m_unavailable_pages;       // the exact byte numbers below change if _LARGE_PAGES_ is defined as 1.
        pool_type m_pages_with_6_25_capacity;// ~256B
        pool_type m_pages_with_12_5_capacity;// ~512B
        pool_type m_pages_with_25_capacity;  // ~1KiB
        pool_type m_pages_with_50_capacity;  // ~2KiB
        pool_type m_pages_with_75_capacity;  // ~3KiB
        pool_type m_pages_with_100_capacity; // ~4KiB

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        using page_validation_table = absl::flat_hash_set<chunk_type*,
            FE::hash<chunk_type*>, std::equal_to<chunk_type*>,
            FE::cache_aligned_allocator<chunk_type*>>;
        page_validation_table m_page_validation_table;
#endif

    public:
        pool() noexcept
            : m_unavailable_pages(),
            m_pages_with_6_25_capacity(),
            m_pages_with_12_5_capacity(),
            m_pages_with_25_capacity(),
            m_pages_with_50_capacity(),
            m_pages_with_75_capacity(),
            m_pages_with_100_capacity()
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            , m_page_validation_table()
#endif
        {
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            m_page_validation_table.reserve(512);
#endif
            FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, FE::internal::pool::__enable_large_pages(););
            m_pages_with_100_capacity.emplace_front();
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            m_page_validation_table.insert(&m_pages_with_100_capacity.front());
#endif
        }

         ~pool() noexcept = default;

        pool(pool&& other_p) noexcept
            : m_unavailable_pages(std::move(other_p.m_unavailable_pages)),
            m_pages_with_6_25_capacity(std::move(other_p.m_pages_with_6_25_capacity)),
            m_pages_with_12_5_capacity(std::move(other_p.m_pages_with_12_5_capacity)),
            m_pages_with_25_capacity(std::move(other_p.m_pages_with_25_capacity)),
            m_pages_with_50_capacity(std::move(other_p.m_pages_with_50_capacity)),
            m_pages_with_75_capacity(std::move(other_p.m_pages_with_75_capacity)),
            m_pages_with_100_capacity(std::move(other_p.m_pages_with_100_capacity))
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            , m_page_validation_table(std::move(other_p.m_page_validation_table))
#endif
        {
        }

        pool& operator=(pool&& other_p) noexcept
        {
            if (this == &other_p) // prevent self-move-assignment
            {
                return *this;
            }

            m_unavailable_pages = std::move(other_p.m_unavailable_pages);
            m_pages_with_6_25_capacity = std::move(other_p.m_pages_with_6_25_capacity);
            m_pages_with_12_5_capacity = std::move(other_p.m_pages_with_12_5_capacity);
            m_pages_with_25_capacity = std::move(other_p.m_pages_with_25_capacity);
            m_pages_with_50_capacity = std::move(other_p.m_pages_with_50_capacity);
            m_pages_with_75_capacity = std::move(other_p.m_pages_with_75_capacity);
            m_pages_with_100_capacity = std::move(other_p.m_pages_with_100_capacity);
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
            FE::uint32 l_queried_allocation_in_bytes = (FE::uint32)FE::calculate_aligned_memory_size_in_bytes<U, Alignment>(size_p);
            FE_ASSERT((l_queried_allocation_in_bytes % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the requested allocation size '${%d@0}' is not properly aligned by ${%lu@1}.", &l_queried_allocation_in_bytes, &Alignment::size);
            FE_ASSERT(l_queried_allocation_in_bytes <= page_capacity, "Fatal Error: Unable to allocate ${%d0} bytes of memory that exceeds the pool chunk's capacity.", &size_p);
            FE_ASSERT(m_pages_with_100_capacity.end() == nullptr, "Assertion failed: FE::list::end must return an iterator equivalent to a null pointer.");


            typename pool_type::iterator l_page_list_iterator;
            internal::pool::PageListClass l_required_page_class = internal::large::pool::__select_page_list_by_capacity(l_queried_allocation_in_bytes);
            do
            {
                switch (l_required_page_class)
                {
                case internal::pool::PageListClass::_6_25_PercentRemaining:
                    l_page_list_iterator = m_pages_with_6_25_capacity.begin();
                    if (l_page_list_iterator == m_pages_with_6_25_capacity.end())
                    {
                        l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                        continue;
                    }
                    break;

                case internal::pool::PageListClass::_12_5_Percent:
                    l_page_list_iterator = m_pages_with_12_5_capacity.begin();
                    if (l_page_list_iterator == m_pages_with_12_5_capacity.end())
                    {
                        l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                        continue;
                    }
                    break;

                case internal::pool::PageListClass::_25_Percent:
                    l_page_list_iterator = m_pages_with_25_capacity.begin();
                    if (l_page_list_iterator == m_pages_with_25_capacity.end())
                    {
                        l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                        continue;
                    }
                    break;

                case internal::pool::PageListClass::_50_Percent:
                    l_page_list_iterator = m_pages_with_50_capacity.begin();
                    if (l_page_list_iterator == m_pages_with_50_capacity.end())
                    {
                        l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                        continue;
                    }
                    break;

                case internal::pool::PageListClass::_75_Percent:
                    l_page_list_iterator = m_pages_with_75_capacity.begin();
                    if (l_page_list_iterator == m_pages_with_75_capacity.end())
                    {
                        l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                        continue;
                    }
                    break;

                case internal::pool::PageListClass::_100_Percent:
                    l_page_list_iterator = m_pages_with_100_capacity.begin();
                    if (l_page_list_iterator == m_pages_with_100_capacity.end())
                    {
                        m_pages_with_100_capacity.emplace_front();
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
                        m_page_validation_table.insert(&m_pages_with_100_capacity.front());
#endif
                        l_page_list_iterator = m_pages_with_100_capacity.begin();
                    }
                    break;

                case internal::pool::PageListClass::_Unavailable:
                    return nullptr;

                    _FE_NODEFAULT_;
                }

                if (l_page_list_iterator->_remaining_capacity_in_bytes < l_queried_allocation_in_bytes)
                {
                    if (l_required_page_class != internal::pool::PageListClass::_100_Percent)
                    {
                        l_required_page_class = static_cast<internal::pool::PageListClass>((int)l_required_page_class + 1);
                        l_page_list_iterator = m_pages_with_100_capacity.end();
                        continue;
                    }

                    m_pages_with_100_capacity.emplace_front();
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
                    m_page_validation_table.insert(&m_pages_with_100_capacity.front());
#endif
                    l_page_list_iterator = m_pages_with_100_capacity.begin();
                    break;
                }
            } while (l_page_list_iterator == m_pages_with_100_capacity.end());


            alignas(16) internal::pool::block_info l_memblock_info{};
            var::boolean l_was_allocation_successful = __try_allocation_from_page(l_page_list_iterator, l_memblock_info, l_queried_allocation_in_bytes);
            while (l_was_allocation_successful == false)
            {
                ++l_page_list_iterator;
                if (l_page_list_iterator == m_pages_with_100_capacity.end())
                {
                    m_pages_with_100_capacity.emplace_front();
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
                    m_page_validation_table.insert(&m_pages_with_100_capacity.front());
#endif
                    l_page_list_iterator = m_pages_with_100_capacity.begin();
                }
                l_was_allocation_successful = __try_allocation_from_page(l_page_list_iterator, l_memblock_info, l_queried_allocation_in_bytes);
            }


#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            l_page_list_iterator->check_double_allocation(l_memblock_info);
#endif
            if constexpr (FE::is_trivial_v<U> == false)
            {
                U* const l_end = reinterpret_cast<U*>(l_memblock_info._address) + size_p;
                for (U* it = reinterpret_cast<U*>(l_memblock_info._address); it != l_end; ++it)
                {
                    new(it) U();
                }
            }

            FE_ASSERT((reinterpret_cast<FE::uintptr>(l_memblock_info._address) % Alignment::size) == 0, "FE.Core.scalable_allocator has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", l_memblock_info._address, &Alignment::size);
            l_page_list_iterator->_usage_in_bytes += l_memblock_info._size_in_bytes;


            pool_type* l_previous_page_list = nullptr;
            internal::pool::PageListClass l_previous_class = internal::large::pool::__select_page_list_by_capacity(l_page_list_iterator->_remaining_capacity_in_bytes);
            switch (l_previous_class) // class before allocation
            {
            case internal::pool::PageListClass::_6_25_PercentRemaining:
                l_previous_page_list = &m_pages_with_6_25_capacity;
                break;

            case internal::pool::PageListClass::_12_5_Percent:
                l_previous_page_list = &m_pages_with_12_5_capacity;
                break;

            case internal::pool::PageListClass::_25_Percent:
                l_previous_page_list = &m_pages_with_25_capacity;
                break;

            case internal::pool::PageListClass::_50_Percent:
                l_previous_page_list = &m_pages_with_50_capacity;
                break;

            case internal::pool::PageListClass::_75_Percent:
                l_previous_page_list = &m_pages_with_75_capacity;
                break;

            case internal::pool::PageListClass::_100_Percent:
                l_previous_page_list = &m_pages_with_100_capacity;
                break;

                _FE_NODEFAULT_;
            }
            FE_ASSERT(l_previous_page_list != nullptr, "Assertion Failure: The previous page list cannot be null.");

            l_page_list_iterator->_remaining_capacity_in_bytes -= l_memblock_info._size_in_bytes;

            internal::pool::PageListClass l_new_class = internal::large::pool::__select_page_list_by_capacity(l_page_list_iterator->_remaining_capacity_in_bytes);
            if (l_new_class != l_previous_class) // if the page has changed its class after allocation, move it to the corresponding list.
            {
                switch (l_new_class) // new class after allocation
                {
                case internal::pool::PageListClass::_Unavailable:
                    m_unavailable_pages.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_unavailable_pages.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                    break;

                case internal::pool::PageListClass::_6_25_PercentRemaining:
                    m_pages_with_6_25_capacity.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_6_25_capacity.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                    break;

                case internal::pool::PageListClass::_12_5_Percent:
                    m_pages_with_12_5_capacity.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_12_5_capacity.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                    break;

                case internal::pool::PageListClass::_25_Percent:
                    m_pages_with_25_capacity.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_25_capacity.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                    break;

                case internal::pool::PageListClass::_50_Percent:
                    m_pages_with_50_capacity.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_50_capacity.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                    break;

                case internal::pool::PageListClass::_75_Percent:
                    m_pages_with_75_capacity.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_75_capacity.rbegin()), *l_previous_page_list, FE::iterator_cast<typename pool_type::const_iterator>(l_page_list_iterator));
                    break;

                default:
                    break;
                }
            }

            return reinterpret_cast<U*>(l_memblock_info._address);
        }

        // Incorrect type will cause a critical runtime error.
        template <typename T>
        void _FE_VECTOR_CALL_ deallocate(T* pointer_p, FE::size count_p) noexcept
        {
            FE_ASSERT(pointer_p != nullptr, "Critical Error in FE.Core.scalable_allocator: Unable to deallocate() a nullptr.");
            FE_ASSERT(count_p > 0, "${%s@0}: ${%s@1} was 0", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(count_p));
            FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this scalable_allocator instance.", pointer_p, &Alignment::size);

            alignas(16) internal::pool::block_info l_block_to_free;
            l_block_to_free._address = reinterpret_cast<var::byte*>(pointer_p);
            l_block_to_free._size_in_bytes = (FE::uint32)FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(count_p);
            FE_ASSERT(l_block_to_free._size_in_bytes <= page_capacity, "Critical Error in FE.Core.scalable_allocator: the pointer '${%p@0}' does not belong to this scalable_allocator instance located at '${%p@1}'.", pointer_p, this);

            // use a pointer arithmetic to find which page the pointer belongs to.
            chunk_type* l_page_base = reinterpret_cast<chunk_type*>((FE::uintptr)l_block_to_free._address - (((FE::uintptr)(l_block_to_free._address)) % page_capacity));

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            FE_EXIT_IF(m_page_validation_table.find(l_page_base) == m_page_validation_table.end(), ErrorCode::_FatalMemoryError_1XX_FalseDeallocation, "Critical Error in FE.Core.scalable_allocator: the pointer '${%p@0}' does not belong to this scalable_allocator instance located at '${%p@1}'.", pointer_p, this);
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
            l_page_base->_usage_in_bytes -= l_block_to_free._size_in_bytes;
            FE_ASSERT(l_page_base->_usage_in_bytes >= 0, "Critical Error in FE.Core.scalable_allocator: the internal usage counter has gone negative. Memory corruption might have occurred.");


            pool_type* l_previous_page_list = nullptr;
            internal::pool::PageListClass l_previous_class = internal::large::pool::__select_page_list_by_capacity(l_page_base->_remaining_capacity_in_bytes);
            switch (l_previous_class) // class before deallocation
            {
            case internal::pool::PageListClass::_Unavailable:
                l_previous_page_list = &m_unavailable_pages;
                break;

            case internal::pool::PageListClass::_6_25_PercentRemaining:
                l_previous_page_list = &m_pages_with_6_25_capacity;
                break;

            case internal::pool::PageListClass::_12_5_Percent:
                l_previous_page_list = &m_pages_with_12_5_capacity;
                break;

            case internal::pool::PageListClass::_25_Percent:
                l_previous_page_list = &m_pages_with_25_capacity;
                break;

            case internal::pool::PageListClass::_50_Percent:
                l_previous_page_list = &m_pages_with_50_capacity;
                break;

            case internal::pool::PageListClass::_75_Percent:
                l_previous_page_list = &m_pages_with_75_capacity;
                break;

            case internal::pool::PageListClass::_100_Percent:
                break;

                _FE_NODEFAULT_;
            }


            // After deallocation, the page might have more remaining capacity and thus change its class. Move it to the corresponding list if it has changed its class.
            l_page_base->_remaining_capacity_in_bytes += l_block_to_free._size_in_bytes;
            typename pool_type::const_iterator l_page_list_iterator = FE::iterator_cast<typename pool_type::const_iterator>((typename pool_type::const_iterator::wrapped_iterator_type::pointer)l_page_base);


            internal::pool::PageListClass l_new_class = internal::large::pool::__select_page_list_by_capacity(l_page_base->_remaining_capacity_in_bytes);
            if (l_new_class != l_previous_class) // if the page has changed its class after allocation, move it to the corresponding list.
            {
                switch (l_new_class) // new class after deallocation
                {
                case internal::pool::PageListClass::_Unavailable:
                    FE_ASSERT(false);
                    return;

                case internal::pool::PageListClass::_6_25_PercentRemaining:
                    m_pages_with_6_25_capacity.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_6_25_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                    return;

                case internal::pool::PageListClass::_12_5_Percent:
                    m_pages_with_12_5_capacity.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_12_5_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                    return;

                case internal::pool::PageListClass::_25_Percent:
                    m_pages_with_25_capacity.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_25_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                    return;

                case internal::pool::PageListClass::_50_Percent:
                    m_pages_with_50_capacity.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_50_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                    return;

                case internal::pool::PageListClass::_75_Percent:
                    m_pages_with_75_capacity.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_75_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                    return;

                case internal::pool::PageListClass::_100_Percent:
                    m_pages_with_100_capacity.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_pages_with_100_capacity.rbegin()), *l_previous_page_list, l_page_list_iterator);
                    break;

                default:
                    return;
                }
            }

        }




        bool try_trim_a_page() noexcept
        {
            for (typename pool_type::iterator page = m_pages_with_100_capacity.begin(); page != m_pages_with_100_capacity.end(); ++page)
            {
                if (page->_usage_in_bytes == 0)
                {
                    m_pages_with_100_capacity->erase(page);
                    return true;
                }
            }
            return false;
        }

        void try_trim_all_pages() noexcept
        {
            for (typename pool_type::iterator page = m_pages_with_100_capacity.begin(); page != m_pages_with_100_capacity.end();)
            {
                if (page->_usage_in_bytes == 0)
                {
                    page = m_pages_with_100_capacity->erase(page);
                    continue;
                }
                ++page;
            }
        }

        _FE_FORCE_INLINE_ FE::size get_page_count() const noexcept { return m_pages_with_100_capacity.size(); }

    private:
        /* Time complexity:
        Allocation from a stack: O(1)
        Allocation from a heap tree: O(2 log n)
        Defragmentation: O(5n + n log n) [rarely gets called]

        Best: O(1)
        Worst: O(5n + n log n) + O(2 log n)
        */
        static FE::boolean _FE_VECTOR_CALL_ __try_allocation_from_page(page_iterator page_p, internal::pool::block_info& out_result_p, FE::uint32 bytes_p) noexcept
        {
            FE_ASSERT((bytes_p % Alignment::size) == 0, "Critical Error in FE.Core.scalable_allocator: the requested allocation size '${%lu@0}' is not properly aligned by ${%lu@1}.", &bytes_p, &Alignment::size);
            if (page_p->is_page_heapified() == true)
            {
                if (page_p->retrieve_from_the_free_list(out_result_p, bytes_p) == _FE_FAILED_)
                {
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

        // Try reducing the pointer offset data to uint32 to save memory, since the page size is no larger than 2MiB.
        static void _FE_VECTOR_CALL_ __defragment(page_iterator page_p) noexcept
        {
            if (page_p->get_free_list_size() <= 1) _FE_UNLIKELY_
            {
                return;
            }
            std::sort<std::execution::parallel_unsequenced_policy, free_list_iterator, internal::pool::from_low_address>(   std::execution::parallel_unsequenced_policy{},
                                                                                                                            page_p->get_free_list(),
                                                                                                                            page_p->get_free_list() + page_p->get_free_list_size(),
                                                                                                                            internal::pool::from_low_address{});
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
            l_end = std::partition(std::execution::par_unseq, page_p->get_free_list(), l_end,
                [](const internal::pool::block_info& block_p)
                {
                    return block_p._address != nullptr;
                }
            );
            // Reset it.
            page_p->set_free_list_size(static_cast<FE::uint32>(l_end - page_p->get_free_list()));

            // Heapify the free list. Time complexity: O(3n)
            std::make_heap(page_p->get_free_list(), l_end, internal::pool::less_than{});

            page_p->set_page_heapified(); // Switch the allocation strategy to binary search.
        }
    };


    template<class Alignment = FE::SIMD_auto_alignment>
    using scalable_allocator = pool<PoolType::_ScalableLargePage, Alignment>;
}
END_NAMESPACE
#endif