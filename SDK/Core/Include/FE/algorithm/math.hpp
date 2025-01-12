#ifndef _FE_CORE_ALGORITHM_MATH_HPP_
#define _FE_CORE_ALGORITHM_MATH_HPP_
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
#include <FE/prerequisites.h>
#include <FE/type_traits.hxx>

// std
#include <cmath>
#include <limits> 




BEGIN_NAMESPACE(FE::algorithm::math)


_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::int64 approx_log2(FE::float64 value_p) noexcept
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


template<typename T>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ T abs(const T& x_p) noexcept
{
	return (x_p < 0) ? (x_p * -1) : x_p;
}


template<typename T>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ T max(const T& lhs_p, const T& rhs_p) noexcept
{
	return (lhs_p >= rhs_p) ? lhs_p : rhs_p;
}

template<typename T>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ T min(const T& lhs_p, const T& rhs_p) noexcept
{
	return (lhs_p <= rhs_p) ? lhs_p : rhs_p;
}


template<typename T>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ T clamp(const T& value_p, const T& min_p, const T& max_p) noexcept
{
	return ((min_p > value_p) ? min_p : ((value_p > max_p) ? max_p : value_p));
}


template<typename T>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::boolean is_nearly_equal(const T& lhs_p, const T& rhs_p, const T& offset_p) noexcept
{
	return  ::FE::algorithm::math::abs(lhs_p - rhs_p) <= offset_p;
}


template<typename N>
_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ N calculate_index_of_a_matrix(const N coordinate_x_p, const N coordinate_y_p, const N row_p) noexcept
{
	static_assert(FE::is_numeric<N>::value == true, "static assertion failed: the template argument N must be a numerical type.");
	return coordinate_x_p + (row_p * coordinate_y_p);
}


constexpr static inline FE::float64 pi = 3.1415926535897932;

_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::float64 radian_to_degree(FE::float64 radian_p) noexcept
{
	return (radian_p * 180.0) / pi;
}

_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ FE::float64 degree_to_radian(FE::float64 degree_p) noexcept
{
	return (degree_p * pi) / 180.0;
}


_FE_FORCE_INLINE_ FE::float64 calculate_2D_direction(FE::float64 vertical_p, FE::float64 horizontal_p) noexcept
{
	return ((180.0 * ::std::atan2(horizontal_p, vertical_p)) / pi);
}


constexpr inline ::FE::uint64 smallest_prime_number = 2;

_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ boolean is_prime(uint64 number_p) noexcept
{
	var::uint64 l_count = 2;
	while ((l_count < number_p) && (number_p % l_count) != 0)
	{
		++l_count;
	}

	return l_count == number_p;
}

uint64 to_upper_prime(uint64 number_p) noexcept;

uint64 to_lower_prime(uint64 number_p) noexcept;


template<typename T>
_FE_CONSTEXPR20_ T select_minimum(std::initializer_list<T>&& initializer_list_p) noexcept
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

	return l_buffer;
}

template<typename T>
_FE_CONSTEXPR20_ T select_maximum(std::initializer_list<T>&& initializer_list_p) noexcept
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

	return l_buffer;
}

END_NAMESPACE
#endif