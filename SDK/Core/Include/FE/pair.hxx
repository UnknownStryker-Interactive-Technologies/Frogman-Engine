#ifndef _FE_CORE_PAIR_HXX_
#define _FE_CORE_PAIR_HXX_
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
#include <FE/definitions.h>
#include <utility>




BEGIN_NAMESPACE(FE)


template<typename First, typename Second>
class pair
{
public:
	using first_type = First;
	using second_type = Second;

	First _first;
	Second _second;

	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ pair() noexcept = default;
	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ pair(const First& first_p, const Second& second_p) noexcept : _first(first_p), _second(second_p) {};
	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ pair(First&& first_p, Second&& second_p) noexcept : _first(std::forward<First&&>(first_p)), _second(std::forward<Second&&>(second_p)) {};
	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ ~pair() noexcept = default;

	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ pair(const pair&) noexcept = default;
	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ pair(pair&&) noexcept = default;

	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ pair& operator=(const pair& other_p) noexcept
	{
		this->_first = other_p._first;
		this->_second = other_p._second;

		return *this;
	}

	_FE_FORCE_INLINE_ _FE_CONSTEXPR17_ pair& operator=(pair&& rvalue_p) noexcept
	{
		this->_first = std::move(rvalue_p._first);
		this->_second = std::move(rvalue_p._second);

		return *this;
	}
};


#ifdef _FE_HAS_CXX20_
template<typename First, typename Second>
class compressed_pair final
{
public:
	using first_type = First;
	using second_type = Second;

private:
	_FE_NO_UNIQUE_ADDRESS_ First m_first;
	_FE_NO_UNIQUE_ADDRESS_ Second m_second;

public:
	_FE_FORCE_INLINE_ First& get_first() noexcept { return this->m_first; }
	_FE_FORCE_INLINE_ Second& get_second() noexcept { return this->m_second; }

};
#else
template<typename First, typename Second, boolean IsFirstEmpty = std::is_empty<First>::value, boolean IsSecondEmpty = std::is_empty<Second>::value>
class compressed_pair {};

template<typename First, typename Second>
class compressed_pair<First, Second, true, false> : private First
{
public:
	using first_type = First;
	using second_type = Second;

private:
	Second m_second;

public:
	_FE_FORCE_INLINE_ First& get_first() noexcept { return *this; }
	_FE_FORCE_INLINE_ Second& get_second() noexcept { return this->m_second; }
};

template<typename First, typename Second>
class compressed_pair<First, Second, false, true> : private Second
{
public:
	using first_type = First;
	using second_type = Second;

private:
	First m_first;

public:
	_FE_FORCE_INLINE_ First& get_first() noexcept { return this->m_first; }
	_FE_FORCE_INLINE_ Second& get_second() noexcept { return *this; }
};
#endif


END_NAMESPACE
#endif