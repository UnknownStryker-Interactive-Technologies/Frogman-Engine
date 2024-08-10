// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/private/allocator_base.hpp>
#include <FE/clock.hpp>


#ifdef _WINDOWS_X86_64_
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <psapi.h>
    #undef WIN32_LEAN_AND_MEAN
#elif defined(_LINUX_X86_64_)
    #include <sys/types.h>
    #include <sys/sysinfo.h>
#endif




var::uint64 FE::request_app_memory_utilization(const HEAP_MEMORY_UTIL_INFO select_data_p) noexcept
{
#ifdef _WINDOWS_X86_64_
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
	#ifdef _LINUX_X86_64_
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
	return FE_ALIGNED_ALLOC(bytes_p, FE::SIMD_auto_alignment::size);
}
void* operator new[](std::size_t bytes_p)
{
	return FE_ALIGNED_ALLOC(bytes_p, FE::SIMD_auto_alignment::size);
}

void operator delete(void* ptr_p) noexcept
{
	FE_ALIGNED_FREE(ptr_p);
}
void operator delete[](void* ptr_p) noexcept
{
	FE_ALIGNED_FREE(ptr_p);
}