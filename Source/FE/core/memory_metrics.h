#ifndef _FE_CORE_MEMORY_METRICS_HXX_
#define _FE_CORE_MEMORY_METRICS_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>




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

	_THIS_PROCESS_PHYSICAL_MEMORY_UTIL = 5,

	_LIBRARY_TOTAL_HEAP_UTIL_SIZE = 6,

	_LIBRARY_HEAP_UTIL_SIZE_BY_THREAD_ID = 7,
	_LIBRARY_HEAP_UTIL_SIZE_BY_TYPE = 8
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
#endif // !_MEMORY_METRIC_HXX_

