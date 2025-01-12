#ifndef _FE_CORE_PRIVATE_ALLOCATOR_BASE_HPP_
#define _FE_CORE_PRIVATE_ALLOCATOR_BASE_HPP_
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
#include <FE/memory.hxx>

// std
#include <cstdlib>
#include <memory>
#include <thread>




#ifdef FE_ALIGNED_ALLOC
	#error FE_ALIGNED_ALLOC is a reserved Frogman Engine macro function.
#endif
#ifdef FE_ALIGNED_FREE
	#error FE_ALIGNED_FREE is a reserved Frogman Engine macro function.
#endif
#ifdef _FE_ON_WINDOWS_X86_64_
	#define FE_ALIGNED_ALLOC(size_p, alignment_p) ::_aligned_malloc(size_p, alignment_p)
	#define FE_ALIGNED_FREE(ptr_to_memory_p) ::_aligned_free(ptr_to_memory_p)
#elif defined(_FE_ON_LINUX_X86_64_)
	#define FE_ALIGNED_ALLOC(size_p, alignment_p) _mm_malloc(size_p, alignment_p)
	#define FE_ALIGNED_FREE(ptr_to_memory_p) _mm_free(ptr_to_memory_p)
#endif




BEGIN_NAMESPACE(FE)


_FE_MAYBE_UNUSED_ constexpr var::uint64 invalid_memory_util_query = max_value<var::uint64>;

_FE_MAYBE_UNUSED_ constexpr FE::uint64 one_kb = 1024;
_FE_MAYBE_UNUSED_ constexpr FE::uint64 one_mb = 1048576;
_FE_MAYBE_UNUSED_ constexpr FE::uint64 one_gb = 1073741824;

enum struct HEAP_MEMORY_UTIL_INFO : FE::uint8
{
	_TOTAL_VIRTUAL_MEMORY_SIZE = 0,
	_TOTAL_VIRTUAL_MEMORY_UTIL = 1,

	_THIS_PROCESS_VIRTUAL_MEMORY_UTIL = 2,

	_TOTAL_PHYSICAL_MEMORY_SIZE = 3,
	_TOTAL_PHYSICAL_MEMORY_UTIL = 4,

	_THIS_PROCESS_PHYSICAL_MEMORY_UTIL = 5/*,

	_LIBRARY_TOTAL_HEAP_UTIL_SIZE = 6,

	_LIBRARY_HEAP_UTIL_SIZE_BY_THREAD_ID = 7,
	_LIBRARY_HEAP_UTIL_SIZE_BY_TYPE = 8*/
};

_FE_FORCE_INLINE_ var::float64 convert_bytes_to_kilobytes(uint64 bytes_p) noexcept
{
	return static_cast<var::float64>(bytes_p) / static_cast<var::float64>(one_kb);
}
_FE_FORCE_INLINE_ var::float64 convert_bytes_to_megabytes(uint64 bytes_p) noexcept
{
	return static_cast<var::float64>(bytes_p) / static_cast<var::float64>(one_mb);
}
_FE_FORCE_INLINE_ var::float64 convert_bytes_to_gigabytes(uint64 bytes_p) noexcept
{
	return static_cast<var::float64>(bytes_p) / static_cast<var::float64>(one_gb);
}

var::uint64 request_app_memory_utilization(const HEAP_MEMORY_UTIL_INFO select_data_p) noexcept;


END_NAMESPACE








BEGIN_NAMESPACE(FE::internal)


struct memory_utilization
{
	var::int64 _total_bytes = 0;
	var::int64 _thread_local_bytes = 0;

	var::int64 _total_pool_bytes = 0;
	var::int64 _thread_local_pool_bytes = 0;
};

/*
The allocator_base class in the FE::internal namespace serves as a base class for memory allocation management
providing static methods to track and log memory usage across threads, as well as manage memory pools.
*/
class allocator_base
{
protected:
	static std::atomic_int64_t s_total_memory_util;
	thread_local static var::int64 tl_s_thread_local_memory_util;

	static std::atomic_int64_t s_total_memory_pool_util;
	thread_local static var::int64 tl_s_thread_local_memory_pool_util;

	_FE_FORCE_INLINE_ static void add(int64 size_bytes_p) noexcept
	{
		s_total_memory_util.fetch_add(size_bytes_p, ::std::memory_order_relaxed);
		tl_s_thread_local_memory_util += size_bytes_p;
	}

	_FE_FORCE_INLINE_ static void sub(int64 size_bytes_p) noexcept
	{
		s_total_memory_util.fetch_sub(size_bytes_p, ::std::memory_order_relaxed);
		tl_s_thread_local_memory_util -= size_bytes_p;
	}


	_FE_FORCE_INLINE_ static void add_to_pool(int64 size_bytes_p) noexcept
	{
		s_total_memory_pool_util.fetch_add(size_bytes_p, ::std::memory_order_relaxed);
		tl_s_thread_local_memory_pool_util += size_bytes_p;
	}

	_FE_FORCE_INLINE_ static void sub_from_pool(int64 size_bytes_p) noexcept
	{
		s_total_memory_pool_util.fetch_sub(size_bytes_p, ::std::memory_order_relaxed);
		tl_s_thread_local_memory_pool_util -= size_bytes_p;
	}

#if defined(_ENABLE_MEMORY_TRACKER_) && defined(_ENABLE_LOG_)
	template <typename T>
	_FE_FORCE_INLINE_ static void __log_heap_memory_allocation(uint64 size_in_bytes_to_allocate_p, void* const allocated_address_p) noexcept
	{
		uint64 l_this_thread_id = std::this_thread::get_id();
		add(size_in_bytes_to_allocate_p);
		memory_utilization l_new_heap_mem_usage_in_bytes = query_all_data();
		FE_LOG("This application is using ${%lu@0} bytes. \n Current thread[id:${%lu@1}] is using ${%lu@2} bytes.", &(l_new_heap_mem_usage_in_bytes._total_bytes), &l_this_thread_id, &(l_new_heap_mem_usage_in_bytes._thread_local_bytes));
		FE_LOG("Allocated data information: [address: 0x${%p@0} | allocation size: ${%lu@1} bytes | type name: ${%s@2} | size of type: ${%lu@3} bytes].\n", &allocated_address_p, &size_in_bytes_to_allocate_p, typeid(T).name(), &FE::buffer<var::size>::set_and_get(sizeof(T)));
	}

	template <typename T>
	_FE_FORCE_INLINE_ static void __log_heap_memory_reallocation(uint64 prev_size_in_bytes_p, uint64 new_size_in_bytes_to_allocate_p, void* const realloc_target_p) noexcept
	{
		uint64 l_this_thread_id = std::this_thread::get_id();

		sub(prev_size_in_bytes_p);
		add(new_size_in_bytes_to_allocate_p);
		memory_utilization l_new_heap_mem_usage_in_bytes = query_all_data();
		FE_LOG("This application is using ${%lu@0} bytes. \n Current thread[id:${%lu@1}] is using ${%lu@2} bytes.", &(l_new_heap_mem_usage_in_bytes._total_bytes), &l_this_thread_id, &(l_new_heap_mem_usage_in_bytes._thread_local_bytes));
		FE_LOG("Reallocated data information: [address: 0x${%p@0} | Reallocation size: ${%lu@1} bytes | type name: ${%s@2} | size of type: ${%lu@3} bytes].\n", &realloc_target_p, &new_size_in_bytes_to_allocate_p, typeid(T).name(), &FE::buffer<var::size>::set_and_get(sizeof(T)));
	}

	template <typename T>
	_FE_FORCE_INLINE_ static void __log_heap_memory_deallocation(uint64 size_in_bytes_to_deallocate_p, void* const address_to_be_freed_p) noexcept
	{
		uint64 l_this_thread_id = std::this_thread::get_id();

		sub(size_in_bytes_to_deallocate_p);
		memory_utilization l_new_heap_mem_usage_in_bytes = query_all_data();
		FE_LOG("This application is using ${%lu@0} bytes. \n Current thread[id:${%lu@1}] is using ${%lu@2} bytes.", &(l_new_heap_mem_usage_in_bytes._total_bytes), &l_this_thread_id, &(l_new_heap_mem_usage_in_bytes._thread_local_bytes));
		FE_LOG("Deleted data information: [address: 0x${%p@0} | Deleted size: ${%lu@1} bytes | type name: ${%s@2} | size of type: ${%lu@3} bytes].\n", &address_to_be_freed_p, &size_in_bytes_to_deallocate_p, typeid(T).name(), &FE::buffer<var::size>::set_and_get(sizeof(T)));
	}
#endif

public:
	_FE_FORCE_INLINE_ static memory_utilization query_all_data() noexcept
	{
#if defined(_ENABLE_MEMORY_TRACKER_) && defined(_ENABLE_LOG_)
		memory_utilization l_data
		{
			s_total_memory_util.load(std::memory_order_relaxed), tl_s_thread_local_memory_util,
			s_total_memory_pool_util.load(std::memory_order_relaxed), tl_s_thread_local_memory_pool_util,
		};
		return l_data;
#else
		memory_utilization l_null{ null };
		return l_null;
#endif
	}

protected:
	template<class Pool>
	_FE_FORCE_INLINE_ Pool* __get_default_pool() const noexcept
	{
		static Pool s_pool;
		return &s_pool;
	} 
};

END_NAMESPACE

/*
The operator new function allocates a specified number of bytes of memory
aligned to the size of the CPU's L1 cache line, using a custom memory allocation function FE_ALIGNED_ALLOC.
*/
void* operator new(std::size_t bytes_p);
void* operator new[](std::size_t bytes_p);

void operator delete(void* ptr_p) noexcept;
void operator delete[](void* ptr_p) noexcept;

void operator delete(void* ptr_p, std::size_t size_p) noexcept;
void operator delete[](void* ptr_p, std::size_t size_p) noexcept;

#endif 
