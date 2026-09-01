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
#define _CRT_SECURE_NO_WARNINGS
#include "reflexcode_generator.hpp"
#include "error_code.hpp"
#include "header_tool.hpp"

#include <FE/algorithm/string.hxx>

// std::find
#include <algorithm>
#include <cstdlib>
// std::ofstream
#include <fstream>




namespace FHT::reflexcode_generator
{
	_FE_NODISCARD_ metadata generate_metadata(const header_file_root& tree_p) noexcept
	{
		metadata l_metadata;
		l_metadata._class_and_structs = std::pmr::vector<std::pmr::wstring>(::framework::get_framework().get_memory_resource());
		l_metadata._header_file_path = std::pmr::wstring(tree_p._path_to_the_header_file.data(), framework::get_framework().get_memory_resource());

		for (const std::optional<class_node>& node : tree_p._classes)
		{
			if (node == std::nullopt)
			{
				continue;
			}

			if (node->_has_pure_virtual == true)
			{
				continue; // skip pure virtual classes
			}

			output_class_metadata(l_metadata, *node);
		}

		for (const std::optional<struct_node>& node : tree_p._structs)
		{
			if (node == std::nullopt)
			{
				continue;
			}
			output_struct_metadata(l_metadata, *node);
		}

		for (const std::optional<enum_struct_node>& node : tree_p._enum_structs)
		{
			if (node == std::nullopt)
			{
				continue;
			}

			output_enum_struct_metadata(l_metadata, *node);
		}

		for (const std::optional<namespace_node>& node : tree_p._namespaces)
		{
			if (node == std::nullopt)
			{
				continue;
			}

			output_namespace_metadata_recursive(l_metadata, *node);
		}

		for (const std::optional<system_node>& c_style_system_function : tree_p._system_fptrs)
		{
			if (c_style_system_function == std::nullopt)
			{
				continue;
			}

			typename metadata::system_info l_system_node
			{
				std::pmr::wstring(framework::get_framework().get_memory_resource()), 
				std::pmr::wstring(framework::get_framework().get_memory_resource()),
				std::pmr::wstring(framework::get_framework().get_memory_resource())
			};

			l_system_node._world_tag_enum.resize(c_style_system_function->_world_tag_enum.length());
			std::mbstowcs(l_system_node._world_tag_enum.data(), reinterpret_cast<const char*>(c_style_system_function->_world_tag_enum.data()), c_style_system_function->_world_tag_enum.length());

			l_system_node._system_name.resize(c_style_system_function->_sysname.length());
			std::mbstowcs(l_system_node._system_name.data(), reinterpret_cast<const char*>(c_style_system_function->_sysname.data()), c_style_system_function->_sysname.length());

			l_system_node._system_call_phase.resize(c_style_system_function->_syscall_phase.length());
			std::mbstowcs(l_system_node._system_call_phase.data(), reinterpret_cast<const char*>(c_style_system_function->_syscall_phase.data()), c_style_system_function->_syscall_phase.length());

			l_metadata._system_fptrs.push_back(std::move(l_system_node));
		}

		return l_metadata;
	}


	void output_namespace_metadata_recursive(metadata& out_return_p, const namespace_node& node_p) noexcept
	{
		for (const std::optional<class_node>& node : node_p._classes)
		{
			if (node == std::nullopt)
			{
				continue;
			}

			if (node->_has_pure_virtual == true)
			{
				continue; // skip pure virtual classes
			}

			output_class_metadata(out_return_p, *node);
		}

		for (const std::optional<struct_node>& node : node_p._structs)
		{
			if (node == std::nullopt)
			{
				continue;
			}
			output_struct_metadata(out_return_p, *node);
		}

		for (const std::optional<enum_struct_node>& node : node_p._enum_structs)
		{
			if (node == std::nullopt)
			{
				continue;
			}
			output_enum_struct_metadata(out_return_p, *node);
		}

		for (const std::optional<namespace_node>& node : node_p._nested_namespaces)
		{
			if (node == std::nullopt)
			{
				continue;
			}
			output_namespace_metadata_recursive(out_return_p, *node);
		}

		for (const std::optional<system_node>& c_style_system_function : node_p._system_fptrs)
		{
			if (c_style_system_function == std::nullopt)
			{
				continue;
			}

			typename metadata::system_info l_system_node
			{
				std::pmr::wstring(framework::get_framework().get_memory_resource()), 
				std::pmr::wstring(framework::get_framework().get_memory_resource()),
				std::pmr::wstring(framework::get_framework().get_memory_resource()), 
			};

			l_system_node._world_tag_enum.resize(c_style_system_function->_world_tag_enum.length());
			std::mbstowcs(l_system_node._world_tag_enum.data(), reinterpret_cast<const char*>(c_style_system_function->_world_tag_enum.data()), c_style_system_function->_world_tag_enum.length());

			l_system_node._system_name.resize(c_style_system_function->_sysname.length());
			std::mbstowcs(l_system_node._system_name.data(), reinterpret_cast<const char*>(c_style_system_function->_sysname.data()), c_style_system_function->_sysname.length());

			l_system_node._system_call_phase.resize(c_style_system_function->_syscall_phase.length());
			std::mbstowcs(l_system_node._system_call_phase.data(), reinterpret_cast<const char*>(c_style_system_function->_syscall_phase.data()), c_style_system_function->_syscall_phase.length());

			out_return_p._system_fptrs.push_back(std::move(l_system_node));
		}
	}


	void output_class_metadata(metadata& out_return_p, const class_node& node_p) noexcept
	{
		std::pmr::wstring l_identifier(framework::get_framework().get_memory_resource());
		l_identifier.resize(node_p._this_class_name.length());

		std::mbstowcs(l_identifier.data(), reinterpret_cast<const char*>(node_p._this_class_name.data()), node_p._this_class_name.length());

		out_return_p._class_and_structs.push_back(std::move(l_identifier));
	}


	void output_struct_metadata(metadata& out_return_p, const struct_node& node_p) noexcept
	{
		std::pmr::wstring l_identifier(framework::get_framework().get_memory_resource());
		l_identifier.resize(node_p._identifier.length());

		std::mbstowcs(l_identifier.data(), reinterpret_cast<const char*>(node_p._identifier.data()), node_p._identifier.length());
		out_return_p._class_and_structs.push_back(std::move(l_identifier));
	}

	void output_enum_struct_metadata(metadata& out_return_p, const enum_struct_node& node_p) noexcept
	{
		std::pmr::wstring l_identifier(framework::get_framework().get_memory_resource());
		l_identifier.resize(node_p._target_enum_struct_name.length());

		std::mbstowcs(l_identifier.data(), reinterpret_cast<const char*>(node_p._target_enum_struct_name.data()), node_p._target_enum_struct_name.length());

		out_return_p._enum_structs.emplace_back(std::pmr::vector<std::pmr::wstring>{ framework::get_framework().get_memory_resource() });
		out_return_p._enum_structs.back().push_back(std::move(l_identifier));

		for (const identifier& enum_value : node_p._enum_struct_fields)
		{
			std::pmr::wstring l_enum_value(framework::get_framework().get_memory_resource());
			l_enum_value.resize(enum_value.length() + 1);

			std::mbstowcs(l_enum_value.data(), reinterpret_cast<const char*>(enum_value.data()), enum_value.length());
			l_enum_value = l_enum_value.c_str();
			out_return_p._enum_structs.back().push_back(std::move(l_enum_value));
		}
	}




	void generate_reflexcode(const metadata_set_t& metadata_set_p) noexcept
	{
		std::pmr::wstring l_generated_code(framework::get_framework().get_memory_resource());
		l_generated_code.reserve(1 * FE::one_MiB);

		l_generated_code += L"// Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved. \n#include <FE/framework/reflection/private/load_reflection_data.hxx> \n#include <FE/framework.hxx> \n";
		l_generated_code += L"#include <memory>\n\n";
		for (const metadata& header_file : metadata_set_p) // #include <> statements gereration
		{
			l_generated_code += L"#include <";
			l_generated_code += header_file._header_file_path;
			l_generated_code += L">\n";
		}

		l_generated_code += L"\n\n\n\n";

		l_generated_code += L"void load_reflection_data()\n{\n"; // The void load_reflection_data() implementation generation

		for (const metadata& header_file : metadata_set_p)
		{
			for (const typename metadata::system_info& system_node : header_file._system_fptrs) // C-style system functions reflection
			{
				l_generated_code += L"    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(";
				l_generated_code += system_node._world_tag_enum;
				l_generated_code += L", ";
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
				if (enum_struct.size() == 1) _FE_UNLIKELY_
				{
					continue;
				}

				// The first element of the enum_struct vector is the name of the enum struct, and the rest are the enum values.
				const std::pmr::wstring& identifier = enum_struct.front();
				l_generated_code += l_enum_reflexpr_frame;
				l_generated_code += identifier;
				l_generated_code += L" >(\"";
				l_generated_code += identifier;
				l_generated_code += L"\",\n    {\n";

				for (auto it = std::next(enum_struct.begin(), 1); it < enum_struct.end(); ++it)
				{
					l_generated_code += L"        { ";
					l_generated_code += identifier;
					l_generated_code += L"::";
					l_generated_code += *it;
					l_generated_code += L", \"";
					l_generated_code += *it;
					l_generated_code += L"\" },\n";
				}
				l_generated_code.erase(l_generated_code.length() - 2, 1); // Remove last comma
				l_generated_code += L"    });\n";
			}
		}
		l_generated_code += L"}\n";


		std::pmr::wstring l_path_to_generated(framework::get_framework().get_memory_resource());
		FE::size l_path_length = FE::algorithm::string::length(::header_tool::get_program_options().get_path_to_project());
		FE_EXIT_IF(l_path_length == 0, FrogmanEngineHeaderToolError::_FatalCmdInputError_InvalidPathToCMakeProject, "Frogman Engine Header Tool: the directory value for -path-to-project= is not given or specified to this header tool program. \nThe string length of the path to the desired folder to create the generated.cpp file is ZERO.");

		l_path_to_generated.resize(l_path_length + 1);
		std::mbstowcs(l_path_to_generated.data(), ::header_tool::get_program_options().get_path_to_project(), l_path_length);
		l_path_to_generated = l_path_to_generated.c_str();
		l_path_to_generated += L"\\generated.cpp";

		std::wofstream l_generated_file;
		l_generated_file.open(l_path_to_generated);
		FE_EXIT_IF(l_generated_file.is_open() == false, FrogmanEngineHeaderToolError::_FatalCmdInputError_InvalidPathToCMakeProject, "Frogman Engine Header Tool: failed to generate the generated.cpp file.");
		l_generated_file << l_generated_code;
		l_generated_file.close();
	}
}
