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
	l_reflection_metadata._class_and_structs = std::pmr::vector<std::pmr::wstring>(get_memory_resource());
	l_reflection_metadata._header_file_path = std::pmr::wstring(tree_p._path_to_the_header_file.data(), get_memory_resource());

	for (const std::optional<class_node>& node : tree_p._classes)
	{
		if (node == std::nullopt)
		{
			continue;
		}
		__output_class_metadata(l_reflection_metadata, *node);
	}

	for (const std::optional<struct_node>& node : tree_p._structs)
	{
		if (node == std::nullopt)
		{
			continue;
		}
		__output_struct_metadata(l_reflection_metadata, *node);
	}

	for (const std::optional<enum_struct_node>& node : tree_p._enum_structs)
	{
		if (node == std::nullopt)
		{
			continue;
		}

		__output_enum_struct_metadata(l_reflection_metadata, *node);
	}

	for (const std::optional<namespace_node>& node : tree_p._namespaces)
	{
		if (node == std::nullopt)
		{
			continue;
		}

		__output_namespace_metadata_recursive(l_reflection_metadata, *node);
	}

	for (const std::optional<system_node>& c_style_system_function : tree_p._system_fptrs)
	{
		if (c_style_system_function == std::nullopt)
		{
			continue;
		}

		typename reflection_metadata::system_info l_system_node{ std::pmr::wstring(get_memory_resource()), std::pmr::wstring(get_memory_resource()), std::pmr::wstring(get_memory_resource()) };

		l_system_node._system_name.resize(c_style_system_function->_sysname.length());
		std::mbstowcs(l_system_node._system_name.data(), reinterpret_cast<const char*>(c_style_system_function->_sysname.data()), c_style_system_function->_sysname.length());

		l_system_node._system_target.resize(c_style_system_function->_systarget.length());
		std::mbstowcs(l_system_node._system_target.data(), reinterpret_cast<const char*>(c_style_system_function->_systarget.data()), c_style_system_function->_systarget.length());

		l_system_node._system_call_phase.resize(c_style_system_function->_syscall_phase.length());
		std::mbstowcs(l_system_node._system_call_phase.data(), reinterpret_cast<const char*>(c_style_system_function->_syscall_phase.data()), c_style_system_function->_syscall_phase.length());

		l_reflection_metadata._system_fptrs.push_back( std::move(l_system_node) );
	}

	return l_reflection_metadata;
}


void header_tool_engine::__output_namespace_metadata_recursive(reflection_metadata& out_return_p, const namespace_node& node_p) noexcept
{
	for (const std::optional<class_node>& node : node_p._classes)
	{	
		if (node == std::nullopt)
		{
			continue;
		}
		__output_class_metadata(out_return_p, *node);
	}

	for (const std::optional<struct_node>& node : node_p._structs)
	{
		if (node == std::nullopt)
		{
			continue;
		}
		__output_struct_metadata(out_return_p, *node);
	}

	for (const std::optional<enum_struct_node>& node : node_p._enum_structs)
	{
		if (node == std::nullopt)
		{
			continue;
		}
		__output_enum_struct_metadata(out_return_p, *node);
	}

	for (const std::optional<namespace_node>& node : node_p._nested_namespaces)
	{
		if (node == std::nullopt)
		{
			continue;
		}
		__output_namespace_metadata_recursive(out_return_p, *node);
	}

	for (const  std::optional<system_node>& c_style_system_function : node_p._system_fptrs)
	{
		if (c_style_system_function == std::nullopt)
		{
			continue;
		}

		typename reflection_metadata::system_info l_system_node{ std::pmr::wstring(get_memory_resource()), std::pmr::wstring(get_memory_resource()), std::pmr::wstring(get_memory_resource()) };

		l_system_node._system_name.resize(c_style_system_function->_sysname.length());
		std::mbstowcs(l_system_node._system_name.data(), reinterpret_cast<const char*>(c_style_system_function->_sysname.data()), c_style_system_function->_sysname.length());

		l_system_node._system_target.resize(c_style_system_function->_systarget.length());
		std::mbstowcs(l_system_node._system_target.data(), reinterpret_cast<const char*>(c_style_system_function->_systarget.data()), c_style_system_function->_systarget.length());

		l_system_node._system_call_phase.resize(c_style_system_function->_syscall_phase.length());
		std::mbstowcs(l_system_node._system_call_phase.data(), reinterpret_cast<const char*>(c_style_system_function->_syscall_phase.data()), c_style_system_function->_syscall_phase.length());

		out_return_p._system_fptrs.push_back(std::move(l_system_node));
	}
}


void header_tool_engine::__output_class_metadata(reflection_metadata& out_return_p, const class_node& node_p) noexcept
{
	std::pmr::wstring l_identifier(get_memory_resource());
	l_identifier.resize( node_p._this_class_name.length());

	std::mbstowcs( l_identifier.data(), reinterpret_cast<const char*>(node_p._this_class_name.data()), node_p._this_class_name.length() );

	switch (node_p._class_type)
	{
	case ClassType::_ChildOfArchetypeBase:
		out_return_p._archetype_base_children.push_back( std::move(l_identifier) );
		break;

	case ClassType::_ChildOfComponentBase:
		out_return_p._component_base_children.push_back( std::move(l_identifier) );
		break;

	//case ClassType::_ChildOfSystemBase:
	//	out_return_p._system_base_children.push_back( std::move(l_identifier) );
	//	break;

	case ClassType::_ChildOfCppClass:
		_FE_FALLTHROUGH_;
	case ClassType::_None:
		out_return_p._class_and_structs.push_back( std::move(l_identifier) );
		break;
	}
}


void header_tool_engine::__output_struct_metadata(reflection_metadata& out_return_p, const struct_node& node_p) noexcept
{
	std::pmr::wstring l_identifier(get_memory_resource());
	l_identifier.resize( node_p._target_struct_name.length());

	std::mbstowcs( l_identifier.data(), reinterpret_cast<const char*>(node_p._target_struct_name.data()), node_p._target_struct_name.length() );
	out_return_p._class_and_structs.push_back( std::move(l_identifier) );
}

void header_tool_engine::__output_enum_struct_metadata(reflection_metadata& out_return_p, const enum_struct_node& node_p) noexcept
{
	std::pmr::wstring l_identifier(get_memory_resource());
	l_identifier.resize( node_p._target_enum_struct_name.length());

	std::mbstowcs( l_identifier.data(), reinterpret_cast<const char*>(node_p._target_enum_struct_name.data()), node_p._target_enum_struct_name.length() );

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
	l_generated_code.reserve(1 * FE::one_MiB);
	l_generated_code += L"// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. \n#include <FE/framework/reflection/private/load_reflection_data.hxx> \n#include <FE/framework.hxx> \n";
	for (const reflection_metadata& header_file : metadata_set_p) // #include <> statements gereration
	{
		l_generated_code += L"#include <";
		l_generated_code += header_file._header_file_path;
		l_generated_code += L">\n";
	}


	l_generated_code += L"\n\n";
	std::pmr::wstring l_namespace_concat_replaced_with_underscores(get_memory_resource());
	for (const reflection_metadata& header_file : metadata_set_p)
	{
		for (const std::pmr::wstring& identifier : header_file._component_base_children)
		{
			l_namespace_concat_replaced_with_underscores = identifier; // The identifier may contain namespace concatenation, so we have to replace "::" with "_"
			for (std::pmr::wstring::size_type pos = l_namespace_concat_replaced_with_underscores.find(L"::"); pos != std::pmr::wstring::npos; pos = l_namespace_concat_replaced_with_underscores.find(L"::"))
			{
				l_namespace_concat_replaced_with_underscores.replace(pos, 2, L"_");
			}

			l_generated_code += L"void serialize_component";
			l_generated_code += l_namespace_concat_replaced_with_underscores;
			l_generated_code += L"(::std::pmr::string& out_buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept\n";
			l_generated_code += L"{\n";
			l_generated_code += L"    ::FE::framework::framework_base::get_framework().get_property_reflection().serialize(out_buffer_p, *::FE::polymorphic_cast<";
			l_generated_code += identifier;
			l_generated_code += L"* const>(component_p), version_p);\n";
			l_generated_code += L"}\n\n";

			l_generated_code += L"void deserialize_component";
			l_generated_code += l_namespace_concat_replaced_with_underscores;
			l_generated_code += L"(const ::std::pmr::string& buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept\n";
			l_generated_code += L"{\n";
			l_generated_code += L"    ::FE::framework::framework_base::get_framework().get_property_reflection().deserialize(buffer_p, *::FE::polymorphic_cast<";
			l_generated_code += identifier;
			l_generated_code += L"* const>(component_p), version_p);\n";
			l_generated_code += L"}\n\n";
		}
	}


	l_generated_code += L"\nvoid load_reflection_data()\n{\n"; // The void load_reflection_data() implementation generation
	//::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<FE::ECS, FE::entity<player>(FE::ASCII* const)> >("player", &::FE::framework::ECS::instanciate_entity<player>);
	//::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<FE::ECS, void(const FE::entity<player>&)> >("~player", &::FE::framework::ECS::destruct_entity<player>);
	constexpr FE::wchar* l_ECS_reflection_registry_frame = L"    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::framework::ECS, ";
	for (const reflection_metadata& header_file : metadata_set_p)
	{
		for (const std::pmr::wstring& identifier : header_file._archetype_base_children) // Archetypes
		{
			l_generated_code += l_ECS_reflection_registry_frame; // Archetype instantiator reflection
			l_generated_code += L"::FE::entity<::FE::archetype_base>(::FE::ASCII* const, const ::FE::framework::initializer&)> >(\"";
			l_generated_code += identifier;
			l_generated_code += L"\", &::FE::framework::ECS::instanciate_entity_from_initializer<";
			l_generated_code += identifier;
			l_generated_code += L">);\n";

			l_generated_code += l_ECS_reflection_registry_frame; // Archetype destructor reflection
			l_generated_code += L"void(::FE::entity<::FE::archetype_base>)> >(\"~";
			l_generated_code += identifier;
			l_generated_code += L"\", &::FE::framework::ECS::destruct_entity);\n\n";
		}


		for (const std::pmr::wstring& identifier : header_file._component_base_children)
		{
			l_generated_code += l_ECS_reflection_registry_frame; // Component adder reflection
			l_generated_code += L"::FE::component_view<::FE::component_base>(::FE::entity<::FE::archetype_base>)> >(\"";
			l_generated_code += identifier;
			l_generated_code += L"\", &::FE::framework::ECS::instanciate_component<";
			l_generated_code += identifier;
			l_generated_code += L">);\n";

			l_generated_code += l_ECS_reflection_registry_frame; // Component remover reflection
			l_generated_code += L"void(::FE::entity<::FE::archetype_base>)> >(\"~";
			l_generated_code += identifier;
			l_generated_code += L"\", &::FE::framework::ECS::remove_component<";
			l_generated_code += identifier;
			l_generated_code += L">);\n";

			l_generated_code += L"\n";
			l_namespace_concat_replaced_with_underscores = identifier; // The identifier may contain namespace concatenation, so we have to replace "::" with "_"
			for (std::pmr::wstring::size_type pos = l_namespace_concat_replaced_with_underscores.find(L"::"); pos != std::pmr::wstring::npos; pos = l_namespace_concat_replaced_with_underscores.find(L"::"))
			{
				l_namespace_concat_replaced_with_underscores.replace(pos, 2, L"_");
			}

			l_generated_code += L"    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >(\"serialize_component";
			l_generated_code += l_namespace_concat_replaced_with_underscores;
			l_generated_code += L"\", &serialize_component";
			l_generated_code += l_namespace_concat_replaced_with_underscores;
			l_generated_code += L");\n";

			l_generated_code += L"    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(const ::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >(\"deserialize_component";
			l_generated_code += l_namespace_concat_replaced_with_underscores;
			l_generated_code += L"\", &deserialize_component";
			l_generated_code += l_namespace_concat_replaced_with_underscores;
			l_generated_code += L");\n\n";
		}


		for (const typename reflection_metadata::system_info& system_node : header_file._system_fptrs) // C-style system functions reflection
		{
			l_generated_code += L"    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system<";
			l_generated_code += system_node._system_target;
			l_generated_code += L">(";
			l_generated_code += system_node._system_call_phase;
			l_generated_code += L", &";
			l_generated_code += system_node._system_name;
			l_generated_code += L");\n";
		}


		constexpr FE::wchar* l_class_and_structs_reflection_frame = L"    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<";
		for (const std::pmr::wstring& identifier : header_file._class_and_structs) // Classes and structs reflection
		{
			l_generated_code += l_class_and_structs_reflection_frame;
			l_generated_code += identifier;
			l_generated_code += L"*(";
			l_generated_code += identifier;
			l_generated_code += L"*)> >(\"construct ";
			l_generated_code += identifier;
			l_generated_code += L"\", ";
			l_generated_code += L"&::std::construct_at<";
			l_generated_code += identifier;
			l_generated_code += L">);\n";

			l_generated_code += l_class_and_structs_reflection_frame;
			l_generated_code += L"void(";
			l_generated_code += identifier;
			l_generated_code += L"*)> >(\"destruct ";
			l_generated_code += identifier;
			l_generated_code += L"\", ";
			l_generated_code += L"&::std::destroy_at<";
			l_generated_code += identifier;
			l_generated_code += L">);\n";
		}


		constexpr FE::wchar* l_enum_reflexpr_frame = L"\n    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ";
		for (const std::pmr::vector<std::pmr::wstring>& enum_struct : header_file._enum_structs) // Enum structs reflection
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