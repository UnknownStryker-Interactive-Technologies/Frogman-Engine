/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/algorithm/math.hxx>

#ifdef _FE_ON_X86_64_
#include <immintrin.h>
#endif




BEGIN_NAMESPACE(FE::algorithm::math)


FE::int64 approx_log2(FE::float64 value_p) noexcept
{
	var::int64 l_exponent = 0;
	var::float64 l_value = value_p;

	if (1.0 < value_p)
	{
		while (1.0 < l_value)
		{
			l_value /= 2.0;
			++l_exponent;
		}
		return l_exponent;
	}

	while (1.0 > l_value)
	{
		l_value *= 2.0;
		--l_exponent;
	}
	return l_exponent;
}

boolean is_prime(uint64 number_p) noexcept
{
	var::uint64 l_count = 2;
	while ((l_count < number_p) && (number_p % l_count) != 0)
	{
		++l_count;
	}

	return l_count == number_p;
}

uint64 to_upper_prime(uint64 number_p) noexcept
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