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

#ifdef _FE_ON_WINDOWS_X86_64_
#define WIN32_LEAN_AND_MEAN
// to use VirtualAlloc and VirtualFree
#include <Windows.h>
#endif




BEGIN_NAMESPACE(FE)


enum struct PoolType : uint8
{
    _Block = 0,
    _Scalable = 1,
	_ConcurrentBlock = 2
};


enum struct PoolPageCapacity : FE::uint64
{
    _4KB = 4096,
	_16KB = 16384,
	_64KB = 65536,
	_256KB = 262144,
	_1MB = 1048576,
	_4MB = 4194304,
	_16MB = 16777216,
	_64MB = 67108864,
	_256MB = 268435456,
	_1GB = 1073741824,
    _Max = (0x7fffffff - 31)
};


namespace internal::pool
{
    struct block_info
    {
        var::byte* _address;
        var::int64 _size_in_bytes;
    };

    template<PoolType PoolType, PoolPageCapacity PageCapacity, class Alignment>
    class chunk;

	//template<PoolType PoolType, PoolPageCapacity PageCapacity, class Alignment>
	//struct page_deleter
	//{
	//	_FE_FORCE_INLINE_ void operator()(chunk<PoolType, PageCapacity, Alignment>* ptr_p) const noexcept
	//	{
	//		ptr_p->~chunk<PoolType, PageCapacity, Alignment>();
	//		FE_EXIT(VirtualUnlock(ptr_p, sizeof(chunk<PoolType, PageCapacity, Alignment>)) == _FE_FAILED_, FE::ErrorCode::_FatalMemoryError_1XX_VirtualUnlockFailure, "Failed to VirtualUnlock() a memory page.");
	//		FE_EXIT(VirtualFree(ptr_p, 0, MEM_RELEASE) == _FE_FAILED_, FE::ErrorCode::_FatalMemoryError_1XX_VirtualFreeFailure, "Failed to VirtualFree() a memory page.");
	//	}
	//};

	//template<PoolType PoolType, PoolPageCapacity PageCapacity, class Alignment>
	//using page_ptr = std::unique_ptr<chunk<PoolType, PageCapacity, Alignment>, page_deleter<PoolType, PageCapacity, Alignment>>;

	//template<PoolType PoolType, PoolPageCapacity PageCapacity, class Alignment>
	//_FE_FORCE_INLINE_ page_ptr<PoolType, PageCapacity, Alignment> make_page() noexcept
	//{
	//	typename page_ptr<PoolType, PageCapacity, Alignment>::pointer l_virtual_alloc_result = (typename page_ptr<PoolType, PageCapacity, Alignment>::pointer)VirtualAlloc(nullptr, sizeof(typename page_ptr<PoolType, PageCapacity, Alignment>::element_type), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	//	FE_EXIT(VirtualLock( l_virtual_alloc_result, sizeof(typename page_ptr<PoolType, PageCapacity, Alignment>::element_type) ) == _FE_FAILED_, FE::ErrorCode::_FatalMemoryError_1XX_VirtualLockFailure, "Failed to VirtualLock() a memory page.");
	//	new(l_virtual_alloc_result) typename page_ptr<PoolType, PageCapacity, Alignment>::element_type();
	//	return page_ptr<PoolType, PageCapacity, Alignment>(l_virtual_alloc_result);
	//}
}


template<PoolType PoolType, PoolPageCapacity PageCapacity, class Alignment>
class pool;


END_NAMESPACE
#endif