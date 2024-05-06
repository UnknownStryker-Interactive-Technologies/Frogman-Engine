#ifndef _FE_CORE_HASH_HXX_
#define _FE_CORE_HASH_HXX_
#pragma warning(push)
#pragma warning(disable: 4702)
#include <city.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/algorithm/string.hxx>
#include <FE/core/random.hxx>
#include <string>
#include <string_view>




BEGIN_NAMESPACE(FE)


template<typename CharT>
class basic_string_view;


enum struct HASH_INPUT_DATA_TYPE : uint8
{
	_ADDRESS = 0,
	_C_STRING = 1,
	_STRING_CLASS = 2,
	_BINARY = 3
};


template<typename T>
_FORCE_INLINE_ constexpr HASH_INPUT_DATA_TYPE evaluate_hash_input_data_type()
{
	if constexpr (FE::is_c_style_constant_string<T>::value == true)
	{
		return HASH_INPUT_DATA_TYPE::_C_STRING;
	}
	else if constexpr (std::is_pointer<T>::value == true)
	{
		return HASH_INPUT_DATA_TYPE::_ADDRESS;
	}
	else if constexpr (std::is_same<T, std::string>::value == true)
	{
		return HASH_INPUT_DATA_TYPE::_STRING_CLASS;
	}
	else if constexpr (std::is_same<T, std::string_view>::value == true)
	{
		return HASH_INPUT_DATA_TYPE::_STRING_CLASS;
	}
	else if constexpr (std::is_same<T, FE::basic_string_view<T>>::value == true)
	{
		return HASH_INPUT_DATA_TYPE::_STRING_CLASS;
	}

	return HASH_INPUT_DATA_TYPE::_BINARY;
}


template<typename T, HASH_INPUT_DATA_TYPE hash_input_data_type = FE::evaluate_hash_input_data_type<T>()>
struct hash;

struct hash_base
{
	static inline FE::random_integer<var::uint64> s_random_integer{0, FE::max_value<var::uint64>};
protected:
	static uint64 s_seed;
};


template<typename T>
struct hash<T, HASH_INPUT_DATA_TYPE::_ADDRESS> final : public hash_base
{
	using base = hash_base;
	using output_type = var::uintptr_t;
	using input_type = T;

	_FORCE_INLINE_ _NODISCARD_ output_type operator()(input_type value_p) const noexcept
	{
		return CityHash64WithSeed((const char*)value_p, sizeof(input_type), s_seed);
	}
};

template<typename T>
struct hash<T, HASH_INPUT_DATA_TYPE::_C_STRING> final : public hash_base
{
	using base = hash_base;
	using output_type = var::uint64;
	using input_type = T;
	
	_FORCE_INLINE_ _NODISCARD_ output_type operator()(input_type value_p) const noexcept
	{
		return CityHash64((const char*)value_p, sizeof(typename std::remove_pointer<T>::type) * algorithm::string::length(value_p));
	}
};

template<typename T>
struct hash<T, HASH_INPUT_DATA_TYPE::_STRING_CLASS> final : public hash_base
{
	using base = hash_base;
	using output_type = var::uint64;
	using input_type = T;

	_FORCE_INLINE_ _NODISCARD_ output_type operator()(const input_type& value_p) const noexcept
	{

		return CityHash64((const char*)value_p.data(), sizeof(typename std::remove_pointer<T>::type) * value_p.length());
	}
};

template<typename T>
struct hash<T, HASH_INPUT_DATA_TYPE::_BINARY> final : public hash_base
{
	using base = hash_base;
	using output_type = var::uint64;
	using input_type = T;

	_FORCE_INLINE_ _NODISCARD_ output_type operator()(const input_type& value_p) const noexcept
	{
		return CityHash64WithSeed((const char*)(&value_p), sizeof(input_type), s_seed);
	}
};


END_NAMESPACE
#pragma warning(pop)
#endif
