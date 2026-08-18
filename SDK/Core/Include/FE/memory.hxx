#ifndef _FE_CORE_MEMORY_HXX_
#define _FE_CORE_MEMORY_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
/*
* These algorithms are little bit faster than the GCC STL(version g++ 6) when compiled with the clang++ compiler with - O3 optimization level.
* However, they perform poorly when compiled with MSVC v143, even with /Ox optimization level.
*/
#ifdef FE_UNALIGNED_MEMZERO
	#error FE_ALIGNED_MEMZERO is a reserved Frogman Engine macro keyword.
#endif
#ifdef FE_ALIGNED_MEMZERO
	#error FE_ALIGNED_MEMZERO is a reserved Frogman Engine macro keyword.
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


#ifdef FE_BITWISE_AND
	#error FE_BITWISE_AND is a reserved Frogman Engine macro keyword.
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


#include <FE/prerequisites.hxx>
#include <FE/algorithm/math.hxx>
#include <FE/pair.hxx>


#ifdef _FE_ON_WINDOWS_X86_64_
#include <windows.h>
#include <sysinfoapi.h> // to use GetSystemInfo
#endif


#ifdef _FE_ON_X86_64_
	#include <xmmintrin.h>
	#include <emmintrin.h>
	#include <pmmintrin.h>
	#include <tmmintrin.h>
	#include <smmintrin.h>
	#include <nmmintrin.h>
	#include <immintrin.h>

#if !defined(__AVX__) || !defined(__AVX2__)
	#error AVX/AVX2 support is required for running Frogman Engine programs on AMD64.
#else
	#define _SSE_
	#define _SSE2_
	#define _AVX_
	#define _AVX2_
#endif

	#ifdef __AVX512F__
		#define _AVX512F_
	#endif

	#ifdef __AVX512BW__
		#define _AVX512BW_
	#endif

	#ifdef __AVX512VL__
		#define _AVX512VL_
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


// The FE::is_power_of_two function is a constexpr function that checks if a given size_t value is a power of two by using a bitwise operation.
_FE_CONSTEXPR17_ FE::boolean is_power_of_two(FE::size value_p) noexcept
{
	if (value_p == 0)
	{
		return false;
	}

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
	_FE_MAYBE_UNUSED_ static constexpr size size = std::hardware_destructive_interference_size; // not available in llvm clang.
};

struct page_alignment final
{
	_FE_MAYBE_UNUSED_ static inline size size = 4096;
};

struct large_page_alignment final
{
	_FE_MAYBE_UNUSED_ static inline size size = 2097152;
};

/*
The FE::SIMD_auto_alignment struct defines a type alias for alignment based on the available SIMD instruction set
specifying different alignment sizes for AVX512, AVX/AVX2, and fallback to a default alignment.
*/
struct SIMD_auto_alignment
{
#if defined(_AVX512F_) || defined(_AVX512BW_) || defined(_AVX512VL_)
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




#if defined(_AVX2_) && defined(_SSE2_)

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_bitwise_and_AVX2_SSE2(	void* out_dest_p, FE::size dest_capacity_in_bytes_p, 
																void* lhs_p, FE::size lhs_capacity_in_bytes_p,
																void* rhs_p, FE::size rhs_capacity_in_bytes_p) noexcept
{
	FE_ASSERT(out_dest_p != nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	var::size l_bytes_to_process = FE::algorithm::math::min(dest_capacity_in_bytes_p, FE::algorithm::math::min(lhs_capacity_in_bytes_p, rhs_capacity_in_bytes_p));
	
	// __FE_DIVIDE_BY_32(l_bytes_to_process) == SIMD operation count
	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(l_bytes_to_process); out_dest_p != end;)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p),
							_mm256_and_si256(	_mm256_loadu_si256( static_cast<const __m256i*>(lhs_p) ),
												_mm256_loadu_si256( static_cast<const __m256i*>(rhs_p) )
												)
		);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m256i*>(lhs_p) + 1;
		rhs_p = static_cast<__m256i*>(rhs_p) + 1;
	}

	l_bytes_to_process = __FE_MODULO_BY_32(l_bytes_to_process);
	if (l_bytes_to_process >= 16)
	{
		_mm_storeu_si128(	static_cast<__m128i*>(out_dest_p),
							_mm_and_si128(	_mm_loadu_si128(static_cast<const __m128i*>(lhs_p)),
											_mm_loadu_si128(static_cast<const __m128i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m128i*>(lhs_p) + 1;
		rhs_p = static_cast<__m128i*>(rhs_p) + 1;
		l_bytes_to_process -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + l_bytes_to_process; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(lhs_p) & *static_cast<byte*>(rhs_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
		lhs_p = static_cast<var::byte*>(lhs_p) + 1;
		rhs_p = static_cast<var::byte*>(rhs_p) + 1;
	}
}
#define FE_BITWISE_AND(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p) ::FE::__x86_64_bitwise_and_AVX2_SSE2(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p)

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_bitwise_or_AVX2_SSE2(void* out_dest_p, FE::size dest_capacity_in_bytes_p,
	void* lhs_p, FE::size lhs_capacity_in_bytes_p,
	void* rhs_p, FE::size rhs_capacity_in_bytes_p) noexcept
{
	FE_ASSERT(out_dest_p != nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	var::size l_bytes_to_process = FE::algorithm::math::min(dest_capacity_in_bytes_p, FE::algorithm::math::min(lhs_capacity_in_bytes_p, rhs_capacity_in_bytes_p));

	// __FE_DIVIDE_BY_32(l_bytes_to_process) == SIMD operation count
	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(l_bytes_to_process); out_dest_p != end;)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p),
							_mm256_or_si256(_mm256_loadu_si256(static_cast<const __m256i*>(lhs_p)),
											_mm256_loadu_si256(static_cast<const __m256i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m256i*>(lhs_p) + 1;
		rhs_p = static_cast<__m256i*>(rhs_p) + 1;
	}

	l_bytes_to_process = __FE_MODULO_BY_32(l_bytes_to_process);
	if (l_bytes_to_process >= 16)
	{
		_mm_storeu_si128(static_cast<__m128i*>(out_dest_p),
						_mm_or_si128(	_mm_loadu_si128(static_cast<const __m128i*>(lhs_p)),
										_mm_loadu_si128(static_cast<const __m128i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m128i*>(lhs_p) + 1;
		rhs_p = static_cast<__m128i*>(rhs_p) + 1;
		l_bytes_to_process -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + l_bytes_to_process; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(lhs_p) | *static_cast<byte*>(rhs_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
		lhs_p = static_cast<var::byte*>(lhs_p) + 1;
		rhs_p = static_cast<var::byte*>(rhs_p) + 1;
	}
}
#define FE_BITWISE_OR(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p) ::FE::__x86_64_bitwise_or_AVX2_SSE2(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p)

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_bitwise_xor_AVX2_SSE2(void* out_dest_p, FE::size dest_capacity_in_bytes_p,
	void* lhs_p, FE::size lhs_capacity_in_bytes_p,
	void* rhs_p, FE::size rhs_capacity_in_bytes_p) noexcept
{
	FE_ASSERT(out_dest_p != nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	var::size l_bytes_to_process = FE::algorithm::math::min(dest_capacity_in_bytes_p, FE::algorithm::math::min(lhs_capacity_in_bytes_p, rhs_capacity_in_bytes_p));

	// __FE_DIVIDE_BY_32(l_bytes_to_process) == SIMD operation count
	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(l_bytes_to_process); out_dest_p != end;)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p),
							_mm256_xor_si256(	_mm256_loadu_si256(static_cast<const __m256i*>(lhs_p)),
												_mm256_loadu_si256(static_cast<const __m256i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m256i*>(lhs_p) + 1;
		rhs_p = static_cast<__m256i*>(rhs_p) + 1;
	}

	l_bytes_to_process = __FE_MODULO_BY_32(l_bytes_to_process);
	if (l_bytes_to_process >= 16)
	{
		_mm_storeu_si128(static_cast<__m128i*>(out_dest_p),
						_mm_xor_si128(	_mm_loadu_si128(static_cast<const __m128i*>(lhs_p)),
										_mm_loadu_si128(static_cast<const __m128i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m128i*>(lhs_p) + 1;
		rhs_p = static_cast<__m128i*>(rhs_p) + 1;
		l_bytes_to_process -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + l_bytes_to_process; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(lhs_p) xor *static_cast<byte*>(rhs_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
		lhs_p = static_cast<var::byte*>(lhs_p) + 1;
		rhs_p = static_cast<var::byte*>(rhs_p) + 1;
	}
}
#define FE_BITWISE_XOR(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p) ::FE::__x86_64_bitwise_xor_AVX2_SSE2(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p)

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_bitwise_not_AVX2_SSE2(void* out_dest_p, var::size bytes_p) noexcept
{
	FE_ASSERT(out_dest_p != nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));

	alignas(sizeof(__m256i)) __m256i l_mask = _mm256_set1_epi32(0xF); // 0b1111'1111

	// __FE_DIVIDE_BY_32(l_bytes_to_process) == SIMD operation count
	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(bytes_p); out_dest_p != end;)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p),
							_mm256_xor_si256(	l_mask, 
												_mm256_loadu_si256(static_cast<const __m256i*>(out_dest_p))
							)
		);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
	}

	bytes_p = __FE_MODULO_BY_32(bytes_p);
	if (bytes_p >= 16)
	{
		_mm_storeu_si128(static_cast<__m128i*>(out_dest_p),
			_mm_xor_si128(_mm_load_si128(reinterpret_cast<const __m128i*>(&l_mask)), // _mm_load_si128 because l_mask is aligned
										_mm_loadu_si128(static_cast<const __m128i*>(out_dest_p))
						)
		);
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		bytes_p -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + bytes_p; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(out_dest_p) xor 0xFF; // 0b1111'1111
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
	}
}
#define FE_BITWISE_NOT(out_dest_p, bytes_p) ::FE::__x86_64_bitwise_not_AVX2_SSE2(out_dest_p, bytes_p)


#ifdef _AVX512F_
	#undef FE_BITWISE_AND
	#undef FE_BITWISE_OR
	#undef FE_BITWISE_XOR
	#undef FE_BITWISE_NOT

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_bitwise_and_AVX512F_AVX2_SSE2(void* out_dest_p, FE::size dest_capacity_in_bytes_p,
	void* lhs_p, FE::size lhs_capacity_in_bytes_p,
	void* rhs_p, FE::size rhs_capacity_in_bytes_p) noexcept
{
	FE_ASSERT(out_dest_p != nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	var::size l_bytes_to_process = FE::algorithm::math::min(dest_capacity_in_bytes_p, FE::algorithm::math::min(lhs_capacity_in_bytes_p, rhs_capacity_in_bytes_p));

	// __FE_DIVIDE_BY_64(l_bytes_to_process) == SIMD operation count
	for (__m512i* const end = static_cast<__m512i*>(out_dest_p) + __FE_DIVIDE_BY_64(l_bytes_to_process); out_dest_p != end;)
	{
		_mm512_storeu_si512(static_cast<__m512i*>(out_dest_p),
			_mm512_and_si512(_mm512_loadu_si512(static_cast<const __m512i*>(lhs_p)),
				_mm512_loadu_si512(static_cast<const __m512i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m512i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m512i*>(lhs_p) + 1;
		rhs_p = static_cast<__m512i*>(rhs_p) + 1;
	}

	l_bytes_to_process = __FE_MODULO_BY_64(l_bytes_to_process);
	if (l_bytes_to_process >= 32)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p),
			_mm256_and_si256(_mm256_loadu_si256(static_cast<const __m256i*>(lhs_p)),
				_mm256_loadu_si256(static_cast<const __m256i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m256i*>(lhs_p) + 1;
		rhs_p = static_cast<__m256i*>(rhs_p) + 1;
		l_bytes_to_process -= 32;
	}

	l_bytes_to_process = __FE_MODULO_BY_32(l_bytes_to_process);
	if (l_bytes_to_process >= 16)
	{
		_mm_storeu_si128(static_cast<__m128i*>(out_dest_p),
			_mm_and_si128(_mm_loadu_si128(static_cast<const __m128i*>(lhs_p)),
				_mm_loadu_si128(static_cast<const __m128i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m128i*>(lhs_p) + 1;
		rhs_p = static_cast<__m128i*>(rhs_p) + 1;
		l_bytes_to_process -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + l_bytes_to_process; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(lhs_p) & *static_cast<byte*>(rhs_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
		lhs_p = static_cast<var::byte*>(lhs_p) + 1;
		rhs_p = static_cast<var::byte*>(rhs_p) + 1;
	}
}
#define FE_BITWISE_AND(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p) ::FE::__x86_64_bitwise_and_AVX512F_AVX2_SSE2(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p)

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_bitwise_or_AVX512F_AVX2_SSE2(void* out_dest_p, FE::size dest_capacity_in_bytes_p,
	void* lhs_p, FE::size lhs_capacity_in_bytes_p,
	void* rhs_p, FE::size rhs_capacity_in_bytes_p) noexcept
{
	FE_ASSERT(out_dest_p != nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	var::size l_bytes_to_process = FE::algorithm::math::min(dest_capacity_in_bytes_p, FE::algorithm::math::min(lhs_capacity_in_bytes_p, rhs_capacity_in_bytes_p));

	// __FE_DIVIDE_BY_64(l_bytes_to_process) == SIMD operation count
	for (__m512i* const end = static_cast<__m512i*>(out_dest_p) + __FE_DIVIDE_BY_64(l_bytes_to_process); out_dest_p != end;)
	{
		_mm512_storeu_si512(static_cast<__m512i*>(out_dest_p),
			_mm512_or_si512(_mm512_loadu_si512(static_cast<const __m512i*>(lhs_p)),
				_mm512_loadu_si512(static_cast<const __m512i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m512i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m512i*>(lhs_p) + 1;
		rhs_p = static_cast<__m512i*>(rhs_p) + 1;
	}

	l_bytes_to_process = __FE_MODULO_BY_64(l_bytes_to_process);
	if (l_bytes_to_process >= 32)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p),
			_mm256_or_si256(_mm256_loadu_si256(static_cast<const __m256i*>(lhs_p)),
				_mm256_loadu_si256(static_cast<const __m256i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m256i*>(lhs_p) + 1;
		rhs_p = static_cast<__m256i*>(rhs_p) + 1;
		l_bytes_to_process -= 32;
	}

	l_bytes_to_process = __FE_MODULO_BY_32(l_bytes_to_process);
	if (l_bytes_to_process >= 16)
	{
		_mm_storeu_si128(static_cast<__m128i*>(out_dest_p),
			_mm_or_si128(_mm_loadu_si128(static_cast<const __m128i*>(lhs_p)),
				_mm_loadu_si128(static_cast<const __m128i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m128i*>(lhs_p) + 1;
		rhs_p = static_cast<__m128i*>(rhs_p) + 1;
		l_bytes_to_process -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + l_bytes_to_process; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(lhs_p) | *static_cast<byte*>(rhs_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
		lhs_p = static_cast<var::byte*>(lhs_p) + 1;
		rhs_p = static_cast<var::byte*>(rhs_p) + 1;
	}
}
#define FE_BITWISE_OR(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p) ::FE::__x86_64_bitwise_or_AVX512F_AVX2_SSE2(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p)

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_bitwise_xor_AVX512F_AVX2_SSE2(void* out_dest_p, FE::size dest_capacity_in_bytes_p,
	void* lhs_p, FE::size lhs_capacity_in_bytes_p,
	void* rhs_p, FE::size rhs_capacity_in_bytes_p) noexcept
{
	FE_ASSERT(out_dest_p != nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	var::size l_bytes_to_process = FE::algorithm::math::min(dest_capacity_in_bytes_p, FE::algorithm::math::min(lhs_capacity_in_bytes_p, rhs_capacity_in_bytes_p));

	// __FE_DIVIDE_BY_64(l_bytes_to_process) == SIMD operation count
	for (__m512i* const end = static_cast<__m512i*>(out_dest_p) + __FE_DIVIDE_BY_64(l_bytes_to_process); out_dest_p != end;)
	{
		_mm512_storeu_si512(static_cast<__m512i*>(out_dest_p),
			_mm512_xor_si512(_mm512_loadu_si512(static_cast<const __m512i*>(lhs_p)),
				_mm512_loadu_si512(static_cast<const __m512i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m512i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m512i*>(lhs_p) + 1;
		rhs_p = static_cast<__m512i*>(rhs_p) + 1;
	}

	l_bytes_to_process = __FE_MODULO_BY_64(l_bytes_to_process);
	if (l_bytes_to_process >= 32)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p),
			_mm256_xor_si256(_mm256_loadu_si256(static_cast<const __m256i*>(lhs_p)),
				_mm256_loadu_si256(static_cast<const __m256i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m256i*>(lhs_p) + 1;
		rhs_p = static_cast<__m256i*>(rhs_p) + 1;
		l_bytes_to_process -= 32;
	}

	l_bytes_to_process = __FE_MODULO_BY_32(l_bytes_to_process);
	if (l_bytes_to_process >= 16)
	{
		_mm_storeu_si128(static_cast<__m128i*>(out_dest_p),
			_mm_xor_si128(_mm_loadu_si128(static_cast<const __m128i*>(lhs_p)),
				_mm_loadu_si128(static_cast<const __m128i*>(rhs_p))
			)
		);
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		lhs_p = static_cast<__m128i*>(lhs_p) + 1;
		rhs_p = static_cast<__m128i*>(rhs_p) + 1;
		l_bytes_to_process -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + l_bytes_to_process; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(lhs_p) xor *static_cast<byte*>(rhs_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
		lhs_p = static_cast<var::byte*>(lhs_p) + 1;
		rhs_p = static_cast<var::byte*>(rhs_p) + 1;
	}
}
#define FE_BITWISE_XOR(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p) ::FE::__x86_64_bitwise_xor_AVX512F_AVX2_SSE2(out_dest_p, dest_capacity_in_bytes_p, lhs_p, lhs_capacity_in_bytes_p, rhs_p, rhs_capacity_in_bytes_p)

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_bitwise_not_AVX512F_AVX2_SSE2(void* out_dest_p, var::size bytes_p) noexcept
{
	FE_ASSERT(out_dest_p != nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));

	alignas(sizeof(__m512i)) __m512i l_mask = _mm512_set1_epi32(0xF); // 0b1111'1111 __m256i

	// __FE_DIVIDE_BY_64(l_bytes_to_process) == SIMD operation count
	for (__m512i* const end = static_cast<__m512i*>(out_dest_p) + __FE_DIVIDE_BY_64(bytes_p); out_dest_p != end;)
	{
		_mm512_storeu_si512(static_cast<__m512i*>(out_dest_p),
			_mm512_xor_si512(l_mask,
				_mm512_loadu_si512(static_cast<const __m512i*>(out_dest_p))
			)
		);
		out_dest_p = static_cast<__m512i*>(out_dest_p) + 1;
	}

	bytes_p = __FE_MODULO_BY_64(bytes_p);
	if (bytes_p >= 32)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p),
			_mm256_xor_si256(_mm256_load_si256(reinterpret_cast<const __m256i*>(&l_mask)), // _mm256_load_si256 because l_mask is aligned
				_mm256_loadu_si256(static_cast<const __m256i*>(out_dest_p))
			)
		);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
		bytes_p -= 32;
	}

	bytes_p = __FE_MODULO_BY_32(bytes_p);
	if (bytes_p >= 16)
	{
		_mm_storeu_si128(static_cast<__m128i*>(out_dest_p),
			_mm_xor_si128(_mm_load_si128(reinterpret_cast<const __m128i*>(&l_mask)), // _mm_load_si128 because l_mask is aligned
				_mm_loadu_si128(static_cast<const __m128i*>(out_dest_p))
			)
		);
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		bytes_p -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + bytes_p; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(out_dest_p) xor 0xFF; // 0b1111'1111
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
	}
}
#define FE_BITWISE_NOT(out_dest_p, bytes_p) ::FE::__x86_64_bitwise_not_AVX512F_AVX2_SSE2(out_dest_p, bytes_p)

#endif
#endif




#if defined(_AVX_) && defined(_SSE2_)

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_unaligned_memzero_AVX_SSE2(void* out_dest_p, var::size bytes_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));

	__m256i l_value = _mm256_setzero_si256();
	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(bytes_p); out_dest_p != end;)
	{
		_mm256_storeu_si256(static_cast<__m256i*>(out_dest_p), l_value);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
	}

	bytes_p = __FE_MODULO_BY_32(bytes_p);
	if (bytes_p >= 16)
	{
		_mm_storeu_si128(static_cast<__m128i*>(out_dest_p), _mm_setzero_si128());
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		bytes_p -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + bytes_p; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(out_dest_p) xor *static_cast<byte*>(out_dest_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
	}
}

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_aligned_memzero_AVX_SSE2(void* out_dest_p, var::size bytes_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));

	__m256i l_value = _mm256_setzero_si256();
	for (__m256i* const end = static_cast<__m256i*>(out_dest_p) + __FE_DIVIDE_BY_32(bytes_p); out_dest_p != end;)
	{
		_mm256_store_si256(static_cast<__m256i*>(out_dest_p), l_value);
		out_dest_p = static_cast<__m256i*>(out_dest_p) + 1;
	}

	bytes_p = __FE_MODULO_BY_32(bytes_p);
	if (bytes_p >= 16)
	{
		_mm_store_si128(static_cast<__m128i*>(out_dest_p), _mm_setzero_si128());
		out_dest_p = static_cast<__m128i*>(out_dest_p) + 1;
		bytes_p -= 16;
	}

	for (var::byte* const end = static_cast<var::byte*>(out_dest_p) + bytes_p; out_dest_p != end;)
	{
		*static_cast<var::byte*>(out_dest_p) = *static_cast<byte*>(out_dest_p) xor *static_cast<byte*>(out_dest_p);
		out_dest_p = static_cast<var::byte*>(out_dest_p) + 1;
	}
}


_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_unaligned_memcpy_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
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

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_aligned_memcpy_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
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

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_dest_aligned_memcpy_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
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

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_source_aligned_memcpy_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
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
_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_unaligned_memmove_AVX_SSE2(void* out_dest_p, const void* source_p, size bytes_to_move_p) noexcept
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

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_unaligned_memzero_AVX512F_AVX_SSE2(void* out_dest_p, FE::size bytes_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is a nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));

	__m512i l_value = _mm512_setzero_si512();
	for (__m512* const end = static_cast<__m512*>(out_dest_p) + __FE_DIVIDE_BY_64(bytes_p); out_dest_p != end;)
	{
		_mm512_storeu_si512(static_cast<__m512*>(out_dest_p), l_value);
		out_dest_p = static_cast<__m512*>(out_dest_p) + 1;
	}

	if (__FE_MODULO_BY_64(bytes_p) > 0)
	{
		__x86_64_unaligned_memzero_AVX_SSE2( out_dest_p, bytes_p - (64 * __FE_DIVIDE_BY_64(bytes_p)) );
	}
}

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_aligned_memzero_AVX512F_AVX_SSE2(void* out_dest_p, var::size bytes_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_ASSERT(__FE_MODULO_BY_64(reinterpret_cast<uintptr>(out_dest_p)) == 0, "${%s@0}: ${%s@1} is not aligned by 64.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_IncorrectAddressAlignment), TO_STRING(out_dest_p));

	__m512i l_value = _mm512_setzero_si512();
	for (__m512* const end = static_cast<__m512*>(out_dest_p) + __FE_DIVIDE_BY_64(bytes_p); out_dest_p != end;)
	{
		_mm512_store_si512(static_cast<__m512*>(out_dest_p), l_value);
		out_dest_p = static_cast<__m512*>(out_dest_p) + 1;
	}

	if (__FE_MODULO_BY_64(bytes_p) > 0)
	{
		__x86_64_aligned_memzero_AVX_SSE2( out_dest_p, bytes_p - (64 * __FE_DIVIDE_BY_64(bytes_p)) );
	}
}


_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_unaligned_memcpy_AVX512F_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
{
	FE_NEGATIVE_ASSERT(out_dest_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(out_dest_p));
	FE_NEGATIVE_ASSERT(source_p == nullptr, "${%s@0}: ${%s@1} is nullptr.", TO_STRING(FE::ErrorCode::_FatalMemoryError_1XX_InvalidSize), TO_STRING(source_p));

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

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_aligned_memcpy_AVX512F_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
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

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_dest_aligned_memcpy_AVX512F_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
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

_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_source_aligned_memcpy_AVX512F_AVX_SSE2(void* out_dest_p, const void* source_p, var::size bytes_to_copy_p) noexcept
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


_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ __x86_64_unaligned_memmove_AVX512F_AVX_SSE2(void* out_dest_p, const void* source_p, FE::size bytes_to_move_p) noexcept
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
	#define FE_UNALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_unaligned_memcpy_AVX512F_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_aligned_memcpy_AVX512F_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_DEST_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_dest_aligned_memcpy_AVX512F_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_SOURCE_ALIGNED_MEMCPY(out_dest_p, source_p, bytes_to_copy_p) ::FE::__x86_64_source_aligned_memcpy_AVX512F_AVX_SSE2(out_dest_p, source_p, bytes_to_copy_p)
	#define FE_UNALIGNED_MEMMOVE(out_dest_p, source_p, bytes_to_move_p) ::FE::__x86_64_unaligned_memmove_AVX512F_AVX_SSE2(out_dest_p, source_p, bytes_to_move_p)
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
_FE_FORCE_INLINE_ constexpr size _FE_VECTOR_CALL_ calculate_aligned_memory_size_in_bytes(uint64 elements_p) noexcept
{
	FE_NEGATIVE_ASSERT(elements_p == 0, "Assertion Failure: ${%s@0} cannot be zero.", TO_STRING(elements_p));

	size l_actual_size = sizeof(T) * elements_p;
	var::size l_multiplier = l_actual_size / sizeof(FE::aligned<T, Alignment>);
	l_multiplier += ((l_actual_size % sizeof(FE::aligned<T, Alignment>)) != 0);

	return sizeof(FE::aligned<T, Alignment>) * l_multiplier;
}

template<typename T>
_FE_FORCE_INLINE_ constexpr size _FE_VECTOR_CALL_ calculate_aligned_size_of_T(uint64 alignment_p) noexcept
{
	FE_ASSERT(FE::is_power_of_two(alignment_p) == true, "Assertion failed: the alignment is not a power of two.");
	var::size l_multiplier =  sizeof(T) / alignment_p;
	l_multiplier += ((sizeof(T) % alignment_p) != 0);
	return alignment_p * l_multiplier;
}

template<class ConstIterator>
FE::boolean _FE_VECTOR_CALL_ memcmp(ConstIterator left_iterator_begin_p, ConstIterator left_iterator_end_p, ConstIterator right_iterator_begin_p, ConstIterator right_iterator_end_p) noexcept
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
_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ memcpy(void* out_dest_p, size dest_capacity_in_bytes_p, const void* source_p, size source_capacity_in_bytes_p) noexcept
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
_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ memcpy(void* out_dest_p, const void* source_p, size bytes_p) noexcept
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
_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ memzero(void* out_dest_p, size bytes_p) noexcept
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
_FE_FORCE_INLINE_ void _FE_VECTOR_CALL_ memmove(void* out_dest_p, const void* source_p, size bytes_p) noexcept
{
	static_assert(DestAddressAlignment == Address::_NotAligned, "Static Assertion Failure: address aligned memmove not supported.");
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


_FE_MAYBE_UNUSED_ constexpr ::FE::uint32 one_KiB = 1024;
_FE_MAYBE_UNUSED_ constexpr ::FE::uint32 one_MiB = 1048576;
_FE_MAYBE_UNUSED_ constexpr ::FE::uint32 one_GiB = 1073741824;


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
_FE_NODISCARD_ void* _FE_CDECL_ ::operator new(size_t bytes_p);
_FE_NODISCARD_ void* _FE_CDECL_ ::operator new[](size_t bytes_p);

void _FE_CDECL_ ::operator delete(void* ptr_p) noexcept;
void _FE_CDECL_ ::operator delete[](void* ptr_p) noexcept;

void _FE_CDECL_ ::operator delete(void* ptr_p, size_t size_p) noexcept;
void _FE_CDECL_ ::operator delete[](void* ptr_p, size_t size_p) noexcept;


namespace internal
{
	template<typename T>
	class pmr_deleter
	{
		std::pmr::memory_resource* m_allocator;
	public:
		_FE_FORCE_INLINE_ pmr_deleter() noexcept = default;
		_FE_FORCE_INLINE_ pmr_deleter(std::pmr::memory_resource* const memory_resource_p) noexcept
			: m_allocator( ((memory_resource_p == nullptr) ? std::pmr::get_default_resource() : memory_resource_p) )
		{
		}

		_FE_FORCE_INLINE_ void operator()(T* ptr_p) noexcept
		{
			if (ptr_p == nullptr)
			{
				return;
			}

			if (m_allocator == nullptr) _FE_UNLIKELY_
			{
				return;
			}

			std::pmr::polymorphic_allocator<T>(m_allocator).deallocate(ptr_p, 1);
		}
	};
}

template <typename T>
using unique_ptr = ::std::unique_ptr<T, ::FE::internal::pmr_deleter<T>>;

template <typename T, typename... Arguments>
_FE_FORCE_INLINE_ unique_ptr<T> _FE_VECTOR_CALL_ make_unique(::std::pmr::memory_resource* const memory_resource_p, Arguments&&... arguments_p) noexcept
{
	T* l_object = (T*)std::pmr::polymorphic_allocator<T>(memory_resource_p).allocate_bytes(sizeof(T));
	new (l_object) T(std::forward<Arguments&&>(arguments_p)...);
	return unique_ptr<T>{ l_object, internal::pmr_deleter<T>(memory_resource_p) };
}


namespace internal
{
	class vpage_cache
	{
	public:
		FE::pair<void*, var::uint64> _table[500] = {};
		var::uint64 _size = 0;

		~vpage_cache()
		{
			for (var::uint64 i = 0; i < _size; ++i)
			{
				const auto& l_entry = _table[i];
				if (l_entry._first == nullptr)
				{
					continue;
				}

				VirtualFree(l_entry._first, 0, MEM_RELEASE);
			}
		}
	};
}

template <typename T>
class page_aligned_allocator
{
	static_assert(std::is_const_v<T> == false, "Static assertion failed: the C++ standard forbids containers of const elements, because page_aligned_allocator<const T> is ill-formed.");
	static_assert(std::is_function_v<T> == false, "Static assertion failed: the C++ standard forbids allocators for function elements.");
	static_assert(std::is_reference_v<T> == false, "Static assertion failed: the C++ standard forbids allocators for reference elements.");
	//static_assert(sizeof(T) >= (4 * FE::one_KiB), "Static assertion failed: page_aligned_allocator can only be used for types with size greater than or equal to 4 KiB.");

	template <typename>
	friend class page_aligned_allocator;

	std::shared_ptr<internal::vpage_cache> m_vpage_cache;

public:
	using value_type = T;
	using size_type = var::size;
	using difference_type = var::ptrdiff;
	using propagate_on_container_copy_assignment = std::true_type;
	using propagate_on_container_move_assignment = std::true_type;
	using propagate_on_container_swap = std::true_type;

public:
	constexpr page_aligned_allocator() noexcept
		:	m_vpage_cache(std::make_shared<internal::vpage_cache>())
	{
	}
	~page_aligned_allocator() noexcept = default;

	constexpr page_aligned_allocator(const page_aligned_allocator& other_p) noexcept
		:	m_vpage_cache(other_p.m_vpage_cache)
	{
	}
	constexpr page_aligned_allocator(page_aligned_allocator&& other_p) noexcept
		: m_vpage_cache(other_p.m_vpage_cache)
	{
	}

	template <typename U>
	constexpr page_aligned_allocator(const page_aligned_allocator<U>& other_p) noexcept
		: m_vpage_cache(other_p.m_vpage_cache)
	{
	}

	template <typename U>
	constexpr page_aligned_allocator(page_aligned_allocator<U>&& other_p) noexcept
		: m_vpage_cache(other_p.m_vpage_cache)
	{
	}

	constexpr page_aligned_allocator& operator=(const page_aligned_allocator& other_p) noexcept
	{
		m_vpage_cache = other_p.m_vpage_cache;
		return *this; 
	}
	constexpr page_aligned_allocator& operator=(page_aligned_allocator&& other_p) noexcept
	{
		m_vpage_cache = other_p.m_vpage_cache;
		return *this;
	}

	template <typename U>
	constexpr page_aligned_allocator& operator=(const page_aligned_allocator<U>& other_p) noexcept
	{
		m_vpage_cache = other_p.m_vpage_cache;
		return *this;
	}

	template <typename U>
	constexpr page_aligned_allocator& operator=(page_aligned_allocator<U>&& other_p) noexcept
	{
		m_vpage_cache = other_p.m_vpage_cache;
		return *this;
	}


	_FE_NODISCARD_ constexpr T* allocate(FE::size count_p) noexcept
	{
		FE_ASSERT(count_p > 0, "Assertion Failure: ${%s@0} cannot be zero.", TO_STRING(count_p));

		var::size l_bytes = __align_to_page_boundary(count_p);

		FE::pair<void*, var::uint64>* l_begin = (FE::pair<void*, var::uint64>*)m_vpage_cache->_table;
		FE::pair<void*, var::uint64>* l_end = (FE::pair<void*, var::uint64>*)m_vpage_cache->_table + m_vpage_cache->_size;

		auto l_predicate =
			[=](const FE::pair<void*, var::uint64>& value_p)
			{
				var::size l_150_per = (l_bytes + __FE_DIVIDE_BY_2(l_bytes));
				return (l_bytes <= value_p._second) && (l_150_per > value_p._second);
			};

		FE::pair<void*, var::uint64>* l_slot = std::find_if(l_begin, l_end, l_predicate);
		if (l_slot != l_end)
		{
			_FE_MAYBE_UNUSED_ DWORD l_errcode = GetLastError();
			FE_ASSERT(l_slot->_first != nullptr, "Assertion Failed: VirtualAlloc page allocation has failed due to the error code ${%d@0}.", &l_errcode);

			void* l_page = l_slot->_first;
			--(m_vpage_cache->_size);
			*l_slot = m_vpage_cache->_table[m_vpage_cache->_size]; // swap and pop
			return (T*)l_page;
		}


		void* l_result = nullptr;
		if (l_bytes >= FE::system_large_page_size)
		{
			l_result = VirtualAlloc(nullptr, l_bytes, MEM_COMMIT | MEM_RESERVE | MEM_LARGE_PAGES, PAGE_READWRITE);
		}

		if (l_result == nullptr)
		{
			l_result = VirtualAlloc(nullptr, l_bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		}

		_FE_MAYBE_UNUSED_ DWORD l_errcode = GetLastError();
		FE_EXIT_IF(l_result == nullptr, l_errcode, "Assertion Failed: VirtualAlloc page allocation has failed due to the error code ${%d@0}.", &l_errcode);
		return (T*)l_result;
	}

	constexpr void deallocate(T* const ptr_p, const size_t count_p) noexcept
	{
		FE_ASSERT(ptr_p != nullptr, "Static assertion failed: nullptr detected.");

		var::size l_bytes = __align_to_page_boundary(count_p);
#ifdef _ENABLE_ASSERT_
		{
			FE::pair<void*, var::uint64>* l_begin = (FE::pair<void*, var::uint64>*)m_vpage_cache->_table;
			FE::pair<void*, var::uint64>* l_end = (FE::pair<void*, var::uint64>*)m_vpage_cache->_table + m_vpage_cache->_size;

			auto l_predicate =
				[=](const FE::pair<void*, var::uint64>& value_p)
				{
					return value_p._first == (void*)ptr_p;
				};

			FE_ASSERT(std::find_if(l_begin, l_end, l_predicate) == l_end,
				"Assertion failed: double free detected. The pointer already resides in the cache.");
		}
#endif
		if (m_vpage_cache->_size < sizeof(m_vpage_cache->_table) / sizeof(FE::pair<void*, var::uint64>))
		{
			m_vpage_cache->_table[m_vpage_cache->_size]._first = ptr_p;
			m_vpage_cache->_table[m_vpage_cache->_size]._second = l_bytes;
			++(m_vpage_cache->_size);
			return;
		}


		VirtualFree(ptr_p, 0, MEM_RELEASE);
	}

	template <typename U>
	constexpr bool operator==(const page_aligned_allocator<U>& other_p) const noexcept { return m_vpage_cache == other_p.m_vpage_cache; }

private:
	var::size __align_to_page_boundary(FE::size count_p) noexcept
	{
		FE::size l_actual_size = sizeof(T) * count_p;

		var::size l_multiplier = l_actual_size / FE::system_page_size;
		l_multiplier += ((l_actual_size % FE::system_page_size) != 0);

		var::size l_bytes = FE::system_page_size * l_multiplier;

		if (l_bytes >= FE::system_large_page_size)
		{
			l_multiplier = l_actual_size / FE::system_large_page_size;
			l_multiplier += ((l_actual_size % FE::system_large_page_size) != 0);
			l_bytes = FE::system_large_page_size * l_multiplier;
		}
		return l_bytes;
	}
};


class cache_aligned_resource : public std::pmr::memory_resource
{
public:
	cache_aligned_resource() noexcept = default;
	virtual ~cache_aligned_resource() noexcept = default;

	cache_aligned_resource(cache_aligned_resource&&) noexcept {}
	cache_aligned_resource& operator=(cache_aligned_resource&&) noexcept {}
	cache_aligned_resource(const cache_aligned_resource&) noexcept {}
	cache_aligned_resource& operator=(const cache_aligned_resource&) noexcept {}

protected:
	virtual void* do_allocate(std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p = FE::CPU_L1_cache_line::size) noexcept override;
	virtual void do_deallocate(void* ptr_p, std::size_t bytes_p, _FE_MAYBE_UNUSED_ std::size_t alignment_p = FE::CPU_L1_cache_line::size) noexcept override;

	virtual bool do_is_equal(const std::pmr::memory_resource& other_p) const noexcept override;
};


template <typename T>
class cache_aligned_allocator
{
	static_assert(std::is_const_v<T> == false, "Static assertion failed: the C++ standard forbids containers of const elements, because cache_aligned_allocator<const T> is ill-formed.");
	static_assert(std::is_function_v<T> == false, "Static assertion failed: the C++ standard forbids allocators for function elements.");
	static_assert(std::is_reference_v<T> == false, "Static assertion failed: the C++ standard forbids allocators for reference elements.");

	template <typename>
	friend class cache_aligned_allocator;

public:
	using value_type = T;
	using size_type = var::size;
	using difference_type = var::ptrdiff;
	using propagate_on_container_copy_assignment = std::true_type;
	using propagate_on_container_move_assignment = std::true_type;

public:
	constexpr cache_aligned_allocator() noexcept = default;
	constexpr ~cache_aligned_allocator() noexcept = default;

	template <typename U>
	constexpr cache_aligned_allocator(const cache_aligned_allocator<U>&) noexcept {}


	_FE_NODISCARD_ constexpr T* allocate(FE::size count_p) noexcept
	{
		static_assert(sizeof(value_type) > 0, "Static assertion failed: value_type must be complete before calling allocate.");
		return static_cast<T*>(FE_ALIGNED_ALLOC(sizeof(T) * count_p, FE::CPU_L1_cache_line::size));
	}

	constexpr void deallocate(T* const ptr_p, _FE_MAYBE_UNUSED_ const size_t count_p) noexcept
	{
		FE_ASSERT(ptr_p != nullptr || count_p == 0, "Static assertion failed: null pointer cannot point to a block of non-zero size.");
		FE_ALIGNED_FREE(ptr_p);
	}

	template <typename U>
	constexpr bool operator==(const cache_aligned_allocator<U>&) const noexcept { return true; }
};


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
