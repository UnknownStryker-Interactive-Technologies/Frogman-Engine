#ifndef _FE_HEADER_TOOL_PARSER_HPP_
#define _FE_HEADER_TOOL_PARSER_HPP_
/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include "context.hpp"




namespace FHT::parser
{
	_FE_NODISCARD_ header_file_root build_reflextree(const directory_t& file_path_p, const std::pmr::list<token>& token_list_p);

	_FE_NODISCARD_ namespace_node build_namespace_node_recursive(const identifier& parent_namespace_p,
																 typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p,
											  					 context_stack_t& context_stack_p);

	_FE_NODISCARD_ class_node build_class_node(const identifier& parent_namespace_p, 
																  typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);

	_FE_NODISCARD_ struct_node build_struct_node(const identifier& parent_namespace_p, 
		                                                            typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);

	_FE_NODISCARD_ enum_struct_node build_enum_struct_node(const identifier& parent_namespace_p, 
		                                                   typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);

	_FE_NODISCARD_ system_node build_ecs_system_node(const identifier& parent_namespace_p, 
		                                                          typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);
}
#endif