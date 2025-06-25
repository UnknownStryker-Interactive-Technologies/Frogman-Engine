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




void header_tool_engine::__skip_code_block(typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p) const noexcept
{
	for (; out_token_iterator_p != end_p; ++out_token_iterator_p)
	{
		if (out_token_iterator_p->_vocabulary == Vocabulary::_RightCurlyBracket)
		{
			auto l_next = std::next(out_token_iterator_p, 1);
			if ((l_next != end_p) &&
				(l_next->_vocabulary == Vocabulary::_Semicolon))
			{
				++out_token_iterator_p;
				return;
			}
		}
	}
}

_FE_NODISCARD_ header_file_root header_tool_engine::__try_build_reflection_tree(const directory_t& file_path_p, const std::pmr::list<token>& token_list_p)
{
	// returns an optional error message object.
	std::optional<FE::ASCII*> l_error_message = __validate_parentheses(token_list_p);
	if (l_error_message != std::nullopt)
	{
		throw FE::pair<FrogmanEngineHeaderToolError, FE::ASCII*>{FrogmanEngineHeaderToolError::_InputError_IncorrectCppSyntax, *l_error_message};
	}

	{
		static std::mutex l_s_log_lock;
		symbol_count l_total_nums = __try_count_all_symbols(token_list_p.begin(), token_list_p.cend());

		std::lock_guard<std::mutex> l_guard(l_s_log_lock);
		std::wcout << L"Frogman Engine Header Tool: In the file located at '" << file_path_p.c_str() << "'\n";
		std::cout << "Frogman Engine Header Tool: the total number of namespaces is " << l_total_nums._namespaces << '\n';
		std::cout << "Frogman Engine Header Tool: the total number of classes is " << l_total_nums._classes << '\n';
		std::cout << "Frogman Engine Header Tool: the total number of structs is " << l_total_nums._structs << "\n";
		std::cout << "Frogman Engine Header Tool: the total number of enum structs is " << l_total_nums._enums << "\n\n";
	}

	header_file_root l_root;
	l_root._path_to_the_header_file = file_path_p;
	l_root._namespaces = std::pmr::vector<namespace_node>(get_memory_resource());
	l_root._classes = std::pmr::vector<class_node>(get_memory_resource());
	l_root._structs = std::pmr::vector<struct_node>(get_memory_resource());
	l_root._enum_structs = std::pmr::vector<enum_struct_node>(get_memory_resource());

	for (auto iterator = token_list_p.begin(); iterator != token_list_p.end(); ++iterator)
	{
		switch (iterator->_vocabulary)
		{
		case Vocabulary::_Namespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_BeginNamespace:
			l_root._namespaces.push_back( __try_build_namespace_node_recursive(u8"::", iterator, token_list_p.end()) );
			break;

		case Vocabulary::_Template:
			__try_skip_template_args(iterator);
			break;

		case Vocabulary::_Class:
			if (std::prev(iterator, 1)->_vocabulary == Vocabulary::_EndTemplateArgs)
			{
				__skip_code_block(iterator, token_list_p.end());
				break;
			}
			l_root._classes.push_back( __try_build_class_node_mutually_recursive(u8"::", iterator, token_list_p.end()) );
			break;

		case Vocabulary::_Struct:
			if (std::prev(iterator, 1)->_vocabulary == Vocabulary::_EndTemplateArgs)
			{
				__skip_code_block(iterator, token_list_p.end());
				break;
			}
			l_root._structs.push_back( __try_build_struct_node_mutually_recursive(u8"::", iterator, token_list_p.end()) );
			break;

		case Vocabulary::_FrogmanEngineEnumStructReflectionMacro:
			l_root._enum_structs.push_back( __try_build_enum_struct_node(u8"::", iterator, token_list_p.end()) );
			break;

		case Vocabulary::_Enum:
			__skip_code_block(iterator, token_list_p.end());
			break;

		default:
			break;
		}

		if (iterator == token_list_p.end())
		{
			break;
		}
	}

	return l_root;
}

_FE_NODISCARD_ namespace_node header_tool_engine::__try_build_namespace_node_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
{
	FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_Namespace, "Frogman Engine Header Tool Assertion Failure: the 'namespace' keyword is missing from the current token, but the header tool is attempting to build a namespace node.");
	namespace_node l_node;
	if (out_token_iterator_p == end_p)
	{
		return l_node;
	}

	l_node._target_namespace_name = file_buffer_t(parent_namespace_p, get_memory_resource());
	l_node._classes = std::pmr::vector<class_node>(get_memory_resource());
	l_node._structs = std::pmr::vector<struct_node>(get_memory_resource());
	l_node._enum_structs = std::pmr::vector<enum_struct_node>(get_memory_resource());

	switch (out_token_iterator_p->_vocabulary)
	{
	case Vocabulary::_Namespace:
		{
			++out_token_iterator_p;
			FE::clock l_loop_timer;
			l_loop_timer.start_clock();
			while (out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket)
			{
				l_node._target_namespace_name += out_token_iterator_p->_code;
				l_node._target_namespace_name += u8"::";
				THROW_CPP_SYNTAX_ERROR((l_loop_timer.get_delta_milliseconds() >= 1000.0) || (out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode), " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \n'{' is missing from the 'namespace Identifier {'.", &(out_token_iterator_p->_line_number));
				l_loop_timer.end_clock();
				++out_token_iterator_p;
			}
			++out_token_iterator_p;
		}
		break;

	case Vocabulary::_BeginNamespace:
		{
			++out_token_iterator_p;
			THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary != Vocabulary::_LeftParen, " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \n'(' is missing from the 'BEGIN_NAMESPACE(Identifier)'.", &(out_token_iterator_p->_line_number));
			while (out_token_iterator_p->_vocabulary != Vocabulary::_RightParen)
			{
				l_node._target_namespace_name += out_token_iterator_p->_code;
				l_node._target_namespace_name += u8"::";
				THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \n'(' is missing from the 'BEGIN_NAMESPACE(Identifier)'.", &(out_token_iterator_p->_line_number));
				++out_token_iterator_p;
			}
			++out_token_iterator_p;
		}
		break;

	_FE_NODEFAULT_;
	}

	// Allocate the necessary children nodes.
	{
		symbol_count l_nums = __try_count_the_current_scope_level_symbols(out_token_iterator_p, end_p);
		l_node._nested_namespaces = (0 == l_nums._namespaces) ? nullptr : std::make_unique<std::pmr::vector<namespace_node>>();
		l_node._classes.reserve(l_nums._classes);
		l_node._structs.reserve(l_nums._structs);
		l_node._enum_structs.reserve(l_nums._enums);
	}

	if (out_token_iterator_p == end_p)
	{
		return l_node;
	}

	for (; out_token_iterator_p != end_p; ++out_token_iterator_p)
	{
		switch (out_token_iterator_p->_vocabulary)
		{
		case Vocabulary::_Namespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_BeginNamespace:
			l_node._nested_namespaces->push_back( __try_build_namespace_node_recursive(l_node._target_namespace_name, out_token_iterator_p, end_p) );
			break;

		case Vocabulary::_Template:
			__try_skip_template_args(out_token_iterator_p);
			break;

		case Vocabulary::_Class:
			/*
				This line of code checks if it is a template class and skips the code.
				(out_token_iterator_p - 1)->_vocabulary technically is unsafe, but it won't read the memory before 'begin' since everybody uses the header guard or #pragma once to avoid header collisions.
			*/
			if (std::prev(out_token_iterator_p, 1)->_vocabulary == Vocabulary::_EndTemplateArgs)
			{
				__skip_code_block(out_token_iterator_p, end_p);
				break;
			}
			l_node._classes.push_back( __try_build_class_node_mutually_recursive(l_node._target_namespace_name, out_token_iterator_p, end_p) );
			break;

		case Vocabulary::_Struct:
			/*
				This line of code checks if it is a template class and skips the code.
				(out_token_iterator_p - 1)->_vocabulary technically is unsafe, but it won't read the memory before 'begin' since everybody uses the header guard or #pragma once to avoid header collisions.
			*/
			if (std::prev(out_token_iterator_p, 1)->_vocabulary == Vocabulary::_EndTemplateArgs)
			{
				__skip_code_block(out_token_iterator_p, end_p);
				break;
			}
			l_node._structs.push_back( __try_build_struct_node_mutually_recursive(l_node._target_namespace_name, out_token_iterator_p, end_p) );
			break;

		case Vocabulary::_FrogmanEngineEnumStructReflectionMacro:
			l_node._enum_structs.push_back( __try_build_enum_struct_node(l_node._target_namespace_name, out_token_iterator_p, end_p) );
			break;

		case Vocabulary::_Enum:
			__skip_code_block(out_token_iterator_p, end_p);
			break;

		default:
			return l_node;
		}

		if (out_token_iterator_p == end_p)
		{
			break;
		}
	}
	return l_node;
}

_FE_NODISCARD_ class_node header_tool_engine::__try_build_class_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
{
	FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_Class, "Frogman Engine Header Tool Assertion Failure: the 'class' keyword is missing from the current token, but the header tool is attempting to build a class node.");
	++out_token_iterator_p; // move to the class name.
	const file_buffer_t& l_class_name = out_token_iterator_p->_code; // get the class name.
	++out_token_iterator_p; // skip the class name.

	class_node l_node;
	if (out_token_iterator_p == end_p)
	{
		return l_node;
	}

	auto l_searchable_range_end = std::find_if(out_token_iterator_p, end_p, [&](const token& token_p) { return token_p._vocabulary == Vocabulary::_RightCurlyBracket; });
	// Check if the class has a base class, and it is reflective.
	if ((out_token_iterator_p->_vocabulary == Vocabulary::_Colon) &&
		(std::find_if(out_token_iterator_p, l_searchable_range_end, [&](const token& token_p) { return token_p._vocabulary == Vocabulary::_FrogmanEngineBaseClassReflectionMacro; }) != l_searchable_range_end))
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

		case Vocabulary::_EndOfCode:
			THROW_CPP_SYNTAX_ERROR(true, " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \nThe class definition is incomplete.", &(out_token_iterator_p->_line_number));
			break;

		default: // the base class name found.
			l_node._base_class_reflection_macro->_target_base_class_name = identifier(out_token_iterator_p->_code, get_memory_resource());
			++out_token_iterator_p; // skip the base class name.
			THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket, " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \n'{' is missing from the 'class Identifier {'.", &(out_token_iterator_p->_line_number));
			break;
		}
	}

	// the class has a base but reflection unenabled.
	if (out_token_iterator_p->_vocabulary != Vocabulary::_Colon)
	{
		THROW_CPP_SYNTAX_ERROR((out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket), " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \n'{' is missing from the 'class Identifier {'.", &(out_token_iterator_p->_line_number));
	}

	l_searchable_range_end = std::find_if(out_token_iterator_p, end_p, [&](const token& token_p) { return token_p._vocabulary == Vocabulary::_RightCurlyBracket; });
	auto l_class_reflection_macro_search_result = std::find_if(out_token_iterator_p, l_searchable_range_end, [&](const token& token_p) { return token_p._vocabulary == Vocabulary::_FrogmanEngineClassReflectionMacro; });
	if (l_class_reflection_macro_search_result != l_searchable_range_end)
	{
		l_node._class_reflection_macro = std::make_unique<frogman_engine_class_macro_node>();
		l_node._class_reflection_macro->_target_class_name = identifier(parent_namespace_p, get_memory_resource());
		l_node._class_reflection_macro->_target_class_name += l_class_name;
		out_token_iterator_p = l_class_reflection_macro_search_result;

		// To do: build n
		l_node._class_reflection_macro->_property_reflection_macros;

		// To do: build n
		l_node._class_reflection_macro->_method_reflection_macros;

		// To do: build n
		l_node._class_reflection_macro->_static_method_reflection_macros;

		for (; out_token_iterator_p != end_p; ++out_token_iterator_p)
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
					auto l_next = std::next(out_token_iterator_p, 1);
					if ((l_next != end_p) && 
						(l_next->_vocabulary == Vocabulary::_Semicolon))
					{
						++out_token_iterator_p;
						return l_node;
					}
				}
				break;
			}

			if (out_token_iterator_p == end_p)
			{
				break;
			}
		}
	}

	__skip_code_block(out_token_iterator_p, end_p);
	return l_node;
}

_FE_NODISCARD_ struct_node header_tool_engine::__try_build_struct_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
{
	FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_Struct, "Frogman Engine Header Tool Assertion Failure: the 'struct' keyword is missing from the current token, but the header tool is attempting to build a struct node.");
	++out_token_iterator_p; // move to the struct name.
	const file_buffer_t& l_class_name = out_token_iterator_p->_code; // get the struct name.
	++out_token_iterator_p; // skip the struct name.
	THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket, " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \n'{' is missing from the 'struct Identifier {'.", &(out_token_iterator_p->_line_number));

	struct_node l_node;
	if (out_token_iterator_p == end_p)
	{
		return l_node;
	}

	auto l_searchable_range_end = std::find_if(out_token_iterator_p, end_p, [&](const token& token_p) { return token_p._vocabulary == Vocabulary::_RightCurlyBracket; });
	auto l_struct_reflection_macro_search_result = std::find_if(out_token_iterator_p, l_searchable_range_end, [&](const token& token_p) { return token_p._vocabulary == Vocabulary::_FrogmanEngineStructReflectionMacro; });
	if (l_struct_reflection_macro_search_result != l_searchable_range_end)
	{
		l_node._struct_reflection_macro = std::make_unique<frogman_engine_struct_macro_node>();
		l_node._struct_reflection_macro->_target_struct_name = identifier(parent_namespace_p, get_memory_resource());
		l_node._struct_reflection_macro->_target_struct_name += l_class_name;
		out_token_iterator_p = l_struct_reflection_macro_search_result;

		// To do: build n
		l_node._struct_reflection_macro->_property_reflection_macros;

		for (; out_token_iterator_p != end_p; ++out_token_iterator_p)
		{
			switch (out_token_iterator_p->_vocabulary)
			{
			case Vocabulary::_FrogmanEnginePropertyReflectionMacro:

				break;

			case Vocabulary::_RightCurlyBracket:
				{
					auto l_next = std::next(out_token_iterator_p, 1);
					if ((l_next != end_p) &&
						(l_next->_vocabulary == Vocabulary::_Semicolon))
					{
						++out_token_iterator_p;
						return l_node;
					}
				}
				break;
			}
			
			if (out_token_iterator_p == end_p)
			{
				break;
			}
		}
	}

	__skip_code_block(out_token_iterator_p, end_p);
	return l_node;
}

_FE_NODISCARD_ enum_struct_node header_tool_engine::__try_build_enum_struct_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
{
	FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_FrogmanEngineEnumStructReflectionMacro, "Frogman Engine Header Tool Assertion Failure: the 'FE_ENUM_STRUCT()' macro indicator is missing from the current token, but the header tool is attempting to build an enum struct node.");
	++out_token_iterator_p; // skip the 'FE_ENUM_STRUCT()' macro.
	enum_struct_node l_enum_struct_node;
	{
		var::UTF8 l_buffer[4]{ '\0' };
		for (var::uint32 i = 0; i < 3; ++i)
		{
			while (static_cast<FE::ASCII>(*out_token_iterator_p->_code.c_str()) <= ' ')
			{
				++out_token_iterator_p;
			}
			l_buffer[i] = *out_token_iterator_p->_code.c_str();
			++out_token_iterator_p;
		}
		THROW_CPP_SYNTAX_ERROR((FE::algorithm::string::compare(l_buffer, u8"();") == false), " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \nPlease check if any letter is missing from the 'FE_ENUM_STRUCT();'.", &(out_token_iterator_p->_line_number));
	}

	if (out_token_iterator_p->_vocabulary == Vocabulary::_Enum)
	{
		for (var::int32 distance = 0; distance < 3; ++distance)
		{
			++out_token_iterator_p; // skip the 'enum' keyword and the 'struct' keyword.
			if (out_token_iterator_p->_vocabulary == Vocabulary::_Undefined)
			{
				break;
			}
			THROW_CPP_SYNTAX_ERROR(distance == 2, " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \nIncorrect C++ enum struct syntax.", &(out_token_iterator_p->_line_number));
		}
		// build the enum struct node here:
		l_enum_struct_node._target_enum_struct_name = identifier(parent_namespace_p, get_memory_resource());
		l_enum_struct_node._target_enum_struct_name += out_token_iterator_p->_code;
		++out_token_iterator_p; // skip the enum struct name.
		l_enum_struct_node._enum_struct_fields = std::pmr::vector<identifier>(get_memory_resource());
		
		for (; out_token_iterator_p != end_p; ++out_token_iterator_p)
		{
			switch (out_token_iterator_p->_vocabulary)
			{
			case Vocabulary::_Colon:
				while (out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket)
				{
					THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \nIncorrect C++ enum struct syntax.", &(out_token_iterator_p->_line_number));
					++out_token_iterator_p;
				}
				break;

			case Vocabulary::_Undefined:
				l_enum_struct_node._enum_struct_fields.push_back(out_token_iterator_p->_code);
				THROW_CPP_SYNTAX_ERROR(std::next(out_token_iterator_p)->_vocabulary == Vocabulary::_Undefined, " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \nIncorrect C++ enum struct syntax.", &(out_token_iterator_p->_line_number));
				break;
				
			case Vocabulary::_RightCurlyBracket:
				{
					auto l_next = std::next(out_token_iterator_p, 1);
					if ((l_next != end_p) &&
					(l_next->_vocabulary == Vocabulary::_Semicolon))
					{
						++out_token_iterator_p;
						return l_enum_struct_node;
					}
				}
				break;

			case Vocabulary::_Semicolon:
				return l_enum_struct_node;

			case Vocabulary::_AssignmentOperator:
				while ((out_token_iterator_p->_vocabulary != Vocabulary::_Comma) && (out_token_iterator_p->_vocabulary != Vocabulary::_RightCurlyBracket))
				{
					THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, " FHT C++ code syntax Error: \nThe line number: ${%u32@1} \nIncorrect C++ enum struct syntax.", &(out_token_iterator_p->_line_number));
					++out_token_iterator_p;
				}
				break;

			default:
				break;
			}

			if (out_token_iterator_p == end_p)
			{
				break;
			}
		}
	}
	return l_enum_struct_node;
}

void header_tool_engine::__try_skip_template_args(typename std::pmr::list<token>::const_iterator& iterator_p) const
{
	++iterator_p;
	THROW_CPP_SYNTAX_ERROR(iterator_p->_vocabulary != Vocabulary::_BeginTemplateArgs, "FHT C++ code syntax Error: \nThe line number: ${%u32@0} \n'<' is missing from the 'template<...>'.", &(iterator_p->_line_number));
	FE::clock l_loop_timer;
	l_loop_timer.start_clock();
	while (iterator_p->_vocabulary != Vocabulary::_EndTemplateArgs)
	{
		THROW_CPP_SYNTAX_ERROR((l_loop_timer.get_delta_milliseconds() >= 1000.0) || (iterator_p->_vocabulary == Vocabulary::_EndOfCode), "FHT C++ code syntax Error: \nThe line number: ${%u32@0} \n'>' is missing from the 'template<...>'.", &(iterator_p->_line_number));
		++iterator_p;
		l_loop_timer.end_clock();
	}
}