#ifndef _FE_CORE_PRIVATE_ALLOCATOR_BASE_HPP_
#define _FE_CORE_PRIVATE_ALLOCATOR_BASE_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/memory.hxx>
#include <FE/core/thread.hpp>

// std
#include <cstdlib>
#include <memory>


#ifdef FE_ALIGNED_ALLOC
	#error FE_ALIGNED_ALLOC is a reserved Frogman Engine macro function.
#endif
#ifdef FE_ALIGNED_FREE
	#error FE_ALIGNED_FREE is a reserved Frogman Engine macro function.
#endif
#ifdef _WINDOWS_X86_64_
	#define FE_ALIGNED_ALLOC(size_p, alignment_p) ::_aligned_malloc(size_p, alignment_p)
	#define FE_ALIGNED_FREE(ptr_to_memory_p) ::_aligned_free(ptr_to_memory_p)
#elif defined(_LINUX_X86_64_)
	#define FE_ALIGNED_ALLOC(size_p, alignment_p) _mm_malloc(size_p, alignment_p)
	#define FE_ALIGNED_FREE(ptr_to_memory_p) _mm_free(ptr_to_memory_p)
#endif




BEGIN_NAMESPACE(FE)

_MAYBE_UNUSED_ constexpr var::uint64 invalid_memory_util_query = max_value<var::uint64>;

_MAYBE_UNUSED_ constexpr uint64 one_kb = 1024;
_MAYBE_UNUSED_ constexpr uint64 one_mb = 1048576;
_MAYBE_UNUSED_ constexpr uint64 one_gb = 1073741824;

#define KB * ::FE::one_kb
#define MB * ::FE::one_mb
#define GB * ::FE::one_gb

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

enum struct SIZE_BYTE_UNIT : FE::uint8
{
	_BYTE = 0,
	_KILOBYTE = 1,
	_MEGABYTE = 2,
	_GIGABYTE = 3
};

_FORCE_INLINE_ var::float64 convert_bytes_to_kilobytes(uint64 bytes_p) noexcept
{
	return static_cast<var::float64>(bytes_p) / static_cast<var::float64>(one_kb);
}
_FORCE_INLINE_ var::float64 convert_bytes_to_megabytes(uint64 bytes_p) noexcept
{
	return static_cast<var::float64>(bytes_p) / static_cast<var::float64>(one_mb);
}
_FORCE_INLINE_ var::float64 convert_bytes_to_gigabytes(uint64 bytes_p) noexcept
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


class allocator_base
{
protected:
	static std::atomic_int64_t s_total_memory_util;
	thread_local static var::int64 tl_s_thread_local_memory_util;

	static std::atomic_int64_t s_total_memory_pool_util;
	thread_local static var::int64 tl_s_thread_local_memory_pool_util;

	_FORCE_INLINE_ static void add(int64 size_bytes_p) noexcept
	{
		s_total_memory_util.fetch_add(size_bytes_p, ::std::memory_order_relaxed);
		tl_s_thread_local_memory_util += size_bytes_p;
	}

	_FORCE_INLINE_ static void sub(int64 size_bytes_p) noexcept
	{
		s_total_memory_util.fetch_sub(size_bytes_p, ::std::memory_order_relaxed);
		tl_s_thread_local_memory_util -= size_bytes_p;
	}


	_FORCE_INLINE_ static void add_to_pool(int64 size_bytes_p) noexcept
	{
		s_total_memory_pool_util.fetch_add(size_bytes_p, ::std::memory_order_relaxed);
		tl_s_thread_local_memory_pool_util += size_bytes_p;
	}

	_FORCE_INLINE_ static void sub_from_pool(int64 size_bytes_p) noexcept
	{
		s_total_memory_pool_util.fetch_sub(size_bytes_p, ::std::memory_order_relaxed);
		tl_s_thread_local_memory_pool_util -= size_bytes_p;
	}

#ifdef _ENABLE_MEMORY_TRACKER_
	template <typename T>
	_FORCE_INLINE_ static void __log_heap_memory_allocation(uint64 size_in_bytes_to_allocate_p, void* const allocated_address_p) noexcept
	{
		uint64 l_this_thread_id = FE::thread::this_thread_id();
		add(size_in_bytes_to_allocate_p);
		memory_utilization l_new_heap_mem_usage_in_bytes = query_all_data();
		FE_LOG("This application is using ${%lu@0} bytes. \n Current thread[id:${%lu@1}] is using ${%lu@2} bytes.", &(l_new_heap_mem_usage_in_bytes._total_bytes), &l_this_thread_id, &(l_new_heap_mem_usage_in_bytes._thread_local_bytes));
		FE_LOG("Allocated data information: [address: 0x${%p@0} | allocation size: ${%lu@1} bytes | type name: ${%s@2} | size of type: ${%lu@3} bytes].\n", &allocated_address_p, &size_in_bytes_to_allocate_p, typeid(T).name(), &FE::buffer<var::size>::set_and_get(sizeof(T)));
	}

	template <typename T>
	_FORCE_INLINE_ static void __log_heap_memory_reallocation(uint64 prev_size_in_bytes_p, uint64 new_size_in_bytes_to_allocate_p, void* const realloc_target_p) noexcept
	{
		uint64 l_this_thread_id = FE::thread::this_thread_id();

		sub(prev_size_in_bytes_p);
		add(new_size_in_bytes_to_allocate_p);
		memory_utilization l_new_heap_mem_usage_in_bytes = query_all_data();
		FE_LOG("This application is using ${%lu@0} bytes. \n Current thread[id:${%lu@1}] is using ${%lu@2} bytes.", &(l_new_heap_mem_usage_in_bytes._total_bytes), &l_this_thread_id, &(l_new_heap_mem_usage_in_bytes._thread_local_bytes));
		FE_LOG("Reallocated data information: [address: 0x${%p@0} | Reallocation size: ${%lu@1} bytes | type name: ${%s@2} | size of type: ${%lu@3} bytes].\n", &realloc_target_p, &new_size_in_bytes_to_allocate_p, typeid(T).name(), &FE::buffer<var::size>::set_and_get(sizeof(T)));
	}

	template <typename T>
	_FORCE_INLINE_ static void __log_heap_memory_deallocation(uint64 size_in_bytes_to_deallocate_p, void* const address_to_be_freed_p) noexcept
	{
		uint64 l_this_thread_id = FE::thread::this_thread_id();

		sub(size_in_bytes_to_deallocate_p);
		memory_utilization l_new_heap_mem_usage_in_bytes = query_all_data();
		FE_LOG("This application is using ${%lu@0} bytes. \n Current thread[id:${%lu@1}] is using ${%lu@2} bytes.", &(l_new_heap_mem_usage_in_bytes._total_bytes), &l_this_thread_id, &(l_new_heap_mem_usage_in_bytes._thread_local_bytes));
		FE_LOG("Deleted data information: [address: 0x${%p@0} | Deleted size: ${%lu@1} bytes | type name: ${%s@2} | size of type: ${%lu@3} bytes].\n", &address_to_be_freed_p, &size_in_bytes_to_deallocate_p, typeid(T).name(), &FE::buffer<var::size>::set_and_get(sizeof(T)));
	}
#endif

public:
	_FORCE_INLINE_ static memory_utilization query_all_data() noexcept
	{
#ifdef _ENABLE_MEMORY_TRACKER_
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

	template<typename T, class Alignment = typename FE::SIMD_auto_alignment>
	_FORCE_INLINE_ T* trackable_alloc(size bytes_p) noexcept
	{
		T* const l_result = (T*)FE_ALIGNED_ALLOC(bytes_p, Alignment::size);
		FE_ASSERT(l_result == nullptr, "${%s@0}: Failed to allocate memory from scalable_aligned_malloc()", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
		FE_ASSERT((reinterpret_cast<uintptr>(l_result) % Alignment::size) != 0, "${%s@0}: The allocated heap memory address not aligned by ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_ADDRESS_ALIGNMENT), &Alignment::size);
#ifndef _RELEASE_
		FE_ALIGNED_MEMSET(l_result, null, bytes_p);
#endif
#ifdef _ENABLE_MEMORY_TRACKER_
		allocator_base::__log_heap_memory_allocation<T>(bytes_p, l_result);
#endif

		return l_result;
	}

	template<typename T, class Alignment = typename FE::SIMD_auto_alignment>
	_FORCE_INLINE_ void trackable_free(T* const ptr_to_memory_p, _MAYBE_UNUSED_ size bytes_p) noexcept
	{
		FE_ASSERT((reinterpret_cast<uintptr>(ptr_to_memory_p) % Alignment::size) != 0, "${%s@0}: The allocated heap memory address not aligned by ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_ADDRESS_ALIGNMENT), &Alignment::size);

		FE_ALIGNED_FREE(ptr_to_memory_p);

#ifdef _ENABLE_MEMORY_TRACKER_
		allocator_base::__log_heap_memory_deallocation<T>(bytes_p, ptr_to_memory_p);
#endif 
	}

	template<typename T, class Alignment = typename FE::SIMD_auto_alignment>
	_FORCE_INLINE_ T* trackable_realloc(T* const ptr_to_memory_p, size prev_bytes_p, size new_bytes_p) noexcept
	{
#ifdef _WINDOWS_X86_64_
		T* l_realloc_result = (T*)::_aligned_realloc(ptr_to_memory_p, new_bytes_p, Alignment::size);

		if (l_realloc_result == nullptr) _UNLIKELY_
		{
			l_realloc_result = (T*)FE_ALIGNED_ALLOC(new_bytes_p, Alignment::size);
			FE_ASSERT(l_realloc_result == nullptr, "${%s@0}: Failed to re-allocate memory from FE_ALIGNED_ALLOC()", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
	#ifndef _RELEASE_
			FE_ALIGNED_MEMSET(l_realloc_result, null, new_bytes_p);
	#endif
			FE::memcpy<ADDRESS::_ALIGNED, ADDRESS::_ALIGNED>(l_realloc_result, new_bytes_p, ptr_to_memory_p, prev_bytes_p);
			if (l_realloc_result != ptr_to_memory_p) _LIKELY_
			{
				FE_ALIGNED_FREE(ptr_to_memory_p);
			}
		}

#elif defined(_LINUX_X86_64_)
		T* l_realloc_result = (T*)FE_ALIGNED_ALLOC(new_bytes_p, Alignment::size);
		FE_ASSERT(l_realloc_result == nullptr, "${%s@0}: Failed to re-allocate memory from FE_ALIGNED_ALLOC()", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR));
#ifndef _RELEASE_
		FE_ALIGNED_MEMSET(l_realloc_result, null, new_bytes_p);
#endif
		if (ptr_to_memory_p != nullptr)
		{
			FE::memcpy<ADDRESS::_ALIGNED, ADDRESS::_ALIGNED>(l_realloc_result, new_bytes_p, ptr_to_memory_p, prev_bytes_p);
			FE_ALIGNED_FREE(ptr_to_memory_p);
		}
#endif


#ifdef _ENABLE_MEMORY_TRACKER_
		allocator_base::__log_heap_memory_reallocation<T>(prev_bytes_p, new_bytes_p, l_realloc_result);
#endif
		FE_ASSERT((reinterpret_cast<uintptr>(l_realloc_result) % Alignment::size) != 0, "${%s@0}: The allocated heap memory address not aligned by ${%lu@1}.", TO_STRING(FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_ILLEGAL_ADDRESS_ALIGNMENT), &Alignment::size);
		return l_realloc_result;
	}

	template<class Resource>
	_FORCE_INLINE_ std::shared_ptr<Resource> get_default_resource() const noexcept
	{
		thread_local static std::shared_ptr<Resource> tl_s_shared_resource = std::make_shared<Resource>();
		return tl_s_shared_resource;
	} 
};

END_NAMESPACE

void* operator new(std::size_t bytes_p);
void* operator new[](std::size_t bytes_p);

void operator delete(void* ptr_p) noexcept;
void operator delete[](void* ptr_p) noexcept;

#endif 
