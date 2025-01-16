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

// Microsoft Parallel Patterns Library. This header is specific to the Microsoft Visual Studio.
#include <concurrent_vector.h>




using directory_t = std::pmr::basic_string<var::wchar>;
using file_buffer_t = std::pmr::basic_string<var::UTF8>;




struct token
{
	Vocabulary _vocabulary;
	file_buffer_t _code;

	_FE_NODISCARD_ FE::boolean operator==(const token& rhs_p) const noexcept
	{
		return (this->_vocabulary == rhs_p._vocabulary);
	}

	_FE_NODISCARD_ FE::boolean operator!=(const token& rhs_p) const noexcept
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
	header_tool_engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
	~header_tool_engine() noexcept override = default;

	virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) override;
	virtual FE::int32 run() override;
	virtual FE::int32 shutdown() override;

private:
	_FE_NODISCARD_ FE::boolean __is_the_file_encoded_with_UTF8_BOM(FE::wchar* directory_p) const noexcept;
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
	};
	_FE_NODISCARD_ symbol_count __count_all_symbols(typename std::pmr::vector<token>::const_iterator begin_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept;
	_FE_NODISCARD_ symbol_count __count_the_current_scope_level_symbols(typename std::pmr::vector<token>::const_iterator begin_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept;

	struct member_symbol_count
	{
		var::uint16 _methods;
		var::uint16 _properties;
	};
	_FE_NODISCARD_ member_symbol_count __count_the_current_class_member_symbols(typename std::pmr::vector<token>::const_iterator begin_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept;

private:
	_FE_NODISCARD_ std::optional<std::pmr::vector<token>> __parse_header(const file_buffer_t& file_p) noexcept;
	void __purge_comments(std::pmr::vector<token>& out_list_p) noexcept;
	_FE_NODISCARD_ token __tokenize(typename file_buffer_t::const_pointer code_iterator_p) noexcept;
	_FE_NODISCARD_ token __tokenize_undefined(typename file_buffer_t::const_pointer code_iterator_p) noexcept;
	_FE_NODISCARD_ token __tokenize_comment(typename file_buffer_t::const_pointer code_iterator_p) noexcept;
	_FE_NODISCARD_ token __tokenize_reflection_relevant(typename file_buffer_t::const_pointer code_iterator_p) noexcept;
	_FE_NODISCARD_ token __tokenize_operator(typename file_buffer_t::const_pointer code_iterator_p) noexcept;

	_FE_FORCE_INLINE_ _FE_NODISCARD_ FE::boolean __verify_if_subject_is_equal_to_key(typename file_buffer_t::const_pointer subject_p, FE::ASCII* key_p) noexcept
	{
		FE::uint64 l_length = FE::algorithm::string::length(key_p);
		if (FE::algorithm::string::compare_ranged<var::ASCII>(FE::iterator_cast<FE::ASCII*>(subject_p), FE::algorithm::string::range{ 0, l_length },
			key_p, FE::algorithm::string::range{ 0, l_length }))
		{
			return true;
		}
		return false;
	}

private:
	_FE_NODISCARD_ header_file_root __build_reflection_tree(const directory_t& file_path_p, const std::pmr::vector<token>& token_list_p) noexcept;
	_FE_NODISCARD_ namespace_node __build_namespace_node_recursive(const identifier_t& parent_namespace_p, typename std::pmr::vector<token>::const_iterator& out_token_iterator_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept;
	_FE_NODISCARD_ class_node __build_class_node_mutually_recursive(const identifier_t& parent_namespace_p, typename std::pmr::vector<token>::const_iterator& out_token_iterator_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept;
	_FE_NODISCARD_ struct_node __build_struct_node_mutually_recursive(const identifier_t& parent_namespace_p, typename std::pmr::vector<token>::const_iterator& out_token_iterator_p, typename std::pmr::vector<token>::const_iterator end_p) noexcept;

	void __handle_template(typename std::pmr::vector<token>::const_iterator& iterator_p) noexcept;
	void __handle_enum(typename std::pmr::vector<token>::const_iterator& iterator_p) noexcept;

private:
	struct reflection_metadata
	{
		directory_t _header_file_path;
		std::pmr::vector<std::pmr::wstring> _class_and_struct_identifiers;
		std::pmr::vector<std::pmr::wstring> _method_identifiers;
		std::pmr::vector<std::pmr::wstring> _property_identifiers;
	};
	using reflection_metadata_set_t = concurrency::concurrent_vector<reflection_metadata, std::pmr::polymorphic_allocator<reflection_metadata>>;
	reflection_metadata_set_t m_reflection_metadata_set;

	_FE_NODISCARD_ reflection_metadata __generate_reflection_metadata(const header_file_root& tree_p) noexcept;
	void __output_namespace_metadata_recursive(reflection_metadata& out_return_p, const namespace_node& node_p) noexcept;
	void __output_class_metadata(reflection_metadata& out_return_p, const class_node& node_p) noexcept;
	void __output_struct_metadata(reflection_metadata& out_return_p, const struct_node& node_p) noexcept;

private:
	void __generate_reflection_code(const reflection_metadata_set_t& metadata_set_p) noexcept;
};


#endif