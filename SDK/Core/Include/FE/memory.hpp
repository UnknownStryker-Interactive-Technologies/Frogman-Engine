#ifndef _FE_CORE_MEMORY_HXX_
#define _FE_CORE_MEMORY_HXX_
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

#ifdef FE_UNALIGNED_MEMZERO
	#error FE_UNALIGNED_MEMSET is a reserved Frogman Engine macro keyword.
#endif
#ifdef FE_ALIGNED_MEMZERO
	#error FE_ALIGNED_MEMSET is a reserved Frogman Engine macro keyword.
#endif
#ifdef FE_UNALIGNED_MEMCPY
	#error FE_UNALIGNED_MEMCPY is a reserved Frogman Engine macro keyword.
#endif
#ifdef FE_ALIGNED_MEMCPY
	#error FE_ALIGNED_MEMCPY is a reserved Frogman Engine macro keyword.
#endif
#ifdef FE_DEST_ALIGNED_MEMCPY
	#error FE_DEST_ALIGNED_MEMCPY is a reserved Frogman Engine macro keyword.
#endif
#ifdef FE_SOURCE_ALIGNED_MEMCPY
	#error FE_SOURCE_ALIGNED_MEMCPY is a reserved Frogman Engine macro keyword.
#endif
#ifdef FE_ALIGNED_MEMMOVE
	#error FE_ALIGNED_MEMMOVE is a reserved Frogman Engine macro keyword.
#endif
#ifdef FE_UNALIGNED_MEMMOVE
	#error FE_UNALIGNED_MEMMOVE is a reserved Frogman Engine macro keyword.
#endif


#ifdef __FE_DIVIDE_BY_2
	#error __FE_DIVIDE_BY_2 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_DIVIDE_BY_2(input) ((input) >> 1)

#ifdef __FE_DIVIDE_BY_4
	#error __FE_DIVIDE_BY_4 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_DIVIDE_BY_4(input) ((input) >> 2)

#ifdef __FE_DIVIDE_BY_8
	#error __FE_DIVIDE_BY_8 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_DIVIDE_BY_8(input) ((input) >> 3)

#ifdef __FE_DIVIDE_BY_16
	#error __FE_DIVIDE_BY_16 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_DIVIDE_BY_16(input) ((input) >> 4)

#ifdef __FE_DIVIDE_BY_32
	#error __FE_DIVIDE_BY_32 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_DIVIDE_BY_32(input) ((input) >> 5)

#ifdef __FE_DIVIDE_BY_64
	#error __FE_DIVIDE_BY_64 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_DIVIDE_BY_64(input) ((input) >> 6)

#ifdef __FE_DIVIDE_BY_128
	#error __FE_DIVIDE_BY_128 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_DIVIDE_BY_128(input) ((input) >> 7)


#ifdef __FE_MODULO_BY_2
	#error __FE_MODULO_BY_2 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_MODULO_BY_2(input) ((input) & 1)

#ifdef __FE_MODULO_BY_4
	#error __FE_MODULO_BY_4 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_MODULO_BY_4(input) ((input) & 3)

#ifdef __FE_MODULO_BY_8
	#error __FE_MODULO_BY_8 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_MODULO_BY_8(input) ((input) & 7)

#ifdef __FE_MODULO_BY_16
	#error __FE_MODULO_BY_16 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_MODULO_BY_16(input) ((input) & 15)

#ifdef __FE_MODULO_BY_32
	#error __FE_MODULO_BY_32 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_MODULO_BY_32(input) ((input) & 31)

#ifdef __FE_MODULO_BY_64
	#error __FE_MODULO_BY_64 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_MODULO_BY_64(input) ((input) & 63)

#ifdef __FE_MODULO_BY_128
	#error __FE_MODULO_BY_128 is a reserved Frogman Engine macro keyword.
#endif
#define __FE_MODULO_BY_128(input) ((input) & 127)


#include <FE/prerequisites.h>
#include <FE/algorithm/math.hpp>


#ifdef _FE_ON_WINDOWS_X86_64_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <sysinfoapi.h> // to use GetSystemInfo
#endif


#ifdef _FE_ON_X86_64_
	// AVX
	#include <immintrin.h>

// You won't be able to run any modern game software if your CPU does not support SSE and SSE2.
#define _SSE_
#define _SSE2_

	#ifdef __AVX__
		#define _AVX_
	#endif

	#ifdef __AVX2__
		#define _AVX2_
	#endif

	#ifdef __AVX512F__
		#define _AVX512F_
	#endif

#elif defined(_FE_ON_ARM64_)
	#if defined(__ARM_NEON) || defined(__ARM_NEON__)
		#define _ARM_NEON_
		#include <arm_neon.h>
	#endif

	#ifdef __ARM_NEON_FP
		#define _ARM_NEON_FP_
		#include <arm_neon.h>
	#endif
#endif

// For FE::unique_ptr
#include <memory_resource>
#include <type_traits>
#include <utility>




BEGIN_NAMESPACE(FE)


_FE_MAYBE_UNUSED_ constexpr uint8 byte_size = 1;
_FE_MAYBE_UNUSED_ constexpr uint8 word_size = 2;
_FE_MAYBE_UNUSED_ constexpr uint8 dword_size = 4;
_FE_MAYBE_UNUSED_ constexpr uint8 qword_size = 8;

_FE_MAYBE_UNUSED_ extern FE::uint64 system_page_size;
_FE_MAYBE_UNUSED_ extern FE::uint64 system_large_page_size;


using reserve = size;
using resize_to = size;
using extend = size;

// The FE::is_power_of_two function is a constexpr function that checks if a given size_t value is a power of two by using a bitwise operation.
_FE_CONSTEXPR17_ FE::boolean is_power_of_two(FE::size value_p) noexcept
{
	/* Since 4 is a power of 2 and
		0b0001 == 1
		0b0010 == 2
		0b0011 == 3
		0b0100 == 4
		...

		if you can see the pattern, you can find that
		4 - 1 is 3 and equals to 0b0011 in binary.

		  0b0100
		& 0b0011
		--------
		  0b0000

		Therefore,
		0b0100 AND 0b0011 == 0b0000.
	*/
	return (value_p & (value_p - 1)) == 0;
}

struct align_8bytes final
{
	_FE_MAYBE_UNUSED_ static constexpr size size = 8;
};

struct align_16bytes final
{
	_FE_MAYBE_UNUSED_ static constexpr size size = 16;
};

struct align_32bytes final
{
	_FE_MAYBE_UNUSED_ static constexpr size size = 32;
};

struct align_64bytes final
{
	_FE_MAYBE_UNUSED_ static constexpr size size = 64;
};

struct align_128bytes final
{
	_FE_MAYBE_UNUSED_ static constexpr size size = 128;
};

struct CPU_L1_cache_line final
{
	_FE_MAYBE_UNUSED_ static constexpr size size = std::hardware_destructive_interference_size;
};

/*
The FE::SIMD_auto_alignment struct defines a type alias for alignment based on the available SIMD instruction set
specifying different alignment sizes for AVX512, AVX/AVX2, and fallback to a default alignment.
*/
struct SIMD_auto_alignment
{
#ifdef _AVX512F_
	using alignment_type = align_64bytes;
#elif defined(_AVX_) || defined(_AVX2_)
	using alignment_type = align_32bytes;
#else
	using alignment_type = align_16bytes;
#endif

	_FE_MAYBE_UNUSED_ static constexpr size size = alignment_type::size;
};

#pragma warning(push)
#pragma warning(disable:4324)
template<typename T, class Alignment = typename FE::SIMD_auto_alignment>
struct alignas(Alignment::size) aligned final
{
	using value_type = T;
	using alignment_type = Alignment;

	T _data;
};
#pragma warning(pop)

template<FE::size SizeInBytes, class Alignment>
struct align_as final
{
	_FE_MAYBE_UNUSED_ static constexpr inline FE::size size = sizeof(FE::aligned<FE::byte[SizeInBytes], Alignment>);
};

enum struct Address : bool
{
	_NotAligned = false,
	_Aligned = true
};




#if defined(_AVX_) && defined(_SSE2_)

_FE_FORCE_INLINE_ void __x86_64_unaligned_memzero_AVX_SSE2(void* out_dest_p, var::size bytes_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));

	// __FE_DIVIDE_BY_32(bytes_to_copy_p) == SIMD operation count
	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(bytes_p); out_dest_p != end;)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p),
			_mm256_xor_si256(	_mm256_loadu_si256(static_cast<const __m256i*>(out_dest_p)),
								_mm256_loadu_si256(static_cast<const __m256i*>(out_dest_p))
			)
		);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
	}

	bytes_p = __FE_MODULO_BY_32(bytes_p);
	if (bytes_p >= 16)
	{
		_mm_storeu_si128(static_cast<__m128i*>(out_dest_p),  
						_mm_xor_si128(	_mm_loadu_si128( static_cast<const __m128i*>(out_dest_p) ),
										_mm_loadu_si128( static_cast<const __m128i*>(out_dest_p) )
										)
						);
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		bytes_p -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + bytes_p; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(out_dest_p) xor *static_cast<byte*>(out_dest_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
	}
}

_FE_FORCE_INLINE_ void __x86_64_aligned_memzero_AVX_SSE2(void* out_dest_p, var::size bytes_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));

	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(bytes_p); out_dest_p != end;)
	{
		_mm256_store_si256(static_cast<__m256i*>(out_dest_p),
			_mm256_xor_si256(_mm256_load_si256(static_cast<const __m256i*>(out_dest_p)),
				_mm256_load_si256(static_cast<const __m256i*>(out_dest_p))
			)
		);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
	}

	bytes_p = __FE_MODULO_BY_32(bytes_p);
	if (bytes_p >= 16)
	{
		_mm_store_si128(static_cast<__m128i*>(out_dest_p),
			_mm_xor_si128(_mm_load_si128(static_cast<const __m128i*>(out_dest_p)),
				_mm_load_si128(static_cast<const __m128i*>(out_dest_p))
			)
		);
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		bytes_p -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + bytes_p; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(out_dest_p) xor *static_cast<byte*>(out_dest_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
	}
}


_FE_FORCE_INLINE_ void __x86_64_unaligned_memcpy_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(source_p));

	// __FE_DIVIDE_BY_32(bytes_to_copy_p) == SIMD operation count
	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(bytes_to_copy_p); out_dest_p != end;)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p), _mm256_loadu_si256(static_cast<const __m256i*>(source_p)));
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
		source_p = static_cast<const __m256i*>(source_p) + 1;
	}

	bytes_to_copy_p = __FE_MODULO_BY_32(bytes_to_copy_p);
	if (bytes_to_copy_p >= 16)
	{
		_mm_storeu_si128(static_cast<__m128i*>(out_dest_p), _mm_loadu_si128(static_cast<const __m128i*>(source_p)));
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		source_p = static_cast<const __m128i*>(source_p) + 1;
		bytes_to_copy_p -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + bytes_to_copy_p; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(source_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
		source_p = static_cast<byte*>(source_p) + 1;
	}
}

_FE_FORCE_INLINE_ void __x86_64_aligned_memcpy_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(source_p));
	FE_NEGATIVE_ASSERT(__FE_MODULO_BY_32(reinterpret_cast<uintptr>(out_dest_p)) != 0, "${%s@0}: ${%s@1} is not aligned by 32.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_IncorrectAddressAlignment), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(__FE_MODULO_BY_32(reinterpret_cast<uintptr>(source_p)) != 0, "${%s@0}: ${%s@1} is not aligned by 32.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_IncorrectAddressAlignment), TO_STRING(source_p));

	// __FE_DIVIDE_BY_32(bytes_to_copy_p) == SIMD operation count
	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(bytes_to_copy_p); out_dest_p != end;)
	{
		_mm256_store_si256(static_cast<__m256i*>(out_dest_p), _mm256_load_si256(static_cast<const __m256i*>(source_p)));
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
		source_p = static_cast<const __m256i*>(source_p) + 1;
	}

	bytes_to_copy_p = __FE_MODULO_BY_32(bytes_to_copy_p);
	if (bytes_to_copy_p >= 16)
	{
		_mm_store_si128(static_cast<__m128i*>(out_dest_p), _mm_load_si128(static_cast<const __m128i*>(source_p)));
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		source_p = static_cast<const __m128i*>(source_p) + 1;
		bytes_to_copy_p -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + bytes_to_copy_p; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(source_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
		source_p = static_cast<byte*>(source_p) + 1;
	}
}

_FE_FORCE_INLINE_ void __x86_64_dest_aligned_memcpy_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(source_p));
	FE_NEGATIVE_ASSERT(__FE_MODULO_BY_32(reinterpret_cast<uintptr>(out_dest_p)) != 0, "${%s@0}: ${%s@1} is not aligned by 32.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_IncorrectAddressAlignment), TO_STRING(out_dest_p));

	// __FE_DIVIDE_BY_32(bytes_to_copy_p) == SIMD operation count
	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(bytes_to_copy_p); out_dest_p != end;)
	{
		_mm256_store_si256(static_cast<__m256i*>(out_dest_p), _mm256_loadu_si256(static_cast<const __m256i*>(source_p)));
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
		source_p = static_cast<const __m256i*>(source_p) + 1;
	}

	bytes_to_copy_p = __FE_MODULO_BY_32(bytes_to_copy_p);
	if (bytes_to_copy_p >= 16)
	{
		_mm_store_si128(static_cast<__m128i*>(out_dest_p), _mm_loadu_si128(static_cast<const __m128i*>(source_p)));
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		source_p = static_cast<const __m128i*>(source_p) + 1;
		bytes_to_copy_p -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + bytes_to_copy_p; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(source_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
		source_p = static_cast<byte*>(source_p) + 1;
	}
}

_FE_FORCE_INLINE_ void __x86_64_source_aligned_memcpy_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(source_p));
	FE_NEGATIVE_ASSERT(__FE_MODULO_BY_32(reinterpret_cast<uintptr>(source_p)) != 0, "${%s@0}: ${%s@1} is not aligned by 32.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_IncorrectAddressAlignment), TO_STRING(source_p));

	// __FE_DIVIDE_BY_32(bytes_to_copy_p) == SIMD operation count
	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(bytes_to_copy_p); out_dest_p != end;)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p), _mm256_load_si256(static_cast<const __m256i*>(source_p)));

		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
		source_p = static_cast<const __m256i*>(source_p) + 1;
	}

	bytes_to_copy_p = __FE_MODULO_BY_32(bytes_to_copy_p);
	if (bytes_to_copy_p >= 16)
	{
		_mm_storeu_si128(static_cast<__m128i*>(out_dest_p), _mm_load_si128(static_cast<const __m128i*>(source_p)));
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		source_p = static_cast<const __m128i*>(source_p) + 1;
		bytes_to_copy_p -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + bytes_to_copy_p; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(source_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
		source_p = static_cast<byte*>(source_p) + 1;
	}
}


/*
if (source_p < out_dest_p) being true means that the two void*s "out_dest_p and source_p" are pointing to the same range of memory
possibly overlap each other. For that case
memmove iterates and copies the data in the reverse order of memcpy operation.

		copy & traversal order
	<---------------------------
  front                       back
	++++++++++++++++++++++++++++
	|                          |
	++++++++++++++++++++++++++++
low address               high address
*/
_FE_FORCE_INLINE_ void __x86_64_unaligned_memmove_AVX_SSE2(void* out_dest_p, const void* source_p, size bytes_to_move_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(source_p));
	FE_NEGATIVE_ASSERT(bytes_to_move_p == 0, "${%s@0}: ${%s@1} is 0.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(bytes_to_move_p));

	out_dest_p = static_cast<var::byte*>(out_dest_p) + (bytes_to_move_p - 1);
	source_p = static_cast<byte*>(source_p) + (bytes_to_move_p - 1);

	for (var::size n = __FE_MODULO_BY_16(bytes_to_move_p); n > 0; --n)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<FE::byte*>(source_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) - 1;
		source_p = static_cast<byte*>(source_p) - 1;
	}

	out_dest_p = reinterpret_cast<__m256i*>(static_cast<var::byte*>(out_dest_p) - 31);
	source_p = reinterpret_cast<const __m256i*>(static_cast<byte*>(source_p) - 31);

	var::size l_operation_count = __FE_DIVIDE_BY_16(bytes_to_move_p);
	for (; l_operation_count >= 2; l_operation_count -= 2)
	{
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(out_dest_p), _mm256_loadu_si256(reinterpret_cast<const __m256i*>(source_p)));
		out_dest_p = reinterpret_cast<__m256i*>(out_dest_p) - 1;
		source_p = reinterpret_cast<const __m256i*>(source_p) - 1;
	}

	out_dest_p = reinterpret_cast<__m128i*>(out_dest_p) + 1;
	source_p = reinterpret_cast<const __m128i*>(source_p) + 1;

	for (; l_operation_count > 0; --l_operation_count)
	{
		_mm_storeu_si128(reinterpret_cast<__m128i*>(out_dest_p), _mm_loadu_si128(reinterpret_cast<const __m128i*>(source_p)));
		out_dest_p = reinterpret_cast<__m128i*>(out_dest_p) - 1;
		source_p = reinterpret_cast<const __m128i*>(source_p) - 1;
	}
}


#if defined(_AVX512F_)
_FE_FORCE_INLINE_ void __x86_64_unaligned_memzero_AVX512_AVX_SSE2(void* out_dest_p, FE::size bytes_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is a nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));

	for (__m512* const end = static_cast<__m512*>(out_dest_p) + __FE_DIVIDE_BY_64(bytes_p); out_dest_p != end;)
	{
		_mm512_storeu_si512(static_cast<__m512*>(out_dest_p),
							_mm512_xor_si512(	_mm512_loadu_si512(static_cast<const __m512*>(out_dest_p)),
												_mm512_loadu_si512(static_cast<const __m512*>(out_dest_p))
			)
		);
		out_dest_p = static_cast<__m512*>(out_dest_p) + 1;
	}

	if (__FE_MODULO_BY_64(bytes_p) > 0)
	{
		__x86_64_unaligned_memzero_AVX_SSE2( out_dest_p, bytes_p - (64 * __FE_DIVIDE_BY_64(bytes_p)) );
	}
}

_FE_FORCE_INLINE_ void __x86_64_aligned_memzero_AVX512_AVX_SSE2(void* out_dest_p, var::size bytes_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_ASSERT(__FE_MODULO_BY_64(reinterpret_cast<uintptr>(out_dest_p)) == 0, "${%s@0}: ${%s@1} is not aligned by 64.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_IncorrectAddressAlignment), TO_STRING(out_dest_p));

	for (__m512* const end = static_cast<__m512*>(out_dest_p) + __FE_DIVIDE_BY_64(bytes_p); out_dest_p != end;)
	{
		_mm512_store_si512(static_cast<__m512*>(out_dest_p),
							_mm512_xor_si512(	_mm512_load_si512(static_cast<const __m512*>(out_dest_p)),
												_mm512_load_si512(static_cast<const __m512*>(out_dest_p))
			)
		);
		out_dest_p = static_cast<__m512*>(out_dest_p) + 1;
	}

	if (__FE_MODULO_BY_64(bytes_p) > 0)
	{
		__x86_64_aligned_memzero_AVX_SSE2( out_dest_p, bytes_p - (64 * __FE_DIVIDE_BY_64(bytes_p)) );
	}
}


_FE_FORCE_INLINE_ void __x86_64_unaligned_memcpy_AVX512_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(source_p));
	//if(FE_UNLIKELY(out_dest_p == source_p)) _FE_UNLIKELY_
	//{
 //   	return;
	//}

	// __FE_DIVIDE_BY_64(bytes_to_copy_p) == SIMD operation count
	for (__m512i* const end = static_cast<__m512i*>(out_dest_p) + __FE_DIVIDE_BY_64(bytes_to_copy_p); out_dest_p != end;)
	{
		_mm512_storeu_si512(static_cast<__m512i*>(out_dest_p), _mm512_loadu_si512(static_cast<const __m512i*>(source_p)));
		out_dest_p = static_cast<__m512i*>(out_dest_p) + 1;
		source_p = static_cast<const __m512i*>(source_p) + 1;
	}

	bytes_to_copy_p = __FE_MODULO_BY_64(bytes_to_copy_p);
    if(bytes_to_copy_p > 0)
	{
		__x86_64_unaligned_memcpy_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p);
	}
}

_FE_FORCE_INLINE_ void __x86_64_aligned_memcpy_AVX512_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(source_p));
	FE_NEGATIVE_ASSERT(__FE_MODULO_BY_64(reinterpret_cast<uintptr>(out_dest_p)) != 0, "${%s@}: out_dest_p is not aligned by 64.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_IncorrectAddressAlignment));
	FE_NEGATIVE_ASSERT(__FE_MODULO_BY_64(reinterpret_cast<uintptr>(source_p)) != 0, "${%s@}: source_p is not aligned by 64.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_IncorrectAddressAlignment));
	//if(FE_UNLIKELY(out_dest_p == source_p)) _FE_UNLIKELY_
	//{
 //   	return;
	//}

	// __FE_DIVIDE_BY_64(bytes_to_copy_p) == SIMD operation count
	for (__m512i* const end = static_cast<__m512i*>(out_dest_p) + __FE_DIVIDE_BY_64(bytes_to_copy_p); out_dest_p != end;)
	{
		_mm512_store_si512(static_cast<__m512i*>(out_dest_p), _mm512_load_si512(static_cast<const __m512i*>(source_p)));
		out_dest_p = static_cast<__m512i*>(out_dest_p) + 1;
		source_p = static_cast<const __m512i*>(source_p) + 1;
	}

	bytes_to_copy_p = __FE_MODULO_BY_64(bytes_to_copy_p);
    if(bytes_to_copy_p > 0)
	{
		__x86_64_aligned_memcpy_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p);
	}
}

_FE_FORCE_INLINE_ void __x86_64_dest_aligned_memcpy_AVX512_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(source_p));
	FE_NEGATIVE_ASSERT(__FE_MODULO_BY_64(reinterpret_cast<uintptr>(out_dest_p)) != 0, "${%s@}: out_dest_p is not aligned by 64.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_IncorrectAddressAlignment));
	//if(FE_UNLIKELY(out_dest_p == source_p)) _FE_UNLIKELY_
	//{
 //   	return;
	//}

	// __FE_DIVIDE_BY_64(bytes_to_copy_p) == SIMD operation count
	for (__m512i* const end = static_cast<__m512i*>(out_dest_p) + __FE_DIVIDE_BY_64(bytes_to_copy_p); out_dest_p != end;)
	{
		_mm512_store_si512(static_cast<__m512i*>(out_dest_p), _mm512_loadu_si512(static_cast<const __m512i*>(source_p)));
		out_dest_p = static_cast<__m512i*>(out_dest_p) + 1;
		source_p = static_cast<const __m512i*>(source_p) + 1;
	}

	bytes_to_copy_p = __FE_MODULO_BY_64(bytes_to_copy_p);
    if(bytes_to_copy_p > 0)
	{
		__x86_64_dest_aligned_memcpy_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p);
	}
}

_FE_FORCE_INLINE_ void __x86_64_source_aligned_memcpy_AVX512_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(source_p));
	FE_NEGATIVE_ASSERT(__FE_MODULO_BY_64(reinterpret_cast<uintptr>(source_p)) != 0, "${%s@}: source_p is not aligned by 64.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_IncorrectAddressAlignment));
	//if(FE_UNLIKELY(out_dest_p == source_p)) _FE_UNLIKELY_
	//{
 //   	return;
	//}
	
	// __FE_DIVIDE_BY_64(bytes_to_copy_p) == SIMD operation count
	for (__m512i* const end = static_cast<__m512i*>(out_dest_p) + __FE_DIVIDE_BY_64(bytes_to_copy_p); out_dest_p != end;)
	{
		_mm512_storeu_si512(static_cast<__m512i*>(out_dest_p), _mm512_load_si512(static_cast<const __m512i*>(source_p)));
		out_dest_p = static_cast<__m512i*>(out_dest_p) + 1;
		source_p = static_cast<const __m512i*>(source_p) + 1;
	}

	bytes_to_copy_p = __FE_MODULO_BY_64(bytes_to_copy_p);
    if(bytes_to_copy_p > 0)
	{
		__x86_64_source_aligned_memcpy_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p);
	}
}


_FE_FORCE_INLINE_ void __x86_64_unaligned_memmove_AVX512_AVX_SSE2(void* out_dest_p, const void* source_p, FE::size bytes_to_move_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(source_p));
	FE_NEGATIVE_ASSERT(bytes_to_move_p == 0, "${%s@0}: ${%s@1} is 0.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(bytes_to_move_p));
	
	out_dest_p = static_cast<var::byte*>(out_dest_p) + (bytes_to_move_p - 1);
	source_p = static_cast<byte*>(source_p) + (bytes_to_move_p - 1);

	for (var::size n = __FE_MODULO_BY_16(bytes_to_move_p); n > 0; --n)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<FE::byte*>(source_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) - 1;
		source_p = static_cast<byte*>(source_p) - 1;
	}

	out_dest_p = reinterpret_cast<__m512i*>(static_cast<var::byte*>(out_dest_p) - 63);
	source_p = reinterpret_cast<const __m512i*>(static_cast<byte*>(source_p) - 63);

	var::size l_operation_count = __FE_DIVIDE_BY_16(bytes_to_move_p);
	for (; l_operation_count >= 4; l_operation_count -= 4)
	{
		_mm512_storeu_si512(reinterpret_cast<__m512i*>(out_dest_p), _mm512_loadu_si512(reinterpret_cast<const __m512i*>(source_p)));
		out_dest_p = reinterpret_cast<__m512i*>(out_dest_p) - 1;
		source_p = reinterpret_cast<const __m512i*>(source_p) - 1;
	}

	if (l_operation_count >= 2)
	{
		out_dest_p = reinterpret_cast<__m256i*>(out_dest_p) + 1;
		source_p = reinterpret_cast<const __m256i*>(source_p) + 1;
		_mm256_storeu_si256(reinterpret_cast<__m256i*>(out_dest_p), _mm256_loadu_si256(reinterpret_cast<const __m256i*>(source_p)));
		l_operation_count -= 2;
	}

	if (l_operation_count > 0)
	{
		out_dest_p = reinterpret_cast<__m128i*>(out_dest_p) - 1;
		source_p = reinterpret_cast<const __m128i*>(source_p) - 1;
		_mm_storeu_si128(reinterpret_cast<__m128i*>(out_dest_p), _mm_loadu_si128(reinterpret_cast<const __m128i*>(source_p)));
	}
}
#endif

#endif


#if defined(_AVX512F_) && defined(_AVX_) && defined(_SSE2_)
	#define FE_UNALIGNED_MEMZERO(out_dest_p, bytes_p) ::FE::__x86_64_unaligned_memzero_AVX_SSE2(out_dest_p, bytes_p)
	#define FE_ALIGNED_MEMZERO(out_dest_p, bytes_p) ::FE::__x86_64_aligned_memzero_AVX_SSE2(out_dest_p, bytes_p)
	#define FE_UNALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_unaligned_memcpy_AVX512_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_aligned_memcpy_AVX512_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_DEST_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_dest_aligned_memcpy_AVX512_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_SOURCE_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_source_aligned_memcpy_AVX512_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_UNALIGNED_MEMMOVE(out_dest_p, source_p, bytes_to_move_p) ::FE::__x86_64_unaligned_memmove_AVX512_AVX_SSE2(out_dest_p, source_p, bytes_to_move_p)
#elif defined(_AVX_) && defined(_SSE2_)
	#define FE_UNALIGNED_MEMZERO(out_dest_p, bytes_p) ::FE::__x86_64_unaligned_memzero_AVX_SSE2(out_dest_p, bytes_p)
	#define FE_ALIGNED_MEMZERO(out_dest_p, bytes_p) ::FE::__x86_64_aligned_memzero_AVX_SSE2(out_dest_p, bytes_p)
	#define FE_UNALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_unaligned_memcpy_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_aligned_memcpy_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_DEST_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_dest_aligned_memcpy_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_SOURCE_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_source_aligned_memcpy_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_UNALIGNED_MEMMOVE(out_dest_p, source_p, bytes_to_move_p) ::FE::__x86_64_unaligned_memmove_AVX_SSE2(out_dest_p, source_p, bytes_to_move_p)
#else
	#define FE_UNALIGNED_MEMZERO(out_dest_p, bytes_p) ::std::memset(out_dest_p, 0, bytes_p)
	#define FE_ALIGNED_MEMZERO(out_dest_p, bytes_p) ::std::memset(out_dest_p, 0, bytes_p)
	#define FE_UNALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::std::memcpy(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::std::memcpy(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_DEST_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::std::memcpy(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_SOURCE_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::std::memcpy(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_UNALIGNED_MEMMOVE(out_dest_p, source_p, bytes_to_move_p) ::std::memmove(out_dest_p, source_p, bytes_to_move_p)
#endif

/*
The calculate_aligned_memory_size_in_bytes function template computes the aligned memory size in bytes required for a specified number of elements of type T
ensuring that the memory alignment adheres to the specified Alignment.
*/
template<typename T, class Alignment>
_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ size calculate_aligned_memory_size_in_bytes(uint64 elements_p) noexcept  
{
	FE_NEGATIVE_ASSERT(elements_p == 0, "Assertion Failure: ${%s@0} cannot be zero.", TO_STRING(elements_p));

	size l_actual_size = sizeof(T) * elements_p;
	var::size l_multiplier = l_actual_size / sizeof(FE::aligned<T, Alignment>);
	l_multiplier += ((l_actual_size % sizeof(FE::aligned<T, Alignment>)) != 0);

	return sizeof(FE::aligned<T, Alignment>) * l_multiplier;
}

template<typename T>
_FE_FORCE_INLINE_ _FE_CONSTEXPR20_ size calculate_aligned_size_of_T(uint64 alignment_p) noexcept
{
	FE_ASSERT(FE::is_power_of_two(alignment_p) == true, "Assertion failed: the alignment is not a power of two.");
	var::size l_multiplier =  sizeof(T) / alignment_p;
	l_multiplier += ((sizeof(T) % alignment_p) != 0);
	return alignment_p * l_multiplier;
}

template<class ConstIterator>
FE::boolean memcmp(ConstIterator left_iterator_begin_p, ConstIterator left_iterator_end_p, ConstIterator right_iterator_begin_p, ConstIterator right_iterator_end_p) noexcept  
{
	static_assert(std::is_class<ConstIterator>::value == true);
	FE_NEGATIVE_ASSERT(left_iterator_begin_p == nullptr, "ERROR: left_iterator_begin_p is nullptr.");
	FE_NEGATIVE_ASSERT(left_iterator_end_p == nullptr, "ERROR: left_iterator_end_p is nullptr.");
	FE_NEGATIVE_ASSERT(right_iterator_begin_p == nullptr, "ERROR: right_iterator_begin_p is nullptr.");
	FE_NEGATIVE_ASSERT(right_iterator_end_p == nullptr, "ERROR: right_iterator_end_p is nullptr.");

	ConstIterator l_left_iterator_begin = left_iterator_begin_p;

	if ((left_iterator_end_p - left_iterator_begin_p) != (right_iterator_end_p - right_iterator_begin_p))
	{
		return false;
	}

	while ((l_left_iterator_begin != left_iterator_end_p) && (*l_left_iterator_begin == *right_iterator_begin_p))
	{
		++l_left_iterator_begin;
		++right_iterator_begin_p;
	}

	if ((l_left_iterator_begin - left_iterator_begin_p) == (left_iterator_end_p - left_iterator_begin_p))
	{
		return true;
	}

	return false;
}

template<Address DestAddressAlignment = Address::_NotAligned, Address SourceAddressAlignment = Address::_NotAligned>
_FE_FORCE_INLINE_ void memcpy(void* out_dest_p, size dest_capacity_in_bytes_p, const void* source_p, uint64 source_capacity_in_bytes_p) noexcept  
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(source_p));

	if constexpr (DestAddressAlignment == Address::_Aligned && SourceAddressAlignment == Address::_Aligned)
	{
		FE_ALIGNED_MEMCPY(out_dest_p, source_p, ::FE::algorithm::math::min(dest_capacity_in_bytes_p, source_capacity_in_bytes_p));
	}
	else if constexpr (DestAddressAlignment == Address::_Aligned && SourceAddressAlignment == Address::_NotAligned)
	{
		FE_DEST_ALIGNED_MEMCPY(out_dest_p, source_p, ::FE::algorithm::math::min(dest_capacity_in_bytes_p, source_capacity_in_bytes_p));
	}
	else if constexpr (DestAddressAlignment == Address::_NotAligned && SourceAddressAlignment == Address::_Aligned)
	{
		FE_SOURCE_ALIGNED_MEMCPY(out_dest_p, source_p, ::FE::algorithm::math::min(dest_capacity_in_bytes_p, source_capacity_in_bytes_p));
	}
	else if constexpr (DestAddressAlignment == Address::_NotAligned && SourceAddressAlignment == Address::_NotAligned)
	{
		FE_UNALIGNED_MEMCPY(out_dest_p, source_p, ::FE::algorithm::math::min(dest_capacity_in_bytes_p, source_capacity_in_bytes_p));
	}
}

/*
The FE::memcpy function is a template function that performs memory copying between a source and destination pointer with optimizations based on the alignment of the addresses involved
while also ensuring that neither pointer is null.
*/
template<Address DestAddressAlignment = Address::_NotAligned, Address SourceAddressAlignment = Address::_NotAligned>
_FE_FORCE_INLINE_ void memcpy(void* out_dest_p, const void* source_p, uint64 bytes_p) noexcept  
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(source_p));

	if constexpr (DestAddressAlignment == Address::_Aligned && SourceAddressAlignment == Address::_Aligned)
	{
		FE_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_p);
	}
	else if constexpr (DestAddressAlignment == Address::_Aligned && SourceAddressAlignment == Address::_NotAligned)
	{
		FE_DEST_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_p);
	}
	else if constexpr (DestAddressAlignment == Address::_NotAligned && SourceAddressAlignment == Address::_Aligned)
	{
		FE_SOURCE_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_p);
	}
	else if constexpr (DestAddressAlignment == Address::_NotAligned && SourceAddressAlignment == Address::_NotAligned)
	{
		FE_UNALIGNED_MEMCPY(out_dest_p, source_p, bytes_p);
	}
}

/*
The FE::memzero function is a template function that initializes a specified number of bytes in a memory block to a given value
with support for both aligned and unaligned memory addresses based on the specified template parameter.
*/
template<Address DestAddressAlignment = Address::_NotAligned>
_FE_FORCE_INLINE_ void memzero(void* out_dest_p, uint64 bytes_p) noexcept  
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(out_dest_p));

	if constexpr (DestAddressAlignment == Address::_Aligned)
	{
		FE_ALIGNED_MEMZERO(out_dest_p, bytes_p);
	}
	else if constexpr (DestAddressAlignment == Address::_NotAligned)
	{
		FE_UNALIGNED_MEMZERO(out_dest_p, bytes_p);
	}
}

/*
The FE::memmove function is a template function that safely copies a specified number of bytes from a source memory location to a destination memory location
with optimizations based on the alignment of the destination address.
*/
template<Address DestAddressAlignment = Address::_NotAligned>
_FE_FORCE_INLINE_ void memmove(void* out_dest_p, const void* source_p, size bytes_p) noexcept  
{
	FE_STATIC_ASSERT(DestAddressAlignment == Address::_NotAligned, "Static Assertion Failure: address aligned memmove not supported.");
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(bytes_p == 0, "${%s@0}: ${%s@1} is zero", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_NullPtr), TO_STRING(bytes_p));

	FE_UNALIGNED_MEMMOVE(out_dest_p, source_p, bytes_p);
}


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


_FE_MAYBE_UNUSED_ constexpr FE::uint32 one_KiB = 1024;
_FE_MAYBE_UNUSED_ constexpr FE::uint32 one_MiB = 1048576;
_FE_MAYBE_UNUSED_ constexpr FE::uint32 one_GiB = 1073741824;


_FE_FORCE_INLINE_ var::float64 convert_bytes_to_kilobytes(uint64 bytes_p) noexcept
{
	return static_cast<var::float64>(bytes_p) / static_cast<var::float64>(one_KiB);
}
_FE_FORCE_INLINE_ var::float64 convert_bytes_to_megabytes(uint64 bytes_p) noexcept
{
	return static_cast<var::float64>(bytes_p) / static_cast<var::float64>(one_MiB);
}
_FE_FORCE_INLINE_ var::float64 convert_bytes_to_gigabytes(uint64 bytes_p) noexcept
{
	return static_cast<var::float64>(bytes_p) / static_cast<var::float64>(one_GiB);
}


/*
The operator new function allocates a specified number of bytes of memory
aligned to the size of the CPU's L1 cache line.
*/
void* __cdecl ::operator new(size_t bytes_p);
void* __cdecl ::operator new[](size_t bytes_p);

void ::operator delete(void* ptr_p) noexcept;
void ::operator delete[](void* ptr_p) noexcept;

void ::operator delete(void* ptr_p, std::size_t size_p) noexcept;
void ::operator delete[](void* ptr_p, std::size_t size_p) noexcept;


namespace internal
{
	template<typename T>
	class pmr_deleter
	{
		std::pmr::memory_resource* m_allocator;
	public:
		_FE_FORCE_INLINE_ pmr_deleter(std::pmr::memory_resource* const memory_resource_p = nullptr) noexcept
			: m_allocator( ((memory_resource_p == nullptr) ? std::pmr::get_default_resource() : memory_resource_p) )
		{
		}

		_FE_FORCE_INLINE_ void operator()(T* ptr_p) noexcept
		{
			if (ptr_p == nullptr)
			{
				return;
			}

			std::pmr::polymorphic_allocator<T>(m_allocator).deallocate(ptr_p, 1);
		}
	};
}

template <typename T>
using pmr_unique_ptr = std::unique_ptr<T, internal::pmr_deleter<T>>;

template <typename T, typename... Arguments>
_FE_FORCE_INLINE_ pmr_unique_ptr<T> make_pmr_unique(std::pmr::memory_resource* const memory_resource_p, Arguments&&... arguments_p) noexcept
{
	T* l_object = (T*)std::pmr::polymorphic_allocator<T>(memory_resource_p).allocate_bytes(sizeof(T));
	new (l_object) T( std::forward<Arguments&&>(arguments_p)... );
	return pmr_unique_ptr<T>{ l_object, internal::pmr_deleter<T>(memory_resource_p) };
}


END_NAMESPACE

#undef __FE_DIVIDE_BY_2
#undef __FE_DIVIDE_BY_4
#undef __FE_DIVIDE_BY_8
#undef __FE_DIVIDE_BY_16
#undef __FE_DIVIDE_BY_32
#undef __FE_DIVIDE_BY_64
#undef __FE_DIVIDE_BY_128

#undef __FE_MODULO_BY_2
#undef __FE_MODULO_BY_4
#undef __FE_MODULO_BY_8
#undef __FE_MODULO_BY_16
#undef __FE_MODULO_BY_32
#undef __FE_MODULO_BY_64
#undef __FE_MODULO_BY_128
#endif
