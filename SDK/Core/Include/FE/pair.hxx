#ifndef _FE_CORE_PAIR_HXX_
#define _FE_CORE_PAIR_HXX_
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
#include <FE/prerequisites.hxx>
#include <FE/definitions.hxx>
#include <utility>




BEGIN_NAMESPACE(FE)


template<typename First, typename Second>
class pair
{
public:
	using first_type = First;
	using second_type = Second;

	_FE_NO_UNIQUE_ADDRESS_ First _first;
	_FE_NO_UNIQUE_ADDRESS_ Second _second;

	pair() noexcept = default;
	pair(const First& first_p, const Second& second_p) noexcept : _first(first_p), _second(second_p) {};
	pair(First&& first_p, Second&& second_p) noexcept : _first(std::forward<First&&>(first_p)), _second(std::forward<Second&&>(second_p)) {};
	~pair() noexcept = default;

	pair(const pair&) noexcept = default;
	pair(pair&&) noexcept = default;

	pair& operator=(const pair& other_p) noexcept
	{
		_first = other_p._first;
		_second = other_p._second;

		return *this;
	}

	pair& operator=(pair&& rvalue_p) noexcept
	{
		_first = std::move(rvalue_p._first);
		_second = std::move(rvalue_p._second);

		return *this;
	}
};


END_NAMESPACE
#endif