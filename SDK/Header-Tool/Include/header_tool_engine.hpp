#ifndef _FE_HEADER_TOOL_ENGINE_HPP_
#define _FE_HEADER_TOOL_ENGINE_HPP_
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
#include "reflection_tree.hpp"
#include "error_code.hpp"
#include "program_options.hpp"
#include "vocabulary.hpp"

// FE.Framework
#include <FE/framework.h>

// FE.Core string algorithm
#include <FE/algorithm/string.hxx>

// std
#include <algorithm> // std::find_if
#include <fstream> // std::basic_ifstream
#include <list> // std::pmr::pmr::list
#include <string> // std::pmr::basic_string
#include <string_view> // std::basic_string_view
#include <vector> // std::pmr::vector

// Microsoft Parallel Patterns Library. This header is specific to the Microsoft Visual Studio.
//#include <concurrent_vector.h>

#include <FE/concurrent_vector.hxx> // The in-house replacement for the Microsoft PPL concurrent_vector




using directory_t = std::pmr::basic_string<var::wchar>;
using file_buffer_t = std::pmr::basic_string<var::UTF8>;



// To Do - Eating my own dog food: integrate the FE ECS.
struct token
{
	Vocabulary _vocabulary;
	var::uint32 _line_number;
	file_buffer_t _code;
	FE::wchar* _header_file_path;

	_FE_NODISCARD_ FE::boolean operator==(const token& rhs_p) const noexcept
	{
		return (_vocabulary == rhs_p._vocabulary);
	}

	_FE_NODISCARD_ FE::boolean operator!=(const token& rhs_p) const noexcept
	{
		return (_vocabulary != rhs_p._vocabulary);
	}
};


/*
* Test arguments:
-fno-code-style-guide -fno-copyright-notice -path-to-project=C:\Users\leeho\Documents\GitHub\Frogman-Engine\SDK\Header-Tool\CMake C:\Users\leeho\Documents\GitHub\Frogman-Engine\SDK\Framework\Include\FE\framework\archetype_base.hpp
*/

/*
The header_tool_engine class is a specialized tool for enforcing copyright notice embedment and generating reflection meta data registry code
while utilizing parallel task execution for efficiency.
*/

// The header_tool_engine will be refactored into multiple smaller classes in the future; this is absurdly large.
class header_tool_engine : public FE::framework::framework_base
{
	FE::uint8 m_UTF8_with_BOM[3];
	std::locale m_UTF8_locale;

	program_options m_header_tool_options;
	file_buffer_t m_copyright_notice;

	std::pmr::vector<directory_t> m_header_file_list;
	std::pmr::vector<file_buffer_t> m_mapped_header_files;
	FE::framework::reflection::enum_metadata* m_FHT_error_codes;

public:
	header_tool_engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
	~header_tool_engine() noexcept override = default;

	virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) override;
	virtual FE::int32 run() override;
	virtual FE::int32 shutdown() override;

private:
	_FE_NODISCARD_ FE::boolean __is_the_file_encoded_with_UTF8_BOM(FE::wchar* directory_p) const noexcept;
	_FE_FORCE_INLINE_ _FE_NODISCARD_ FE::boolean __is_encoded_with_UTF8_BOM(FE::UTF8* const buffer_entry_p) const noexcept
	{
		return ((buffer_entry_p[0] == m_UTF8_with_BOM[0]) && (buffer_entry_p[1] == m_UTF8_with_BOM[1]) && (buffer_entry_p[2] == m_UTF8_with_BOM[2]));
	}

	_FE_NODISCARD_ std::pmr::vector<directory_t> __make_header_file_list(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
	_FE_NODISCARD_ std::pmr::vector<file_buffer_t> __map_header_files(const std::pmr::vector<directory_t>& file_list_p) noexcept;

private:
	_FE_NODISCARD_ file_buffer_t __read_copyright_notice(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;

private:
	struct symbol_count
	{
		var::uint16 _namespaces;
		var::uint16 _classes;
		var::uint16 _structs;
		var::uint16 _enums;
		var::uint16 _systems;
	};
	_FE_NODISCARD_ symbol_count __try_count_all_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p) const;
	_FE_NODISCARD_ symbol_count __try_count_the_current_scope_level_symbols(typename std::pmr::list<token>::const_iterator begin_p, typename std::pmr::list<token>::const_iterator end_p);

	// ___verify_if_token_is_a_paren_or_bracket(Vocabulary paren_p) returns std::nullopt if paren_p is not a paren nor a bracket.
	_FE_NODISCARD_ std::optional<FE::uint32> ___verify_if_token_is_a_paren_or_bracket(Vocabulary paren_p) const noexcept;
	_FE_NODISCARD_ std::optional<FE::ASCII*> __validate_parentheses(const std::pmr::list<token>& token_list_p) noexcept;

private:
	_FE_NODISCARD_ std::optional<std::pmr::list<token>> __tokenize_header(const file_buffer_t& file_p, const directory_t& path_p) noexcept;
	// const char* p = "/* text */", f = "//text"; the 'text' is recognized as comments by FHT, which means that they will be purged from the token list.
	void __purge_comments(std::pmr::list<token>& out_list_p) noexcept;
	void __purge_string_literals(std::pmr::list<token>& out_list_p) noexcept;
	void __purge_preprocessor_directives(std::pmr::list<token>& out_list_p);

	token __tokenize_identifiable(typename file_buffer_t::const_pointer code_iterator_p) noexcept;
	token __tokenize_unidentifiable(typename file_buffer_t::const_pointer code_iterator_p) noexcept;
	void __tokenize_comment(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p) noexcept;
	void __tokenize_operator(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p) noexcept;

	_FE_NODISCARD_ FE::boolean __verify_key_equivalence(typename file_buffer_t::const_pointer subject_p, FE::ASCII* key_p) noexcept;
	 
private:
	_FE_NODISCARD_ header_file_root __try_build_reflection_tree(const directory_t& file_path_p, const std::pmr::list<token>& token_list_p);
	_FE_NODISCARD_ std::optional<namespace_node> __try_build_namespace_node_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);
	_FE_NODISCARD_ std::optional<class_node> __try_build_class_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);
	_FE_NODISCARD_ std::optional<struct_node> __try_build_struct_node_mutually_recursive(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);
	_FE_NODISCARD_ std::optional<enum_struct_node> __try_build_enum_struct_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);
	_FE_NODISCARD_ std::optional<identifier> __try_build_c_style_system_function_node(const identifier& parent_namespace_p, typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p);

	void __skip_template_args(typename std::pmr::list<token>::const_iterator& iterator_p) const;
	void __skip_code_block(typename std::pmr::list<token>::const_iterator& out_token_iterator_p, typename std::pmr::list<token>::const_iterator end_p) const noexcept;
	bool __is_forward_declaration(typename std::pmr::list<token>::const_iterator& out_token_iterator_p) const;
	
private:
	struct reflection_metadata
	{
		directory_t _header_file_path;
		std::pmr::vector<std::pmr::wstring> _archetype_base_children;
		std::pmr::vector<std::pmr::wstring> _component_base_children;
		std::pmr::vector<std::pmr::wstring> _system_base_children;
		std::pmr::vector<std::pmr::wstring> _class_and_structs;
		std::pmr::vector< std::pmr::vector<std::pmr::wstring> > _enum_structs;
		std::pmr::vector<std::pmr::wstring> _c_style_system_functions;
	};
	using reflection_metadata_set_t = FE::concurrent_vector<reflection_metadata>;
	reflection_metadata_set_t m_reflection_metadata_set;

	_FE_NODISCARD_ reflection_metadata __generate_reflection_metadata(const header_file_root& tree_p) noexcept;
	void __output_namespace_metadata_recursive(reflection_metadata& out_return_p, const namespace_node& node_p) noexcept;
	void __output_class_metadata(reflection_metadata& out_return_p, const class_node& node_p) noexcept;
	void __output_struct_metadata(reflection_metadata& out_return_p, const struct_node& node_p) noexcept;
	void __output_enum_struct_metadata(reflection_metadata& out_return_p, const enum_struct_node& node_p) noexcept;

private:
	void __generate_reflection_code(const reflection_metadata_set_t& metadata_set_p) noexcept;
};


#endif