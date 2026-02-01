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
				l_context_stack.push_back(Context::_FrogmanEngineSystemMacro);
				l_root._system_fptrs.emplace_back(build_ecs_system_node(u8"::", iterator, token_list_p.end(), l_context_stack));
				break;


			case Vocabulary::_BeginNamespace:
				_FE_FALLTHROUGH_;
			case Vocabulary::_Namespace:
				l_root._namespaces.emplace_back(build_namespace_node_recursive(u8"::", iterator, token_list_p.end(), l_context_stack));
				break;

			case Vocabulary::_Class:
				if (l_context_stack.back() == Context::_Class)
				{
					l_root._classes.emplace_back(build_class_node_mutually_recursive(u8"::", iterator, token_list_p.end(), l_context_stack));
				}
				break;

			case Vocabulary::_Struct:
				if (l_context_stack.back() == Context::_Struct)
				{
					l_root._structs.emplace_back(build_struct_node_mutually_recursive(u8"::", iterator, token_list_p.end(), l_context_stack));
				}
				break;

			case Vocabulary::_EnumStruct:
				if (l_context_stack.back() == Context::_EnumStruct)
				{
					l_root._enum_structs.emplace_back(build_enum_struct_node(u8"::", iterator, token_list_p.end(), l_context_stack));
				}
				break;


			default:
				if (l_context_stack.back() != FHT::Context::_Global)
				{
					l_context_stack.pop_back();
				}
				break;
			}
		}

		return l_root;
	}

	_FE_NODISCARD_ namespace_node build_namespace_node_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p, context_stack_t& context_stack_p)
	{
		namespace_node l_node = 
		{
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


		switch (out_token_iterator_p->_vocabulary)
		{
		case Vocabulary::_BeginNamespace:
			break;
		case Vocabulary::_Namespace:
			break;

		_FE_NODEFAULT_;
		}


		while (out_token_iterator_p != end_p)
		{
			switch (out_token_iterator_p->_vocabulary)
			{
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
				context_stack_p.push_back(Context::_FrogmanEngineSystemMacro);
				l_node._system_fptrs.emplace_back(build_ecs_system_node(parent_namespace_p, out_token_iterator_p, end_p, context_stack_p));
				break;


			case Vocabulary::_BeginNamespace:
				_FE_FALLTHROUGH_;
			case Vocabulary::_Namespace:
				l_node._nested_namespaces.emplace_back(build_namespace_node_recursive(parent_namespace_p, out_token_iterator_p, end_p, context_stack_p));
				break;

			case Vocabulary::_Class:
				if (context_stack_p.back() == Context::_Class)
				{
					l_node._classes.emplace_back(build_class_node_mutually_recursive(parent_namespace_p, out_token_iterator_p, end_p, context_stack_p));
				}
				break;

			case Vocabulary::_Struct:
				if (context_stack_p.back() == Context::_Struct)
				{
					l_node._structs.emplace_back(build_struct_node_mutually_recursive(parent_namespace_p, out_token_iterator_p, end_p, context_stack_p));
				}
				break;

			case Vocabulary::_EnumStruct:
				if (context_stack_p.back() == Context::_EnumStruct)
				{
					l_node._enum_structs.emplace_back(build_enum_struct_node(parent_namespace_p, out_token_iterator_p, end_p, context_stack_p));
				}
				break;


			default:
				if (context_stack_p.back() != FHT::Context::_Global)
				{
					context_stack_p.pop_back();
				}
				break;
			}

			++out_token_iterator_p;
		}

		return l_node;
	}

	_FE_NODISCARD_ class_node build_class_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p, context_stack_t& context_stack_p)
	{
		class_node l_node;
		(parent_namespace_p);
		(out_token_iterator_p);
		(end_p);
		(context_stack_p);

		return l_node;
	}

	_FE_NODISCARD_ struct_node build_struct_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p, context_stack_t& context_stack_p)
	{
		struct_node l_node;
		(parent_namespace_p);
		(out_token_iterator_p);
		(end_p);
		(context_stack_p);
		return l_node;
	}

	_FE_NODISCARD_ enum_struct_node build_enum_struct_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p, context_stack_t& context_stack_p)
	{
		enum_struct_node l_node;
		(parent_namespace_p);
		(out_token_iterator_p);
		(end_p);
		(context_stack_p);
		return l_node;
	}

	_FE_NODISCARD_ system_node build_ecs_system_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p, context_stack_t& context_stack_p)
	{
		system_node l_node;
		(parent_namespace_p);
		(out_token_iterator_p);
		(end_p);
		(context_stack_p);
		return l_node;
	}
}