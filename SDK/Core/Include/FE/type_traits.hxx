#ifndef _FE_CORE_TYPE_TRAITS_HXX_
#define _FE_CORE_TYPE_TRAITS_HXX_
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
#include <FE/definitions.h>
#include <FE/log/log.h>
#include <FE/types.hxx>
#include <FE/private/strlen.hxx>

// std
#include <array>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>




BEGIN_NAMESPACE(FE)

template <typename T>
struct remove_const_reference 
{
	using type = typename std::remove_const_t< std::remove_reference_t<T> >; 
};


template<typename T>
struct is_constant_string
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = false;
};

template<>
struct is_constant_string<ASCII*>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_constant_string<wchar*>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

#ifdef _FE_HAS_CXX20_
template<>
struct is_constant_string<UTF8*>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};
#endif

template<>
struct is_constant_string<UTF16*>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_constant_string<UTF32*>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_constant_string<ASCII[]>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_constant_string<wchar[]>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

#ifdef _FE_HAS_CXX20_
template<>
struct is_constant_string<UTF8[]>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};
#endif

template<>
struct is_constant_string<UTF16[]>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_constant_string<UTF32[]>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};


template<typename T>
struct is_char
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = false;
};

template<>
struct is_char<ASCII>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::ASCII>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<wchar>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::wchar>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

#ifdef _FE_HAS_CXX20_
template<>
struct is_char<UTF8>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::UTF8>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};
#endif

template<>
struct is_char<UTF16>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::UTF16>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<UTF32>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::UTF32>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};


template<typename T>
struct is_boolean
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = false;
};

template<>
struct is_boolean<var::boolean>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_boolean<boolean>
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = true;
};


template<typename T>
struct is_primitive
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = (
			(FE::is_boolean<T>::value == true) || (FE::is_char<T>::value == true) ||
			(std::is_integral_v<T> == true) || (std::is_floating_point_v<T> == true) ||
			(FE::is_constant_string<T>::value == true) || (std::is_pointer_v<T> == true) || 
			(std::is_null_pointer_v<T> == true)
		);
};


template<typename T>
struct is_numeric
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = (
			((std::is_integral<T>::value == true) && (FE::is_boolean<T>::value == false)) || 
			(std::is_floating_point<T>::value == true)
		);
};


template<typename T>
struct is_function
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = std::is_function_v<T> || std::is_member_function_pointer_v<T>;
};


enum struct TypeTriviality : boolean
{
	_Trivial = true,
	_NotTrivial = false
};

template <typename T>
struct is_trivial
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = std::is_trivially_constructible_v<T> && std::is_trivially_destructible_v<T> && std::is_trivially_copyable_v<T>;
};


template<typename T>
struct is_const_reference
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = std::is_const_v< std::remove_reference_t<T> > && std::is_reference_v< std::remove_const_t<T> >;
};


template <typename T>
struct is_std_vector : std::false_type {};

template <typename T, typename Allocator>
struct is_std_vector<std::vector<T, Allocator>> : std::true_type {};

template <typename T>
struct is_pmr_vector : std::false_type {};

template <typename T>
struct is_pmr_vector<std::pmr::vector<T>> : std::true_type {};

template<typename T>
struct is_scalable_array
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = (FE::is_std_vector<T>::value == true) || (FE::is_pmr_vector<T>::value == true);
};


template <typename T>
struct is_std_array : std::false_type {};

template <typename T, std::size_t N>
struct is_std_array<std::array<T, N>> : std::true_type {};

template<typename T>
struct is_array
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = (std::is_array<T>::value == true) || (FE::is_std_array<T>::value == true);
};


template <typename T>
struct is_std_string : std::false_type {};

template <typename T, typename Traits, typename Allocator>
struct is_std_string<std::basic_string<T, Traits, Allocator>> : std::true_type {};

template <typename T>
struct is_pmr_string : std::false_type {};

template <typename T, typename Traits>
struct is_pmr_string<std::pmr::basic_string<T, Traits>> : std::true_type {};


template <typename T>
struct is_std_string_view : std::false_type {};

template <typename T, typename Traits>
struct is_std_string_view<std::basic_string_view<T, Traits>> : std::true_type {};


template <typename T>
struct is_string_class : std::false_type {};

template <typename T, typename Traits, typename Allocator>
struct is_string_class<std::basic_string<T, Traits, Allocator>> : std::true_type {};

template <typename T, typename Traits>
struct is_string_class<std::pmr::basic_string<T, Traits>> : std::true_type {};

template <typename T, typename Traits>
struct is_string_class<std::basic_string_view<T, Traits>> : std::true_type {};


template <typename T, typename = void>
struct has_base_type : std::false_type {};

template <typename T>
struct has_base_type<T, std::void_t<typename T::base_type>> : std::true_type {};


template <typename T, typename = void>
struct is_reflective : std::false_type {};

template <typename T>
struct is_reflective<T, std::void_t<typename T::is_reflective>> : std::true_type {};


template <typename T, typename = void>
struct has_value_type : std::false_type {};

template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {};


template <typename T, typename = void>
struct has_element_type : std::false_type {};

template <typename T>
struct has_element_type<T, std::void_t<typename T::element_type>> : std::true_type {};


template <typename T, typename = void>
struct has_iterator : std::false_type {};

template <typename T>
struct has_iterator<T, std::void_t<typename T::iterator>> : std::true_type {};


template <typename T, typename = void>
struct has_traits_type : std::false_type {};

template <typename T>
struct has_traits_type<T, std::void_t<typename T::traits_type>> : std::true_type {};


template <typename T, typename = void>
struct has_allocator_type : std::false_type {};

template <typename T>
struct has_allocator_type<T, std::void_t<typename T::allocator_type>> : std::true_type {};


template <typename T, typename = void>
struct has_size_type : std::false_type {};

template <typename T>
struct has_size_type<T, std::void_t<typename T::size_type>> : std::true_type {};


template <typename T, typename = void>
struct has_difference_type : std::false_type {};

template <typename T>
struct has_difference_type<T, std::void_t<typename T::difference_type>> : std::true_type {};


template <typename T, typename = void>
struct has_reference : std::false_type {};

template <typename T>
struct has_reference<T, std::void_t<typename T::reference>> : std::true_type {};


template <typename T, typename = void>
struct has_const_reference : std::false_type {};

template <typename T>
struct has_const_reference<T, std::void_t<typename T::const_reference>> : std::true_type {};


template <typename T, typename = void>
struct has_pointer : std::false_type {};

template <typename T>
struct has_pointer<T, std::void_t<typename T::pointer>> : std::true_type {};


template <typename T, typename = void>
struct has_const_pointer : std::false_type {};

template <typename T>
struct has_const_pointer<T, std::void_t<typename T::const_pointer>> : std::true_type {};


template <typename T, typename = void>
struct has_const_iterator : std::false_type {};

template <typename T>
struct has_const_iterator<T, std::void_t<typename T::const_iterator>> : std::true_type {};


template <typename T, typename = void>
struct has_reverse_iterator : std::false_type {};

template <typename T>
struct has_reverse_iterator<T, std::void_t<typename T::reverse_iterator>> : std::true_type {};


template <typename T, typename = void>
struct has_const_reverse_iterator : std::false_type {};

template <typename T>
struct has_const_reverse_iterator<T, std::void_t<typename T::const_reverse_iterator>> : std::true_type {};


template<typename T>
struct is_serializable
{
	_FE_MAYBE_UNUSED_ static constexpr inline bool value = (
		(FE::is_trivial<T>::value == true) ||
		(FE::is_reflective<T>::value == true) ||
		(FE::is_scalable_array<T>::value == true) ||
		(FE::is_array<T>::value == true) ||
		(FE::is_string_class<T>::value == true)
		);
};


template<class To, class From>
_FE_FORCE_INLINE_ To polymorphic_cast(From& from_p) noexcept
{
	static_assert((std::is_base_of<To, From>::value == false) || (std::is_base_of<From, To>::value == false), "polymorphic_cast<T>() failed: Both types are incompatible to cast.");
#ifdef _DEBUG_
	return (dynamic_cast<To>(from_p));
#else
	return static_cast<To>(from_p);
#endif
}


template<class To, class From>
_FE_FORCE_INLINE_ To forward_to(std::conditional_t< std::is_reference<From>::value, From, std::conditional_t< std::is_rvalue_reference<From>::value, From, From& > > from_p) noexcept
{
	if constexpr (std::is_lvalue_reference<To>::value == true)
	{
		return from_p;
	}
	else if constexpr (std::is_lvalue_reference<To>::value == false)
	{
		return std::move(from_p);
	}
}


END_NAMESPACE
#endif