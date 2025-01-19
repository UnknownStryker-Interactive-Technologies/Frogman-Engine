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




_FE_NODISCARD_ header_tool_engine::symbol_count header_tool_engine::__count_all_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p)
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
			std::advance(begin_p, 2);
			continue;

		case Vocabulary::_Template:
			__handle_template(begin_p);
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

_FE_NODISCARD_ header_tool_engine::symbol_count header_tool_engine::__count_the_current_scope_level_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p)
{
	std::pmr::list<Vocabulary> l_scope_stack(this->get_memory_resource());
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
			std::advance(begin_p, 2);
			continue;

		case Vocabulary::_Template:
			__handle_template(begin_p);
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