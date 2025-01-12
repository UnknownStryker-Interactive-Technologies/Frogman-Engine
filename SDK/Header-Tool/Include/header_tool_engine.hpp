#ifndef _FE_HEADER_TOOL_ENGINE_HPP_
#define _FE_HEADER_TOOL_ENGINE_HPP_
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
#include "reflection_tree.hpp"
#include "error_code.hpp"
#include "program_options.hpp"
#include "vocabulary.hpp"

// FE.Framework
#include <FE/framework/framework.hpp>

// FE.Core string algorithm
#include <FE/algorithm/string.hxx>

// std
#include <algorithm> // std::find_if
#include <fstream> // std::basic_ifstream
#include <string> // std::pmr::basic_string
#include <string_view> // std::basic_string_view
#include <vector> // std::pmr::vector




using directory_t = std::pmr::basic_string<var::tchar>;
using file_buffer_t = std::pmr::basic_string<var::UTF8>;




struct token
{
	Vocabulary _vocabulary;
	file_buffer_t _code;

	FE::boolean operator==(const token& rhs_p) const noexcept
	{
		return (this->_vocabulary == rhs_p._vocabulary);
	}

	FE::boolean operator!=(const token& rhs_p) const noexcept
	{
		return (this->_vocabulary != rhs_p._vocabulary);
	}
};




// sample data: -fno-code-style-guide -path-to-project=C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Header-Tool\CMake -path-to-copyright-notice=C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FE-HT-Test\LICENSE.txt C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FE-HT-Test\HeaderWithCopyright.hpp;C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FE-HT-Test\HeaderWithoutCopyright.hpp
/*
The header_tool_engine class is a specialized component of the Frogman Engine that manages header file processing, including copyright notice verification and reflection code generation
while utilizing parallel task execution for efficiency.
*/
class header_tool_engine : public FE::framework::framework_base
{
	FE::uint8 m_UTF8_with_BOM[3];
	std::locale m_UTF8_locale;

	program_options m_header_tool_options;
	file_buffer_t m_copyright_notice;
	file_buffer_t m_code_style_guide;

	std::pmr::vector<directory_t> m_header_file_list;
	std::pmr::vector<file_buffer_t> m_mapped_header_files;

public:
	header_tool_engine(FE::int32 argc_p, FE::tchar** argv_p) noexcept;
	~header_tool_engine() noexcept override = default;

	virtual FE::int32 launch(FE::int32 argc_p, FE::tchar** argv_p) override;
	virtual FE::int32 run() override;
	virtual FE::int32 shutdown() override;

private:
	FE::boolean __is_the_file_encoded_with_UTF8_BOM(FE::tchar* directory_p) const noexcept;
	std::pmr::vector<directory_t> __make_header_file_list(FE::int32 argc_p, FE::tchar** argv_p) noexcept;
	std::pmr::vector<file_buffer_t> __map_header_files(const std::pmr::vector<directory_t>& file_list_p) noexcept;

private:
	file_buffer_t __read_copyright_notice(FE::int32 argc_p, FE::tchar** argv_p) noexcept;

private:
	struct symbol_count
	{
		var::uint16 _namespaces;
		var::uint16 _classes;
		var::uint16 _structs;
	};
	symbol_count __count_all_symbols(typename std::pmr::vector<token>::const_iterator begin_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept;
	symbol_count __count_the_current_scope_level_symbols(typename std::pmr::vector<token>::const_iterator begin_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept;

	struct member_symbol_count
	{
		var::uint16 _methods;
		var::uint16 _properties;
	};
	member_symbol_count __count_the_current_class_member_symbols(typename std::pmr::vector<token>::const_iterator begin_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept;

private:
	std::optional<std::pmr::vector<token>> __parse_header(const file_buffer_t& file_p) noexcept;
	void __purge_comments(std::pmr::vector<token>& out_list_p) noexcept;
	token __tokenize(typename file_buffer_t::const_pointer code_iterator_p) noexcept;
	token __tokenize_undefined(typename file_buffer_t::const_pointer code_iterator_p) noexcept;
	token __tokenize_comment(typename file_buffer_t::const_pointer code_iterator_p) noexcept;
	token __tokenize_reflection_relevant(typename file_buffer_t::const_pointer code_iterator_p) noexcept;
	token __tokenize_operator(typename file_buffer_t::const_pointer code_iterator_p) noexcept;

	_FE_FORCE_INLINE_ FE::boolean __verify_if_subject_is_equal_to_key(typename file_buffer_t::const_pointer subject_p, FE::tchar* key_p) noexcept
	{
		FE::uint64 l_length = FE::algorithm::string::length(key_p);
		if (FE::algorithm::string::compare_ranged<var::tchar>(FE::iterator_cast<FE::tchar*>(subject_p), FE::algorithm::string::range{ 0, l_length },
			key_p, FE::algorithm::string::range{ 0, l_length }))
		{
			return true;
		}
		return false;
	}

private:
	header_file_root __build_reflection_tree(const directory_t& file_path_p, const std::pmr::vector<token>& token_list_p) noexcept;
	namespace_node __build_namespace_node_recursive(typename std::pmr::vector<token>::const_iterator& out_token_iterator_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept;
	class_node __build_class_node_mutually_recursive(typename std::pmr::vector<token>::const_iterator& out_token_iterator_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept;
	struct_node __build_struct_node_mutually_recursive(typename std::pmr::vector<token>::const_iterator& out_token_iterator_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept
	{
		(void)out_token_iterator_p;
		(void)end_p;
		return struct_node();
	}

	void __handle_template_args(typename std::pmr::vector<token>::const_iterator& iterator_p) noexcept;
	void __handle_enum(typename std::pmr::vector<token>::const_iterator& iterator_p) noexcept;

private:
	void __generate_reflection_code(const header_file_root& tree_p) noexcept;
	void __traverse_reflection_tree(const header_file_root& tree_p) noexcept;
	void __traverse_namespace_node(const namespace_node& node_p) noexcept;
	void __traverse_class_node(const class_node& node_p) noexcept;
	void __traverse_struct_node(const struct_node& node_p) noexcept;
};


#endif