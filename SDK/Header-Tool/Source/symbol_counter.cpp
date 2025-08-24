/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#include "header_tool_engine.hpp"

#include <cassert>
#include <array>
#include <string>
#include <utility>
#include <vector>




_FE_NODISCARD_ header_tool_engine::symbol_count header_tool_engine::__try_count_all_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p) const
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
			if (std::prev(begin_p, 1)->_vocabulary == Vocabulary::_EndTemplateArgs)
			{
				__skip_code_block(begin_p, end_p);
				break;
			}
			++l_count._classes;
			break;

		case Vocabulary::_Struct:
			if (std::prev(begin_p, 1)->_vocabulary == Vocabulary::_EndTemplateArgs)
			{
				__skip_code_block(begin_p, end_p);
				break;
			}
			++l_count._structs;
			break;

		case Vocabulary::_Enum:
			++l_count._enums;
			break;

		case Vocabulary::_Template:
			__try_skip_template_args(begin_p);
			break;

		case Vocabulary::_LineEnd:
			goto Return;

		default:
			break;
		}
		++begin_p;
	}
Return:
	return l_count;
}

_FE_NODISCARD_ header_tool_engine::symbol_count header_tool_engine::__try_count_the_current_scope_level_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p)
{
	std::pmr::list<Vocabulary> l_scope_stack(get_memory_resource());
	symbol_count l_count{ 0, 0, 0 };

	while (begin_p != end_p)
	{
		switch (begin_p->_vocabulary)
		{
		case Vocabulary::_Namespace:
			l_scope_stack.push_back(begin_p->_vocabulary);
			break;

		case Vocabulary::_Class:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Struct:
			if (std::prev(begin_p, 1)->_vocabulary == Vocabulary::_EndTemplateArgs)
			{
				__skip_code_block(begin_p, end_p);
				break;
			}
			l_scope_stack.push_back(begin_p->_vocabulary);
			break;

		case Vocabulary::_Enum:
			++l_count._enums;
			break;

		case Vocabulary::_Template:
			__try_skip_template_args(begin_p);
			break;


		case Vocabulary::_BeginNamespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_LeftCurlyBracket:
			l_scope_stack.push_back(begin_p->_vocabulary);
			break;

		case Vocabulary::_EndNamespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_RightCurlyBracket:
			if (l_scope_stack.empty() == true) { goto Return; }
			l_scope_stack.pop_back();
			if (l_scope_stack.empty() == true) { break; }

			switch (l_scope_stack.back())
			{
			case Vocabulary::_Namespace:
				l_scope_stack.pop_back();
				if (l_scope_stack.size() == 0)
				{
					++l_count._namespaces;
				}
				break;
			case Vocabulary::_Class:
				l_scope_stack.pop_back();
				if (l_scope_stack.size() == 0)
				{
					++l_count._classes;
				}
				break;
			case Vocabulary::_Struct:
				l_scope_stack.pop_back();
				if (l_scope_stack.size() == 0)
				{
					++l_count._structs;
				}
				break;
			default:
				break;
			}
			break;


		case Vocabulary::_LineEnd:
			goto Return;


		default:
			break;
		}

		++begin_p;
	}

Return:
	return l_count;
}

_FE_NODISCARD_ header_tool_engine::member_symbol_count header_tool_engine::__count_the_current_class_member_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p) noexcept
{
	(void)begin_p;
	(void)end_p;
	return member_symbol_count();
}

_FE_NODISCARD_ std::optional<FE::uint32> header_tool_engine::___verify_if_token_is_a_paren_or_bracket(Vocabulary paren_p) const noexcept
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

_FE_NODISCARD_ std::optional<FE::ASCII*> header_tool_engine::__validate_parentheses(const std::pmr::list<token>& token_list_p) noexcept
{
	static const std::array< std::pair<Vocabulary, Vocabulary>, 3 > l_lookup = 
	{ 
		std::pair<Vocabulary, Vocabulary>(Vocabulary::_LeftParen, Vocabulary::_RightParen), 
		std::pair<Vocabulary, Vocabulary>(Vocabulary::_LeftCurlyBracket, Vocabulary::_RightCurlyBracket), 
		std::pair<Vocabulary, Vocabulary>(Vocabulary::_LeftBracket, Vocabulary::_RightBracket)/*,
		std::pair<Vocabulary, Vocabulary>(Vocabulary::_BeginTemplateArgs, Vocabulary::_EndTemplateArgs)*/
	};
	thread_local static std::pmr::vector<Vocabulary> tl_s_stack(get_memory_resource());
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

			std::optional<FE::uint32> l_index = ___verify_if_token_is_a_paren_or_bracket(tl_s_stack.back());
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
			if (l_lookup[ (*l_index) ].second == it->_vocabulary) // is the paren or braket closed and complete?
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




	for (auto it = token_list_p.rbegin(), end = token_list_p.rend(); it != end; ++it)
	{
		switch (it->_vocabulary)
		{
		case Vocabulary::_RightParen:
		case Vocabulary::_RightCurlyBracket:
		case Vocabulary::_RightBracket:
		//case Vocabulary::_EndTemplateArgs:
			tl_s_stack.push_back(it->_vocabulary);
			break;

		default:
			if (tl_s_stack.empty() == true)
			{
				break;
			}

			std::optional<FE::uint32> l_index = ___verify_if_token_is_a_paren_or_bracket(tl_s_stack.back());
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
			if (l_lookup[(*l_index)].first == it->_vocabulary) // is the paren or braket closed and complete?
			{
				tl_s_stack.pop_back(); // pop ), ], or }.
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