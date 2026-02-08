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
#include "header_tool.hpp"
#include "error_code.hpp"
#include "parser.hpp"
#include "symbol_counter.hpp"




namespace FHT::parser
{
	_FE_NODISCARD_ header_file_root build_reflextree(const directory_t& file_path_p, const std::pmr::list<token>& token_list_p)
	{
		FHT::symbol_counter::symbol_count l_total_nums = FHT::symbol_counter::try_count_all_symbols(token_list_p.begin(), token_list_p.cend());
		{
			static std::mutex l_s_log_lock;
			std::lock_guard<std::mutex> l_guard(l_s_log_lock);
			std::wcout << L"Frogman Engine Header Tool: In the file located at '" << file_path_p.c_str() << "'\n";
			std::cout << "Frogman Engine Header Tool: the total number of namespaces is " << l_total_nums._namespaces << '\n';
			std::cout << "Frogman Engine Header Tool: the total number of classes is " << l_total_nums._classes << '\n';
			std::cout << "Frogman Engine Header Tool: the total number of structs is " << l_total_nums._structs << "\n";
			std::cout << "Frogman Engine Header Tool: the total number of enum structs is " << l_total_nums._enum_structs << "\n";
			std::cout << "Frogman Engine Header Tool: the total number of the Frogman Engine ECS framework system methods is " << l_total_nums._systems << "\n\n";
		}

		header_file_root l_root = 
		{ 
			._path_to_the_header_file = file_path_p,
			._namespaces{	framework::get_framework().get_memory_resource()},
			._classes{		framework::get_framework().get_memory_resource()},
			._structs{		framework::get_framework().get_memory_resource()},
			._enum_structs{	framework::get_framework().get_memory_resource()},
			._system_fptrs{	framework::get_framework().get_memory_resource()}
		};

		l_root._namespaces.reserve(l_total_nums._namespaces);
		l_root._classes.reserve(l_total_nums._classes);
		l_root._structs.reserve(l_total_nums._structs);
		l_root._enum_structs.reserve(l_total_nums._enum_structs);
		l_root._system_fptrs.reserve(l_total_nums._systems);


		context_stack_t l_context_stack{ 1, FHT::Context::_Global, framework::get_framework().get_memory_resource() };

		for (auto iterator = token_list_p.begin(); iterator != token_list_p.end(); ++iterator)
		{
			switch (iterator->_vocabulary)
			{
			case Vocabulary::_FrogmanEngineClassReflectionMacro:
				l_context_stack.push_back(Context::_Class);
				while (iterator->_vocabulary != Vocabulary::_RightParen)
				{
					++iterator;
				}
				break;

			case Vocabulary::_FrogmanEngineStructReflectionMacro:
				l_context_stack.push_back(Context::_Struct);
				while (iterator->_vocabulary != Vocabulary::_RightParen)
				{
					++iterator;
				}
				break;

			case Vocabulary::_FrogmanEngineEnumStructReflectionMacro:
				l_context_stack.push_back(Context::_EnumStruct);
				while (iterator->_vocabulary != Vocabulary::_RightParen)
				{
					++iterator;
				}
				break;

			case Vocabulary::_FrogmanEngineSystemMacro:
				l_root._system_fptrs.emplace_back(build_ecs_system_node(u8"::", iterator, token_list_p.end()));
				break;


			case Vocabulary::_BeginNamespace:
				_FE_FALLTHROUGH_;
			case Vocabulary::_Namespace:
				l_root._namespaces.emplace_back(build_namespace_node_recursive(u8"::", iterator, token_list_p.end(), l_context_stack));
				break;

			case Vocabulary::_Class:
				if (l_context_stack.back() == Context::_Class)
				{
					l_root._classes.emplace_back(build_class_node(u8"::", iterator, token_list_p.end()));
					l_context_stack.pop_back();
				}
				break;

			case Vocabulary::_Struct:
				if (l_context_stack.back() == Context::_Struct)
				{
					l_root._structs.emplace_back(build_struct_node(u8"::", iterator, token_list_p.end()));
					l_context_stack.pop_back();
				}
				break;

			case Vocabulary::_EnumStruct:
				if (l_context_stack.back() == Context::_EnumStruct)
				{
					l_root._enum_structs.emplace_back(build_enum_struct_node(u8"::", iterator, token_list_p.end()));
					l_context_stack.pop_back();
				}
				break;


			default:
				break;
			}
		}

		return l_root;
	}

	_FE_NODISCARD_ namespace_node build_namespace_node_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p, context_stack_t& context_stack_p)
	{
		namespace_node l_node = 
		{
				._target_namespace_name{ framework::get_framework().get_memory_resource() },
				._nested_namespaces{ framework::get_framework().get_memory_resource() },
				._classes{ framework::get_framework().get_memory_resource() },
				._structs{ framework::get_framework().get_memory_resource() },
				._enum_structs{ framework::get_framework().get_memory_resource() },
				._system_fptrs{ framework::get_framework().get_memory_resource() }
		};

		FHT::symbol_counter::symbol_count l_total_nums = FHT::symbol_counter::try_count_the_current_scope_level_symbols(out_token_iterator_p, end_p);
		l_node._nested_namespaces.reserve(l_total_nums._namespaces);
		l_node._classes.reserve(l_total_nums._classes);
		l_node._structs.reserve(l_total_nums._structs);
		l_node._enum_structs.reserve(l_total_nums._enum_structs);
		l_node._system_fptrs.reserve(l_total_nums._systems);


		l_node._target_namespace_name = parent_namespace_p;
		switch (out_token_iterator_p->_vocabulary)
		{
		case Vocabulary::_BeginNamespace:
			{
				auto l_right_paren = out_token_iterator_p->_code.find(u8')');
				if (l_right_paren != identifier::npos)
				{
					const auto l_left_paren = out_token_iterator_p->_code.find(u8'(');
					FE_ASSERT(l_left_paren != identifier::npos);
					for (auto idx = l_left_paren+1; idx < l_right_paren; ++idx)
					{
						l_node._target_namespace_name += out_token_iterator_p->_code[idx];
					}
					break;
				}

				++out_token_iterator_p;
				while (out_token_iterator_p->_vocabulary != Vocabulary::_RightParen)
				{
					if (out_token_iterator_p->_vocabulary == Vocabulary::_LeftParen) _FE_UNLIKELY_
					{
						continue;
					}

					l_node._target_namespace_name += out_token_iterator_p->_code;
					++out_token_iterator_p;
				}
			}
			break;


		case Vocabulary::_Namespace:
			++out_token_iterator_p;
			while (out_token_iterator_p->_vocabulary != Vocabulary::_LeftCurlyBracket)
			{
				l_node._target_namespace_name += out_token_iterator_p->_code;
				++out_token_iterator_p;
			}
			break;

		_FE_NODEFAULT_;
		}
		l_node._target_namespace_name += u8"::";
		++out_token_iterator_p;


		while (out_token_iterator_p != end_p)
		{
			switch (out_token_iterator_p->_vocabulary)
			{
			case Vocabulary::_RightCurlyBracket:
				_FE_FALLTHROUGH_;
			case Vocabulary::_EndNamespace:
				return l_node;


			case Vocabulary::_FrogmanEngineClassReflectionMacro:
				context_stack_p.push_back(Context::_Class);
				while (out_token_iterator_p->_vocabulary != Vocabulary::_RightParen)
				{
					++out_token_iterator_p;
				}
				break;

			case Vocabulary::_FrogmanEngineStructReflectionMacro:
				context_stack_p.push_back(Context::_Struct);
				while (out_token_iterator_p->_vocabulary != Vocabulary::_RightParen)
				{
					++out_token_iterator_p;
				}
				break;

			case Vocabulary::_FrogmanEngineEnumStructReflectionMacro:
				context_stack_p.push_back(Context::_EnumStruct);
				while (out_token_iterator_p->_vocabulary != Vocabulary::_RightParen)
				{
					++out_token_iterator_p;
				}
				break;

			case Vocabulary::_FrogmanEngineSystemMacro:
				l_node._system_fptrs.emplace_back(build_ecs_system_node(l_node._target_namespace_name, out_token_iterator_p, end_p));
				break;


			case Vocabulary::_BeginNamespace:
				_FE_FALLTHROUGH_;
			case Vocabulary::_Namespace:
				l_node._nested_namespaces.emplace_back(build_namespace_node_recursive(l_node._target_namespace_name, out_token_iterator_p, end_p, context_stack_p));
				break;

			case Vocabulary::_Class:
				if (context_stack_p.back() == Context::_Class)
				{
					l_node._classes.emplace_back(build_class_node(l_node._target_namespace_name, out_token_iterator_p, end_p));
					context_stack_p.pop_back();
				}
				break;

			case Vocabulary::_Struct:
				if (context_stack_p.back() == Context::_Struct)
				{
					l_node._structs.emplace_back(build_struct_node(l_node._target_namespace_name, out_token_iterator_p, end_p));
					context_stack_p.pop_back();
				}
				break;

			case Vocabulary::_EnumStruct:
				if (context_stack_p.back() == Context::_EnumStruct)
				{
					l_node._enum_structs.emplace_back(build_enum_struct_node(l_node._target_namespace_name, out_token_iterator_p, end_p));
					context_stack_p.pop_back();
				}
				break;


			default:
				break;
			}
			++out_token_iterator_p;
		}

		return l_node;
	}




	_FE_NODISCARD_ class_node build_class_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_Class);
		class_node l_node = 
		{
			._this_class_name{ out_token_iterator_p->_code, framework::get_framework().get_memory_resource() },
			._base_class_name{ framework::get_framework().get_memory_resource() }
		};


		{	// trim 'class'
			constexpr auto l_class = u8"class";
			auto l_class_keyword_len = l_node._this_class_name.find(l_class);
			FE_ASSERT(l_class_keyword_len != std::string::npos);

			l_class_keyword_len += FE::algorithm::string::length(l_class);
			l_node._this_class_name.erase(0, l_class_keyword_len);
		}

		auto l_attr_pos = l_node._this_class_name.rfind(u8']');
		if (l_attr_pos != std::string::npos) // has attributes
		{
			l_node._this_class_name.erase(0, l_attr_pos + 1); // remove the attributes
		}

		{	// trim spaces at front
			var::uint64 l_space_length = 0;
			for (auto character : l_node._this_class_name)
			{
				if ((character <= ' ') == false)
				{
					break;
				}
				++l_space_length;
			}
			l_node._this_class_name.erase(0, l_space_length);
		}

		{
			auto l_class_extension = l_node._this_class_name.find(':');
			if (l_class_extension != std::string::npos)
			{
				l_node._base_class_name.assign(l_node._this_class_name.c_str() + (l_class_extension + 1));
				l_node._this_class_name.erase(l_class_extension, l_node._this_class_name.length() - l_class_extension);
				l_node._class_type = ClassType::_ChildOfCppClass;
			}
			else
			{
				l_node._class_type = ClassType::_None;
			}
		}

		while (l_node._this_class_name.back() <= ' ')
		{
			l_node._this_class_name.pop_back();
		}

		{	// count class identifier length
			var::uint64 l_name_length = 0;
			for (auto character = l_node._this_class_name.rbegin(); character != l_node._this_class_name.rend(); ++character)
			{
				if (*character <= ' ')
				{
					++l_name_length;
					break;
				}
				++l_name_length;
			}

			if (l_name_length != l_node._this_class_name.length())
			{
				auto l_second_word_pos = (l_node._this_class_name.length() - l_name_length) + 1;
				constexpr auto l_final = u8"final";
				auto l_final_keyword_pos = FE::algorithm::string::find_the_first<var::UTF8>(l_node._this_class_name.c_str() + l_second_word_pos, l_final);
				if (l_final_keyword_pos != std::nullopt)
				{
					l_node._this_class_name.erase(l_second_word_pos + l_final_keyword_pos->_begin, l_final_keyword_pos->_end - l_final_keyword_pos->_begin);
				}
				else
				{
					l_node._this_class_name.erase(0, l_second_word_pos);
				}

				while (l_node._this_class_name.length() > 0)
				{
					if (l_node._this_class_name.back() <= ' ')
					{
						l_node._this_class_name.pop_back();
						continue;
					}
					break;
				}
			}

		}
		l_node._this_class_name.insert(0, parent_namespace_p);

		
		if (l_node._base_class_name.find(u8"archetype_base") != std::string::npos)
		{
			l_node._class_type = ClassType::_ChildOfComponentBase;
		}
		else if (l_node._base_class_name.find(u8"component_base") != std::string::npos)
		{
			l_node._class_type = ClassType::_ChildOfComponentBase;
		}


		std::pmr::vector<Vocabulary> l_stack{ framework::get_framework().get_memory_resource() };
		++out_token_iterator_p;
		do
		{
			switch (out_token_iterator_p->_vocabulary)
			{
			case Vocabulary::_LeftCurlyBracket:
				l_stack.push_back(out_token_iterator_p->_vocabulary);
				break;

			case Vocabulary::_RightCurlyBracket:
				l_stack.pop_back();
				break;

			case Vocabulary::_AnyDecl:
				{
					auto l_virtual_keyword_pos = out_token_iterator_p->_code.find(u8"virtual");
					if (l_virtual_keyword_pos == std::string::npos) // break if it does not contain "virtual"; and
					{
						break;
					}

					if (FE::algorithm::string::space_insensitive_contains(	out_token_iterator_p->_code.c_str() + l_virtual_keyword_pos,
																			out_token_iterator_p->_code.length() - l_virtual_keyword_pos,
																			u8"()"
																			) == false) // break if it does not contain "()"; and
					{
						break;
					}

					if (FE::algorithm::string::space_insensitive_contains(	out_token_iterator_p->_code.c_str() + l_virtual_keyword_pos,
																			out_token_iterator_p->_code.length() - l_virtual_keyword_pos,
																			u8"=0"
																			) == false) // break if it does not contain "=0".
					{
						break;
					}
					l_node._has_pure_virtual = true;
				}
				break;

			default:
				THROW_CPP_SYNTAX_ERROR(out_token_iterator_p == end_p, "FHT C++ Syntax Error C1075: the curly braces in the current header file are not closed or properly organized; reached the end of the token stream while parsing a struct declaration.");
				break;
			}
			++out_token_iterator_p;
		} 
		while (l_stack.size() > 0);
		return l_node;
	}

	_FE_NODISCARD_ struct_node build_struct_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_Struct);
		struct_node l_node =
		{
			._identifier{ out_token_iterator_p->_code, framework::get_framework().get_memory_resource() }
		};

		{
			{	// trim 'struct'
				constexpr auto l_struct = u8"struct";
				auto l_struct_keyword_len = l_node._identifier.find(l_struct);
				FE_ASSERT(l_struct_keyword_len != std::string::npos);

				l_struct_keyword_len += FE::algorithm::string::length(l_struct);
				l_node._identifier.erase(0, l_struct_keyword_len);
			}

			auto l_attr_pos = l_node._identifier.rfind(u8']');
			if (l_attr_pos != std::string::npos) // has attributes
			{
				l_node._identifier.erase(0, l_attr_pos + 1); // remove the attributes
			}

			{	// trim spaces at front
				var::uint64 l_space_length = 0;
				for (auto character : l_node._identifier)
				{
					if ((character <= ' ') == false)
					{
						break;
					}
					++l_space_length;
				}
				l_node._identifier.erase(0, l_space_length);
			}

			{
				auto l_struct_extension = l_node._identifier.find(':');
				if (l_struct_extension != std::string::npos)
				{
					l_node._identifier.erase(l_struct_extension, l_node._identifier.length() - l_struct_extension);
				}
			}

			while (l_node._identifier.back() <= ' ')
			{
				l_node._identifier.pop_back();
			}

			{	// count class identifier length
				var::uint64 l_name_length = 0;
				for (auto character = l_node._identifier.rbegin(); character != l_node._identifier.rend(); ++character)
				{
					if (*character <= ' ')
					{
						++l_name_length;
						break;
					}
					++l_name_length;
				}

				if (l_name_length != l_node._identifier.length())
				{
					l_node._identifier.erase(0, (l_node._identifier.length() - l_name_length) + 1);
				}

			}
			l_node._identifier.insert(0, parent_namespace_p);
		}


		std::pmr::vector<Vocabulary> l_stack{ framework::get_framework().get_memory_resource() };
		++out_token_iterator_p;
		do
		{
			switch (out_token_iterator_p->_vocabulary)
			{
			case Vocabulary::_LeftCurlyBracket:
				l_stack.push_back(out_token_iterator_p->_vocabulary);
				break;
				
			case Vocabulary::_RightCurlyBracket:
				l_stack.pop_back();
				break;

			default:
				THROW_CPP_SYNTAX_ERROR(out_token_iterator_p == end_p, "FHT C++ Syntax Error C1075: the curly braces in the current header file are not closed or properly organized; reached the end of the token stream while parsing a struct declaration.");
				break;
			}
			++out_token_iterator_p;
		} 
		while (l_stack.size() > 0);
		return l_node;
	}

	_FE_NODISCARD_ enum_struct_node build_enum_struct_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		FE_ASSERT(out_token_iterator_p->_vocabulary == Vocabulary::_EnumStruct);
		enum_struct_node l_node =
		{
			._target_enum_struct_name{ out_token_iterator_p->_code, framework::get_framework().get_memory_resource() },
			._enum_struct_fields{ framework::get_framework().get_memory_resource() }
		};

		{	// trim 'enum struct'
			constexpr auto l_struct = u8"struct";
			auto l_enum_end_pos = l_node._target_enum_struct_name.find(l_struct);
			FE_ASSERT(l_enum_end_pos != std::string::npos);

			FE::uint64 l_enum_struct_len = l_enum_end_pos + FE::algorithm::string::length(l_struct);
			l_node._target_enum_struct_name.erase(0, l_enum_struct_len);
		}

		auto l_attr_pos = l_node._target_enum_struct_name.rfind(u8']');
		if (l_attr_pos != std::string::npos) // has attributes
		{
			l_node._target_enum_struct_name.erase(0, l_attr_pos + 1); // remove the attributes
		}

		{	// trim spaces at front
			var::uint64 l_space_length = 0;
			for (auto character : l_node._target_enum_struct_name)
			{
				if ((character <= ' ') == false)
				{
					break;
				}
				++l_space_length;
			}
			l_node._target_enum_struct_name.erase(0, l_space_length);
		}

		{	// trim enum struct extension
			auto l_enum_struct_extension = l_node._target_enum_struct_name.find(':');
			if (l_enum_struct_extension != std::string::npos)
			{
				l_node._target_enum_struct_name.erase(l_enum_struct_extension, l_node._target_enum_struct_name.length() - l_enum_struct_extension);
			}
		}
		
		while (l_node._target_enum_struct_name.back() <= ' ')
		{
			l_node._target_enum_struct_name.pop_back();
		}
		
		{	// count enum struct identifier length
			var::uint64 l_name_length = 0;
			for (auto character = l_node._target_enum_struct_name.rbegin(); character != l_node._target_enum_struct_name.rend(); ++character)
			{
				if (*character <= ' ')
				{
					++l_name_length;
					break;
				}
				++l_name_length;
			}

			if (l_name_length != l_node._target_enum_struct_name.length())
			{
				l_node._target_enum_struct_name.erase(0, (l_node._target_enum_struct_name.length()-l_name_length)+1);
			}
			
		}
		l_node._target_enum_struct_name.insert(0, parent_namespace_p);


		while (out_token_iterator_p != end_p)
		{
			if (out_token_iterator_p->_vocabulary == Vocabulary::_RightCurlyBracket)
			{
				++out_token_iterator_p;
				return l_node;
			}

			if (out_token_iterator_p->_vocabulary == Vocabulary::_EnumStructField)
			{
				l_node._enum_struct_fields.emplace_back(out_token_iterator_p->_code);
			}
			++out_token_iterator_p;
		}

		return l_node;
	}

	_FE_NODISCARD_ system_node build_ecs_system_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		system_node l_node;
		while (out_token_iterator_p != end_p)
		{
			switch (out_token_iterator_p->_vocabulary)
			{
			case Vocabulary::_FrogmanEngineSystemMacro:
				_FE_FALLTHROUGH_;
			case Vocabulary::_LeftParen:
				_FE_FALLTHROUGH_;
			case Vocabulary::_Comma:
				++out_token_iterator_p;
				break;

			case Vocabulary::_RightParen:
				++out_token_iterator_p;
				goto ExitLoop;

			case Vocabulary::_FrogmanEngineSystemArgSysCallPhase:
				l_node._syscall_phase = out_token_iterator_p->_code;
				++out_token_iterator_p;
				break;

			case Vocabulary::_FrogmanEngineSystemArgTargetComponentType:
				l_node._target_component_type = out_token_iterator_p->_code;
				++out_token_iterator_p;
				break;

			case Vocabulary::_FrogmanEngineSystemArgWorldTagEnumType:
				l_node._world_tag_enum_type = out_token_iterator_p->_code;
				++out_token_iterator_p;
				break;

			default:
				THROW_CPP_SYNTAX_ERROR(true, "Frogman Engine C++ Reflection Syntax Error: the FE_SYSTEM() macro argument order is incorrect, or is ill-formed.");
				break;
			}
		}
ExitLoop:
		THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_code.starts_with(u8"void") == false, "Frogman Engine C++ Reflection Syntax Error: this function cannot be qualified as the Frogman Engine ECS system method; the function signature requirement is: void(::FE::component_base* const).");
		THROW_CPP_SYNTAX_ERROR(out_token_iterator_p->_vocabulary != Vocabulary::_AnyDecl, "Frogman Engine C++ Reflection Syntax Error: this function cannot be qualified as the Frogman Engine ECS system method; the function signature requirement is: void(::FE::component_base* const).");
		l_node._sysname = out_token_iterator_p->_code;

		constexpr auto l_void_keyword = u8"void";
		auto l_void_pos = l_node._sysname.find(l_void_keyword);
		FE_ASSERT(l_void_pos != identifier::npos);
		l_node._sysname.erase(0, l_void_pos + FE::algorithm::string::length(l_void_keyword));

		THROW_CPP_SYNTAX_ERROR(FE::algorithm::string::space_insensitive_contains(l_node._sysname.c_str(), l_node._sysname.length(), u8"component_base*const") == false, "Frogman Engine C++ Reflection Syntax Error: this function cannot be qualified as the Frogman Engine ECS system method; the function signature requirement is: void(::FE::component_base* const).");

		for (auto i = 0; i < l_node._sysname.length(); ++i)
		{
			if (l_node._sysname[0] == ' ')
			{
				l_node._sysname.erase(0, 1);
				continue;
			}
			break;
		}

		auto l_end_of_fn_name_pos = l_node._sysname.find('(');
		THROW_CPP_SYNTAX_ERROR(l_end_of_fn_name_pos == identifier::npos, "Frogman Engine C++ Reflection Syntax Error: this function cannot be qualified as the Frogman Engine ECS system method; the function signature requirement is: void(::FE::component_base* const).");

		l_node._sysname.erase(l_end_of_fn_name_pos, l_node._sysname.length() - l_end_of_fn_name_pos);
		l_node._sysname.insert(0, parent_namespace_p);
		return l_node;
	}
}