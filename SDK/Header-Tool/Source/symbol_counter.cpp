/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#include "symbol_counter.hpp"

#include "header_tool.hpp"
#include "error_code.hpp"
#include "parser.hpp"
#include "token.hpp"
#include "vocabulary.hpp"

#include <array>
#include <cassert>
#include <list>
#include <string>
#include <utility>
#include <vector>




namespace FHT::symbol_counter
{
	symbol_count try_count_all_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		symbol_count l_count{ 0, 0, 0 };

		while (begin_p != end_p)
		{
			switch (begin_p->_vocabulary)
			{
			case Vocabulary::_Namespace:
				_FE_FALLTHROUGH_;
			case Vocabulary::_BeginNamespace:
				++l_count._namespaces;
				break;

			case Vocabulary::_Class:
				++l_count._classes;
				break;

			case Vocabulary::_Struct:
				++l_count._structs;
				break;

			case Vocabulary::_EnumStruct:
				++l_count._enum_structs;
				break;

			case Vocabulary::_FrogmanEngineSystemMacro:
				++l_count._systems;
				break;

			default:
				THROW_CPP_SYNTAX_ERROR(begin_p == end_p, "FHT C++ Syntax Error C1075: the curly braces in the current header file are not closed or properly organized.");				THROW_CPP_SYNTAX_ERROR(begin_p == end_p, "Frogman Engine Header Tool C++ Syntax Error C1075: the curly braces in the current header file are not closed or properly organized.");
				break;
			}
			++begin_p;
		}
		return l_count;
	}

	symbol_count try_count_the_current_scope_level_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		symbol_count l_count{ 0, 0, 0 };
		file_buffer_t l_scope_stack{ framework::get_framework().get_memory_resource() };

		do
		{
			switch (begin_p->_vocabulary)
			{
			case Vocabulary::_Namespace:
				++l_count._namespaces;
				break;

			case Vocabulary::_LeftCurlyBracket:
				l_scope_stack.push_back('{');
				break;

			case Vocabulary::_BeginNamespace:
				l_scope_stack.push_back('{');
				++l_count._namespaces;
				break;


			case Vocabulary::_EndNamespace:
				_FE_FALLTHROUGH_;
			case Vocabulary::_RightCurlyBracket:
				l_scope_stack.pop_back();
				break;


			case Vocabulary::_Class:
				++l_count._classes;
				break;

			case Vocabulary::_Struct:
				++l_count._structs;
				break;

			case Vocabulary::_EnumStruct:
				++l_count._enum_structs;
				break;

			case Vocabulary::_FrogmanEngineSystemMacro:
				++l_count._systems;
				break;


			default:
				break;
			}
			++begin_p;
			THROW_CPP_SYNTAX_ERROR(begin_p == end_p, "FHT C++ syntax error C1075: the curly braces in the current header file are not closed or properly organized.");
		} 
		while (l_scope_stack.size() > 0);

		return l_count;
	}
}