#ifndef _FE_CORE_HASH_HXX_
#define _FE_CORE_HASH_HXX_
#pragma warning(push)
#pragma warning(disable: 4702)
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

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
#include <FE/private/strlen.hxx>
#include <FE/random.hxx>
#include <FE/type_traits.hxx>

// city hash
#include <city.h>

#include <FE/bitmask.hxx>




BEGIN_NAMESPACE(FE)


enum struct HashInputDataType : uint8
{
	_Address = 0,
	_CString = 1,
	_StringClass = 2,	
	_Binary = 3,
	_Bitmask = 4
};

template<typename T>
_FE_FORCE_INLINE_ constexpr HashInputDataType evaluate_hash_input_data_type()
{
	if constexpr (FE::is_constant_string_v<T>)
	{
		return HashInputDataType::_CString;
	}
	else if constexpr (std::is_pointer_v<T>)
	{
		return HashInputDataType::_Address;
	}
	else if constexpr (FE::is_string_class_v<T>)
	{
		return HashInputDataType::_StringClass;
	}
	else if constexpr (std::same_as<T, FE::bitmask>)
	{
		return HashInputDataType::_Bitmask;
	}

	return HashInputDataType::_Binary;
}

enum struct HasherType : uint8
{
	_CityHash = 1
};

class hash_base
{
	static FE::random_integer<var::uint64> s_random_integer;
protected:
	static uint64 s_seed;
};

/*
The FE::hash class template provides a hashing mechanism for objects of type T
allowing the use of different hashing algorithms (such as Robin Hood Hashing or CityHash) based on the specified HasherType
and is specialized for handling address-based hashing with the HashInputDataType::_Address type.
*/
template<typename T, HasherType HasherType = HasherType::_CityHash, HashInputDataType HashInputDataType = FE::evaluate_hash_input_data_type<T>()>
class hash;

template<typename T, HasherType HasherType>
class hash<T, HasherType, HashInputDataType::_Address> : public hash_base
{
public:
	using base = hash_base;
	static constexpr HashInputDataType hash_input_data_type = HashInputDataType::_Address;

	_FE_NODISCARD_ _FE_FORCE_INLINE_ var::uintptr operator()(T value_p) const noexcept
	{
		if constexpr (HasherType == HasherType::_CityHash)
		{
			return CityHash64(reinterpret_cast<const char*>(value_p), sizeof(T));
		}
	}
};

template<typename T, HasherType HasherType>
class hash<T, HasherType, HashInputDataType::_CString> : public hash_base
{
public:
	using base = hash_base;
	static constexpr HashInputDataType hash_input_data_type = HashInputDataType::_CString;

	_FE_NODISCARD_ _FE_FORCE_INLINE_ var::uint64 operator()(T value_p) const noexcept
	{
		if constexpr (HasherType == HasherType::_CityHash)
		{
			return CityHash64(reinterpret_cast<const char*>(value_p), sizeof(typename std::remove_pointer<T>::type) * internal::strlen(value_p));
		}
	}
};

template<typename T, HasherType HasherType>
class hash<T, HasherType, HashInputDataType::_StringClass> : public hash_base
{
public:
	using base = hash_base;
	static constexpr HashInputDataType hash_input_data_type = HashInputDataType::_StringClass;

	_FE_NODISCARD_ _FE_FORCE_INLINE_ var::uintptr operator()(const T& value_p) const noexcept
	{
		if constexpr (HasherType == HasherType::_CityHash)
		{
			return CityHash64(reinterpret_cast<const char*>(value_p.data()), sizeof(typename std::remove_pointer<T>::type) * value_p.length());
		}
	}
};

template<typename T, HasherType HasherType>
class hash<T, HasherType, HashInputDataType::_Bitmask> : public hash_base
{
public:
	using base = hash_base;
	static constexpr HashInputDataType hash_input_data_type = HashInputDataType::_Bitmask;

	_FE_NODISCARD_ _FE_FORCE_INLINE_ var::uintptr operator()(const T& value_p) const noexcept
	{
		if constexpr (HasherType == HasherType::_CityHash)
		{
			return CityHash64(reinterpret_cast<const char*>(value_p.data()), value_p.capacity_in_bytes());
		}
	}
};

template<typename T, HasherType HasherType>
class hash<T, HasherType, HashInputDataType::_Binary> : public hash_base
{
public:
	using base = hash_base;
	static constexpr HashInputDataType hash_input_data_type = HashInputDataType::_Binary;

	_FE_NODISCARD_ _FE_FORCE_INLINE_ var::uint64 operator()(const T& value_p) const noexcept
	{
		static_assert(std::is_trivially_copyable_v<T>, "Static assertion failed: only trivially copyable types can be hashed using binary hashing.");

		if constexpr (HasherType == HasherType::_CityHash)
		{
			return CityHash64(reinterpret_cast<const char*>(&value_p), sizeof(T));
		}
	}
};

END_NAMESPACE
#pragma warning(pop)
#endif
