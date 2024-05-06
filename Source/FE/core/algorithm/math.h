#ifndef _FE_CORE_ALGORITHM_MATH_H_
#define _FE_CORE_ALGORITHM_MATH_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.

#define DIVIDE_BY_2(input) ((input) >> 1)
#define DIVIDE_BY_4(input) ((input) >> 2)
#define DIVIDE_BY_8(input) ((input) >> 3)
#define DIVIDE_BY_16(input) ((input) >> 4)
#define DIVIDE_BY_32(input) ((input) >> 5)
#define DIVIDE_BY_64(input) ((input) >> 6)
#define DIVIDE_BY_128(input) ((input) >> 7)

#define MODULO_BY_2(input) ((input) & 1)
#define MODULO_BY_4(input) ((input) & 3)
#define MODULO_BY_8(input) ((input) & 7)
#define MODULO_BY_16(input) ((input) & 15)
#define MODULO_BY_32(input) ((input) & 31)
#define MODULO_BY_64(input) ((input) & 63)
#define MODULO_BY_128(input) ((input) & 127)

#define ABS(x) ((x < 0) ? x * -1 : x)
#define CALCULATE_INDEX(row, coordinate_x, coordinate_y) (coordinate_x + (row * coordinate_y))
#define IS_NEARLY_EQUAL(left, right, offset) (ABS(left_p - right_p) <= offset_p)

#define MAX(a, b) ((a >= b) ? a : b)
#define MIN(a, b) ((a <= b) ? a : b)


#define _SMALLEST_PRIME_NUMBER_ 2


#include <FE/core/prerequisites.h>




BEGIN_NAMESPACE(FE::algorithm::math)


_CONSTEXPR17_ _FORCE_INLINE_ boolean is_prime(uint64 number_p) noexcept
{
	var::uint64 l_count = 2;
	while ((l_count < number_p) && (number_p % l_count) != 0)
	{
		++l_count;
	}

	return l_count == number_p;
}

_CONSTEXPR17_ uint64 to_higher_prime(uint64 number_p) noexcept;
_CONSTEXPR17_ uint64 to_lower_prime(uint64 number_p) noexcept;


_CONSTEXPR17_ uint64 abs(int64 value_p) noexcept
{
	return ABS(value_p);
}

_CONSTEXPR17_ float64 abs(float64 value_p) noexcept
{
	return ABS(value_p);
}


_CONSTEXPR17_ index_t calculate_index(index_t row_p, index_t coordinate_x_p, index_t coordinate_y_p) noexcept
{
	return CALCULATE_INDEX(row_p, coordinate_x_p, coordinate_y_p);
}


_CONSTEXPR17_ boolean is_nearly_equal(float64 left_p, float64 right_p, float64 offset_p) noexcept
{
	return IS_NEARLY_EQUAL(left_p, right_p, offset_p);
}


template<typename T>
_CONSTEXPR17_ _FORCE_INLINE_ const T& select_minimum(const T& left_p, const T& right_p) noexcept
{
	return MIN(left_p, right_p);
}

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
_CONSTEXPR17_ _FORCE_INLINE_ const T& select_maximum(const T& left_p, const T& right_p) noexcept
{
	return MAX(left_p, right_p);
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


template<typename N>
_CONSTEXPR17_ N clamp(N value_p, N min_p, N max_p) noexcept
{
	if (min_p > value_p)
	{
		return min_p;
	}
	else if (value_p > max_p)
	{
		return max_p;
	}
	
	return value_p;
}


END_NAMESPACE
#endif