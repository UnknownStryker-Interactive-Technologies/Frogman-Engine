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
#include <FE/memory.hpp>
#include <FE/clock.hpp>


#ifdef _FE_ON_WINDOWS_X86_64_
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <psapi.h>
    #undef WIN32_LEAN_AND_MEAN
#elif defined(_FE_ON_LINUX_X86_64_)
    #include <sys/types.h>
    #include <sys/sysinfo.h>
#endif




void* operator new(std::size_t bytes_p)
{
	return FE_ALIGNED_ALLOC(bytes_p, FE::align_CPU_L1_cache_line::size);
}
void* operator new[](std::size_t bytes_p)
{
	return FE_ALIGNED_ALLOC(bytes_p, FE::align_CPU_L1_cache_line::size);
}

void operator delete(void* ptr_p) noexcept
{
	FE_ALIGNED_FREE(ptr_p);
}
void operator delete[](void* ptr_p) noexcept
{
	FE_ALIGNED_FREE(ptr_p);
}

void operator delete(void* ptr_p, std::size_t size_p) noexcept
{
	(void)size_p;
	FE_ALIGNED_FREE(ptr_p);
}

void operator delete[](void* ptr_p, std::size_t size_p) noexcept
{
	(void)size_p;
	FE_ALIGNED_FREE(ptr_p);
}
