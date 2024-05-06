#ifndef _FE_CORE_PAIR_HXX_
#define _FE_CORE_PAIR_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/macros/attributes.h>
#include <utility>




BEGIN_NAMESPACE(FE)


template<typename First, typename Second>
struct pair
{
	using first_type = First;
	using second_type = Second;

	First _first;
	Second _second;

	_FORCE_INLINE_ pair() noexcept = default;
	_FORCE_INLINE_ pair(const First& first_p, const Second& second_p) noexcept : _first(first_p), _second(second_p) {};
	_FORCE_INLINE_ pair(First&& first_p, Second&& second_p) noexcept : _first(std::forward<First&&>(first_p)), _second(std::forward<Second&&>(second_p)) {};
	_FORCE_INLINE_ ~pair() noexcept = default;

	_FORCE_INLINE_ pair(const pair&) noexcept = default;
	_FORCE_INLINE_ pair(pair&&) noexcept = default;

	_FORCE_INLINE_ pair& operator=(const pair& other_p) noexcept
	{
		this->_first = other_p._first;
		this->_second = other_p._second;

		return *this;
	}

	_FORCE_INLINE_ pair& operator=(pair&& rvalue_p) noexcept
	{
		this->_first = std::move(rvalue_p._first);
		this->_second = std::move(rvalue_p._second);

		return *this;
	}
};


#ifdef _HAS_CXX20_
template<typename First, typename Second>
class compressed_pair final
{
public:
	using first_type = First;
	using second_type = Second;

private:
	_NO_UNIQUE_ADDRESS_ First m_first;
	_NO_UNIQUE_ADDRESS_ Second m_second;

public:
	First& get_first() noexcept { return this->m_first; }
	Second& get_second() noexcept { return this->m_second; }

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
	First& get_first() noexcept { return *this; }
	Second& get_second() noexcept { return this->m_second; }
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
	First& get_first() noexcept { return this->m_first; }
	Second& get_second() noexcept { return *this; }
};
#endif


END_NAMESPACE
#endif