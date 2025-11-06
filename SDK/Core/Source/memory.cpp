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
#include <FE/memory.hxx>
#include <FE/clock.hxx>


#ifdef _FE_ON_WINDOWS_X86_64_
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <psapi.h>
    #undef WIN32_LEAN_AND_MEAN
#elif defined(_FE_ON_LINUX_X86_64_)
    #include <sys/types.h>
    #include <sys/sysinfo.h>
#endif




_FE_MAYBE_UNUSED_ FE::uint64 FE::system_page_size = []() -> FE::uint64
	{
		SYSTEM_INFO l_sysinfo;
		GetSystemInfo(&l_sysinfo);
		return l_sysinfo.dwPageSize;
	}();

_FE_MAYBE_UNUSED_ FE::uint64 FE::system_large_page_size = []() -> FE::uint64
	{
		SYSTEM_INFO l_sysinfo;
		GetSystemInfo(&l_sysinfo);
		return (GetLargePageMinimum() == 0) ? l_sysinfo.dwPageSize : GetLargePageMinimum();
	}();


_FE_NODISCARD_ void* _FE_CDECL_ operator new(size_t bytes_p)
{
	FE_ASSERT(bytes_p != 0, "Allocating zero byte is not allowed.");
	return FE_ALIGNED_ALLOC(bytes_p, FE::CPU_L1_cache_line::size);
}
_FE_NODISCARD_ void* _FE_CDECL_ operator new[](size_t bytes_p)
{
	FE_ASSERT(bytes_p != 0, "Allocating zero byte is not allowed.");
	return FE_ALIGNED_ALLOC(bytes_p, FE::CPU_L1_cache_line::size);
}

void _FE_CDECL_ operator delete(void* ptr_p) noexcept
{
	FE_ALIGNED_FREE(ptr_p);
}
void _FE_CDECL_ operator delete[](void* ptr_p) noexcept
{
	FE_ALIGNED_FREE(ptr_p);
}

void _FE_CDECL_ operator delete(void* ptr_p, std::size_t size_p) noexcept
{
	(void)size_p;
	FE_ALIGNED_FREE(ptr_p);
}
void _FE_CDECL_ operator delete[](void* ptr_p, std::size_t size_p) noexcept
{
	(void)size_p;
	FE_ALIGNED_FREE(ptr_p);
}


void* FE::cache_aligned_resource::do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	FE_ASSERT(bytes_p != 0, "Allocating zero byte is not allowed.");
	return FE_ALIGNED_ALLOC(bytes_p, FE::CPU_L1_cache_line::size);
}

void FE::cache_aligned_resource::do_deallocate(void* ptr_p, _FE_MAYBE_UNUSED_ std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p) noexcept
{
	FE_ASSERT(bytes_p != 0, "Deallocating zero byte is not allowed.");
	FE_ALIGNED_FREE(ptr_p);
}

bool FE::cache_aligned_resource::do_is_equal(const std::pmr::memory_resource& other_p) const noexcept
{
	return this == &other_p;
}
