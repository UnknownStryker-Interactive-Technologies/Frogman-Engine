#ifndef _FE_HEADER_TOOL_REFLEXTREE_HPP_
#define _FE_HEADER_TOOL_REFLEXTREE_HPP_
/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

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

// std::unique_ptr
#include <memory>

// std::string_view
#include <string_view>
#include <optional>
#include <vector>




enum struct ClassType : FE::int32
{
	_None = 0,
	_ChildOfArchetypeBase = 1,
	_ChildOfComponentBase = 2,
	//_ChildOfSystemBase = 3,
	_ChildOfCppClass = 4
};


using identifier = std::pmr::basic_string<var::UTF8>;


struct class_node
{
	ClassType _class_type = ClassType::_None;
	var::boolean _has_pure_virtual = false;
	identifier _this_class_name;
	identifier _base_class_name;
};


struct struct_node
{
	identifier _identifier;
};


struct enum_struct_node
{
	identifier _target_enum_struct_name;
	std::pmr::vector<identifier> _enum_struct_fields;
};

struct system_node
{
	identifier _syscall_phase;
	identifier _target_component_type;
	identifier _world_tag_enum_type;

	identifier _sysname;
};

struct namespace_node
{
	identifier _target_namespace_name;
	std::pmr::vector<namespace_node> _nested_namespaces;
	std::pmr::vector<class_node> _classes;
	std::pmr::vector<struct_node> _structs;
	std::pmr::vector<enum_struct_node> _enum_structs;
	std::pmr::vector<system_node> _system_fptrs;
};


struct header_file_root
{
	std::wstring_view _path_to_the_header_file;
	std::pmr::vector<namespace_node> _namespaces;
	std::pmr::vector<class_node> _classes;
	std::pmr::vector<struct_node> _structs;
	std::pmr::vector<enum_struct_node> _enum_structs;
	std::pmr::vector<system_node> _system_fptrs;
};


#endif