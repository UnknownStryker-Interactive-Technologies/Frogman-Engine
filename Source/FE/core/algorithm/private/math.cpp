#include <FE/core/algorithm/math.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.




BEGIN_NAMESPACE(FE::algorithm::math)


_CONSTEXPR17_ uint64 to_higher_prime(uint64 number_p) noexcept
{
	constexpr uint64 l_magical_seven = 7;

	switch (number_p)
	{
	case 0:
		_FALLTHROUGH_;
	case 1:
		return _SMALLEST_PRIME_NUMBER_;

	case 3:
		_FALLTHROUGH_;
	case 5:
		_FALLTHROUGH_;
	case 7:
		_FALLTHROUGH_;
	case 11:
		_FALLTHROUGH_;
	case 13:
		return number_p;

	default:
		{
			var::uint64 l_number = number_p;
	
			if (MODULO_BY_2(l_number) == 0)
			{
				l_number ^= l_magical_seven;
	
				if (MODULO_BY_2(l_number) == 0)
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

_CONSTEXPR17_ uint64 to_lower_prime(uint64 number_p) noexcept
{
	constexpr uint64 l_magical_seven = 7;

	switch (number_p)
	{
	case 0:
		_FALLTHROUGH_;
	case 1:
		return _SMALLEST_PRIME_NUMBER_;

	case 3:
		_FALLTHROUGH_;
	case 5:
		_FALLTHROUGH_;
	case 7:
		_FALLTHROUGH_;
	case 11:
		_FALLTHROUGH_;
	case 13:
		return number_p;

	default:
		{
			var::uint64 l_number = number_p;

			if (MODULO_BY_2(l_number) == 0)
			{
				l_number ^= l_magical_seven;

				if (MODULO_BY_2(l_number) == 0)
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