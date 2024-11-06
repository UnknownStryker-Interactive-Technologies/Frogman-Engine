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
#include <FE/algorithm/math.hpp>

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