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




header_file_root header_tool_engine::__build_reflection_tree(const directory_t& file_path_p, const std::pmr::vector<token>& token_list_p) noexcept
{
	{
		static std::mutex l_s_log_lock;
		symbol_count l_total_nums = __count_all_symbols(token_list_p.begin(), token_list_p.cend());
		std::lock_guard<std::mutex> l_guard(l_s_log_lock);
		std::cout << "Frogman Engine Header Tool: In the file located at '" << file_path_p.c_str() << "'\n";
		std::cout << "Frogman Engine Header Tool: the total number of namespaces is " << l_total_nums._namespaces << '\n';
		std::cout << "Frogman Engine Header Tool: the total number of classes is " << l_total_nums._classes << '\n';
		std::cout << "Frogman Engine Header Tool: the total number of structs is " << l_total_nums._structs << "\n\n";
	}

	std::pmr::polymorphic_allocator<namespace_node>{get_memory_resource()}
	header_file_root l_root{ file_path_p };

	for (auto iterator = token_list_p.begin(); iterator != token_list_p.end(); ++iterator)
	{
		switch (iterator->_vocabulary)
		{
		case Vocabulary::_Namespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_BeginNamespace:
			l_root._namespaces.push_back( __build_namespace_node_recursive(iterator, token_list_p.end()) );
			break;

		case Vocabulary::_Template:
			__handle_template_args(iterator);
			break;

		case Vocabulary::_Class:
			l_root._classes.push_back( __build_class_node_mutually_recursive(iterator, token_list_p.end()) );
			break;

		case Vocabulary::_Struct:
			//l_root._structs.push_back( __build_struct_node_mutually_recursive(iterator, token_list_p.end()) );
			break;

		case Vocabulary::_Enum:
			__handle_enum(iterator);
			break;

		default:
			break;
		}
	}

	return l_root;
}

namespace_node header_tool_engine::__build_namespace_node_recursive(typename std::pmr::vector<token>::const_iterator& out_token_iterator_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept
{
	namespace_node l_node;
	switch (out_token_iterator_p->_vocabulary)
	{
	case Vocabulary::_Namespace:
		++out_token_iterator_p;
		l_node._namespace_name = out_token_iterator_p->_code;
		++out_token_iterator_p;
		REPORT_CPP_SYNTEX_ERROR(out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket, "Frogman Engine Header Tool Error: the C++ code syntex is incorrect; '{' is missing from 'namespace Identifier {'. ");
		++out_token_iterator_p;
		break;

	case Vocabulary::_BeginNamespace:
		++out_token_iterator_p;
		REPORT_CPP_SYNTEX_ERROR(out_token_iterator_p->_vocabulary != Vocabulary::_LeftParen, "Frogman Engine Header Tool Error: the C++ code syntex is incorrect; '(' is missing from 'BEGIN_NAMESPACE(Identifier)'. ");
		++out_token_iterator_p;
		l_node._namespace_name = out_token_iterator_p->_code;
		++out_token_iterator_p;
		REPORT_CPP_SYNTEX_ERROR(out_token_iterator_p->_vocabulary != Vocabulary::_RightParen, "Frogman Engine Header Tool Error: the C++ code syntex is incorrect; ')' is missing from 'BEGIN_NAMESPACE(Identifier)'. ");
		++out_token_iterator_p;
		break;

	_FE_NODEFAULT_;
	}

	// Allocate the necessary children nodes.
	{
		symbol_count l_nums = __count_the_current_scope_level_symbols(out_token_iterator_p, end_p);
		l_node._nested_namespaces = (0 == l_nums._namespaces) ? nullptr : std::make_unique<namespace_node[]>(l_nums._namespaces);
		l_node._classes = (0 == l_nums._classes) ? nullptr : std::make_unique<class_node[]>(l_nums._classes);
		l_node._structs = (0 == l_nums._structs) ? nullptr : std::make_unique<struct_node[]>(l_nums._structs);
	}

	var::uint32 l_namespace_idx = 0;
	var::uint32 l_class_idx = 0;
	//var::uint32 l_struct_idx = 0;
	while (out_token_iterator_p != end_p)
	{
		switch (out_token_iterator_p->_vocabulary)
		{
		case Vocabulary::_Namespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_BeginNamespace:
			l_node._nested_namespaces[l_namespace_idx] = __build_namespace_node_recursive(out_token_iterator_p, end_p);
			++l_namespace_idx;
			break;

		case Vocabulary::_Template:
			__handle_template_args(out_token_iterator_p);
			break;

		case Vocabulary::_Class:
			l_node._classes[l_class_idx] = __build_class_node_mutually_recursive(out_token_iterator_p, end_p);
			++l_class_idx;
			break;

		case Vocabulary::_Struct:
			//l_node._structs[l_struct_idx] = __build_struct_node_mutually_recursive(out_token_iterator_p, end_p);
			//++l_struct_idx;
			break;

		case Vocabulary::_Enum:
			__handle_enum(out_token_iterator_p);
			break;

		default:
			return l_node;
		}
		++out_token_iterator_p;
	}
	return l_node;
}

class_node header_tool_engine::__build_class_node_mutually_recursive(typename std::pmr::vector<token>::const_iterator& out_token_iterator_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept
{
	FE_EXIT(out_token_iterator_p->_vocabulary != Vocabulary::_Class, FrogmanEngineHeaderToolError::_FatalError_InvalidToken, "Frogman Engine Header Tool Error: the 'class' keyword is missing from the current token, but the header tool is attempting to build a class node.");
	++out_token_iterator_p; // move to the class name.
	const file_buffer_t& l_class_name = out_token_iterator_p->_code; // get the class name.
	++out_token_iterator_p; // skip the class name.

	class_node l_node;
	// Allocate the necessary children nodes.
	{
		symbol_count l_nums = __count_the_current_scope_level_symbols(out_token_iterator_p, end_p);
		l_node._nested_classes = (0 == l_nums._classes) ? nullptr : std::make_unique<class_node[]>(l_nums._classes);
		l_node._nested_structs = (0 == l_nums._structs) ? nullptr : std::make_unique<struct_node[]>(l_nums._structs);
	}

	// Check if the class has a base class, and it is reflective.
	if ((out_token_iterator_p->_vocabulary == Vocabulary::_Colon) &&
		(std::find_if(out_token_iterator_p, out_token_iterator_p + 5, [&](const token& token_p) { return token_p._vocabulary == Vocabulary::_FrogmanEngineBaseClassReflectionMacro; }) != out_token_iterator_p + 5))
	{
		++out_token_iterator_p; // skip the ':'.
		l_node._base_class_reflection_macro = std::make_unique<frogman_engine_class_has_a_base_macro_node>();

		switch (out_token_iterator_p->_vocabulary) // determine if it needs to skip more.
		{
		case Vocabulary::_Public:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Protected:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Private:
			++out_token_iterator_p; // skip the access modifier.
			_FE_FALLTHROUGH_;

		default: // the base class name found.
			l_node._base_class_reflection_macro->_target_base_class_name = out_token_iterator_p->_code;
			++out_token_iterator_p; // skip the base class name.
			break;
		}
	}

	auto l_class_reflection_macro_search_result = std::find_if(out_token_iterator_p, out_token_iterator_p + 5, [&](const token& token_p) { return token_p._vocabulary == Vocabulary::_FrogmanEngineClassReflectionMacro; });
	if (l_class_reflection_macro_search_result != out_token_iterator_p + 5)
	{
		l_node._class_reflection_macro = std::make_unique<frogman_engine_class_macro_node>();
		l_node._class_reflection_macro->_target_class_name = l_class_name;
		out_token_iterator_p = l_class_reflection_macro_search_result;
		out_token_iterator_p += 3;

		// build n
		l_node._class_reflection_macro->_property_reflection_macros;

		// build n
		l_node._class_reflection_macro->_method_reflection_macros;

		// build n
		l_node._class_reflection_macro->_static_method_reflection_macros;

		FE::clock l_loop_timer;
		l_loop_timer.start_clock();
		while (out_token_iterator_p != end_p)
		{
			switch (out_token_iterator_p->_vocabulary)
			{
			case Vocabulary::_FrogmanEnginePropertyReflectionMacro:

				break;

			case Vocabulary::_FrogmanEngineMethodReflectionMacro:

				break;

			case Vocabulary::_FrogmanEngineStaticMethodReflectionMacro:

				break;

			case Vocabulary::_RightCurlyBracket:
				{
					auto l_next = out_token_iterator_p + 1;
					if ((l_next != end_p) && 
						(l_next->_vocabulary == Vocabulary::_Semicolon))
					{
						++out_token_iterator_p;
						return l_node;
					}
				}
				break;
			}
			++out_token_iterator_p;
			REPORT_CPP_SYNTEX_ERROR(l_loop_timer.get_delta_milliseconds() >= 1000.0, "Frogman Engine Header Tool Parser Timed Out: the C++ class code syntex is incorrect; check if the class code is correctly formatted.");
		}
	}

	FE::clock l_loop_timer;
	l_loop_timer.start_clock();
	while (out_token_iterator_p != end_p)
	{
		if (out_token_iterator_p->_vocabulary == Vocabulary::_RightCurlyBracket)
		{
			auto l_next = out_token_iterator_p + 1;
			if ((l_next != end_p) && 
				(l_next->_vocabulary == Vocabulary::_Semicolon))
			{
				++out_token_iterator_p;
				return l_node;
			}
		}
		++out_token_iterator_p;
		REPORT_CPP_SYNTEX_ERROR(l_loop_timer.get_delta_milliseconds() >= 1000.0, "Frogman Engine Header Tool Parser Timed Out: the C++ class code syntex is incorrect; check if the class code is correctly formatted.");
	}

	return l_node;
}

void header_tool_engine::__handle_template_args(typename std::pmr::vector<token>::const_iterator& iterator_p) noexcept
{
	++iterator_p;
	REPORT_CPP_SYNTEX_ERROR(iterator_p->_vocabulary != Vocabulary::_BeginTemplateArgs, "Frogman Engine Header Tool Error: the C++ code syntex is incorrect; '<' is missing from 'template<...>'. ");
	++iterator_p;
	FE::clock l_loop_timer;
	l_loop_timer.start_clock();
	while (iterator_p->_vocabulary != Vocabulary::_EndTemplateArgs)
	{
		++iterator_p;
		l_loop_timer.end_clock();
		REPORT_CPP_SYNTEX_ERROR(l_loop_timer.get_delta_milliseconds() >= 1000.0, "Frogman Engine Header Tool Parser Timed Out: the C++ code syntex is incorrect; '>' is missing from 'template<...>'. ");
	}
}

void header_tool_engine::__handle_enum(typename std::pmr::vector<token>::const_iterator& iterator_p) noexcept
{
	iterator_p += 2;
	FE::clock l_loop_timer;
	l_loop_timer.start_clock();
	while (iterator_p->_vocabulary != Vocabulary::_RightCurlyBracket)
	{
		++iterator_p;
		l_loop_timer.end_clock();
		REPORT_CPP_SYNTEX_ERROR(l_loop_timer.get_delta_milliseconds() >= 1000.0, "Frogman Engine Header Tool Parser Timed Out: the C++ code syntex is incorrect; '}' is missing from 'enum struct Identifier {...};'. ");
	}
	++iterator_p;
	REPORT_CPP_SYNTEX_ERROR(iterator_p->_vocabulary != Vocabulary::_Semicolon, "Frogman Engine Header Tool Error: the C++ code syntex is incorrect; ';' is missing from 'enum struct Identifier {...};'. ");
}