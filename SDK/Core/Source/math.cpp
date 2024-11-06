#include <FE/algorithm/math.hpp>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.

#ifdef _FE_ON_X86_64_
	#ifdef __AVX__
		// x86-64 AVX
		#include <immintrin.h>
	#else
		#error "Please check if the current CPU supports AVX SIMD instruction set and has ymm registers."
	#endif

	#ifdef __SSE2__
		// x86-64 SSE2
		#include <emmintrin.h>
	#else
		#error "Please check if the current CPU supports SSE2 SIMD instruction set and has xmm registers."
	#endif
#endif




BEGIN_NAMESPACE(FE::algorithm::math)


uint64 to_higher_prime(uint64 number_p) noexcept
{
	constexpr uint64 l_magical_seven = 7;

	switch (number_p)
	{
	case 0:
		_FE_FALLTHROUGH_;
	case 1:
		return smallest_prime_number;

	case 3:
		_FE_FALLTHROUGH_;
	case 5:
		_FE_FALLTHROUGH_;
	case 7:
		_FE_FALLTHROUGH_;
	case 11:
		_FE_FALLTHROUGH_;
	case 13:
		return number_p;

	default:
		{
			var::uint64 l_number = number_p;
	
			if ((l_number % 2) == 0)
			{
				l_number ^= l_magical_seven;
	
				if ((l_number % 2) == 0)
				{
					++l_number;
				}
			}
	
			while (is_prime(l_number) == false)
			{
				++l_number;
			}
			return l_number;
		}
	}
}

uint64 to_lower_prime(uint64 number_p) noexcept
{
	constexpr uint64 l_magical_seven = 7;

	switch (number_p)
	{
	case 0:
		_FE_FALLTHROUGH_;
	case 1:
		return smallest_prime_number;

	case 3:
		_FE_FALLTHROUGH_;
	case 5:
		_FE_FALLTHROUGH_;
	case 7:
		_FE_FALLTHROUGH_;
	case 11:
		_FE_FALLTHROUGH_;
	case 13:
		return number_p;

	default:
		{
			var::uint64 l_number = number_p;

			if ((l_number % 2) == 0)
			{
				l_number ^= l_magical_seven;

				if ((l_number % 2) == 0)
				{
					++l_number;
				}
			}

			while (is_prime(l_number) == false)
			{
				--l_number;
			}
			return l_number;
		}
	}
}


END_NAMESPACE