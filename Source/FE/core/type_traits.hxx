#ifndef _FE_CORE_TYPE_TRAITS_HXX_
#define _FE_CORE_TYPE_TRAITS_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/macros/definitions.h>
#include <FE/core/log/log.h>
#include <FE/core/types.hxx>
#include <FE/core/private/strlen.hxx>

// std
#include <string>
#include <string_view>
#include <type_traits>




BEGIN_NAMESPACE(FE)

template <typename T>
struct remove_const_reference { using type = typename std::remove_const<typename std::remove_reference<T>::type>::type; };

template <bool TestResult, typename TrueType, typename FalseType>
struct conditional_type {};

template<typename TrueType, typename FalseType>
struct conditional_type<true, TrueType, FalseType> { using type = TrueType; };

template<typename TrueType, typename FalseType>
struct conditional_type<false, TrueType, FalseType> { using type = FalseType; };


template<typename T>
struct is_c_style_constant_string
{
	_MAYBE_UNUSED_ static constexpr inline bool value = false;
};

template<>
struct is_c_style_constant_string<ASCII*>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_c_style_constant_string<schar*>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_c_style_constant_string<uchar*>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_c_style_constant_string<wchar*>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

#ifdef _HAS_CXX20_
template<>
struct is_c_style_constant_string<UTF8*>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};
#endif

template<>
struct is_c_style_constant_string<UTF16*>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_c_style_constant_string<UTF32*>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_c_style_constant_string<ASCII[]>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_c_style_constant_string<schar[]>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_c_style_constant_string<uchar[]>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_c_style_constant_string<wchar[]>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

#ifdef _HAS_CXX20_
template<>
struct is_c_style_constant_string<UTF8[]>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};
#endif

template<>
struct is_c_style_constant_string<UTF16[]>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_c_style_constant_string<UTF32[]>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};


template<typename T>
struct is_char
{
	_MAYBE_UNUSED_ static constexpr inline bool value = false;
};

template<>
struct is_char<ASCII>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::ASCII>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<schar>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::schar>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<uchar>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::uchar>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<wchar>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::wchar>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

#ifdef _HAS_CXX20_
template<>
struct is_char<UTF8>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::UTF8>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};
#endif

template<>
struct is_char<UTF16>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::UTF16>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<UTF32>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::UTF32>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};


template<typename T>
struct is_boolean
{
	_MAYBE_UNUSED_ static constexpr inline bool value = false;
};

template<>
struct is_boolean<var::boolean>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_boolean<boolean>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};


template<typename T>
struct is_nullptr
{
	_MAYBE_UNUSED_ static constexpr inline bool value = false;
};

template<>
struct is_nullptr<std::nullptr_t>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};


template<typename T>
struct is_primitive
{
	_MAYBE_UNUSED_ static constexpr inline bool value = ((FE::is_boolean<T>::value == true) || (FE::is_char<T>::value == true) || (std::is_integral<T>::value == true) || (std::is_floating_point<T>::value == true) || (FE::is_c_style_constant_string<T>::value == true) || (std::is_pointer<T>::value == true) || (FE::is_nullptr<T>::value == true));
};


template<typename T>
struct is_numeric
{
	_MAYBE_UNUSED_ static constexpr inline bool value = (((std::is_integral<T>::value == true) && (FE::is_boolean<T>::value == false)) || (std::is_floating_point<T>::value == true));
};

template<typename T>
struct is_function
{
	_MAYBE_UNUSED_ static constexpr inline bool value = (std::is_function<T>::value == true) || (std::is_member_function_pointer<T>::value == true);
};


enum struct TYPE_TRIVIALITY : boolean
{
	_TRIVIAL = true,
	_NOT_TRIVIAL = false
};

template <typename T>
struct is_trivial
{
	_MAYBE_UNUSED_ static constexpr inline bool value = std::is_trivially_constructible<T>::value && std::is_trivially_destructible<T>::value;
};


template<typename T>
struct is_string_class
{
	_MAYBE_UNUSED_ static constexpr inline bool value = false;
};

template<>
struct is_string_class<std::string>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_string_class<std::wstring>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

#ifdef __cpp_lib_char8_t
template<>
struct is_string_class<std::u8string>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};
#endif

template<>
struct is_string_class<std::u16string>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_string_class<std::u32string>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};


template<>
struct is_string_class<std::string_view>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_string_class<std::wstring_view>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

#ifdef __cpp_lib_char8_t
template<>
struct is_string_class<std::u8string_view>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};
#endif

template<>
struct is_string_class<std::u16string_view>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_string_class<std::u32string_view>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};


template<typename T>
struct is_const_reference
{
	_MAYBE_UNUSED_ static constexpr inline bool value = (std::is_const< std::remove_reference_t<T> >::value == true) && (std::is_reference< std::remove_const_t<T> >::value == true);
};


template <typename T, typename = void>
struct has_base_type : std::false_type {};

template <typename T>
struct has_base_type<T, std::void_t<typename T::base_type>> : std::true_type {};


template <typename T, typename = void>
struct has_value_type : std::false_type {};

template <typename T>
struct has_value_type<T, std::void_t<typename T::has_value_type>> : std::true_type {};


template <typename T, typename = void>
struct has_element_type : std::false_type {};

template <typename T>
struct has_element_type<T, std::void_t<typename T::has_element_type>> : std::true_type {};


template <typename T, typename = void>
struct is_reflectable : std::false_type {};

template <typename T>
struct is_reflectable<T, std::void_t<typename T::is_reflectable>> : std::true_type {};


template <typename T, typename = void>
struct has_iterator : std::false_type {};

template <typename T>
struct has_iterator<T, std::void_t<typename T::iterator>> : std::true_type {};


template<typename T>
struct is_serializable
{
	_MAYBE_UNUSED_ static constexpr inline bool value = ((FE::is_trivial<T>::value == true) || (FE::is_reflectable<T>::value == true) || (FE::has_iterator<T>::value == true));
};


template<class To, class From>
_FORCE_INLINE_ To polymorphic_cast(From& from_p) noexcept
{
	static_assert((std::is_base_of<To, From>::value == false) || (std::is_base_of<From, To>::value == false), "polymorphic_cast<T>() failed: Both types are incompatible to cast.");
#ifdef _DEBUG_
	return (dynamic_cast<To>(from_p));
#else
	return static_cast<To>(from_p);
#endif
}


template<class To, class From>
_FORCE_INLINE_ To forward_to(std::conditional_t< std::is_reference<From>::value, From, std::conditional_t< std::is_rvalue_reference<From>::value, From, From& > > from_p) noexcept
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