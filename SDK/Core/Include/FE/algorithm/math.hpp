#ifndef _FE_CORE_ALGORITHM_MATH_HPP_
#define _FE_CORE_ALGORITHM_MATH_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.

#ifdef FE_DIVIDE_BY_2
	#error FE_DIVIDE_BY_2 is a reserved Frogman Engine macro keyword.
#endif
#define FE_DIVIDE_BY_2(input) ((input) >> 1)

#ifdef FE_DIVIDE_BY_4
	#error FE_DIVIDE_BY_4 is a reserved Frogman Engine macro keyword.
#endif
#define FE_DIVIDE_BY_4(input) ((input) >> 2)

#ifdef FE_DIVIDE_BY_8
	#error FE_DIVIDE_BY_8 is a reserved Frogman Engine macro keyword.
#endif
#define FE_DIVIDE_BY_8(input) ((input) >> 3)

#ifdef FE_DIVIDE_BY_16
	#error FE_DIVIDE_BY_16 is a reserved Frogman Engine macro keyword.
#endif
#define FE_DIVIDE_BY_16(input) ((input) >> 4)

#ifdef FE_DIVIDE_BY_32
	#error FE_DIVIDE_BY_32 is a reserved Frogman Engine macro keyword.
#endif
#define FE_DIVIDE_BY_32(input) ((input) >> 5)

#ifdef FE_DIVIDE_BY_64
	#error FE_DIVIDE_BY_64 is a reserved Frogman Engine macro keyword.
#endif
#define FE_DIVIDE_BY_64(input) ((input) >> 6)

#ifdef FE_DIVIDE_BY_128
	#error FE_DIVIDE_BY_128 is a reserved Frogman Engine macro keyword.
#endif
#define FE_DIVIDE_BY_128(input) ((input) >> 7)


#ifdef FE_MODULO_BY_2
	#error FE_MODULO_BY_2 is a reserved Frogman Engine macro keyword.
#endif
#define FE_MODULO_BY_2(input) ((input) & 1)

#ifdef FE_MODULO_BY_4
	#error FE_MODULO_BY_4 is a reserved Frogman Engine macro keyword.
#endif
#define FE_MODULO_BY_4(input) ((input) & 3)

#ifdef FE_MODULO_BY_8
	#error FE_MODULO_BY_8 is a reserved Frogman Engine macro keyword.
#endif
#define FE_MODULO_BY_8(input) ((input) & 7)

#ifdef FE_MODULO_BY_16
	#error FE_MODULO_BY_16 is a reserved Frogman Engine macro keyword.
#endif
#define FE_MODULO_BY_16(input) ((input) & 15)

#ifdef FE_MODULO_BY_32
	#error FE_MODULO_BY_32 is a reserved Frogman Engine macro keyword.
#endif
#define FE_MODULO_BY_32(input) ((input) & 31)

#ifdef FE_MODULO_BY_64
	#error FE_MODULO_BY_64 is a reserved Frogman Engine macro keyword.
#endif
#define FE_MODULO_BY_64(input) ((input) & 63)

#ifdef FE_MODULO_BY_128
	#error FE_MODULO_BY_128 is a reserved Frogman Engine macro keyword.
#endif
#define FE_MODULO_BY_128(input) ((input) & 127)


#ifdef FE_IS_EVEN
	#error FE_IS_EVEN is a reserved Frogman Engine macro keyword.
#endif
#define FE_IS_EVEN(input) (FE_MODULO_BY_2(input) == 0)

#ifdef FE_IS_ODD
	#error FE_IS_ODD is a reserved Frogman Engine macro keyword.
#endif
#define FE_IS_ODD(input) (FE_MODULO_BY_2(input) == 1)


#ifdef FE_ABS
	#error FE_ABS is a reserved Frogman Engine macro keyword.
#endif
#define FE_ABS(x) ((x < 0) ? x * -1 : x)


#ifdef FE_CALCULATE_INDEX
	#error FE_CALCULATE_INDEX is a reserved Frogman Engine macro keyword.
#endif
#define FE_CALCULATE_INDEX(row, coordinate_x, coordinate_y) (coordinate_x + (row * coordinate_y))


#ifdef FE_MAX
	#error FE_MAX is a reserved Frogman Engine macro keyword.
#endif
#define FE_MAX(a, b) ((a >= b) ? a : b)

#ifdef FE_MIN
	#error FE_MIN is a reserved Frogman Engine macro keyword.
#endif
#define FE_MIN(a, b) ((a <= b) ? a : b)


#ifdef FE_CLAMP
	#error FE_CLAMP is a reserved Frogman Engine macro keyword.
#endif
#define FE_CLAMP(value, min, max) ((min > value) ? min : ((value > max) ? max : value))


#ifdef FE_IS_NEARLY_EQUAL
	#error FE_IS_NEARLY_EQUAL is a reserved Frogman Engine macro keyword.
#endif
// std for std::numeric_limits<T>::epsilon()
#include <limits> 
#define FE_IS_NEARLY_EQUAL(left, right, offset) (FE_ABS(left - right) <= offset)


#include <FE/prerequisites.h>


constexpr static FE::float64 pi = 3.1415926535897932;

#ifdef FE_DEGREE_TO_RADIAN
    #error FE_DEGREE_TO_RADIAN is a reserved Frogman Engine macro function name.
#endif
#define FE_DEGREE_TO_RADIAN(x) ((x * ::FE::pi) / 180.0)

#ifdef FE_RADIAN_TO_DEGREE
    #error FE_RADIAN_TO_DEGREE is a reserved Frogman Engine macro function name.
#endif
#define FE_RADIAN_TO_DEGREE(x) ((x * 180.0) / ::FE::pi)


#include <cmath>
#ifdef FE_CALCULATE_DIRECTION_2D
    #error FE_CALCULATE_DIRECTION_2D is a reserved Frogman Engine macro function name.
#endif
#define FE_CALCULATE_DIRECTION_2D(vertical, horizontal) ((180.0 * ::std::atan2(vertical_p, horizontal_p)) / FE::pi)


#define _SMALLEST_PRIME_NUMBER_ 2




BEGIN_NAMESPACE(FE::algorithm::math)


_FORCE_INLINE_ _CONSTEXPR17_ boolean is_prime(uint64 number_p) noexcept
{
	var::uint64 l_count = 2;
	while ((l_count < number_p) && (number_p % l_count) != 0)
	{
		++l_count;
	}

	return l_count == number_p;
}

uint64 to_higher_prime(uint64 number_p) noexcept;
uint64 to_lower_prime(uint64 number_p) noexcept;

template<typename T>
_CONSTEXPR17_ T select_minimum(std::initializer_list<T>&& initializer_list_p) noexcept
{
	T* l_initializer_list_begin = const_cast<T*>(initializer_list_p.begin());
	const T* const l_initializer_list_end = initializer_list_p.end();
	T l_buffer = std::move(*l_initializer_list_begin);
	++l_initializer_list_begin;

	while (l_initializer_list_begin != l_initializer_list_end)
	{
		if (l_buffer > *l_initializer_list_begin)
		{
			l_buffer = std::move(*l_initializer_list_begin);
		}
		++l_initializer_list_begin;
	}
}

template<typename T>
_CONSTEXPR17_ T select_maximum(std::initializer_list<T>&& initializer_list_p) noexcept
{
	T* l_initializer_list_begin = const_cast<T*>(initializer_list_p.begin());
	const T* const l_initializer_list_end = initializer_list_p.end();
	T l_buffer = std::move(*l_initializer_list_begin);
	++l_initializer_list_begin;

	while (l_initializer_list_begin != l_initializer_list_end)
	{
		if (l_buffer < *l_initializer_list_begin)
		{
			l_buffer = std::move(*l_initializer_list_begin);
		}
		++l_initializer_list_begin;
	}
}


/*
https://www.wolframalpha.com/
https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#ig_expand=5754,6,5754,5756

    Taylor's series of sin(𝑥)

    sin(𝑥) = 𝑥 - (𝑥^3 / 3!) + (𝑥^5 / 5!) - (𝑥^7 / 7!) + (𝑥^9 / 9!)
               - (𝑥^11 / 11!) + (𝑥^13 / 13!) - (𝑥^15 / 15!) + (𝑥^17 / 17!)

    180°(angle) = π(radian)
*/
FE::float64 sin(::var::float64 radian_p) noexcept;


/*
    Taylor's series of cos(𝑥)

    cos(𝑥) = 1.0 - (𝑥^2 / 2!) + (𝑥^4 / 4!) - (𝑥^6 / 6!) + (𝑥^8 / 8!)
                 - (𝑥^10 / 10!) + (𝑥^12 / 12!) - (𝑥^14 / 14!) + (𝑥^16 / 16!)

    180°(angle) = π(radian)
*/
FE::float64 cos(::var::float64 radian_p) noexcept;


END_NAMESPACE
#endif