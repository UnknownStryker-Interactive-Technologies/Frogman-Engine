#ifndef _FE_CORE_ALGORITHM_MATH_HXX_
#define _FE_CORE_ALGORITHM_MATH_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#include <FE/prerequisites.hxx>
#include <FE/type_traits.hxx>

// std
#include <cmath>
#include <limits> 

#include <glm/vec2.hpp>




BEGIN_NAMESPACE(FE::algorithm::math)


/* The MSVC linker spits:
1>LINK : C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\Unit-Tests\Test-App\Debug\FE_UNIT_TEST.exe not found or not built by the last incremental link; performing full link
1>   Creating library C:/Users/leeho/OneDrive/문서/GitHub/Frogman-Engine/SDK/Tests/Unit-Tests/Solution_X64_AVX/Debug/FE_UNIT_TEST.lib and object C:/Users/leeho/OneDrive/문서/GitHub/Frogman-Engine/SDK/Tests/Unit-Tests/Solution_X64_AVX/Debug/FE_UNIT_TEST.exp
1>FE.algorithm.utility.obj : error LNK2019: unresolved external symbol "__int64 const __cdecl FE::algorithm::math::approx_log2(double)" (?approx_log2@math@algorithm@FE@@YA?B_JN@Z) referenced in function "class std::_Array_iterator<int,10> __cdecl FE::algorithm::utility::binary_search<class std::_Array_iterator<int,10> >(class std::_Array_iterator<int,10>,class std::_Array_iterator<int,10>,int const &)" (??$binary_search@V?$_Array_iterator@H$09@std@@@utility@algorithm@FE@@YA?AV?$_Array_iterator@H$09@std@@V34@0AEBH@Z)
1>C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\Unit-Tests\Test-App\Debug\FE_UNIT_TEST.exe : fatal error LNK1120: 1 unresolved externals

when the function body is defined within a .cpp file.
*/
FE::int64 approx_log2(FE::float64 value_p) noexcept;

template<typename T>
_FE_CONSTEXPR17_ T abs(const T& x_p) noexcept
{
	return (x_p < 0) ? (x_p * -1) : x_p;
}


template<typename T>
_FE_CONSTEXPR17_ T max(const T& lhs_p, const T& rhs_p) noexcept
{
	return (lhs_p >= rhs_p) ? lhs_p : rhs_p;
}

template<typename T>
_FE_CONSTEXPR17_ T min(const T& lhs_p, const T& rhs_p) noexcept
{
	return (lhs_p <= rhs_p) ? lhs_p : rhs_p;
}


template<typename T>
_FE_CONSTEXPR17_ T clamp(const T& value_p, const T& min_p, const T& max_p) noexcept
{
	return ((min_p > value_p) ? min_p : ((value_p > max_p) ? max_p : value_p));
}


template<typename T>
_FE_CONSTEXPR17_ FE::boolean is_nearly_equal(const T& lhs_p, const T& rhs_p, const T& offset_p) noexcept
{
	return  ::FE::algorithm::math::abs(lhs_p - rhs_p) <= offset_p;
}


template<typename N>
_FE_CONSTEXPR17_ N calculate_index_of_a_matrix(const N coordinate_x_p, const N coordinate_y_p, const N row_p) noexcept
{
	static_assert(FE::is_numeric<N>::value == true, "static assertion failed: the template argument N must be a numerical type.");
	return coordinate_x_p + (row_p * coordinate_y_p);
}


constexpr static inline FE::float64 pi = 3.1415926535897932;


struct movement
{
	var::float32 _direction; // value range: (-180.0, 180.0]; in degrees.
	var::float32 _speed; // value range: [0.0, 1.0]
};

_FE_FORCE_INLINE_ movement calculate_movement(const glm::vec2& vector_p)
{
	movement l_result = 
	{
		._direction = std::atan2(vector_p.x, vector_p.y) * 180.0f / pi,
		._speed = std::sqrt((vector_p.x * vector_p.x) + (vector_p.y * vector_p.y))
	};

	if (l_result._speed > 1.0f)
	{
		l_result._speed = 1.0f;
	}
	return l_result;
}


_FE_CONSTEXPR17_ FE::float64 radian_to_degree(FE::float64 radian_p) noexcept
{
	return (radian_p * 180.0) / pi;
}

_FE_CONSTEXPR17_ FE::float64 degree_to_radian(FE::float64 degree_p) noexcept
{
	return (degree_p * pi) / 180.0;
}


_FE_FORCE_INLINE_ FE::float64 calculate_2D_direction(FE::float64 vertical_p, FE::float64 horizontal_p) noexcept
{
	return ((180.0 * ::std::atan2(horizontal_p, vertical_p)) / pi);
}


constexpr inline ::FE::uint64 smallest_prime_number = 2;

boolean is_prime(uint64 number_p) noexcept;

uint64 to_upper_prime(uint64 number_p) noexcept;

uint64 to_lower_prime(uint64 number_p) noexcept;


template<typename T>
constexpr T select_minimum(std::initializer_list<T>&& initializer_list_p) noexcept
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
constexpr T select_maximum(std::initializer_list<T>&& initializer_list_p) noexcept
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