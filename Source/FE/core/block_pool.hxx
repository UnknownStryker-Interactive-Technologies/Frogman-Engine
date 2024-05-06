#ifndef _FE_CORE_BLOCK_POOL_HXX_
#define _FE_CORE_BLOCK_POOL_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/private/pool_common.hxx>
#include <FE/core/containers/stack.hxx>




BEGIN_NAMESPACE(FE)


namespace internal::pool
{
    template <typename T>
    struct block_info<T, POOL_TYPE::_BLOCK>
    {
        using value_type = T;
        using pointer = value_type*;

        pointer _address = nullptr;
    };


    template <size_t InBytes>
    class uninitialized_bytes
    {
        var::byte m_memory[InBytes];
    };


    template<typename T, size_t ChunkCapacity>
    struct chunk<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>>
    {
        FE_STATIC_ASSERT(std::is_array<T>::value == true, "Static Assertion Failed: The T must not be an array[] type.");
        FE_STATIC_ASSERT(std::is_const<T>::value == true, "Static Assertion Failed: The T must not be a const type.");

        using block_info_type = block_info<T, POOL_TYPE::_BLOCK>;
        using value_type = typename block_info_type::value_type;
        using pointer = typename block_info_type::pointer;

    private:
        alignas(FE::SIMD_auto_alignment::size) std::array<var::byte, sizeof(T)* ChunkCapacity> m_memory;

    public:
        constexpr static size_t chunk_capacity = ChunkCapacity;

        FE::fstack<block_info_type, ChunkCapacity> _free_blocks;
        pointer const _begin = reinterpret_cast<pointer const>(m_memory.data());
        pointer _page_iterator = _begin;
        pointer const _end = _begin + ChunkCapacity;

        _FORCE_INLINE_ boolean is_full() const noexcept
        {
            return (_free_blocks.is_empty() == true) && (_page_iterator >= _end);
        }
    };
}


template<typename T, size_t ChunkCapacity, class GlobalAllocator, class NamespaceAllocator>
struct pool_deleter<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>, GlobalAllocator, NamespaceAllocator>
{
    FE_STATIC_ASSERT(std::is_array<T>::value == true, "Static Assertion Failed: The T must not be an array[] type.");
    FE_STATIC_ASSERT(std::is_const<T>::value == true, "Static Assertion Failed: The T must not be a const type.");

    using chunk_type = internal::pool::chunk<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>>;
    using value_type = typename FE::remove_const_reference<typename chunk_type::value_type>::type;
    using block_info_type = typename chunk_type::block_info_type;

private:
    chunk_type* m_host_chunk = nullptr;

public:
    constexpr static count_t chunk_capacity = ChunkCapacity;

    _FORCE_INLINE_ pool_deleter() noexcept : m_host_chunk() {}
    _FORCE_INLINE_ pool_deleter(chunk_type* host_p) noexcept : m_host_chunk(host_p) {}

    _FORCE_INLINE_ void operator()(value_type* ptr_p) const noexcept
    {
        FE_ASSERT(this->m_host_chunk == nullptr, "${%s@0}: ${%s@1} was nullptr", TO_STRING(MEMORY_ERROR_1XX::_FATAL_ERROR_NULLPTR), TO_STRING(this->m_host_chunk));
        if (ptr_p == nullptr || this->m_host_chunk == nullptr) { return; }

        if constexpr (FE::is_trivial<value_type>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
        {
            ptr_p->~value_type();
        }

        this->m_host_chunk->_free_blocks.push(block_info_type{ ptr_p });
    }
};


template<typename T, size_t ChunkCapacity, class GlobalAllocator, class NamespaceAllocator>
class pool<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>, GlobalAllocator, NamespaceAllocator>
{
    FE_STATIC_ASSERT(std::is_array<T>::value == true, "Static Assertion Failed: The T must not be an array[] type.");
    FE_STATIC_ASSERT(std::is_const<T>::value == true, "Static Assertion Failed: The T must not be a const type.");

public:
                    // internal::pool::chunk<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>>
    using chunk_type = internal::pool::chunk<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>>;
    using deleter_type = pool_deleter<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>, GlobalAllocator, NamespaceAllocator>;
    using block_info_type = typename chunk_type::block_info_type;

    FE_STATIC_ASSERT((std::is_same<chunk_type, typename GlobalAllocator::value_type>::value == false), "Static Assertion Failed: The chunk_type has to be equivalent to GlobalAllocator::value_type.");
    using global_pool_type = std::list<chunk_type, GlobalAllocator>;

    FE_STATIC_ASSERT((std::is_same<std::pair<const FE::memory_region_t, chunk_type>, typename NamespaceAllocator::value_type>::value == false), "Static Assertion Failed: NamespaceAllocator::value_type has to be std::pair<const FE::memory_region_t, chunk_type>.");
    using namespace_pool_type = std::unordered_multimap<FE::memory_region_t, chunk_type, FE::hash<FE::memory_region_t>, std::equal_to<FE::memory_region_t>, NamespaceAllocator>;

    constexpr static count_t chunk_capacity = ChunkCapacity;

    FE_STATIC_ASSERT((std::is_same<T, typename chunk_type::value_type>::value == false), "Static Assertion Failed: The value_type does not match.");
    FE_STATIC_ASSERT((std::is_same<T*, typename chunk_type::pointer>::value == false), "Static Assertion Failed: The value_type* does not match.");
    
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

    std::unique_ptr<T, deleter_type> allocate() noexcept
    {
        if ((m_global_memory.empty() == true))
        {
            create_pages(1);
        }

        typename global_pool_type::iterator l_list_iterator = this->m_global_memory.begin();
        typename global_pool_type::const_iterator l_cend = this->m_global_memory.cend();

        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            if (l_list_iterator->is_full() == false)
            {
                T* l_value;
                if (l_list_iterator->_free_blocks.is_empty() == false)
                {
                    l_value = l_list_iterator->_free_blocks.pop()._address;

                    if constexpr (FE::is_trivial<T>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
                    {
                        new(l_value) T();
                    }
                }
                else
                {
                    l_value = l_list_iterator->_page_iterator;

                    if constexpr (FE::is_trivial<T>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
                    {
                        new(l_value) T();
                    }

                    ++(l_list_iterator->_page_iterator);
                }

                return std::unique_ptr<T, deleter_type>(l_value, deleter_type(l_list_iterator.operator->()));
            }
            else
            {
                create_pages(1);
                continue;
            }
        }

        create_pages(1);
        return allocate();
    }

    void deallocate(T* const pointer_p) noexcept
    {
        typename global_pool_type::iterator  l_list_iterator = this->m_global_memory.begin();
        typename global_pool_type::const_iterator l_cend = this->m_global_memory.cend();
        FE_ASSERT(l_list_iterator == l_cend, "Unable to request deallocate() to an empty pool.");


        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            if ((l_list_iterator->_begin <= pointer_p) && (pointer_p < l_list_iterator->_end))
            {
                if constexpr (FE::is_trivial<T>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
                {
                    pointer_p->~T();
                }

                l_list_iterator->_free_blocks.push(block_info_type{ pointer_p });
                return;
            }
        }
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
        typename global_pool_type::iterator l_list_iterator = this->m_global_memory.begin();
        typename global_pool_type::const_iterator l_cend = this->m_global_memory.cend();
        if (l_list_iterator == l_cend)
        {
            FE_ASSERT(l_list_iterator == l_cend, "Unable to shrink_to_fit() an empty pool.");
            return;
        }


        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            var::size_t l_unused_element_size = l_list_iterator->_free_blocks.size();

            FE_ASSERT((l_list_iterator->_end - l_list_iterator->_begin) != ChunkCapacity, "The chunk range is invalid.");

            if (l_list_iterator->_page_iterator < l_list_iterator->_end)
            {
                l_unused_element_size += (l_list_iterator->_end - l_list_iterator->_page_iterator);
            }

            if (l_unused_element_size == ChunkCapacity)
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






    std::unique_ptr<T, deleter_type> allocate(const char* region_name_p) noexcept
    {
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
                T* l_value;
                if (l_list_iterator->second._free_blocks.is_empty() == false)
                {
                    l_value = l_list_iterator->second._free_blocks.pop()._address;

                    if constexpr (FE::is_trivial<T>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
                    {
                        new(l_value) T();
                    }
                }
                else
                {
                    l_value = l_list_iterator->second._page_iterator;

                    if constexpr (FE::is_trivial<T>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
                    {
                        new(l_value) T();
                    }

                    ++(l_list_iterator->second._page_iterator);
                }

                return std::unique_ptr<T, deleter_type>(l_value, deleter_type(&(l_list_iterator->second)));
            }
            else
            {
                create_pages(region_name_p, 1);
                continue;
            }
        }

        create_pages(region_name_p, 1);
        return allocate(region_name_p);
    }

    void deallocate(const char* region_name_p, T* const pointer_p) noexcept
    {
        index_t l_bucket_index = this->m_memory_regions.bucket(region_name_p);
        typename namespace_pool_type::iterator  l_list_iterator = this->m_memory_regions.begin(l_bucket_index);
        typename namespace_pool_type::const_iterator l_cend = this->m_memory_regions.cend(l_bucket_index);
        FE_ASSERT(l_list_iterator == l_cend, "Unable to request deallocate() to an empty pool.");


        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            if ((l_list_iterator->second._begin <= pointer_p) && (pointer_p < l_list_iterator->second._end))
            {
                if constexpr (FE::is_trivial<T>::value == FE::TYPE_TRIVIALITY::_NOT_TRIVIAL)
                {
                    pointer_p->~T();
                }

                l_list_iterator->second._free_blocks.push(block_info_type{ pointer_p });
                return;
            }
        }
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
        typename namespace_pool_type::iterator l_list_iterator = this->m_memory_regions.begin(l_bucket_index);
        typename namespace_pool_type::const_iterator l_cend = this->m_memory_regions.cend(l_bucket_index);
        if (l_list_iterator == l_cend)
        {
            FE_ASSERT(true, "Unable to shrink_to_fit() an empty pool.");
            return;
        }


        for (; l_list_iterator != l_cend; ++l_list_iterator)
        {
            var::size_t l_unused_element_size = l_list_iterator->second._free_blocks.size();

            FE_ASSERT((l_list_iterator->second._end - l_list_iterator->second._begin) != ChunkCapacity, "The chunk range is invalid.");

            if (l_list_iterator->second._page_iterator < l_list_iterator->second._end)
            {
                l_unused_element_size += (l_list_iterator->second._end - l_list_iterator->second._page_iterator);
            }

            if (l_unused_element_size == ChunkCapacity)
            {
                this->m_memory_regions.erase(l_list_iterator);

                if (this->m_memory_regions.size() == 0)
                {
                    break;
                }

                l_list_iterator = this->m_memory_regions.begin(l_bucket_index);
            }
        }
    }
};


template<typename T, size_t ChunkCapacity = 128, class GlobalAllocator = FE::aligned_allocator<internal::pool::chunk<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>>>, class NamespaceAllocator = FE::aligned_allocator<std::pair<const FE::memory_region_t, internal::pool::chunk<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>>>>>
using block_pool = pool<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>, GlobalAllocator, NamespaceAllocator>;

template<typename T, size_t ChunkCapacity = 128, class GlobalAllocator = FE::aligned_allocator<internal::pool::chunk<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>>>, class NamespaceAllocator = FE::aligned_allocator<std::pair<const FE::memory_region_t, internal::pool::chunk<T, POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>>>>>
using block_pool_ptr = std::unique_ptr<T, pool_deleter<T, FE::POOL_TYPE::_BLOCK, ChunkCapacity, FE::align_custom_bytes<sizeof(T)>, GlobalAllocator, NamespaceAllocator>>;


END_NAMESPACE
#endif