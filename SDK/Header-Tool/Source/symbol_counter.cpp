/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "symbol_counter.hpp"

#include "header_tool.hpp"
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
			}
		}
		return l_count;
	}

	symbol_count try_count_the_current_scope_level_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		//FE_ASSERT(begin_p->_vocabulary == Vocabulary::_LeftCurlyBracket);
		while (begin_p != end_p) // wind until {
		{
			if (begin_p->_vocabulary == Vocabulary::_LeftCurlyBracket)
			{
				break;
			}
		}

		symbol_count l_count{ 0, 0, 0 };
		file_buffer_t l_scope_stack{ framework::get_framework().get_memory_resource() };

		do
		{
			switch (begin_p->_vocabulary)
			{
			case Vocabulary::_Namespace:
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


			case Vocabulary::_LeftCurlyBracket:
				l_scope_stack.push_back('{');
				break;

			case Vocabulary::_RightCurlyBracket:
				l_scope_stack.pop_back();
				break;
			}
		} 
		while (l_scope_stack.size() > 0);

		return l_count;
	}

	std::optional<FE::uint32> verify_if_token_is_a_paren_or_bracket(Vocabulary paren_p) noexcept
	{
		switch (paren_p)
		{
		case Vocabulary::_LeftParen:
			_FE_FALLTHROUGH_;
		case Vocabulary::_RightParen:
			return 0;

		case Vocabulary::_LeftCurlyBracket:
			_FE_FALLTHROUGH_;
		case Vocabulary::_RightCurlyBracket:
			return 1;

		case Vocabulary::_LeftBracket:
			_FE_FALLTHROUGH_;
		case Vocabulary::_RightBracket:
			return 2;

			//case Vocabulary::_BeginTemplateArgs:
			//	_FE_FALLTHROUGH_;
			//case Vocabulary::_EndTemplateArgs:
			//	return 3;

		default:
			return std::nullopt;
		}
	}

	std::optional<FE::ASCII*> validate_parentheses(const std::pmr::list<token>& token_list_p) noexcept
	{
		static const std::array< std::pair<Vocabulary, Vocabulary>, 3 > l_lookup =
		{
			std::pair<Vocabulary, Vocabulary>(Vocabulary::_LeftParen, Vocabulary::_RightParen),
			std::pair<Vocabulary, Vocabulary>(Vocabulary::_LeftCurlyBracket, Vocabulary::_RightCurlyBracket),
			std::pair<Vocabulary, Vocabulary>(Vocabulary::_LeftBracket, Vocabulary::_RightBracket)/*,
			std::pair<Vocabulary, Vocabulary>(Vocabulary::_BeginTemplateArgs, Vocabulary::_EndTemplateArgs)*/
		};
		thread_local static std::pmr::vector<Vocabulary> tl_s_stack(framework::get_framework().get_memory_resource());
		tl_s_stack.reserve(std::distance(token_list_p.begin(), token_list_p.end()) / 2);


		for (auto it = token_list_p.begin(), end = token_list_p.end(); it != end; ++it)
		{
			switch (it->_vocabulary)
			{
			case Vocabulary::_LeftParen:
			case Vocabulary::_LeftCurlyBracket:
			case Vocabulary::_LeftBracket:
				//case Vocabulary::_BeginTemplateArgs:
				tl_s_stack.push_back(it->_vocabulary);
				break;

			default:
				if (tl_s_stack.empty() == true)
				{
					break;
				}

				std::optional<FE::uint32> l_index = FHT::symbol_counter::verify_if_token_is_a_paren_or_bracket(tl_s_stack.back());
				if (l_index == std::nullopt) // The token is not a paren nor a bracket.
				{
					break;
				}

				/*
				* index 0: first == LeftParen, second == RightParen
				* index 1: first == LeftCurlyBracket, second == RightCurlyBracket
				* index 2: first == LeftBracket, second == RightBracket
				* index 3: first == BeginTemplateArgs, second == EndTemplateArgs
				*/
				if (l_lookup[(*l_index)].second == it->_vocabulary) // is the paren or braket closed and complete?
				{
					tl_s_stack.pop_back(); // pop (, [, or {.
					break;
				}
			}
		}

		if (tl_s_stack.size() != 0)
		{
			return "Frogman Engine Header Tool C++ syntax error C1075: the parentheses/brackets in the current header file are not closed or properly organized.";
		}
		tl_s_stack.clear();

		return std::nullopt; // no syntax errors relevant to parentheses/brackets found.
	}
}
