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




using identifier_node = std::basic_string<var::UTF8>;
using identifier_t = identifier_node;


struct frogman_engine_property_macro_node
{
	identifier_node _target_property_name;
};


struct frogman_engine_method_macro_node
{
	identifier_node _return_type;
	identifier_node _method_name;
	std::pmr::vector<identifier_node> _parameter_types;
	std::pmr::vector<identifier_node> _specifiers;
};


struct frogman_engine_static_method_macro_node
{
	identifier_node _return_type;
	identifier_node _method_name;
	std::pmr::vector<identifier_node> _parameter_types;
	std::pmr::vector<identifier_node> _specifiers;
};


struct frogman_engine_class_has_a_base_macro_node
{
	identifier_node _target_base_class_name;
};


struct frogman_engine_class_macro_node
{
	identifier_node _target_class_name;
	std::pmr::vector<frogman_engine_property_macro_node> _property_reflection_macros;
	std::pmr::vector<frogman_engine_method_macro_node> _method_reflection_macros;
	std::pmr::vector<frogman_engine_static_method_macro_node> _static_method_reflection_macros;
};


struct frogman_engine_struct_macro_node
{
	identifier_node _target_struct_name;
	std::pmr::vector<frogman_engine_property_macro_node> _property_reflection_macros;
};


struct class_node
{
	std::unique_ptr<frogman_engine_class_has_a_base_macro_node> _base_class_reflection_macro;
	std::unique_ptr<frogman_engine_class_macro_node> _class_reflection_macro;
};


struct struct_node
{
	std::unique_ptr<frogman_engine_struct_macro_node> _struct_reflection_macro;
};


struct namespace_node
{
	identifier_node _namespace_name;
	std::unique_ptr< std::pmr::vector<namespace_node> > _nested_namespaces;
	std::pmr::vector<class_node> _classes;
	std::pmr::vector<struct_node> _structs;
};


struct header_file_root
{
	std::wstring_view _path_to_the_header_file;
	std::pmr::vector<namespace_node> _namespaces;
	std::pmr::vector<class_node> _classes;
	std::pmr::vector<struct_node> _structs;
};


#endif