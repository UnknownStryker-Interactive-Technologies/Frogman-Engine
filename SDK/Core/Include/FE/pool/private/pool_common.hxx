#ifndef _FE_CORE_PRIVATE_POOL_COMMON_HXX_
#define _FE_CORE_PRIVATE_POOL_COMMON_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/do_once.hxx>
#include <FE/hash.hxx>
#include <FE/list.hxx>
#include <FE/memory.hxx>
#include <FE/type_traits.hxx>

// third-party
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
#include <absl/container/flat_hash_set.h> // absl::flat_hash_set for page base pointer validation
#endif
#include <taskflow.hpp> // for parallel defragmentation

// std
#include <algorithm> //  std::sort() for defragmentation
#include <execution> // std::execution::par
#include <memory_resource> // std::pmr::memory_resource


#ifdef _FE_ON_WINDOWS_X86_64_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <memoryapi.h> // to use VirtualAlloc and VirtualFree
#endif




BEGIN_NAMESPACE(FE)


enum struct PoolType : uint8
{
    _Block = 0,
    _Scalable = 1,
	_Arena = 2
};

template<PoolType PoolType, class Alignment>
class pool;


namespace internal::pool
{
    template<PoolType PoolType, class Alignment>
    class chunk;

    struct block_info
    {
        var::byte* _address;
        var::uint32 _size_in_bytes;
    };

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
}


END_NAMESPACE
#endif