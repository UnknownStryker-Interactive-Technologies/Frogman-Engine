#ifndef _FE_CORE_HASH_HXX_
#define _FE_CORE_HASH_HXX_
#pragma warning(push)
#pragma warning(disable: 4702)
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/private/strlen.hxx>
#include <FE/random.hxx>
#include <FE/type_traits.hxx>

// city hash
#include <city.h>

// robin hood hash
#include <robin_hood.h>




BEGIN_NAMESPACE(FE)


enum struct HASH_INPUT_DATA_TYPE : uint8
{
	_ADDRESS = 0,
	_C_STRING = 1,
	_STRING_CLASS = 2,	
	_BINARY = 3
};

template<typename T>
_FE_FORCE_INLINE_ constexpr HASH_INPUT_DATA_TYPE evaluate_hash_input_data_type()
{
	if constexpr (FE::is_c_style_constant_string<T>::value == true)
	{
		return HASH_INPUT_DATA_TYPE::_C_STRING;
	}
	else if constexpr (std::is_pointer<T>::value == true)
	{
		return HASH_INPUT_DATA_TYPE::_ADDRESS;
	}
	else if constexpr (FE::is_string_class<T>::value == true)
	{
		return HASH_INPUT_DATA_TYPE::_STRING_CLASS;
	}

	return HASH_INPUT_DATA_TYPE::_BINARY;
}

enum struct HASHER_TYPE : uint8
{
	_ROBIN_HOOD_HASH = 0,
	_CITY_HASH = 1
};

class hash_base
{
	static FE::random_integer<var::uint64> s_random_integer;
protected:
	static uint64 s_seed;
};

template<typename T, HASHER_TYPE HasherType = HASHER_TYPE::_ROBIN_HOOD_HASH, HASH_INPUT_DATA_TYPE HashInputDataType = FE::evaluate_hash_input_data_type<T>()>
class hash;

template<typename T, HASHER_TYPE HasherType>
class hash<T, HasherType, HASH_INPUT_DATA_TYPE::_ADDRESS> : public hash_base
{
public:
	using base = hash_base;
	static constexpr HASH_INPUT_DATA_TYPE hash_input_data_type = HASH_INPUT_DATA_TYPE::_ADDRESS;

	_FE_NODISCARD_ _FE_FORCE_INLINE_ var::uintptr operator()(T value_p) const noexcept
	{
		if constexpr (HasherType == HASHER_TYPE::_ROBIN_HOOD_HASH)
		{
			return robin_hood::hash_int(reinterpret_cast<var::uintptr>(value_p));
		}
		else if constexpr (HasherType == HASHER_TYPE::_CITY_HASH)
		{
			return CityHash64(reinterpret_cast<const char*>(value_p), sizeof(T));
		}
	}
};

template<typename T, HASHER_TYPE HasherType>
class hash<T, HasherType, HASH_INPUT_DATA_TYPE::_C_STRING> : public hash_base
{
public:
	using base = hash_base;
	static constexpr HASH_INPUT_DATA_TYPE hash_input_data_type = HASH_INPUT_DATA_TYPE::_C_STRING;

	_FE_NODISCARD_ _FE_FORCE_INLINE_ var::uint64 operator()(T value_p) const noexcept
	{
		if constexpr (HasherType == HASHER_TYPE::_ROBIN_HOOD_HASH)
		{
			return robin_hood::hash_bytes(value_p, sizeof(typename std::remove_pointer<T>::type) * internal::strlen(value_p));
		}
		else if constexpr (HasherType == HASHER_TYPE::_CITY_HASH)
		{
			return CityHash64(reinterpret_cast<const char*>(value_p), sizeof(typename std::remove_pointer<T>::type) * internal::strlen(value_p));
		}
	}
};

template<typename T, HASHER_TYPE HasherType>
class hash<T, HasherType, HASH_INPUT_DATA_TYPE::_STRING_CLASS> : public hash_base
{
public:
	using base = hash_base;
	static constexpr HASH_INPUT_DATA_TYPE hash_input_data_type = HASH_INPUT_DATA_TYPE::_STRING_CLASS;

	_FE_NODISCARD_ _FE_FORCE_INLINE_ var::uintptr operator()(const T& value_p) const noexcept
	{
		if constexpr (HasherType == HASHER_TYPE::_ROBIN_HOOD_HASH)
		{
			return robin_hood::hash_bytes(value_p.data(), value_p.length());
		}
		else if constexpr (HasherType == HASHER_TYPE::_CITY_HASH)
		{
			return CityHash64(reinterpret_cast<const char*>(value_p.data()), sizeof(typename std::remove_pointer<T>::type) * value_p.length());
		}
	}
};

template<typename T, HASHER_TYPE HasherType>
class hash<T, HasherType, HASH_INPUT_DATA_TYPE::_BINARY> : public hash_base
{
public:
	using base = hash_base;
	static constexpr HASH_INPUT_DATA_TYPE hash_input_data_type = HASH_INPUT_DATA_TYPE::_BINARY;

	_FE_NODISCARD_ _FE_FORCE_INLINE_ var::uint64 operator()(const T& value_p) const noexcept
	{
		if constexpr (HasherType == HASHER_TYPE::_ROBIN_HOOD_HASH)
		{
			return robin_hood::hash_bytes(&value_p, sizeof(T));
		}
		else if constexpr (HasherType == HASHER_TYPE::_CITY_HASH)
		{
			return CityHash64(reinterpret_cast<const char*>(&value_p), sizeof(T));
		}
	}
};

END_NAMESPACE
#pragma warning(pop)
#endif
