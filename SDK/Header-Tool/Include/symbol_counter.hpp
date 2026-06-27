#ifndef _FE_HEADER_TOOL_SYMBOL_COUNTER_HPP_
#define _FE_HEADER_TOOL_SYMBOL_COUNTER_HPP_
/*
Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

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

#include "token.hpp"
#include "vocabulary.hpp"

#include <list>
#include <optional>




namespace FHT::symbol_counter
{
	struct symbol_count
	{
		var::uint16 _namespaces;
		var::uint16 _classes;
		var::uint16 _structs;
		var::uint16 _enum_structs;
		var::uint16 _systems;
	};

	symbol_count try_count_all_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p);
	symbol_count try_count_the_current_scope_level_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p);
}
#endif
