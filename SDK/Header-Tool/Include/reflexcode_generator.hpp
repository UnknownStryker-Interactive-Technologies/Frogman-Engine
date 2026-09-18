#ifndef _FE_HEADER_TOOL_REFLEXCODE_GENERATOR_HPP_
#define _FE_HEADER_TOOL_REFLEXCODE_GENERATOR_HPP_
/*
Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
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
#include <FE/concurrent_vector.hxx>

#include "reflextree.hpp"
#include "token.hpp"

#include <string>
#include <vector>
#include <unordered_map>




namespace FHT::reflexcode_generator
{
	struct metadata
	{
		struct system_info
		{
			std::pmr::wstring _system_call_phase;
			std::pmr::wstring _world_tag_enum;
		};

		struct class_info
		{
			DefaultConstructorState _default_constructor_state = DefaultConstructorState::_ImplicitPublic;
			var::boolean _has_constructor_variants = false;
			var::boolean _is_destructor_deleted_or_not_public = false;
			var::boolean _has_marker_fht_generated = false;
		};

		struct struct_info
		{
			DefaultConstructorState _default_constructor_state = DefaultConstructorState::_ImplicitPublic;
			var::boolean _has_constructor_variants = false;
			var::boolean _is_destructor_deleted_or_not_public = false;
			var::boolean _has_marker_fht_generated = false;
		};

		directory_t _header_file_path;
		std::pmr::unordered_map<std::pmr::wstring, class_info> _classes;
		std::pmr::unordered_map<std::pmr::wstring, struct_info> _structs;
		std::pmr::unordered_map<std::pmr::wstring, std::pmr::vector<std::pmr::wstring> > _enum_structs;
		std::pmr::unordered_map<std::pmr::wstring, system_info> _system_fptrs;
	};
	using metadata_set_t = FE::concurrent_array<metadata>;

	_FE_NODISCARD_ metadata generate_metadata(const header_file_root& tree_p) noexcept;
	void output_namespace_metadata_recursive(metadata& out_return_p, const namespace_node& node_p) noexcept;
	void output_class_metadata(metadata& out_return_p, const class_node& node_p) noexcept;
	void output_struct_metadata(metadata& out_return_p, const struct_node& node_p) noexcept;
	void output_enum_struct_metadata(metadata& out_return_p, const enum_struct_node& node_p) noexcept;

	void generate_reflexcode(const metadata_set_t& metadata_set_p) noexcept;
} 
#endif