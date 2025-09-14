#ifndef _FE_HEADER_TOOL_REFLECTION_TREE_HPP_
#define _FE_HEADER_TOOL_REFLECTION_TREE_HPP_
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
#include <FE/prerequisites.h>

// std::unique_ptr
#include <memory>

// std::string_view
#include <string_view>

#include <vector>




enum struct ClassType : FE::int32
{
	_None = 0,
	_ChildOfArchetypeBase = 1,
	_ChildOfComponentBase = 2,
	_ChildOfSystemBase = 3,
	_ChildOfCppClass = 4
};


using identifier = std::pmr::basic_string<var::UTF8>;


struct frogman_engine_struct_macro_node
{
	identifier _target_struct_name;
	std::pmr::vector<identifier> _property_reflection_macros;
};


struct class_node
{
	ClassType _class_type = ClassType::_None;
	identifier _this_class_name;
	identifier _base_class_name;
};


struct struct_node
{
	std::unique_ptr<frogman_engine_struct_macro_node> _struct_reflection_macro;
};


struct enum_struct_node
{
	identifier _target_enum_struct_name;
	std::pmr::vector<identifier> _enum_struct_fields;
};


struct namespace_node
{
	identifier _target_namespace_name;
	std::unique_ptr< std::pmr::vector<namespace_node> > _nested_namespaces;
	std::pmr::vector<class_node> _classes;
	std::pmr::vector<struct_node> _structs;
	std::pmr::vector<enum_struct_node> _enum_structs;
	std::pmr::vector<identifier> _c_style_systems;
};


struct header_file_root
{
	std::wstring_view _path_to_the_header_file;
	std::pmr::vector<namespace_node> _namespaces;
	std::pmr::vector<class_node> _classes;
	std::pmr::vector<struct_node> _structs;
	std::pmr::vector<enum_struct_node> _enum_structs;
	std::pmr::vector<identifier> _c_style_systems;
};


#endif