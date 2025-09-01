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
#define _CRT_SECURE_NO_WARNINGS
#include "header_tool_engine.hpp"

// std::find
#include <algorithm>
#include <cstdlib>
// std::ofstream
#include <fstream>




_FE_NODISCARD_ header_tool_engine::reflection_metadata header_tool_engine::__generate_reflection_metadata(const header_file_root& tree_p) noexcept
{
	reflection_metadata l_reflection_metadata;
	l_reflection_metadata._class_and_struct_identifiers = std::pmr::vector<std::pmr::wstring>(get_memory_resource());
	l_reflection_metadata._header_file_path = std::pmr::wstring(tree_p._path_to_the_header_file.data(), get_memory_resource());

	for (const class_node& node : tree_p._classes)
	{
		if (node._class_reflection_macro == nullptr)
		{
			continue;
		}

		__output_class_metadata(l_reflection_metadata, node);
	}

	for (const struct_node& node : tree_p._structs)
	{
		if (node._struct_reflection_macro == nullptr)
		{
			continue;
		}

		__output_struct_metadata(l_reflection_metadata, node);
	}

	for (const enum_struct_node& node : tree_p._enum_structs)
	{
		FE_ASSERT(node._target_enum_struct_name.empty() != true, "Assertion failed: the target enum struct name cannot be null.");

		__output_enum_struct_metadata(l_reflection_metadata, node);
	}

	for (const namespace_node& node : tree_p._namespaces)
	{
		__output_namespace_metadata_recursive(l_reflection_metadata, node);
	}

	return l_reflection_metadata;
}


void header_tool_engine::__output_namespace_metadata_recursive(reflection_metadata& out_return_p, const namespace_node& node_p) noexcept
{
	for (const class_node& node : node_p._classes)
	{
		if (node._class_reflection_macro == nullptr)
		{
			continue;
		}
		
		__output_class_metadata(out_return_p, node);
	}

	for (const struct_node& node : node_p._structs)
	{
		if (node._struct_reflection_macro == nullptr)
		{
			continue;
		}

		__output_struct_metadata(out_return_p, node);
	}

	for (const enum_struct_node& node : node_p._enum_structs)
	{
		FE_ASSERT(node._target_enum_struct_name.empty() != true, "Assertion failed: the target enum struct name cannot be null.");

		__output_enum_struct_metadata(out_return_p, node);
	}

	if (node_p._nested_namespaces != nullptr)
	{
		for (const namespace_node& node : *(node_p._nested_namespaces))
		{
			__output_namespace_metadata_recursive(out_return_p, node);
		}
	}
}


void header_tool_engine::__output_class_metadata(reflection_metadata& out_return_p, const class_node& node_p) noexcept
{
	std::pmr::wstring l_identifier(get_memory_resource());
	l_identifier.resize( node_p._class_reflection_macro->_target_class_name.length() + 1 );

	std::mbstowcs( l_identifier.data(), reinterpret_cast<const char*>(node_p._class_reflection_macro->_target_class_name.data()), node_p._class_reflection_macro->_target_class_name.length() );
	l_identifier = l_identifier.c_str();
	out_return_p._class_and_struct_identifiers.push_back( std::move(l_identifier) );
}


void header_tool_engine::__output_struct_metadata(reflection_metadata& out_return_p, const struct_node& node_p) noexcept
{
	std::pmr::wstring l_identifier(get_memory_resource());
	l_identifier.resize( node_p._struct_reflection_macro->_target_struct_name.length() + 1 );

	std::mbstowcs( l_identifier.data(), reinterpret_cast<const char*>(node_p._struct_reflection_macro->_target_struct_name.data()), node_p._struct_reflection_macro->_target_struct_name.length() );
	l_identifier = l_identifier.c_str();
	out_return_p._class_and_struct_identifiers.push_back( std::move(l_identifier) );
}

void header_tool_engine::__output_enum_struct_metadata(reflection_metadata& out_return_p, const enum_struct_node& node_p) noexcept
{
	std::pmr::wstring l_identifier(get_memory_resource());
	l_identifier.resize( node_p._target_enum_struct_name.length() + 1 );

	std::mbstowcs( l_identifier.data(), reinterpret_cast<const char*>(node_p._target_enum_struct_name.data()), node_p._target_enum_struct_name.length() );
	l_identifier = l_identifier.c_str();

	out_return_p._enum_structs.emplace_back( std::pmr::vector<std::pmr::wstring>{ get_memory_resource() } );
	out_return_p._enum_structs.back().push_back( std::move(l_identifier) );

	for (const identifier& enum_value : node_p._enum_struct_fields)
	{
		std::pmr::wstring l_enum_value(get_memory_resource());
		l_enum_value.resize( enum_value.length() + 1 );

		std::mbstowcs( l_enum_value.data(), reinterpret_cast<const char*>(enum_value.data()), enum_value.length() );
		l_enum_value = l_enum_value.c_str();
		out_return_p._enum_structs.back().push_back( std::move(l_enum_value) );
	}
}




void header_tool_engine::__generate_reflection_code(const reflection_metadata_set_t& metadata_set_p) noexcept
{
	std::pmr::wstring l_generated_code(get_memory_resource());
	l_generated_code.reserve(10240);
	l_generated_code += L"// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. \n#include <FE/framework/reflection/private/load_reflection_data.h> \n#include <FE/framework/reflection.hpp> \n#include <FE/framework/framework.hpp> \n";
	for (const reflection_metadata& header_file : metadata_set_p)
	{
		l_generated_code += L"#include <";
		l_generated_code += header_file._header_file_path;
		l_generated_code += L">\n";
	}
	l_generated_code += L"\nvoid load_reflection_data()\n{\n";

	constexpr FE::wchar* l_class_reflexpr_frame = L"    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(void*), typename ::FE::function<void(void*)>::arguments_type> >(\"";
	for (const reflection_metadata& header_file : metadata_set_p)
	{
		for (const std::pmr::wstring& identifier : header_file._class_and_struct_identifiers)
		{
			l_generated_code += l_class_reflexpr_frame;
			std::pmr::wstring l_mapped_constructor_string(L"FHT Gen ", get_memory_resource());
			l_mapped_constructor_string += identifier;
			l_mapped_constructor_string += L"()";
			l_generated_code += l_mapped_constructor_string;

			l_generated_code += L"\", &::FE::framework::reflection::construct_object<";
			l_generated_code += identifier;
			l_generated_code += L">); \n";
		}

		constexpr FE::wchar* l_enum_reflexpr_frame = L"    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ";
		for (const std::pmr::vector<std::pmr::wstring>& enum_struct : header_file._enum_structs)
		{
			// The first element of the enum_struct vector is the name of the enum struct, and the rest are the enum values.
			const std::pmr::wstring& identifier = enum_struct.front();
			l_generated_code += l_enum_reflexpr_frame;
			l_generated_code += identifier;
			l_generated_code += L" >(\"";
			l_generated_code += identifier;
			l_generated_code += L"\",\n    {\n";

			for (auto it = std::next( enum_struct.begin(), 1 ); it < enum_struct.end(); ++it)
			{
				l_generated_code += L"        { ";
				l_generated_code += identifier;
				l_generated_code += L"::";
				l_generated_code += *it;
				l_generated_code += L", \"";
				l_generated_code += *it;
				l_generated_code += L"\" },\n";
			}
			l_generated_code.erase( l_generated_code.length() - 2, 1 ); // Remove last comma
			l_generated_code += L"    });\n";
		}
	}
	l_generated_code += L"}\n";

	std::pmr::wstring l_path_to_generated(get_memory_resource());
	FE::size l_path_length = FE::algorithm::string::length( m_header_tool_options.get_path_to_project() );
	FE_EXIT_IF(l_path_length == 0, FrogmanEngineHeaderToolError::_FatalCmdInputError_InvalidPathToCMakeProject, "Frogman Engine Header Tool: the directory value for -path-to-project= is not given or specified to this header tool program. \nThe string length of the path to the desired folder to create the generated.cpp file is ZERO.");
	
	l_path_to_generated.resize(l_path_length + 1);
	std::mbstowcs(l_path_to_generated.data(), m_header_tool_options.get_path_to_project(), l_path_length);
	l_path_to_generated = l_path_to_generated.c_str();
	l_path_to_generated += L"\\generated.cpp";
	
	std::wofstream l_generated_file;
	l_generated_file.open(l_path_to_generated);
	FE_EXIT_IF(l_generated_file.is_open() == false, FrogmanEngineHeaderToolError::_FatalCmdInputError_InvalidPathToCMakeProject, "Frogman Engine Header Tool: failed to generate the generated.cpp file.");
	l_generated_file << l_generated_code;
	l_generated_file.close();
} 
