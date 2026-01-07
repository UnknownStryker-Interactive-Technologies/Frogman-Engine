#ifndef _FE_HEADER_TOOL_REFLECTION_HPP_
#define _FE_HEADER_TOOL_REFLECTION_HPP_
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
#include <FE/concurrent_vector.hxx>

#include "reflection_tree.hpp"
#include "token.hpp"

#include <string>
#include <vector>




namespace FHT::reflexcode_generator
{
	struct metadata
	{
		struct system_info
		{
			std::pmr::wstring _world_category;
			std::pmr::wstring _system_name;
			std::pmr::wstring _system_target;
			std::pmr::wstring _system_call_phase;
		};

		directory_t _header_file_path;
		std::pmr::vector<std::pmr::wstring> _archetype_base_children;
		std::pmr::vector<std::pmr::wstring> _component_base_children;
		//std::pmr::vector<std::pmr::wstring> _system_base_children; 
		std::pmr::vector<std::pmr::wstring> _class_and_structs;
		std::pmr::vector< std::pmr::vector<std::pmr::wstring> > _enum_structs;
		std::pmr::vector<system_info> _system_fptrs;
	};
	using metadata_set_t = FE::concurrent_vector<metadata>;

	_FE_NODISCARD_ metadata generate_metadata(const header_file_root& tree_p) noexcept;
	void output_namespace_metadata_recursive(metadata& out_return_p, const namespace_node& node_p) noexcept;
	void output_class_metadata(metadata& out_return_p, const class_node& node_p) noexcept;
	void output_struct_metadata(metadata& out_return_p, const struct_node& node_p) noexcept;
	void output_enum_struct_metadata(metadata& out_return_p, const enum_struct_node& node_p) noexcept;

	void generate_reflection_code(const metadata_set_t& metadata_set_p) noexcept;
} 
#endif