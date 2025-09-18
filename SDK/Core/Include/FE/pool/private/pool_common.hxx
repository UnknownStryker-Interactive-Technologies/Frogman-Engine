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
#include <FE/do_once.hxx>
#include <FE/type_traits.hxx>
#include <FE/memory.hpp>

// std
#include <array>
#include <cstring>
#include <list>
#include <memory>

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
	_ConcurrentBlock = 2
};


namespace internal::pool
{
    struct block_info
    {
        var::byte* _address;
        var::int32 _size_in_bytes;
    };

    template<PoolType PoolType, class Alignment>
    class chunk;
}


template<PoolType PoolType, class Alignment>
class pool;


END_NAMESPACE
#endif