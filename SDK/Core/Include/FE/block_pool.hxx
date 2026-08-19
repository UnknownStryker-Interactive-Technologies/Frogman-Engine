#ifndef _FE_CORE_BLOCK_POOL_HXX_
#define _FE_CORE_BLOCK_POOL_HXX_
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
#include <FE/fstack.hxx>




BEGIN_NAMESPACE(FE)


namespace internal::pool
{
    template<class Alignment>
    class chunk<PoolType::_Block, Alignment>
    {
    public:
        constexpr static FE::int32 fixed_block_size_in_bytes = Alignment::size;
        constexpr static FE::int32 page_size_in_bytes = 64 * FE::one_KiB;
        constexpr static FE::int32 possible_address_count = page_size_in_bytes / fixed_block_size_in_bytes;
        static_assert(possible_address_count > 1, "Static assertion failed: possible_address_count is less than 1.");

        using block_pointer = var::int32;

    private: // DO NOT MEMZERO THIS ARRAY. IT WILL PUT THE COMPILER INTO AN INFINITE COMPLIATION LOOP.
        var::byte m_page[page_size_in_bytes];
		static_assert(sizeof(m_page) < FE::max_value<FE::int32>, "Static assertion failed: sizeof(m_page) is exceeds the maximum allowed size.");

    public:
        FE::fstack<block_pointer, possible_address_count> _free_blocks;
        var::byte* const _begin;
        var::byte* _page_iterator;
        var::byte* const _end;
        var::int32 _usage_in_bytes;
		PageGroup _availability;

    public:
        chunk() noexcept
            :   _begin(static_cast<var::byte*>(m_page)),
                _page_iterator(_begin),
			    _end(_page_iterator + page_size_in_bytes),
			    _usage_in_bytes(0),
			    _availability(PageGroup::_AvailablePages)
        {
#ifdef _ENABLE_ASSERT_
            std::memset(m_double_free_tracker, 0, possible_address_count);
#endif
        }
		~chunk() noexcept = default;

        chunk(const chunk&) = delete;
        chunk& operator=(const chunk&) = delete;
        chunk(chunk&&) = delete;
        chunk& operator=(chunk&&) = delete;

#ifdef _ENABLE_ASSERT_
    private:
        var::boolean m_double_free_tracker[possible_address_count];

    public:
        void check_double_allocation(FE::byte* const address_p) noexcept
        {
            FE::int32 l_idx = static_cast<FE::int32>((address_p - _begin) / Alignment::size);
            FE_ASSERT(m_double_free_tracker[l_idx] == false, "Double allocation detected: cannot allocate the same address twice.");
            m_double_free_tracker[l_idx] = true;
        }

        void check_double_free(FE::byte* const address_p) noexcept
        {
            FE::int32 l_idx = static_cast<FE::int32>((address_p - _begin) / Alignment::size);
            FE_ASSERT(m_double_free_tracker[l_idx] == true, "Double free detected: cannot deallocate the same address twice.");
            m_double_free_tracker[l_idx] = false;
        }
#endif
        _FE_FORCE_INLINE_ boolean is_out_of_memory() const noexcept
        {
            return (_free_blocks.is_empty() == true) && (_page_iterator >= _end);
        }

        _FE_FORCE_INLINE_ FE::int32 get_usage_as_percentile() const noexcept
        {
            return static_cast<FE::int32>(((FE::float32)_usage_in_bytes / (FE::float32)page_size_in_bytes) * 100.0f);
        }
    };
}




template<class Alignment>
class pool<PoolType::_Block, Alignment>
{
    static_assert(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

    using chunk_type = internal::pool::chunk<PoolType::_Block, Alignment>;
    using block_pointer = typename chunk_type::block_pointer;

public:
    constexpr static FE::int32 fixed_block_size_in_bytes = Alignment::size;
    constexpr static FE::int32 page_capacity = chunk_type::page_size_in_bytes;

    using alignment_type = Alignment;

private:
    using pool_type = FE::list< chunk_type, FE::page_aligned_allocator<chunk_type> >;
    pool_type m_available_pages;
    pool_type m_unavailable_pages;

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
    using page_validation_table = absl::flat_hash_set<chunk_type*,
        typename absl::lts_20260107::container_internal::FlatHashSetPolicy<chunk_type*>::DefaultHash, std::equal_to<chunk_type*>,
        FE::cache_aligned_allocator<chunk_type*>>;
    page_validation_table m_page_validation_table;
#endif

public:
    pool() noexcept
		:   m_available_pages(),
            m_unavailable_pages()
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
		,   m_page_validation_table()
#endif
    {
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        m_page_validation_table.reserve(1024);
#endif

        create_new_page_at_front();
        create_new_page_at_front();
    }

     ~pool() noexcept
    {}

    pool(pool&& other_p) noexcept
		:   m_available_pages( std::move(other_p.m_available_pages) ),
            m_unavailable_pages( std::move(other_p.m_unavailable_pages) )
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        ,   m_page_validation_table(std::move(other_p.m_page_validation_table))
#endif
	{}

    pool& operator=(pool&& other_p) noexcept
    {
		m_available_pages = std::move(other_p.m_available_pages);
        m_unavailable_pages = std::move(other_p.m_unavailable_pages);

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
		m_page_validation_table = std::move(other_p.m_page_validation_table);
#endif
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

public:
    template<typename U>
    U* _FE_VECTOR_CALL_ allocate() noexcept
    {
        static_assert(sizeof(U) <= fixed_block_size_in_bytes, "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
        static_assert(Alignment::size == fixed_block_size_in_bytes, "Static assertion failed: incorrect Alignment::size detected.");
		FE_ASSERT(m_available_pages.is_empty() == false, "Critical Error in FE.Core.block_allocator: No available pages to allocate from. This should never happen because the constructor always creates a page.");

        if (m_available_pages.front().is_out_of_memory() == true)
        {
			m_available_pages.front()._availability = PageGroup::_UnavailablePages; // mark the page as unavailable before moving it to the unavailable list.
			m_unavailable_pages.splice(m_unavailable_pages.cbegin(), m_available_pages, m_available_pages.cbegin()); // move the page to the Unavailable List (UL).
        }

        if (m_available_pages.size() == 0)
        {
            create_new_page_at_front();
        }

        void* l_allocation_result = nullptr;
        if (m_available_pages.front()._free_blocks.is_empty() == false)
        {
            l_allocation_result = m_available_pages.front()._begin + m_available_pages.front()._free_blocks.pop();
        }
        else
        {
            l_allocation_result = m_available_pages.front()._page_iterator;
            m_available_pages.front()._page_iterator += fixed_block_size_in_bytes;
        }
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        m_available_pages.front().check_double_allocation(reinterpret_cast<var::byte*>(l_allocation_result));
#endif
        if constexpr (FE::is_trivial<U>::value == false)
        {
            new(static_cast<U*>(l_allocation_result)) U();
        }

        FE_ASSERT((reinterpret_cast<FE::uintptr>(l_allocation_result) % Alignment::size) == 0, "FE.Core.block_allocator has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", l_allocation_result, &Alignment::size);
        m_available_pages.front()._usage_in_bytes += fixed_block_size_in_bytes;
        return static_cast<U*>(l_allocation_result);
    }

    // Incorrect type will cause a critical runtime error.
    template<typename U> 
    void _FE_VECTOR_CALL_ deallocate(U* const pointer_p) noexcept
    {
        static_assert(sizeof(U) <= fixed_block_size_in_bytes, "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
		FE_NEGATIVE_ASSERT(pointer_p == nullptr, "Critical Error in FE.Core.block_allocator: Unable to deallocate() a nullptr.");
        FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.Core.block_allocator: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this block_allocator instance.", pointer_p, &Alignment::size);
        var::byte* l_to_be_freed = reinterpret_cast<var::byte*>(pointer_p);

        // use a pointer arithmetic to find which page the pointer belongs to.
        chunk_type* l_page_base = reinterpret_cast<chunk_type*>( reinterpret_cast<FE::uintptr>(l_to_be_freed) - (reinterpret_cast<FE::uintptr>(l_to_be_freed) % page_capacity) );
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        FE_EXIT_IF(m_page_validation_table.find(l_page_base) == m_page_validation_table.end(), FE::ErrorCode::_FatalMemoryError_1XX_FalseDeallocation, "Fatal Error: the pointer '${%p@0}' does not belong to this block_allocator instance located at '${%p@1}'.", pointer_p, this);
		l_page_base->check_double_free(l_to_be_freed);
#endif
        if constexpr (FE::is_trivial<U>::value == false)
        {
            pointer_p->~U();
        }
        l_page_base->_free_blocks.push(static_cast<FE::int32>(l_to_be_freed - l_page_base->_begin));
        l_page_base->_usage_in_bytes -= fixed_block_size_in_bytes;
        FE_ASSERT(l_page_base->_usage_in_bytes >= 0, "Critical Error in FE.Core.block_allocator: the internal usage counter has gone negative. Memory corruption might have occurred.");
        
        if (l_page_base->_availability == PageGroup::_UnavailablePages)
        {
			l_page_base->_availability = PageGroup::_AvailablePages; // mark the page as available before moving it to the available list.
            
            m_available_pages.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_available_pages.rbegin()), m_unavailable_pages,
                FE::iterator_cast<typename pool_type::const_iterator>((typename pool_type::const_iterator::wrapped_iterator_type::pointer)l_page_base)); // move the page to the Available List (AL).
        }
    }

    bool try_trim_a_page() noexcept
    {
        for (typename pool_type::iterator page = m_available_pages.begin(); page != m_available_pages.end(); ++page)
        {
            if (page->_usage_in_bytes == 0)
            {
                m_available_pages->erase(page);
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
                m_page_validation_table.erase(&*page);
#endif
                return true;
            }
        }
        return false;
    }

    void try_trim_all_pages() noexcept
    {
        for (typename pool_type::iterator page = m_available_pages.begin(); page != m_available_pages.end();)
        {
            if (page->_usage_in_bytes == 0)
            {
                page = m_available_pages->erase(page);
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
                m_page_validation_table.erase(&*page);
#endif
                continue;
            }
            ++page;
        }
    }

    void create_new_page_at_front() noexcept
    {
        m_available_pages.emplace_front();
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        m_page_validation_table.insert(&m_available_pages.front());
#endif
    }
};


/*
- allocate(): O(1)
- deallocate(): O(1)

The FE::block_allocator class template implements a memory pool allocator for managing fixed-size blocks of memory with a specified alignment and page capacity
ensuring efficient allocation and deallocation while providing mechanisms for free-list corruption detection.
*/
template<FE::size FixedBlockSizeInBytes, class Alignment = FE::SIMD_auto_alignment>
using block_allocator = pool<PoolType::_Block, FE::align_as<FixedBlockSizeInBytes, Alignment>>;




namespace internal::pool
{
    template<class Alignment>
    class chunk<PoolType::_BlockLargePage, Alignment>
    {
    public:
        constexpr static FE::size fixed_block_size_in_bytes = Alignment::size;

        constexpr static FE::int32 page_granularity_in_bytes = (2 * FE::one_MiB);
        constexpr static FE::int32 page_size_in_bytes = page_granularity_in_bytes - FE::CPU_L1_cache_line::size; // To avoid using an extra large page

        constexpr static FE::size possible_address_count = page_size_in_bytes / fixed_block_size_in_bytes;
        static_assert(possible_address_count > 1, "Static assertion failed: possible_address_count is less than 1.");

        using block_pointer = var::int32;

    private: // DO NOT MEMZERO THIS ARRAY. IT WILL PUT THE COMPILER INTO AN INFINITE COMPLIATION LOOP.
        var::byte m_page[page_size_in_bytes];
        static_assert(sizeof(m_page) < FE::max_value<FE::int32>, "Static assertion failed: sizeof(m_page) is exceeds the maximum allowed size.");

    public:
        std::unique_ptr<FE::fstack<block_pointer, possible_address_count>> _free_blocks;
        var::byte* const _begin;
        var::byte* _page_iterator;
        var::byte* const _end;
        var::int32 _usage_in_bytes;
        PageGroup _availability;

    public:
        chunk() noexcept
            :   _begin(static_cast<var::byte*>(m_page)),
                _page_iterator(_begin),
                _end(_page_iterator + page_size_in_bytes),
                _usage_in_bytes(0),
                _availability(PageGroup::_AvailablePages)
        {
			_free_blocks = std::make_unique<FE::fstack<block_pointer, possible_address_count>>();


#ifdef _ENABLE_ASSERT_
            m_double_free_tracker = std::make_unique<var::boolean[]>(possible_address_count);
            std::memset(m_double_free_tracker.get(), 0, possible_address_count);
#endif
        }
        ~chunk() noexcept = default;

        chunk(const chunk&) = delete;
        chunk& operator=(const chunk&) = delete;
        chunk(chunk&&) = delete;
        chunk& operator=(chunk&&) = delete;

#ifdef _ENABLE_ASSERT_
    private:
        std::unique_ptr<var::boolean[]> m_double_free_tracker;

    public:
        void check_double_allocation(FE::byte* const address_p) noexcept
        {
            FE::int32 l_idx = static_cast<FE::int32>((address_p - _begin) / Alignment::size);
            FE_ASSERT(m_double_free_tracker[l_idx] == false, "Double allocation detected: cannot allocate the same address twice.");
            m_double_free_tracker[l_idx] = true;
        }

        void check_double_free(FE::byte* const address_p) noexcept
        {
            FE::int32 l_idx = static_cast<FE::int32>((address_p - _begin) / Alignment::size);
            FE_ASSERT(m_double_free_tracker[l_idx] == true, "Double free detected: cannot deallocate the same address twice.");
            m_double_free_tracker[l_idx] = false;
        }
#endif
        _FE_FORCE_INLINE_ boolean is_out_of_memory() const noexcept
        {
            return (_free_blocks->is_empty() == true) && (_page_iterator >= _end);
        }

        _FE_FORCE_INLINE_ FE::int32 get_usage_as_percentile() const noexcept
        {
            return static_cast<FE::int32>(((FE::float32)_usage_in_bytes / (FE::float32)page_size_in_bytes) * 100.0f);
        }
    };
}


namespace large
{
    template<class Alignment>
    class pool<PoolType::_BlockLargePage, Alignment>
    {
        static_assert(FE::is_power_of_two(Alignment::size) == true, "Static Assertion Failure: Alignment::size must be a power of two.");

        using chunk_type = internal::pool::chunk<PoolType::_BlockLargePage, Alignment>;
        static_assert(sizeof(chunk_type) <= (2 * FE::one_MiB), "Static assertion failed: chunk must fit within a single 2 MiB-sized memory page.");
        using block_pointer = typename chunk_type::block_pointer;

    public:
        constexpr static FE::int32 fixed_block_size_in_bytes = Alignment::size;
        constexpr static FE::int32 page_capacity = chunk_type::page_size_in_bytes;
		constexpr static FE::int32 page_granularity_in_bytes = chunk_type::page_granularity_in_bytes;

        using alignment_type = Alignment;

    private:
        using pool_type = FE::list< chunk_type, FE::page_aligned_allocator<chunk_type> >;
        pool_type m_available_pages;
        pool_type m_unavailable_pages;

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        using page_validation_table = absl::flat_hash_set<chunk_type*>;
        page_validation_table m_page_validation_table;
#endif

    public:
        pool() noexcept
            : m_available_pages(),
            m_unavailable_pages()
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            , m_page_validation_table()
#endif
        {
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            m_page_validation_table.reserve(512);
#endif
            FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, FE::internal::pool::__enable_large_pages(););

            create_new_page_at_front();
            create_new_page_at_front();
        }

         ~pool() noexcept
        {
        }

        pool(pool&& other_p) noexcept
            : m_available_pages(std::move(other_p.m_available_pages)),
            m_unavailable_pages(std::move(other_p.m_unavailable_pages))
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            , m_page_validation_table(std::move(other_p.m_page_validation_table))
#endif
        {
        }

        pool& operator=(pool&& other_p) noexcept
        {
            m_available_pages = std::move(other_p.m_available_pages);
            m_unavailable_pages = std::move(other_p.m_unavailable_pages);

#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            m_page_validation_table = std::move(other_p.m_page_validation_table);
#endif
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

    public:
        template<typename U>
        U* _FE_VECTOR_CALL_ allocate() noexcept
        {
            static_assert(sizeof(U) <= fixed_block_size_in_bytes, "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
            static_assert(Alignment::size == fixed_block_size_in_bytes, "Static assertion failed: incorrect Alignment::size detected.");
            FE_ASSERT(m_available_pages.is_empty() == false, "Critical Error in FE.Core.block_allocator: No available pages to allocate from. This should never happen because the constructor always creates a page.");

            if (m_available_pages.front().is_out_of_memory() == true)
            {
                m_available_pages.front()._availability = PageGroup::_UnavailablePages; // mark the page as unavailable before moving it to the unavailable list.
                m_unavailable_pages.splice(m_unavailable_pages.cbegin(), m_available_pages, m_available_pages.cbegin()); // move the page to the Unavailable List (UL).
            }

            if (m_available_pages.size() == 0)
            {
                create_new_page_at_front();
            }

            void* l_allocation_result = nullptr;
            if (m_available_pages.front()._free_blocks->is_empty() == false)
            {
                l_allocation_result = m_available_pages.front()._begin + m_available_pages.front()._free_blocks->pop();
            }
            else
            {
                l_allocation_result = m_available_pages.front()._page_iterator;
                m_available_pages.front()._page_iterator += fixed_block_size_in_bytes;
            }
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            m_available_pages.front().check_double_allocation(reinterpret_cast<var::byte*>(l_allocation_result));
#endif
            if constexpr (FE::is_trivial<U>::value == false)
            {
                new(static_cast<U*>(l_allocation_result)) U();
            }

            FE_ASSERT((reinterpret_cast<FE::uintptr>(l_allocation_result) % Alignment::size) == 0, "FE.Core.block_allocator has failed to allocate an address: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}.", l_allocation_result, &Alignment::size);
            m_available_pages.front()._usage_in_bytes += fixed_block_size_in_bytes;
            return static_cast<U*>(l_allocation_result);
        }

        // Incorrect type will cause a critical runtime error.
        template<typename U>
        void _FE_VECTOR_CALL_ deallocate(U* const pointer_p) noexcept
        {
            static_assert(sizeof(U) <= fixed_block_size_in_bytes, "Static assertion failed: sizeof(U) must not be greater than fixed_block_size_in_bytes.");
            FE_NEGATIVE_ASSERT(pointer_p == nullptr, "Critical Error in FE.Core.block_allocator: Unable to deallocate() a nullptr.");
            FE_ASSERT((reinterpret_cast<FE::uintptr>(pointer_p) % Alignment::size) == 0, "Critical Error in FE.Core.block_allocator: the pointer value '${%p@0}' is not properly aligned by ${%lu@1}. It might not belong to this block_allocator instance.", pointer_p, &Alignment::size);
            var::byte* l_to_be_freed = reinterpret_cast<var::byte*>(pointer_p);

            // use a pointer arithmetic to find which page the pointer belongs to.
            chunk_type* l_page_base = reinterpret_cast<chunk_type*>(reinterpret_cast<FE::uintptr>(l_to_be_freed) - (reinterpret_cast<FE::uintptr>(l_to_be_freed) % page_granularity_in_bytes));
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            FE_EXIT_IF(m_page_validation_table.find(l_page_base) == m_page_validation_table.end(), FE::ErrorCode::_FatalMemoryError_1XX_FalseDeallocation, "Fatal Error: the pointer '${%p@0}' does not belong to this block_allocator instance located at '${%p@1}'.", pointer_p, this);
            l_page_base->check_double_free(l_to_be_freed);
#endif
            if constexpr (FE::is_trivial<U>::value == false)
            {
                pointer_p->~U();
            }
            l_page_base->_free_blocks->push(static_cast<FE::int32>(l_to_be_freed - l_page_base->_begin));
            l_page_base->_usage_in_bytes -= fixed_block_size_in_bytes;
            FE_ASSERT(l_page_base->_usage_in_bytes >= 0, "Critical Error in FE.Core.block_allocator: the internal usage counter has gone negative. Memory corruption might have occurred.");

            if (l_page_base->_availability == PageGroup::_UnavailablePages)
            {
                l_page_base->_availability = PageGroup::_AvailablePages; // mark the page as available before moving it to the available list.

                m_available_pages.splice(FE::iterator_cast<typename pool_type::const_iterator>(m_available_pages.rbegin()), m_unavailable_pages,
                    FE::iterator_cast<typename pool_type::const_iterator>((typename pool_type::const_iterator::wrapped_iterator_type::pointer)l_page_base)); // move the page to the Available List (AL).
            }
        }

        bool try_trim_a_page() noexcept
        {
            for (typename pool_type::iterator page = m_available_pages.begin(); page != m_available_pages.end(); ++page)
            {
                if (page->_usage_in_bytes == 0)
                {
                    m_available_pages->erase(page);
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
                    m_page_validation_table.erase(&*page);
#endif
                    return true;
                }
            }
            return false;
        }

        void try_trim_all_pages() noexcept
        {
            for (typename pool_type::iterator page = m_available_pages.begin(); page != m_available_pages.end();)
            {
                if (page->_usage_in_bytes == 0)
                {
                    page = m_available_pages->erase(page);
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
                    m_page_validation_table.erase(&*page);
#endif
                    continue;
                }
                ++page;
            }
        }

        void create_new_page_at_front() noexcept
        {
            m_available_pages.emplace_front();
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
            m_page_validation_table.insert(&m_available_pages.front());
#endif
        }
    };


    /*
    - allocate(): O(1)
    - deallocate(): O(1)

    The FE::block_allocator class template implements a memory pool allocator for managing fixed-size blocks of memory with a specified alignment and page capacity
    ensuring efficient allocation and deallocation while providing mechanisms for free-list corruption detection.
    */
    template<FE::size FixedBlockSizeInBytes, class Alignment = FE::SIMD_auto_alignment>
    using block_allocator = pool<PoolType::_BlockLargePage, FE::align_as<FixedBlockSizeInBytes, Alignment>>;
}


END_NAMESPACE
#endif