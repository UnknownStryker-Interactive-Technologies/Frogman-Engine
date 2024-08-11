#ifndef _FE_CORE_PRIVATE_POOL_COMMON_HXX_
#define _FE_CORE_PRIVATE_POOL_COMMON_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/allocator.hxx>
#include <FE/type_traits.hxx>

// std
#include <array>
#include <cstring>
#include <list>
#include <map>
#include <memory>
#pragma warning (push)

// boost
#include <boost/smart_ptr/intrusive_ptr.hpp>




BEGIN_NAMESPACE(FE)


enum struct POOL_TYPE : uint8
{
    _STATIC = 0,
    _DYNAMIC = 1,
};


namespace internal::pool
{
    struct block_info
    {
        var::byte* _address;
        var::size _size_in_bytes;
    };

    template<POOL_TYPE PoolType, size PageCapacity, class Alignment>
    struct chunk;
}


template<POOL_TYPE PoolType, size PageCapacity, class Alignment, class Allocator>
class pool;


template<uint64 Capacity>
struct capacity final
{
    _MAYBE_UNUSED_ static constexpr inline size size = Capacity;
};

template<uint64 Count>
struct object_count final
{
    _MAYBE_UNUSED_ static constexpr inline size size = Count;
};

template<uint64 SizeInBytes>
struct size_in_bytes final
{
    _MAYBE_UNUSED_ static constexpr inline size size = SizeInBytes;
};


END_NAMESPACE
#pragma warning (pop)
#endif