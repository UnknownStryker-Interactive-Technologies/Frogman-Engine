#ifndef _FE_CORE_TYPE_TRAITS_HXX_
#define _FE_CORE_TYPE_TRAITS_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/types.hxx>
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
struct is_c_style_constant_string<character*>
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
struct is_c_style_constant_string<character[]>
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
struct is_char<character>
{
	_MAYBE_UNUSED_ static constexpr inline bool value = true;
};

template<>
struct is_char<var::character>
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


enum struct TYPE_TRAIT : int8
{
	_UNDEFINED = -1,
	_PRIMITIVE = 0,
	_BOOLEAN = 1,
	_CHARACTER = 2,
	_SIGNED_INTEGER = 3,
	_UNSIGNED_INTEGER = 4,
	_FLOAT = 5,
	_C_STYLE_STRING = 6,
	_POINTER = 7,
	_NULLPTR = 8,
	_CLASS = 9,
	_FUNCTION = 10
};

template<typename T>
constexpr inline TYPE_TRAIT check_type() noexcept
{
	if constexpr (FE::is_boolean<T>::value)
	{
		return TYPE_TRAIT::_BOOLEAN;
	}
	else if constexpr (FE::is_char<T>::value)
	{
		return TYPE_TRAIT::_CHARACTER;
	}
	else if constexpr (std::is_signed<T>::value)
	{
		return TYPE_TRAIT::_SIGNED_INTEGER;
	}
	else if constexpr (std::is_unsigned<T>::value)
	{
		return TYPE_TRAIT::_UNSIGNED_INTEGER;
	}
	else if constexpr (std::is_floating_point<T>::value)
	{
		return TYPE_TRAIT::_FLOAT;
	}
	else if constexpr (FE::is_c_style_constant_string<T>::value)
	{
		return TYPE_TRAIT::_C_STYLE_STRING;
	}
	else if constexpr (std::is_pointer<T>::value)
	{
		return TYPE_TRAIT::_POINTER;
	}
	else if constexpr (FE::is_nullptr<T>::value)
	{
		return TYPE_TRAIT::_NULLPTR;
	}
	else if constexpr (std::is_function<T>::value)
	{
		return TYPE_TRAIT::_FUNCTION;
	}

	return TYPE_TRAIT::_UNDEFINED;
}


enum struct TYPE_TRIVIALITY : boolean
{
	_TRIVIAL = true,
	_NOT_TRIVIAL = false
};

template <typename T>
struct is_trivial
{
	_MAYBE_UNUSED_ static constexpr inline TYPE_TRIVIALITY value = static_cast<TYPE_TRIVIALITY>(std::is_trivially_constructible<T>::value && std::is_trivially_destructible<T>::value);
};




END_NAMESPACE
#endif