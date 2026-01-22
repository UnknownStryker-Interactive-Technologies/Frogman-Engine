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
	_FE_NODISCARD_ header_file_root try_build_reflection_tree(const directory_t& file_path_p, const std::pmr::list<token>& token_list_p)
	{
		// returns an optional error message object.
		std::optional<FE::ASCII*> l_error_message = FHT::symbol_counter::validate_parentheses(token_list_p);
		if (l_error_message != std::nullopt)
		{
			throw FE::pair<FrogmanEngineHeaderToolError, FE::ASCII*>{FrogmanEngineHeaderToolError::_InputError_IncorrectCppSyntax, * l_error_message};
		}

		{
			FHT::symbol_counter::symbol_count l_total_nums = FHT::symbol_counter::try_count_all_symbols(token_list_p.begin(), token_list_p.cend());

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
		l_root._namespaces = std::pmr::vector< std::optional<namespace_node> >(framework::get_framework().get_memory_resource());
		l_root._classes = std::pmr::vector< std::optional<class_node> >(framework::get_framework().get_memory_resource());
		l_root._structs = std::pmr::vector< std::optional<struct_node> >(framework::get_framework().get_memory_resource());
		l_root._enum_structs = std::pmr::vector< std::optional<enum_struct_node> >(framework::get_framework().get_memory_resource());

		for (auto iterator = token_list_p.begin(); iterator != token_list_p.end(); ++iterator)
		{
			switch (iterator->_vocabulary)
			{
			
			}
		}

		return l_root;
	}

	_FE_NODISCARD_ std::optional<namespace_node> try_build_namespace_node_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		namespace_node l_node;

		l_node._target_namespace_name = file_buffer_t(parent_namespace_p, framework::get_framework().get_memory_resource());
		l_node._classes = std::pmr::vector< std::optional<class_node> >(framework::get_framework().get_memory_resource());
		l_node._structs = std::pmr::vector< std::optional<struct_node> >(framework::get_framework().get_memory_resource());
		l_node._enum_structs = std::pmr::vector< std::optional<enum_struct_node> >(framework::get_framework().get_memory_resource());

		// Allocate the necessary children nodes.
		FHT::symbol_counter::symbol_count l_nums = FHT::symbol_counter::try_count_the_current_scope_level_symbols(out_token_iterator_p, end_p);
		l_node._nested_namespaces = std::pmr::vector< std::optional<namespace_node> >(framework::get_framework().get_memory_resource());
		l_node._classes.reserve(l_nums._classes);
		l_node._structs.reserve(l_nums._structs);
		l_node._enum_structs.reserve(l_nums._enums);


		return l_node;
	}

	_FE_NODISCARD_ std::optional<class_node> try_build_class_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		class_node l_node;
		(parent_namespace_p);
		(out_token_iterator_p);
		(end_p);

		return l_node;
	}

	_FE_NODISCARD_ std::optional<struct_node> try_build_struct_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		struct_node l_node;
		(parent_namespace_p);
		(out_token_iterator_p);
		(end_p);
		
		return l_node;
	}

	_FE_NODISCARD_ std::optional<enum_struct_node> try_build_enum_struct_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		enum_struct_node l_node;
		(parent_namespace_p);
		(out_token_iterator_p);
		(end_p);

		return l_node;
	}

	_FE_NODISCARD_ std::optional<system_node> try_build_c_style_system_function_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p)
	{
		system_node l_node;
		(parent_namespace_p);
		(out_token_iterator_p);
		(end_p);

		return l_node;
	}
}