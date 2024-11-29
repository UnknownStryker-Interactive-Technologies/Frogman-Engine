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
#include <FE/private/allocator_base.hpp>
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




var::uint64 FE::request_app_memory_utilization(const HEAP_MEMORY_UTIL_INFO select_data_p) noexcept
{
#ifdef _FE_ON_WINDOWS_X86_64_
	MEMORYSTATUSEX l_memory_information;
	l_memory_information.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&l_memory_information);

	PROCESS_MEMORY_COUNTERS_EX l_process_memory_counters;

	switch (select_data_p)
	{
	case HEAP_MEMORY_UTIL_INFO::_TOTAL_VIRTUAL_MEMORY_SIZE:
		return l_memory_information.ullTotalPageFile;

	case HEAP_MEMORY_UTIL_INFO::_TOTAL_VIRTUAL_MEMORY_UTIL:
		return l_memory_information.ullTotalPageFile - l_memory_information.ullAvailPageFile;

	case HEAP_MEMORY_UTIL_INFO::_THIS_PROCESS_VIRTUAL_MEMORY_UTIL:
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&l_process_memory_counters, sizeof(l_process_memory_counters));
		return l_process_memory_counters.PrivateUsage;

	case HEAP_MEMORY_UTIL_INFO::_TOTAL_PHYSICAL_MEMORY_SIZE:
		return l_memory_information.ullTotalPhys;

	case HEAP_MEMORY_UTIL_INFO::_TOTAL_PHYSICAL_MEMORY_UTIL:
		return l_memory_information.ullTotalPhys - l_memory_information.ullAvailPhys;

	case HEAP_MEMORY_UTIL_INFO::_THIS_PROCESS_PHYSICAL_MEMORY_UTIL:
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&l_process_memory_counters, sizeof(l_process_memory_counters));
		return l_process_memory_counters.WorkingSetSize;

	default:
		return invalid_memory_util_query;
	}

#else
	#ifdef _FE_ON_LINUX_X86_64_
	struct sysinfo l_memory_information;
	sysinfo(&l_memory_information);

	switch (select_data_p)
	{
	case HEAP_MEMORY_UTIL_INFO::_TOTAL_VIRTUAL_MEMORY_SIZE:
		return l_memory_information.totalram;

	case HEAP_MEMORY_UTIL_INFO::_TOTAL_VIRTUAL_MEMORY_UTIL:
		return l_memory_information.totalram - l_memory_information.freeram;

	case HEAP_MEMORY_UTIL_INFO::_THIS_PROCESS_VIRTUAL_MEMORY_UTIL:
		return l_memory_information.totalram - l_memory_information.freeram;

	case HEAP_MEMORY_UTIL_INFO::_TOTAL_PHYSICAL_MEMORY_SIZE:
		return l_memory_information.totalram;

	case HEAP_MEMORY_UTIL_INFO::_TOTAL_PHYSICAL_MEMORY_UTIL:
		return l_memory_information.totalram - l_memory_information.freeram;

	case HEAP_MEMORY_UTIL_INFO::_THIS_PROCESS_PHYSICAL_MEMORY_UTIL:
		return l_memory_information.totalram - l_memory_information.freeram;

	default:
		return invalid_memory_util_query;
	}
	#endif
#endif
}


std::atomic_int64_t FE::internal::allocator_base::s_total_memory_util = 0;
thread_local var::int64 FE::internal::allocator_base::tl_s_thread_local_memory_util = 0;

std::atomic_int64_t FE::internal::allocator_base::s_total_memory_pool_util = 0;
thread_local var::int64 FE::internal::allocator_base::tl_s_thread_local_memory_pool_util = 0;


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
