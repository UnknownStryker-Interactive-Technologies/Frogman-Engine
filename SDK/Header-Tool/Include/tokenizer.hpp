#ifndef _FE_HEADER_TOOL_TOKENIZER_HPP_
#define _FE_HEADER_TOOL_TOKENIZER_HPP_
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
#include <FE/prerequisites.hxx>

#include "context.hpp"
#include "token.hpp"
#include "vocabulary.hpp"

// std
#include <algorithm> // std::find_if
#include <fstream> // std::basic_ifstream
#include <list> // std::pmr::pmr::list
#include <string> // std::pmr::basic_string
#include <string_view> // std::basic_string_view
#include <vector> // std::pmr::vector




namespace FHT::tokenizer
{
	_FE_NODISCARD_ std::pmr::list<token> tokenize_header(const file_buffer_t& file_p, const directory_t& path_p);

	// const char* p = "/* text */", f = "//text"; the 'text' is recognized as comments by FHT are purged from the token list.
	void purge_comments(std::pmr::list<token>& out_list_p) noexcept;

	void purge_preprocessor(std::pmr::list<token>& out_list_p) noexcept;

	void purge_string_literals_and_backslashes(std::pmr::list<token>& out_list_p) noexcept;

	void purge_template(std::pmr::list<token>& out_list_p) noexcept;

	void purge_forward_declaration(std::pmr::list<token>& out_list_p) noexcept;


	_FE_NODISCARD_ token tokenize_identifiable(typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p);

	_FE_NODISCARD_ token tokenize_unidentifiable(typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p);


	void tokenize_comment(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p) noexcept;

	void tokenize_preprocessor(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p) noexcept;


	void tokenize_string_literal(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p);
	
	void extract_raw_text_delimiter_from_the_left_quote(file_buffer_t& out_return_p, typename file_buffer_t::const_pointer code_iterator_p);


	void tokenize_template(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p);


	void tokenize_other(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p) noexcept;


	void tokenize_reflection_macros(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p);


	void tokenize_class(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p);

	void tokenize_struct(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p);

	void tokenize_enum_struct(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p);

	void tokenize_namespace(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p);

	void tokenize_class_struct_enum_forward_decl_and_using_namespace(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p);

	void tokenize_template_body(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p);
}

#endif