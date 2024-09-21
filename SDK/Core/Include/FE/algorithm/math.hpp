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


#include <FE/prerequisites.h>
#include <FE/type_traits.hxx>

// std
#include <cmath>
#include <limits> 




BEGIN_NAMESPACE(FE::algorithm::math)


template <typename Integer>
_FORCE_INLINE_ _CONSTEXPR17_ FE::boolean is_even(const Integer integer_p) noexcept
{
	static_assert(std::is_integral<Integer>::value == true, "static assertion failed: the template argument N must be a integral type.");
	return (FE_MODULO_BY_2(integer_p) == 0);
}

template <typename Integer>
_FORCE_INLINE_ _CONSTEXPR17_ FE::boolean is_odd(const Integer integer_p) noexcept
{
	static_assert(std::is_integral<Integer>::value == true, "static assertion failed: the template argument N must be a integral type.");
	return (FE_MODULO_BY_2(integer_p) == 1);
}


template<typename T>
_FORCE_INLINE_ _CONSTEXPR20_ T abs(const T& x_p) noexcept
{
	return (x_p < 0) ? (x_p * -1) : x_p;
}


template<typename T>
_FORCE_INLINE_ _CONSTEXPR20_ T max(const T& lhs_p, const T& rhs_p) noexcept
{
	return (lhs_p >= rhs_p) ? lhs_p : rhs_p;
}

template<typename T>
_FORCE_INLINE_ _CONSTEXPR20_ T min(const T& lhs_p, const T& rhs_p) noexcept
{
	return (lhs_p <= rhs_p) ? lhs_p : rhs_p;
}


template<typename T>
_FORCE_INLINE_ _CONSTEXPR20_ T clamp(const T& value_p, const T& min_p, const T& max_p) noexcept
{
	return ((min_p > value_p) ? min_p : ((value_p > max_p) ? max_p : value_p));
}


template<typename T>
_FORCE_INLINE_ _CONSTEXPR20_ FE::boolean is_nearly_equal(const T& lhs_p, const T& rhs_p, const T& offset_p) noexcept
{
	return  ::FE::algorithm::math::abs(lhs_p - rhs_p) <= offset_p;
}


template<typename N>
_FORCE_INLINE_ _CONSTEXPR17_ N calculate_index_of_a_matrix(const N coordinate_x_p, const N coordinate_y_p, const N row_p) noexcept
{
	static_assert(FE::is_numeric<N>::value == true, "static assertion failed: the template argument N must be a numerical type.");
	return coordinate_x_p + (row_p * coordinate_y_p);
}


constexpr static inline FE::float64 pi = 3.1415926535897932;

_FORCE_INLINE_ _CONSTEXPR17_ FE::float64 radian_to_degree(FE::float64 radian_p) noexcept
{
	return (radian_p * 180.0) / pi;
}

_FORCE_INLINE_ _CONSTEXPR17_ FE::float64 degree_to_radian(FE::float64 degree_p) noexcept
{
	return (degree_p * pi) / 180.0;
}


_FORCE_INLINE_ _CONSTEXPR17_ FE::float64 calculate_2D_direction(FE::float64 vertical_p, FE::float64 horizontal_p) noexcept
{
	return ((180.0 * ::std::atan2(vertical_p, horizontal_p)) / pi);
}


constexpr inline ::FE::uint64 smallest_prime_number = 2;

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
_CONSTEXPR20_ T select_minimum(std::initializer_list<T>&& initializer_list_p) noexcept
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
_CONSTEXPR20_ T select_maximum(std::initializer_list<T>&& initializer_list_p) noexcept
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