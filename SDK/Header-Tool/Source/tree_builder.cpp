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
#include "header_tool_engine.hpp"




void header_tool_engine::__skip_code_block(typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p) const noexcept
{
	var::uint64 l_block_scope_count = 0;
	for (; out_token_iterator_p != end_p; ++out_token_iterator_p)
	{
		switch (out_token_iterator_p->_vocabulary)
		{
		case Vocabulary::_LeftCurlyBracket:
			++l_block_scope_count;
			break;

		case Vocabulary::_RightCurlyBracket:
			{
				auto l_next = std::next(out_token_iterator_p, 1);
				if ((l_next != end_p) && (l_block_scope_count == 1) &&
					(l_next->_vocabulary == Vocabulary::_Semicolon))
				{
					++out_token_iterator_p;
					return;
				}
				--l_block_scope_count;
				break;
			}

		default:
			break;
		}
	}
}

bool header_tool_engine::__is_forward_declaration(typename std::pmr::list<token>::const_iterator& out_token_iterator_p) const
{
	auto l_origin = out_token_iterator_p;
	while (out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket)
	{
		switch (out_token_iterator_p->_vocabulary)
		{
		case Vocabulary::_EndOfCode:
			THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "C++ code syntax Error: \nThe line number: ${%u32@0} \n'{' is missing from the 'class Identifier {'.", &(out_token_iterator_p->_line_number));
			_FE_FALLTHROUGH_;

		case Vocabulary::_Comma:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Pointer:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Semicolon:
			return true;

		default:
			break;
		}
		++out_token_iterator_p;
	}
	out_token_iterator_p = l_origin; // reset to the origin; it is not a forward declaration.
	return false;
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
		symbol_count l_total_nums = __try_count_all_symbols(token_list_p.begin(), token_list_p.cend());

		static std::mutex l_s_log_lock;
		std::lock_guard<std::mutex> l_guard(l_s_log_lock);
		std::wcout << L"Frogman Engine Header Tool: In the file located at '" << file_path_p.c_str() << "'\n";
		std::cout << "Frogman Engine Header Tool: the total number of namespaces is " << l_total_nums._namespaces << '\n';
		std::cout << "Frogman Engine Header Tool: the total number of classes is " << l_total_nums._classes << '\n';
		std::cout << "Frogman Engine Header Tool: the total number of structs is " << l_total_nums._structs << "\n";
		std::cout << "Frogman Engine Header Tool: the total number of enum structs is " << l_total_nums._enums << "\n\n";
	}

	header_file_root l_root;
	l_root._path_to_the_header_file = file_path_p;
	l_root._namespaces = std::pmr::vector< std::optional<namespace_node> >(get_memory_resource());
	l_root._classes = std::pmr::vector< std::optional<class_node> >(get_memory_resource());
	l_root._structs = std::pmr::vector< std::optional<struct_node> >(get_memory_resource());
	l_root._enum_structs = std::pmr::vector< std::optional<enum_struct_node> >(get_memory_resource());

	bool l_is_template = false;
	for (auto iterator = token_list_p.begin(); iterator != token_list_p.end(); ++iterator)
	{
		switch (iterator->_vocabulary)
		{
		case Vocabulary::_Namespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_BeginNamespace:
			THROW_CPP_SYNTAX_ERROR(l_is_template == true, "C++ code syntax Error: \nThe line number: ${%u32@0} \nCan't place 'template' before 'namespace.'", &(iterator->_line_number));
			l_root._namespaces.push_back( __try_build_namespace_node_recursive(u8"", iterator, token_list_p.end()) );
			break;

		case Vocabulary::_Template:
			__skip_template_args(iterator);
			l_is_template = true;
			break;

		case Vocabulary::_Semicolon:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Using:
			l_is_template = false;
			break;

		case Vocabulary::_Class:
			if (__is_forward_declaration(iterator) == true)
			{
				l_is_template = false;
				break;
			}

			if (l_is_template == true)
			{
				__skip_code_block(iterator, token_list_p.end());
				l_is_template = false;
				break;
			}

			l_root._classes.push_back( __try_build_class_node_mutually_recursive(u8"", iterator, token_list_p.end()) );
			break;

		case Vocabulary::_Struct:
			if (__is_forward_declaration(iterator) == true)
			{
				l_is_template = false;
				break;
			}

			if (l_is_template == true)
			{
				__skip_code_block(iterator, token_list_p.end());
				l_is_template = false;
				break;
			}

			l_root._structs.push_back( __try_build_struct_node_mutually_recursive(u8"", iterator, token_list_p.end()) );
			break;

		case Vocabulary::_FrogmanEngineEnumStructReflectionMacro:
			THROW_CPP_SYNTAX_ERROR(l_is_template == true, "C++ code syntax Error: \nThe line number: ${%u32@0} \nCan't place 'template' before 'FE_ENUM_STRUCT().'", &(iterator->_line_number));
			l_root._enum_structs.push_back( __try_build_enum_struct_node(u8"", iterator, token_list_p.end()) );
			break;

		case Vocabulary::_Enum:
			THROW_CPP_SYNTAX_ERROR(l_is_template == true, "C++ code syntax Error: \nThe line number: ${%u32@0} \nCan't place 'template' before 'enum.'", &(iterator->_line_number));

			if (__is_forward_declaration(iterator) == true)
			{
				break;
			}

			__skip_code_block(iterator, token_list_p.end());
			break;

		case Vocabulary::_LeftParen:
			{
				std::optional<identifier> l_system_function_name = __try_build_c_style_system_function_node(u8"", iterator, token_list_p.end());
				if (l_system_function_name == std::nullopt)
				{
					break;
				}
				l_root._c_style_systems.push_back(l_system_function_name);
			}
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

_FE_NODISCARD_ std::optional<namespace_node> header_tool_engine::__try_build_namespace_node_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
{
	FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_Namespace || out_token_iterator_p->_vocabulary == Vocabulary::_BeginNamespace, "Frogman Engine Header Tool Assertion Failure: the 'namespace' keyword is missing from the current token, but the header tool is attempting to build a namespace node.");
	/* Five tokens are needed at least to build a namespace node.
	namespace Some
	{
	};
	*/
	THROW_CPP_SYNTAX_ERROR(std::distance(out_token_iterator_p, end_p) < 5, "C++ code syntax Error: \nThe line number: ${%u32@0}.", &(out_token_iterator_p->_line_number));

	namespace_node l_node;

	l_node._target_namespace_name = file_buffer_t(parent_namespace_p, get_memory_resource());
	l_node._classes = std::pmr::vector< std::optional<class_node> >(get_memory_resource());
	l_node._structs = std::pmr::vector< std::optional<struct_node> >(get_memory_resource());
	l_node._enum_structs = std::pmr::vector< std::optional<enum_struct_node> >(get_memory_resource());

	// Allocate the necessary children nodes.
	symbol_count l_nums = __try_count_the_current_scope_level_symbols(out_token_iterator_p, end_p);
	l_node._nested_namespaces = (0 == l_nums._namespaces) ? nullptr : std::make_unique<std::pmr::vector< std::optional<namespace_node> >>();
	l_node._classes.reserve(l_nums._classes);
	l_node._structs.reserve(l_nums._structs);
	l_node._enum_structs.reserve(l_nums._enums);

	switch (out_token_iterator_p->_vocabulary)
	{
	case Vocabulary::_Namespace:
		{
			while (out_token_iterator_p->_vocabulary != Vocabulary::_Undefined)
			{
				THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "C++ code syntax Error: \nThe line number: ${%u32@0} \n'Identifier' is missing from the 'BEGIN_NAMESPACE(Identifier)'.", &(out_token_iterator_p->_line_number));
				++out_token_iterator_p;
			}
			l_node._target_namespace_name += u8"::";
			while (out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket)
			{
				l_node._target_namespace_name += out_token_iterator_p->_code;
				THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "C++ code syntax Error: \nThe line number: ${%u32@0} \n'{' is missing from the 'namespace Identifier {'.", &(out_token_iterator_p->_line_number));
				++out_token_iterator_p;
			}
		}
		break;

	case Vocabulary::_BeginNamespace:
		{
			++out_token_iterator_p;
			THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary != Vocabulary::_LeftParen, "C++ code syntax Error: \nThe line number: ${%u32@0} \n'(' is missing from the 'BEGIN_NAMESPACE(Identifier)'.", &(out_token_iterator_p->_line_number));
			while (out_token_iterator_p->_vocabulary != Vocabulary::_Undefined)
			{
				THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "C++ code syntax Error: \nThe line number: ${%u32@0} \n'Identifier' is missing from the 'BEGIN_NAMESPACE(Identifier)'.", &(out_token_iterator_p->_line_number));
				++out_token_iterator_p;
			}
			l_node._target_namespace_name += u8"::";
			while (out_token_iterator_p->_vocabulary != Vocabulary::_RightParen)
			{
				l_node._target_namespace_name += out_token_iterator_p->_code;
				THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "C++ code syntax Error: \nThe line number: ${%u32@0} \n'(' is missing from the 'BEGIN_NAMESPACE(Identifier)'.", &(out_token_iterator_p->_line_number));
				++out_token_iterator_p;
			}
		}
		break;

	_FE_NODEFAULT_;
	}

	bool l_is_template = false;
	for (; out_token_iterator_p != end_p; ++out_token_iterator_p)
	{
		switch (out_token_iterator_p->_vocabulary)
		{
		case Vocabulary::_Namespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_BeginNamespace:
			THROW_CPP_SYNTAX_ERROR(l_is_template == true, "C++ code syntax Error: \nThe line number: ${%u32@0} \nCan't place 'template' before 'namespace.'", &(out_token_iterator_p->_line_number));
			l_node._nested_namespaces->push_back( __try_build_namespace_node_recursive(l_node._target_namespace_name, out_token_iterator_p, end_p) );
			break;

		case Vocabulary::_EndNamespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_RightCurlyBracket:
			return l_node;

		case Vocabulary::_Template:
			__skip_template_args(out_token_iterator_p);
			l_is_template = true;
			break;

		case Vocabulary::_Semicolon:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Using:
			l_is_template = false;
			break;

		case Vocabulary::_Class:
			if (__is_forward_declaration(out_token_iterator_p) == true)
			{
				l_is_template = false;
				break;
			}

			if (l_is_template == true)
			{
				__skip_code_block(out_token_iterator_p, end_p);
				l_is_template = false;
				break;
			}

			l_node._classes.push_back( __try_build_class_node_mutually_recursive(l_node._target_namespace_name, out_token_iterator_p, end_p) );
			break;

		case Vocabulary::_Struct:
			if (__is_forward_declaration(out_token_iterator_p) == true)
			{
				l_is_template = false;
				break;
			}

			if (l_is_template == true)
			{
				__skip_code_block(out_token_iterator_p, end_p);
				l_is_template = false;
				break;
			}

			l_node._structs.push_back( __try_build_struct_node_mutually_recursive(l_node._target_namespace_name, out_token_iterator_p, end_p) );
			break;

		case Vocabulary::_FrogmanEngineEnumStructReflectionMacro:
			THROW_CPP_SYNTAX_ERROR(l_is_template == true, "C++ code syntax Error: \nThe line number: ${%u32@0} \nCan't place 'template' before 'FE_ENUM_STRUCT().'", &(out_token_iterator_p->_line_number));
			l_node._enum_structs.push_back( __try_build_enum_struct_node(l_node._target_namespace_name, out_token_iterator_p, end_p) );
			break;

		case Vocabulary::_Enum:
			//THROW_CPP_SYNTAX_ERROR(l_is_template == true, "C++ code syntax Error: \nThe line number: ${%u32@0} \nCan't place 'template' before 'enum.'", &(out_token_iterator_p->_line_number));
			__skip_code_block(out_token_iterator_p, end_p);
			break;

		case Vocabulary::_LeftParen:
			{
				std::optional<identifier> l_system_function_name = __try_build_c_style_system_function_node(l_node._target_namespace_name, out_token_iterator_p, end_p);
				if (l_system_function_name == std::nullopt)
				{
					break;
				}
				l_node._c_style_systems.push_back(l_system_function_name);

				while (out_token_iterator_p->_vocabulary != Vocabulary::_Semicolon)
				{
					if (out_token_iterator_p->_vocabulary == Vocabulary::_LeftCurlyBracket)
					{
						__skip_code_block(out_token_iterator_p, end_p);
						break;
					}
					THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "C++ code syntax Error: \nThe line number: ${%u32@0} \n';' is missing from the end of the C-style system function declaration.", &(out_token_iterator_p->_line_number));
					++out_token_iterator_p;
				}
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
	return l_node;
}

_FE_NODISCARD_ std::optional<class_node> header_tool_engine::__try_build_class_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
{
	FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_Class, "Frogman Engine Header Tool Assertion Failure: the 'class' keyword is missing from the current token, but the header tool is attempting to build a class node.");
	/* Five tokens are needed at least to build a class node.
	class Some
	{
	};
	*/
	THROW_CPP_SYNTAX_ERROR(std::distance(out_token_iterator_p, end_p) < 5, "C++ code syntax Error: \nThe line number: ${%u32@0}.", &(out_token_iterator_p->_line_number));
	
	while (out_token_iterator_p->_vocabulary != Vocabulary::_Undefined)
	{
		++out_token_iterator_p; // move to the name.
	}
	const file_buffer_t& l_class_name = out_token_iterator_p->_code; // get the class name.
	++out_token_iterator_p; // skip the class name.

	if (out_token_iterator_p->_vocabulary == Vocabulary::_Final)
	{
		++out_token_iterator_p; // skip the 'final' keyword.
	}

	class_node l_node;

	// Check if the class has a base class.
	switch (out_token_iterator_p->_vocabulary)
	{
	case Vocabulary::_Colon:
		++out_token_iterator_p; // skip the ':'.
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
			l_node._base_class_name = identifier(get_memory_resource());
			
			while (out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket)
			{
				THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "C++ code syntax Error: \nThe line number: ${%u32@0} \n'{' is missing from the 'class Identifier {'.", &(out_token_iterator_p->_line_number));
				l_node._base_class_name += out_token_iterator_p->_code;
				++out_token_iterator_p; // skip the base class name.
			}

			{
				/*
				I wish I could use a constepxr hashing function with a switch statement here.
				To do: create a constexpr function from scratch.
				*/

				// this is safe since all C++ class names are formatted in English, which guarantees them to be ASCII.
				if (l_node._base_class_name.find(u8"archetype_base") != identifier::npos)
				{
					l_node._class_type = ClassType::_ChildOfArchetypeBase;
				}
				else if (l_node._base_class_name.find(u8"component_base") != identifier::npos)
				{
					l_node._class_type = ClassType::_ChildOfComponentBase;
				}
				else if (l_node._base_class_name.find(u8"system_base") != identifier::npos)
				{
					l_node._class_type = ClassType::_ChildOfSystemBase;
				}
				else
				{
					l_node._class_type = ClassType::_ChildOfCppClass;
				}
			}
			break;

		case Vocabulary::_EndOfCode:
			THROW_CPP_SYNTAX_ERROR(true, "C++ code syntax Error: \nThe line number: ${%u32@0} \nThe class definition is incomplete.", &(out_token_iterator_p->_line_number));
			break;
		}
		break;

	case Vocabulary::_BeginTemplateArgs:
		__skip_template_args(out_token_iterator_p);
		return std::nullopt; // forward declaration; do nothing and return an empty class node.

	case Vocabulary::_Semicolon:	
		return std::nullopt; // forward declaration; do nothing and return an empty class node.
	}


	l_node._this_class_name = identifier(parent_namespace_p, get_memory_resource());
	l_node._this_class_name += u8"::";
	l_node._this_class_name += l_class_name;


	// Calculate where the class definition ends.
	auto l_class_end = out_token_iterator_p;
	int l_match_counter = 0;
	while (l_class_end != end_p)
	{
		switch (l_class_end->_vocabulary)
		{
		case Vocabulary::_LeftCurlyBracket:
			++l_match_counter;
			break;

		case Vocabulary::_RightCurlyBracket:
			--l_match_counter;
			break;

		default:
			break;
		}

		if (0 == l_match_counter)
		{
			goto Break;
		}
		++l_class_end;
	}
Break:
	identifier l_function_declaration_buffer(get_memory_resource());
	l_function_declaration_buffer.reserve(64);

	auto l_function_declaration_pos = std::find_if(out_token_iterator_p, l_class_end, [&l_class_name](const token& token_p) { return (token_p._code == l_class_name); });
	if (l_class_end == l_function_declaration_pos) // The class has a default constructor only.
	{
		__skip_code_block(out_token_iterator_p, end_p);
		return l_node;
	}

	identifier l_constructor(l_class_name, get_memory_resource());
	l_constructor += u8"()";
	bool l_does_have_default_constructor = false;

	while (l_function_declaration_pos != l_class_end)
	{
		while (l_function_declaration_pos != l_class_end)
		{
			if ((l_function_declaration_pos->_vocabulary == Vocabulary::_Semicolon) || (l_function_declaration_pos->_vocabulary == Vocabulary::_LeftCurlyBracket))
			{
				break;
			}
			l_function_declaration_buffer += l_function_declaration_pos->_code;
			++l_function_declaration_pos;
		}

		if ((identifier::npos == l_function_declaration_buffer.find(l_constructor)) ||
			(identifier::npos != l_function_declaration_buffer.find(u8"=delete")))
		{
			l_function_declaration_pos = std::find_if(l_function_declaration_pos, l_class_end, [&l_class_name](const token& token_p) { return token_p._code == l_class_name; });
			l_function_declaration_buffer.clear();
			continue;
		}
		l_function_declaration_buffer.clear();
		l_does_have_default_constructor = true;
		break;
	}


	bool l_is_pure_virtual_class = false;
	l_function_declaration_pos = std::find_if(out_token_iterator_p, l_class_end, [](const token& token_p) { return token_p._vocabulary == Vocabulary::_Virtual; });

	while (l_function_declaration_pos != l_class_end)
	{
		while (l_function_declaration_pos != l_class_end)
		{
			if ((l_function_declaration_pos->_vocabulary == Vocabulary::_Semicolon) || (l_function_declaration_pos->_vocabulary == Vocabulary::_LeftCurlyBracket))
			{
				break;
			}
			l_function_declaration_buffer += l_function_declaration_pos->_code;
			++l_function_declaration_pos;
		}

		if (identifier::npos != l_function_declaration_buffer.find(u8"=0"))
		{
			l_is_pure_virtual_class = true;
			break;
		}
		l_function_declaration_pos = std::find_if(l_function_declaration_pos, l_class_end, [](const token& token_p) { return token_p._vocabulary == Vocabulary::_Virtual; });
	}

	out_token_iterator_p = l_class_end;

	if ((true == l_is_pure_virtual_class) ||
		(false == l_does_have_default_constructor))
	{
		return std::nullopt;
	}

	return l_node;
}

_FE_NODISCARD_ std::optional<struct_node> header_tool_engine::__try_build_struct_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
{
	FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_Struct, "Frogman Engine Header Tool Assertion Failure: the 'struct' keyword is missing from the current token, but the header tool is attempting to build a struct node.");
	
	/* Five tokens are needed at least to build a struct node.
	struct Some
	{
	};
	*/
	THROW_CPP_SYNTAX_ERROR(std::distance(out_token_iterator_p, end_p) < 5, "C++ code syntax Error: \nThe line number: ${%u32@0}.", &(out_token_iterator_p->_line_number));

	while (out_token_iterator_p->_vocabulary != Vocabulary::_Undefined)
	{
		++out_token_iterator_p; // move to the name.
	}
	const file_buffer_t& l_class_name = out_token_iterator_p->_code; // get the struct name.
	++out_token_iterator_p; // skip the struct name.

	switch (out_token_iterator_p->_vocabulary)
	{
	case Vocabulary::_Final:
		++out_token_iterator_p; // skip the 'final' keyword.
		break;

	case Vocabulary::_BeginTemplateArgs:
		__skip_template_args(out_token_iterator_p);
		return std::nullopt; // forward declaration; do nothing and return an empty struct node.

	case Vocabulary::_Semicolon:
		return std::nullopt; // forward declaration; do nothing and return an empty struct node.
	}

	THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket, "C++ code syntax Error: \nThe line number: ${%u32@0} \n FHT does not support struct polymorphism; '{' may be missing from the 'struct Identifier {'.", &(out_token_iterator_p->_line_number));

	struct_node l_node;

	l_node._target_struct_name = identifier(parent_namespace_p, get_memory_resource());
	l_node._target_struct_name += u8"::";
	l_node._target_struct_name += l_class_name;

	__skip_code_block(out_token_iterator_p, end_p);
	return l_node;
}

_FE_NODISCARD_ std::optional<enum_struct_node> header_tool_engine::__try_build_enum_struct_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
{
	FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_FrogmanEngineEnumStructReflectionMacro, "Frogman Engine Header Tool Assertion Failure: the 'FE_ENUM_STRUCT()' macro indicator is missing from the current token, but the header tool is attempting to build an enum struct node.");
	++out_token_iterator_p; // skip the 'FE_ENUM_STRUCT()' macro.
	enum_struct_node l_node;
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
		THROW_CPP_SYNTAX_ERROR((FE::algorithm::string::compare(l_buffer, u8"();") == false), "C++ code syntax Error: \nThe line number: ${%u32@0} \nPlease check if any letter is missing from the 'FE_ENUM_STRUCT();'.", &(out_token_iterator_p->_line_number));
	}

	if (out_token_iterator_p->_vocabulary == Vocabulary::_Enum)
	{
		for (var::int32 distance = 0; distance <= 3; ++distance)
		{
			++out_token_iterator_p; // skip the 'enum' keyword and the 'struct' keyword.
			if (out_token_iterator_p->_vocabulary == Vocabulary::_Undefined)
			{
				break;
			}
			THROW_CPP_SYNTAX_ERROR(distance == 3, "C++ code syntax Error: \nThe line number: ${%u32@0} \nIncorrect C++ enum struct syntax.", &(out_token_iterator_p->_line_number));
		}
		// build the enum struct node here:
		l_node._target_enum_struct_name = identifier(parent_namespace_p, get_memory_resource());
		l_node._target_enum_struct_name += u8"::";
		l_node._target_enum_struct_name += out_token_iterator_p->_code;
		++out_token_iterator_p; // skip the enum struct name.
		l_node._enum_struct_fields = std::pmr::vector<identifier>(get_memory_resource());
		
		for (; out_token_iterator_p != end_p; ++out_token_iterator_p)
		{
			switch (out_token_iterator_p->_vocabulary)
			{
			case Vocabulary::_Colon:
				while (out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket)
				{
					THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "C++ code syntax Error: \nThe line number: ${%u32@0} \nIncorrect C++ enum struct syntax.", &(out_token_iterator_p->_line_number));
					++out_token_iterator_p;
				}
				break;

			case Vocabulary::_Undefined:
				l_node._enum_struct_fields.push_back(out_token_iterator_p->_code);
				THROW_CPP_SYNTAX_ERROR(std::next(out_token_iterator_p)->_vocabulary == Vocabulary::_Undefined, "C++ code syntax Error: \nThe line number: ${%u32@0} \nIncorrect C++ enum struct syntax.", &(out_token_iterator_p->_line_number));
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

			case Vocabulary::_Semicolon:
				return l_node;

			case Vocabulary::_AssignmentOperator:
				while ((out_token_iterator_p->_vocabulary != Vocabulary::_Comma) && (out_token_iterator_p->_vocabulary != Vocabulary::_RightCurlyBracket))
				{
					THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary == Vocabulary::_EndOfCode, "C++ code syntax Error: \nThe line number: ${%u32@0} \nIncorrect C++ enum struct syntax.", &(out_token_iterator_p->_line_number));
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
	return l_node;
}

_FE_NODISCARD_ std::optional<identifier> header_tool_engine::__try_build_c_style_system_function_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
{
	FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_LeftParen);
	typename std::pmr::list<token>::const_iterator l_iterator = out_token_iterator_p;

	identifier l_system_parameter(get_memory_resource());
	l_system_parameter.reserve(64);

	while (out_token_iterator_p != end_p)
	{
		++out_token_iterator_p;
		if (out_token_iterator_p->_vocabulary == Vocabulary::_RightParen)
		{
			break;
		}
		l_system_parameter += out_token_iterator_p->_code;
	}

	identifier l_node;
	if (l_system_parameter.length() == 0)
	{
		return std::nullopt;
	}
	l_node = identifier(get_memory_resource());
	l_node.reserve(64);
	l_node = parent_namespace_p;
	l_node += u8"::";

	if ((false == FE::algorithm::string::space_insensitive_contains(l_system_parameter.c_str(), l_system_parameter.length(), u8"component_base* const"))
		|| (identifier::npos != l_system_parameter.find(','))) // it is not a valid C-style system function if there are multiple parameters.)
	{
		l_node.clear();
		return std::nullopt;
	}


	FE_ASSERT(l_iterator->_vocabulary == Vocabulary::_LeftParen);
	--l_iterator;
	if (l_iterator->_vocabulary == Vocabulary::_RightParen)
	{
		l_node.clear();
		return std::nullopt; // it is not a valid C-style system function if there is no return type; it is a system_base's virtual function.
	}

	FE_ASSERT(l_iterator->_vocabulary == Vocabulary::_Undefined);
	l_node += l_iterator->_code;

	--l_iterator;
	if (l_iterator->_code != u8"void")
	{
		l_node.clear();
		return std::nullopt; // it is not a valid C-style system function if the return type is not 'void'.
	}

	return l_node;
}

void header_tool_engine::__skip_template_args(typename std::pmr::list<token>::const_iterator& iterator_p) const
{
	while (iterator_p->_vocabulary != Vocabulary::_EndTemplateArgs)
	{
		THROW_CPP_SYNTAX_ERROR(iterator_p->_vocabulary == Vocabulary::_EndOfCode, "C++ code syntax Error: \nThe line number: ${%u32@0} \n'>' is missing from the 'template<...>'.", &(iterator_p->_line_number));
		++iterator_p;
	}
}