#ifndef _FE_CORE_POOL_HXX_
#define _FE_CORE_POOL_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/block_pool_allocator.hxx>
#include <FE/core/private/pool_common.hxx>

// std
#include <array>
#include <map>




BEGIN_NAMESPACE(FE)


namespace internal::pool
{
    template <>
    struct block_info<void, POOL_TYPE::_GENERIC>
    {
        var::byte* _address = nullptr;
        var::size_t _size_in_bytes = 0;
    };


    template<size_t PossibleAddressCount>
    class address_map : public std::map<FE::var::byte*, FE::var::size_t, std::greater<FE::var::byte*>, FE::block_pool_allocator< std::pair<FE::var::byte* const, FE::var::size_t>, FE::object_count<PossibleAddressCount> >>
    {
    public:
        using base_type = std::map<FE::var::byte*, FE::var::size_t, std::greater<FE::var::byte*>, FE::block_pool_allocator< std::pair<FE::var::byte* const, FE::var::size_t>, FE::object_count<PossibleAddressCount> >>;
        using key_type = var::byte*;
        using key_compare = std::greater<var::byte*>;
        using value_compare = typename base_type::value_compare;
        using value_type = typename base_type::value_type;
        using allocator_type = typename base_type::allocator_type;
        using size_type = typename base_type::size_type;
        using difference_type = typename base_type::difference_type;
        using pointer = typename base_type::pointer;
        using const_pointer = typename base_type::const_pointer;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = typename base_type::iterator;
        using const_iterator = typename base_type::const_iterator;
        using reverse_iterator = typename base_type::reverse_iterator;
        using const_reverse_iterator = typename base_type::const_reverse_iterator;

        _FORCE_INLINE_ address_map() noexcept : base_type() {}
        _FORCE_INLINE_ address_map(const address_map& other_p) noexcept : base_type()
        {
            const base_type* l_other = &other_p;
            base_type* l_this = this;

            *l_this = *l_other;
        }
        _FORCE_INLINE_ address_map(address_map&& rvalue_p) noexcept : base_type()
        {
            base_type* l_rvalue = &rvalue_p;
            base_type* l_this = this;

            *l_this = std::move(*l_rvalue);
        }

        _FORCE_INLINE_ boolean is_empty() const noexcept
        {
            return base_type::empty();
        }
    };


    template<size_t ChunkCapacity, class Alignment>
    struct recycler
    {
        constexpr static count_t chunk_capacity = ChunkCapacity;
        constexpr static size_t recycler_capacity = ChunkCapacity / Alignment::size;

        using underlying_container_type = FE::internal::pool::address_map<recycler_capacity>;
        using iterator = typename underlying_container_type::iterator;

        typename underlying_container_type _begin_tree;
        typename underlying_container_type _end_tree;
    };


    template<size_t ChunkCapacity, class Alignment>
    struct chunk<void, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment>
    {
        constexpr static count_t chunk_capacity = ChunkCapacity;
        constexpr static size_t recycler_capacity = ChunkCapacity / Alignment::size;

        using recycler_type = recycler<recycler_capacity, Alignment>;
        using recycler_iterator = typename recycler_type::iterator;
        using block_info_type = typename recycler_type::underlying_container_type::value_type;

    private:
        alignas(FE::SIMD_auto_alignment::size) std::array<var::byte, ChunkCapacity> m_memory;
        /*
         std::pair's first contains the address of the memory block.
         std::pair's second contains the size of the memory block.
        */

    public:
        recycler_type _free_blocks;
        var::byte* const _begin = m_memory.data();
        var::byte* _page_iterator = _begin;
        var::byte* const _end = _begin + m_memory.size();

        _FORCE_INLINE_ boolean is_full() const noexcept
        {
            return (_free_blocks._begin_tree.is_empty() == true) && (_page_iterator >= _end);
        }
    };
}


template<size_t ChunkCapacity, class Alignment>
struct generic_deleter_base
{
    template<typename T, POOL_TYPE PoolType, size_t ChunkCapacity, class Alignment, class GlobalAllocator, class NamespaceAllocator>
    friend class pool;

    using chunk_type = internal::pool::chunk<void, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment>;
    using block_info_type = typename chunk_type::block_info_type;

    constexpr static size_t chunk_capacity = ChunkCapacity;
    constexpr static size_t temporary_storage_capacity = ChunkCapacity / Alignment::size;

protected:
    chunk_type* m_host_chunk = nullptr;
    var::size_t m_size_in_bytes = 0;

public:
    _FORCE_INLINE_ generic_deleter_base() noexcept : m_host_chunk(), m_size_in_bytes() {}
    _FORCE_INLINE_ generic_deleter_base(chunk_type* host_p, size_t size_in_bytes_p) noexcept : m_host_chunk(host_p), m_size_in_bytes(size_in_bytes_p) {}

    _FORCE_INLINE_ chunk_type* get_host_chunk_ptr() const noexcept
    {
		return this->m_host_chunk;
	}

    _FORCE_INLINE_ size_t get_size_in_bytes() const noexcept
    {
        return this->m_size_in_bytes;
    }
};


template<size_t ChunkCapacity, class Alignment, class GlobalAllocator, class NamespaceAllocator>
struct nondestructive_generic_deleter final : public generic_deleter_base<ChunkCapacity, Alignment>
{
    using base_type = generic_deleter_base<ChunkCapacity, Alignment>;
    using chunk_type = typename base_type::chunk_type;
    using block_info_type = typename base_type::block_info_type;

    _FORCE_INLINE_ nondestructive_generic_deleter() noexcept : base_type() {}
    _FORCE_INLINE_ nondestructive_generic_deleter(chunk_type* host_p, size_t size_in_bytes_p) noexcept : base_type(host_p, size_in_bytes_p) {}

    void operator()(void* ptr_p) const noexcept
    {
        if (ptr_p == nullptr || this->m_host_chunk == nullptr) { return; }

        FE_ASSERT(this->m_size_in_bytes == 0, "Assertion Failed: ${%s@0} cannot be zero.", TO_STRING(this->m_size_in_bytes));

        auto l_begin_tree_insertion_result = this->m_host_chunk->_free_blocks._begin_tree.insert(block_info_type{ static_cast<var::byte*>(ptr_p), this->m_size_in_bytes });
        FE_EXIT(l_begin_tree_insertion_result.second == false, FE::MEMORY_ERROR_1XX::_FATAL_ERROR_DOUBLE_FREE, "Double-free detected.");

        auto l_end_tree_insertion_result = this->m_host_chunk->_free_blocks._end_tree.insert(block_info_type{ static_cast<var::byte*>(ptr_p) + this->m_size_in_bytes,  this->m_size_in_bytes });
        FE_EXIT(l_end_tree_insertion_result.second == false, FE::MEMORY_ERROR_1XX::_FATAL_ERROR_DOUBLE_FREE, "Double-free detected.");

        if (this->m_host_chunk->_free_blocks._begin_tree.size() > 1)
        {
            pool<void, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment, GlobalAllocator, NamespaceAllocator>::__merge(l_begin_tree_insertion_result.first, this->m_host_chunk->_free_blocks);
        }
    }
};


template<typename T, size_t ChunkCapacity, class Alignment, class GlobalAllocator, class NamespaceAllocator>
struct pool_deleter<T, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment, GlobalAllocator, NamespaceAllocator> final : public generic_deleter_base<ChunkCapacity, Alignment>
{
    FE_STATIC_ASSERT(std::is_array<T>::value == true, "Static Assertion Failed: The T must not be an array[] type.");
    FE_STATIC_ASSERT(std::is_const<T>::value == true, "Static Assertion Failed: The T must not be a const type.");

    using base_type = generic_deleter_base<ChunkCapacity, Alignment>;
    using chunk_type = typename base_type::chunk_type;
    using block_info_type = typename base_type::block_info_type;
    using value_type = typename FE::remove_const_reference<T>::type;

private:
    var::count_t m_element_count = 0;

public:
    _FORCE_INLINE_ pool_deleter() noexcept : base_type() {}
    _FORCE_INLINE_ pool_deleter(chunk_type* host_p, count_t elements_p, size_t size_in_bytes_p) noexcept : base_type(host_p, size_in_bytes_p), m_element_count(elements_p) {}

    void operator()(void* ptr_p) const noexcept
    {
        FE_ASSERT(this->m_size_in_bytes < this->m_element_count, "Assertion Failed: Detected arguments with illegal values.");

        if (ptr_p == nullptr || this->m_host_chunk == nullptr) { return; }
     
        if constexpr (FE::is_trivial<value_type>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
        {
            value_type* l_ptr = static_cast<value_type*>(ptr_p);
            void* const l_ptr_end = static_cast<value_type*>(ptr_p) + this->m_element_count;

            while (l_ptr != l_ptr_end)
            {
                l_ptr->~value_type();
                ++l_ptr;
            }
        }

        FE_ASSERT(this->m_size_in_bytes == 0, "FATAL ERROR: ${%s@0} cannot be zero.", TO_STRING(this->m_size_in_bytes));
        
        auto l_begin_tree_insertion_result = this->m_host_chunk->_free_blocks._begin_tree.insert(block_info_type{ static_cast<var::byte*>(ptr_p), this->m_size_in_bytes });
        FE_EXIT(l_begin_tree_insertion_result.second == false, FE::MEMORY_ERROR_1XX::_FATAL_ERROR_DOUBLE_FREE, "Double-free detected.");

        auto l_end_tree_insertion_result = this->m_host_chunk->_free_blocks._end_tree.insert(block_info_type{ static_cast<var::byte*>(ptr_p) + this->m_size_in_bytes,  this->m_size_in_bytes });
        FE_EXIT(l_end_tree_insertion_result.second == false, FE::MEMORY_ERROR_1XX::_FATAL_ERROR_DOUBLE_FREE, "Double-free detected.");

        if (this->m_host_chunk->_free_blocks._begin_tree.size() > 1)
        {
            pool<void, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment, GlobalAllocator, NamespaceAllocator>::__merge(l_begin_tree_insertion_result.first, this->m_host_chunk->_free_blocks);
        }
    }

    _FORCE_INLINE_ count_t get_element_count() const noexcept
    {
        return this->m_element_count;
    }
};




template<size_t ChunkCapacity, class Alignment, class GlobalAllocator, class NamespaceAllocator>
class pool<void, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment, GlobalAllocator, NamespaceAllocator>
{
    template<typename T, POOL_TYPE PoolType, size_t ChunkCapacity, class Alignment, class GlobalAllocator, class NamespaceAllocator>
    friend struct pool_deleter;

    template<size_t ChunkCapacity, class Alignment, class GlobalAllocator, class NamespaceAllocator>
    friend struct nondestructive_generic_deleter;

public:
    using chunk_type = internal::pool::chunk<void, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment>;
    using recycler_type = typename chunk_type::recycler_type;
    using recycler_iterator = typename chunk_type::recycler_iterator;

    template<typename U>
    using deleter_type = pool_deleter<U, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment, GlobalAllocator, NamespaceAllocator>;

    using global_pool_type = std::list<chunk_type, GlobalAllocator>;
    using namespace_pool_type = std::unordered_multimap<FE::memory_region_t, chunk_type, FE::hash<FE::memory_region_t>, std::equal_to<FE::memory_region_t>, NamespaceAllocator>;

    using block_info_type = typename chunk_type::block_info_type;

    constexpr static size_t chunk_capacity = chunk_type::chunk_capacity;
    constexpr static size_t recycler_capacity = chunk_type::recycler_capacity;

private:

    typename global_pool_type m_global_memory;
    typename namespace_pool_type m_memory_regions;

public:
    pool() noexcept : m_global_memory(), m_memory_regions() {}
    ~pool() noexcept = default;

    pool(const pool& other_p) noexcept = delete;
    pool(pool&& rvalue) noexcept = delete;

    pool& operator=(const pool& other_p) noexcept = delete;
    pool& operator=(pool&& rvalue) noexcept = delete;

    template<typename U>
    std::unique_ptr<U, deleter_type<U>> allocate(count_t size_p = 1) noexcept
    {
        FE_STATIC_ASSERT((Alignment::size % 2) != 0, "Static Assertion Failed: The Alignment::size must be an even number.");
        FE_STATIC_ASSERT(std::is_array<U>::value == true, "Static Assertion Failed: The T must not be an array[] type.");
        FE_STATIC_ASSERT(std::is_const<U>::value == true, "Static Assertion Failed: The T must not be a const type.");
        FE_ASSERT(size_p == 0, "${%s@0}: ${%s@1} was 0", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(size_p));
        FE_EXIT(size_p > ChunkCapacity, MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY, "Fatal Error: Unable to allocate the size of memmory that exceeds the pool chunk's capacity.");


        size_t l_queried_allocation_size_in_bytes = FE::calculate_aligned_memory_size_in_bytes<U, Alignment>(size_p);

        if ((this->m_global_memory.empty() == true))
        {
            create_pages(1);
        }

        typename global_pool_type::iterator l_list_iterator = this->m_global_memory.begin();
        typename global_pool_type::const_iterator l_cend = this->m_global_memory.cend();

        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            if (l_list_iterator->is_full() == false)
            {
                void* l_allocation_result = nullptr;
                internal::pool::block_info<void, POOL_TYPE::_GENERIC> l_memblock_info;

                if (l_list_iterator->_free_blocks._begin_tree.is_empty() == false)
                {
                    /*
                    first contains the address of the memory block.
                    second contains the size of the memory block.
                    */

                    __recycle<U>(l_memblock_info, *l_list_iterator, l_queried_allocation_size_in_bytes);

                    if (l_memblock_info._size_in_bytes < l_queried_allocation_size_in_bytes)
                    {
                        if ((l_list_iterator->_page_iterator + l_queried_allocation_size_in_bytes) >= l_list_iterator->_end)
                        {
                            create_pages(1);
                            continue;
                        }

                        l_allocation_result = l_list_iterator->_page_iterator;
                        l_list_iterator->_page_iterator += l_queried_allocation_size_in_bytes;
                    }
                    else
                    {
                        l_allocation_result = l_memblock_info._address;
                    }
                }
                else
                {
                    if ((l_list_iterator->_page_iterator + l_queried_allocation_size_in_bytes) >= l_list_iterator->_end)
                    {
                        create_pages(1);
                        continue;
                    }

                    l_allocation_result = l_list_iterator->_page_iterator;
                    l_list_iterator->_page_iterator += l_queried_allocation_size_in_bytes;
                }


                if constexpr (FE::is_trivial<U>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
                {
                    U* l_iterator = static_cast<U*>(l_allocation_result);
                    for (var::index_t i = 0; i < size_p; ++i)
                    {
                        new(l_iterator) U();
                        ++l_iterator;
                    }
                }


                if (l_list_iterator->_page_iterator > l_list_iterator->_end)
                {
                    create_pages(1);
                    continue;
                }

                return std::unique_ptr<U, deleter_type<U>>{static_cast<U*>(l_allocation_result), deleter_type<U>{reinterpret_cast<chunk_type*>(l_list_iterator.operator->()), size_p, l_queried_allocation_size_in_bytes}};
            }   
            else
            {
                create_pages(1);
            }
        }

        create_pages(1);
        return allocate<U>(size_p);
    }

    _FORCE_INLINE_ void create_pages(size_t chunk_count_p) noexcept
    {
        FE_ASSERT(chunk_count_p == 0, "${%s@0}: ${%s@1} was 0", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(chunk_count_p));

        for (var::size_t i = 0; i < chunk_count_p; ++i)
        {
            this->m_global_memory.emplace_back();
        }
    }

    void shrink_to_fit() noexcept
    {
        typename global_pool_type::iterator  l_list_iterator = this->m_global_memory.begin();
        typename global_pool_type::const_iterator l_cend = this->m_global_memory.cend();
        FE_ASSERT(l_list_iterator == l_cend, "Unable to shrink_to_fit() an empty pool.");
        
        /*
                  first contains the address of the memory block.
                  second contains the size of the memory block.
        */

        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            var::size_t l_unused_memory_size_in_bytes = 0;
            for (auto block : l_list_iterator->_free_blocks._begin_tree)
            {
                l_unused_memory_size_in_bytes += block.second;
            }

            if (l_list_iterator->_page_iterator < l_list_iterator->_end)
            {
                l_unused_memory_size_in_bytes += l_list_iterator->_end - l_list_iterator->_page_iterator;
            }

            if (l_unused_memory_size_in_bytes == ChunkCapacity)
            {
                this->m_global_memory.erase(l_list_iterator);

                if (this->m_global_memory.size() == 0)
                {
                    break;
                }

                l_list_iterator = this->m_global_memory.begin();
            }
        }
    }

    template <typename T>
    void deallocate(T* pointer_p, count_t element_count_p) noexcept 
    {
        typename global_pool_type::iterator  l_list_iterator = this->m_global_memory.begin();
        typename global_pool_type::const_iterator l_cend = this->m_global_memory.cend();
        FE_ASSERT(l_list_iterator == l_cend, "Unable to request deallocate() to an empty pool.");


        var::byte* const l_value = reinterpret_cast<var::byte*>(pointer_p);

        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            if ((l_list_iterator->_begin <= l_value) && (l_value < l_list_iterator->_end))
            {
                if constexpr (FE::is_trivial<T>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
                {
                    for (var::count_t i = 0; i < element_count_p; ++i)
                    {
                        pointer_p->~T();
                        ++pointer_p;
                    }
                }

                size_t l_block_size_in_bytes = FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(element_count_p);
                auto l_begin_tree_insertion_result = l_list_iterator->_free_blocks._begin_tree.insert(block_info_type{ l_value, l_block_size_in_bytes });
                FE_EXIT(l_begin_tree_insertion_result.second == false, FE::MEMORY_ERROR_1XX::_FATAL_ERROR_DOUBLE_FREE, "Double-free detected.");

                auto l_end_tree_insertion_result = l_list_iterator->_free_blocks._end_tree.insert(block_info_type{ l_value + l_block_size_in_bytes, l_block_size_in_bytes });
                FE_EXIT(l_end_tree_insertion_result.second == false, FE::MEMORY_ERROR_1XX::_FATAL_ERROR_DOUBLE_FREE, "Double-free detected.");

                if (l_list_iterator->_free_blocks._begin_tree.size() > 1)
                {
                    __merge(l_begin_tree_insertion_result.first, l_list_iterator->_free_blocks);
                }
                return;
            }
        }
    }






    template<typename U>
    std::unique_ptr<U, deleter_type<U>> allocate(const char* region_name_p, count_t size_p = 1) noexcept
    {
        FE_STATIC_ASSERT((Alignment::size % 2) != 0, "Static Assertion Failed: The Alignment::size must be an even number.");
        FE_STATIC_ASSERT(std::is_array<U>::value == true, "Static Assertion Failed: The T must not be an array[] type.");
        FE_STATIC_ASSERT(std::is_const<U>::value == true, "Static Assertion Failed: The T must not be a const type.");
        FE_ASSERT(size_p == 0, "${%s@0}: ${%s@1} was 0", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(size_p));
        FE_EXIT(size_p > ChunkCapacity, MEMORY_ERROR_1XX::_FATAL_ERROR_OUT_OF_CAPACITY, "Fatal Error: Unable to allocate the size of memmory that exceeds the pool chunk's capacity.");


        size_t l_queried_allocation_size_in_bytes = FE::calculate_aligned_memory_size_in_bytes<U, Alignment>(size_p);

        if ((this->m_memory_regions.empty() == true))
        {
            create_pages(region_name_p, 1);
        }

        index_t l_bucket_index = this->m_memory_regions.bucket(region_name_p);
        typename namespace_pool_type::iterator l_list_iterator = this->m_memory_regions.begin(l_bucket_index);
        typename namespace_pool_type::const_iterator l_cend = this->m_memory_regions.cend(l_bucket_index);

        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            if (l_list_iterator->second.is_full() == false)
            {
                void* l_allocation_result = nullptr;
                internal::pool::block_info<void, POOL_TYPE::_GENERIC> l_memblock_info;

                if (l_list_iterator->second._free_blocks._begin_tree.is_empty() == false)
                {
                    /*
                    first contains the address of the memory block.
                    second contains the size of the memory block.
                    */

                    __recycle<U>(l_memblock_info, l_list_iterator->second, l_queried_allocation_size_in_bytes);

                    if (l_memblock_info._size_in_bytes < l_queried_allocation_size_in_bytes)
                    {
                        if ((l_list_iterator->second._page_iterator + l_queried_allocation_size_in_bytes) >= l_list_iterator->second._end)
                        {
                            create_pages(region_name_p, 1);
                            continue;
                        }

                        l_allocation_result = l_list_iterator->second._page_iterator;
                        l_list_iterator->second._page_iterator += l_queried_allocation_size_in_bytes;
                    }
                    else
                    {
                        l_allocation_result = l_memblock_info._address;
                    }
                }
                else
                {
                    if ((l_list_iterator->second._page_iterator + l_queried_allocation_size_in_bytes) >= l_list_iterator->second._end)
                    {
                        create_pages(region_name_p, 1);
                        continue;
                    }

                    l_allocation_result = l_list_iterator->second._page_iterator;
                    l_list_iterator->second._page_iterator += l_queried_allocation_size_in_bytes;
                }


                if constexpr (FE::is_trivial<U>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
                {
                    U* l_iterator = static_cast<U*>(l_allocation_result);
                    for (var::index_t i = 0; i < size_p; ++i)
                    {
                        new(l_iterator) U();
                        ++l_iterator;
                    }
                }


                if (l_list_iterator->second._page_iterator > l_list_iterator->second._end)
                {
                    create_pages(region_name_p, 1);
                    continue;
                }

                return std::unique_ptr<U, deleter_type<U>>{static_cast<U*>(l_allocation_result), deleter_type<U>{reinterpret_cast<chunk_type*>(&(l_list_iterator->second)), size_p, l_queried_allocation_size_in_bytes}};
            }
            else
            {
                create_pages(region_name_p, 1);
            }
        }

        create_pages(region_name_p, 1);
        return allocate<U>(region_name_p, size_p);
    }

    _FORCE_INLINE_ void create_pages(const char* region_name_p, size_t chunk_count_p) noexcept
    {
        FE_ASSERT(chunk_count_p == 0, "${%s@0}: ${%s@1} was 0", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_INVALID_SIZE), TO_STRING(chunk_count_p));

        static chunk_type l_s_initial_value;
        for (var::size_t i = 0; i < chunk_count_p; ++i)
        {
            this->m_memory_regions.emplace(region_name_p, l_s_initial_value);
        }
    }

    void shrink_to_fit(const char* region_name_p) noexcept
    {
        index_t l_bucket_index = this->m_memory_regions.bucket(region_name_p);
        typename namespace_pool_type::iterator  l_list_iterator = this->m_memory_regions.begin(l_bucket_index);
        typename namespace_pool_type::const_iterator l_cend = this->m_memory_regions.cend(l_bucket_index);
        FE_ASSERT(l_list_iterator == l_cend, "Unable to shrink_to_fit() an empty pool.");

        /*
                  first contains the address of the memory block.
                  second contains the size of the memory block.
        */

        var::size_t l_unused_memory_size_in_bytes = 0;
        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            for (auto block : l_list_iterator->second._free_blocks._begin_tree)
            {
                l_unused_memory_size_in_bytes += block.second;
            }

            if (l_list_iterator->second._page_iterator < l_list_iterator->second._end)
            {
                l_unused_memory_size_in_bytes += l_list_iterator->second._end - l_list_iterator->second._page_iterator;
            }
        }

        if (l_unused_memory_size_in_bytes == (ChunkCapacity * this->m_memory_regions.bucket_size(l_bucket_index)))
        {
            this->m_memory_regions.erase(this->m_memory_regions.find(region_name_p));
        }
    }

    template <typename T>
    void deallocate(const char* region_name_p, T* pointer_p, count_t element_count_p) noexcept
    {
        index_t l_bucket_index = this->m_memory_regions.bucket(region_name_p);
        typename namespace_pool_type::iterator  l_list_iterator = this->m_memory_regions.begin(l_bucket_index);
        typename namespace_pool_type::const_iterator l_cend = this->m_memory_regions.cend(l_bucket_index);
        FE_ASSERT(l_list_iterator == l_cend, "Unable to request deallocate() to an empty pool.");


        var::byte* const l_value = reinterpret_cast<var::byte*>(pointer_p);

        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            if ((l_list_iterator->second._begin <= l_value) && (l_value < l_list_iterator->second._end))
            {
                if constexpr (FE::is_trivial<T>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
                {
                    for (var::count_t i = 0; i < element_count_p; ++i)
                    {
                        pointer_p->~T();
                        ++pointer_p;
                    }
                }

                size_t l_block_size_in_bytes = FE::calculate_aligned_memory_size_in_bytes<T, Alignment>(element_count_p);
                auto l_begin_tree_insertion_result = l_list_iterator->second._free_blocks._begin_tree.insert(block_info_type{ l_value, l_block_size_in_bytes });
                FE_EXIT(l_begin_tree_insertion_result.second == false, FE::MEMORY_ERROR_1XX::_FATAL_ERROR_DOUBLE_FREE, "Double-free detected.");
                
                auto l_end_tree_insertion_result = l_list_iterator->second._free_blocks._end_tree.insert(block_info_type{ l_value + l_block_size_in_bytes, l_block_size_in_bytes });
                FE_EXIT(l_end_tree_insertion_result.second == false, FE::MEMORY_ERROR_1XX::_FATAL_ERROR_DOUBLE_FREE, "Double-free detected.");

                if (l_list_iterator->second._free_blocks._begin_tree.size() > 1)
                {
                    __merge(l_begin_tree_insertion_result.first, l_list_iterator->second._free_blocks);
                }
 
                return;
            }
        }
    }

private:
    /*
    FE.generic_pool

    allocation:


    deallocation:
  
    */
    static void __merge(recycler_iterator in_out_recently_deleted_p, recycler_type& in_out_free_block_list_p) noexcept
    {
        // merge upper part
        auto l_failed_to_find = in_out_free_block_list_p._begin_tree.cend();
        var::size_t l_merged_block_size_in_bytes = in_out_recently_deleted_p->second;
        var::byte* l_next_block_address = in_out_recently_deleted_p->first + l_merged_block_size_in_bytes;
        recycler_iterator l_search_result = in_out_free_block_list_p._begin_tree.find(l_next_block_address);

        if (l_failed_to_find != l_search_result)
        {
            do
            {
                recycler_iterator l_barrier = in_out_free_block_list_p._end_tree.find(l_next_block_address);
                in_out_free_block_list_p._end_tree.erase(l_barrier);

                l_merged_block_size_in_bytes += l_search_result->second;
                l_next_block_address += l_search_result->second;

                in_out_free_block_list_p._begin_tree.erase(l_search_result);
                l_search_result = in_out_free_block_list_p._begin_tree.find(l_next_block_address);
            } while (l_failed_to_find != l_search_result);

            in_out_recently_deleted_p->second = l_merged_block_size_in_bytes;
            in_out_free_block_list_p._end_tree.find(in_out_recently_deleted_p->first + l_merged_block_size_in_bytes)->second = l_merged_block_size_in_bytes;
        }

        // merge lower part
        l_search_result = in_out_free_block_list_p._end_tree.find(in_out_recently_deleted_p->first);

        l_failed_to_find = in_out_free_block_list_p._end_tree.cend();

        if (l_failed_to_find != l_search_result)
        {
            l_next_block_address = l_search_result->first;
            do
            {
                recycler_iterator l_barrier = in_out_free_block_list_p._begin_tree.find(l_next_block_address);
                in_out_free_block_list_p._begin_tree.erase(l_barrier);

                l_merged_block_size_in_bytes += l_search_result->second;
                l_next_block_address -= l_search_result->second;

                in_out_free_block_list_p._end_tree.erase(l_search_result);
                l_search_result = in_out_free_block_list_p._end_tree.find(l_next_block_address);
            } while (l_failed_to_find != l_search_result);

            in_out_free_block_list_p._begin_tree.find(l_next_block_address)->second = l_merged_block_size_in_bytes;
            in_out_free_block_list_p._end_tree.find(l_next_block_address + l_merged_block_size_in_bytes)->second = l_merged_block_size_in_bytes;
        }
    }

    template <typename T>
    static void __recycle(internal::pool::block_info<void, POOL_TYPE::_GENERIC>& out_memblock_info_p, chunk_type& in_out_memory_p, size_t queried_allocation_size_in_bytes_p) noexcept
    {
        FE_ASSERT(in_out_memory_p._free_blocks._begin_tree.is_empty() == true, "Assertion Failure: Cannot recycle from an empty bin.");

        /*
                     first contains the address of the memory block.
                     second contains the size of the memory block.
        */

        auto l_cend = in_out_memory_p._free_blocks._begin_tree.cend();

        for (auto free_block_iterator = in_out_memory_p._free_blocks._begin_tree.begin(); free_block_iterator != l_cend; ++free_block_iterator)
        {
            if (free_block_iterator->second >= queried_allocation_size_in_bytes_p)
            {
                out_memblock_info_p._address = free_block_iterator->first;
                out_memblock_info_p._size_in_bytes = queried_allocation_size_in_bytes_p;
                auto l_leftover_address = free_block_iterator->first + queried_allocation_size_in_bytes_p;
                auto l_leftover_size = free_block_iterator->second - queried_allocation_size_in_bytes_p;
                auto l_end_of_block = in_out_memory_p._free_blocks._end_tree.find(free_block_iterator->first + free_block_iterator->second);
                
                if (l_leftover_size > 0)
                {
                    _MAYBE_UNUSED_ auto l_begin_tree_insertion_result = in_out_memory_p._free_blocks._begin_tree.insert(block_info_type{ l_leftover_address, l_leftover_size });
                    l_end_of_block->second = l_leftover_size;
                    FE_EXIT(l_begin_tree_insertion_result.second == false, FE::MEMORY_ERROR_1XX::_FATAL_ERROR_DOUBLE_FREE, "Double-free detected.");
                }
                in_out_memory_p._free_blocks._begin_tree.erase(free_block_iterator);
                
                if (l_leftover_size == 0)
                {
                    in_out_memory_p._free_blocks._end_tree.erase(l_end_of_block);
                }
                return;
            }
        }
    }
};


template<size_t ChunkCapacity = 512 MB, class Alignment = FE::align_8bytes, class GlobalAllocator = FE::aligned_allocator<internal::pool::chunk<void, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment>>, class NamespaceAllocator = FE::aligned_allocator<std::pair<const FE::memory_region_t, internal::pool::chunk<void, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment>>>>
using generic_pool = pool<void, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment, GlobalAllocator, NamespaceAllocator>;

template<typename T, size_t ChunkCapacity = 512 MB, class Alignment = FE::align_8bytes, class GlobalAllocator = FE::aligned_allocator<internal::pool::chunk<void, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment>>, class NamespaceAllocator = FE::aligned_allocator<std::pair<const FE::memory_region_t, internal::pool::chunk<void, POOL_TYPE::_GENERIC, ChunkCapacity, Alignment>>>>
using generic_pool_ptr = std::unique_ptr<T, pool_deleter<T, FE::POOL_TYPE::_GENERIC, ChunkCapacity, Alignment, GlobalAllocator, NamespaceAllocator>>;


END_NAMESPACE
#endif