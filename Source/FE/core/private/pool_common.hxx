#ifndef _FE_CORE_PRIVATE_POOL_COMMON_HXX_
#define _FE_CORE_PRIVATE_POOL_COMMON_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/allocator.hxx>
#include <FE/core/fstring.hxx>
#include <FE/core/hash.hpp>

// std
#include <memory>
#include <unordered_map>
#pragma warning (push)
#pragma warning (disable: 6262)



BEGIN_NAMESPACE(FE)


enum struct POOL_TYPE : uint8
{
    _BLOCK = 0,
    _GENERIC = 1,
};

using memory_region_t = FE::fstring<64>;


namespace internal::pool
{
    template <typename T, POOL_TYPE PoolType>
    struct block_info;

    template<typename T, POOL_TYPE PoolType, size_t ChunkCapacity, class Alignment>
    struct chunk;
}


template<typename T, POOL_TYPE PoolType, size_t ChunkCapacity, class Alignment, class GlobalAllocator, class NamespaceAllocator>
struct pool_deleter;

template<typename T, POOL_TYPE PoolType, size_t ChunkCapacity, class Alignment, class GlobalAllocator, class NamespaceAllocator>
class pool;


template<uint64 Capacity>
struct capacity final
{
    _MAYBE_UNUSED_ static constexpr inline size_t size = Capacity;
};

template<uint64 Count>
struct object_count final
{
    _MAYBE_UNUSED_ static constexpr inline size_t size = Count;
};


END_NAMESPACE
#pragma warning (pop)
#endif