#ifndef _FE_CORE_PRIVATE_POOL_COMMON_HXX_
#define _FE_CORE_PRIVATE_POOL_COMMON_HXX_
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
#include <FE/type_traits.hxx>

// std
#include <array>
#include <cstring>
#include <list>
#include <memory>
#pragma warning (push)




BEGIN_NAMESPACE(FE)


enum struct PoolType : uint8
{
    _Block = 0,
    _Scalable = 1,
};


namespace internal::pool
{
    struct block_info
    {
        var::byte* _address;
        var::int64 _size_in_bytes;
    };

    template<PoolType PoolType, class Alignment>
    class chunk;
}


template<PoolType PoolType, class Alignment>
class pool;


template<FE::uint64 Capacity>
struct capacity final
{
    _FE_MAYBE_UNUSED_ static constexpr inline FE::size size = Capacity;
};

template<FE::uint64 Count>
struct object_count final
{
    _FE_MAYBE_UNUSED_ static constexpr inline FE::size size = Count;
};

template<FE::uint64 SizeInBytes>
struct size_in_bytes final
{
    _FE_MAYBE_UNUSED_ static constexpr inline FE::size size = SizeInBytes;
};


END_NAMESPACE
#pragma warning (pop)
#endif