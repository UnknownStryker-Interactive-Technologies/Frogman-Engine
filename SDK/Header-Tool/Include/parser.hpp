#ifndef _FE_HEADER_TOOL_PARSER_HPP_
#define _FE_HEADER_TOOL_PARSER_HPP_
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
#include <FE/prerequisites.hxx>




namespace FHT::parser
{
	void skip_code_block(typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p) noexcept;

	bool is_forward_declaration(typename std::pmr::list<token>::const_iterator& out_token_iterator_p);

	_FE_NODISCARD_ header_file_root try_build_reflection_tree(const directory_t& file_path_p, const std::pmr::list<token>& token_list_p);

	_FE_NODISCARD_ std::optional<namespace_node> try_build_namespace_node_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);

	_FE_NODISCARD_ std::optional<class_node> try_build_class_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);

	_FE_NODISCARD_ std::optional<struct_node> try_build_struct_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);

	_FE_NODISCARD_ std::optional<enum_struct_node> try_build_enum_struct_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);

	_FE_NODISCARD_ std::optional<system_node> try_build_c_style_system_function_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);

	void skip_template_args(typename std::pmr::list<token>::const_iterator& iterator_p);
}
#endif