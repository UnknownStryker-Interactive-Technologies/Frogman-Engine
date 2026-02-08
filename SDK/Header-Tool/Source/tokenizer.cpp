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
#include "header_tool.hpp"
#include "file_io.hpp"
#include "context.hpp"
#include "tokenizer.hpp"




namespace FHT::tokenizer
{
	_FE_NODISCARD_ std::pmr::list<token> tokenize_header(const file_buffer_t& file_p, const directory_t& path_p)
	{
		if (file_p.empty() == true)
		{
			throw FE::pair<FrogmanEngineHeaderToolError, FE::ASCII*>{FrogmanEngineHeaderToolError::_EmptyHeaderFile, "FHT Warning: The header file is empty."};
		}

		context_stack_t l_context_stack{ framework::get_framework().get_memory_resource() };
		l_context_stack.reserve(64);
		l_context_stack.emplace_back(FHT::Context::_Global);

		std::pmr::list<token> l_list{ framework::get_framework().get_memory_resource() };

		auto l_end = file_p.c_str() + file_p.size();
		var::uint32 l_token_number = 1;
		for (FE::UTF8* iterator = FE::algorithm::string::skip_BOM(file_p.c_str()); iterator < l_end;)
		{
			if (*iterator <= ' ')
			{
				++iterator;
				continue;
			}


			token l_token = tokenize_identifiable(iterator, l_context_stack);
			l_token._header_file_path = path_p.c_str();
			l_token._token_number = l_token_number;
			if (l_token._vocabulary != Vocabulary::_Undefined)
			{
				iterator += l_token._code.size(); // move to the next.
				l_list.push_back(std::move(l_token)); // push_back the defined vocab.

				if (iterator < l_end) _FE_LIKELY_
				{
					if (*iterator == '\n')
					{
						l_token._vocabulary = Vocabulary::_LineEnd;
						l_token._code = file_buffer_t(1, *iterator, framework::get_framework().get_memory_resource());
						l_token._header_file_path = path_p.c_str();
						l_token._token_number = l_token_number;

						l_list.push_back(std::move(l_token));
						++l_token_number; // Increment the line number.
						++iterator; // move to the next.
					}
				}
				continue;
			}


			l_token = tokenize_unidentifiable(iterator, l_context_stack);
			l_token._header_file_path = path_p.c_str();
			l_token._token_number = l_token_number;
			iterator += l_token._code.size(); // move to the next.
			l_list.push_back(std::move(l_token));

			if (iterator < l_end) _FE_LIKELY_
			{
				if (*iterator == '\n')
				{
					l_token._vocabulary = Vocabulary::_LineEnd;
					l_token._code = file_buffer_t(1, *iterator, framework::get_framework().get_memory_resource());
					l_token._header_file_path = path_p.c_str();
					l_token._token_number = l_token_number;

					l_list.push_back(std::move(l_token));
					++l_token_number; // Increment the line number.
					++iterator; // move to the next.
				}
			}
			continue;
		}

		l_list.emplace_back(Vocabulary::_EndOfCode, l_token_number, u8"\0");
		return l_list;
	}

	// const char* p = "/* text */", f = "//text"; the 'text' is recognized as comments by FHT are purged from the token list.
	void purge_comments(std::pmr::list<token>& out_list_p) noexcept
	{
		for (auto it = out_list_p.begin(); it != out_list_p.end();) 
		{
			switch (it->_vocabulary)
			{
			case Vocabulary::_CommentBegin:
				_FE_FALLTHROUGH_;
			case Vocabulary::_CommentEnd:
				_FE_FALLTHROUGH_;
			case Vocabulary::_LineComment:
				_FE_FALLTHROUGH_;
			case Vocabulary::_CommentBody:
				{
					auto l_to_erase = it;
					++it;
					out_list_p.erase(l_to_erase);
				}
				continue;

			default:
				++it;
				continue;
			}
		}
	}

	void purge_preprocessor(std::pmr::list<token>& out_list_p) noexcept
	{
		for (auto it = out_list_p.begin(); it != out_list_p.end();)
		{
			switch (it->_vocabulary)
			{
			case Vocabulary::_PreprocessorDirective:
				_FE_FALLTHROUGH_;
			case Vocabulary::_Preprocessor:
				_FE_FALLTHROUGH_;
			case Vocabulary::_PreprocessorNextLine:
			{
				auto l_to_erase = it;
				++it;
				out_list_p.erase(l_to_erase);
			}
			continue;

			default:
				++it;
				continue;
			}
		}
	}

	void purge_string_literals_and_backslashes(std::pmr::list<token>& out_list_p) noexcept
	{
		for (auto it = out_list_p.begin(); it != out_list_p.end();)
		{
			switch (it->_vocabulary)
			{
			case Vocabulary::_TextLiteralPrefix:
				_FE_FALLTHROUGH_;
			case Vocabulary::_StringLiteral:
				_FE_FALLTHROUGH_;
			case Vocabulary::_CharLiteral:
				_FE_FALLTHROUGH_;
			case Vocabulary::_BackSlash:
			{
				auto l_to_erase = it;
				++it;
				out_list_p.erase(l_to_erase);
			}
			continue;

			default:
				++it;
				continue;
			}
		}
	}

	void purge_template(std::pmr::list<token>& out_list_p) noexcept
	{
		for (auto it = out_list_p.begin(); it != out_list_p.end();)
		{
			switch (it->_vocabulary)
			{
			case Vocabulary::_Template:
				_FE_FALLTHROUGH_;
			case Vocabulary::_BeginTemplateArgs:
				_FE_FALLTHROUGH_;
			case Vocabulary::_Typename:
				_FE_FALLTHROUGH_;
			case Vocabulary::_TemplateArg:
				_FE_FALLTHROUGH_;
			case Vocabulary::_EndTemplateArgs:
				_FE_FALLTHROUGH_;
			case Vocabulary::_TemplateBody:
			{
				auto l_to_erase = it;
				++it;
				out_list_p.erase(l_to_erase);
			}
			continue;

			default:
				++it;
				continue;
			}
		}
	}

	void purge_forward_declaration(std::pmr::list<token>& out_list_p) noexcept
	{
		for (auto it = out_list_p.begin(); it != out_list_p.end();)
		{
			switch (it->_vocabulary)
			{
			case Vocabulary::_ClassStructEnumForwardDeclaration:
			{
				auto l_to_erase = it;
				++it;
				out_list_p.erase(l_to_erase);
			}
			continue;

			default:
				++it;
				continue;
			}
		}
	}


	_FE_NODISCARD_ token tokenize_identifiable(typename file_buffer_t::const_pointer code_iterator_p, context_stack_t& context_stack_p)
	{
		token l_token = 
		{ 
			._vocabulary = Vocabulary::_Undefined,
			._code = file_buffer_t(u8"\0", framework::get_framework().get_memory_resource())
		};


		// The top priority is marking out the comments.
		tokenize_comment(l_token, code_iterator_p, context_stack_p);
		if (l_token._vocabulary != Vocabulary::_Undefined)
		{
			return l_token; // return if the text is a comment.
		}

		tokenize_preprocessor(l_token, code_iterator_p, context_stack_p);
		if (l_token._vocabulary != Vocabulary::_Undefined)
		{
			return l_token; // return if the text is a preprocessor directive.
		}

		if (context_stack_p.back() != FHT::Context::_EnumStructFieldInitialValue)
		{
			tokenize_string_literal(l_token, code_iterator_p, context_stack_p);
			if (l_token._vocabulary != Vocabulary::_Undefined)
			{
				return l_token; // return if the text is a string literal.
			}
		}

		tokenize_template(l_token, code_iterator_p, context_stack_p);
		if (l_token._vocabulary != Vocabulary::_Undefined)
		{
			return l_token; // return if the text is a template declaration.
		}


		// tokenize operators.
		tokenize_other(l_token, code_iterator_p, context_stack_p);
		if (l_token._vocabulary != Vocabulary::_Undefined)
		{
			return l_token; // return if the text is an operator.
		}


		tokenize_reflection_macros(l_token, code_iterator_p, context_stack_p);
		if (l_token._vocabulary != Vocabulary::_Undefined)
		{
			return l_token;
		}


		tokenize_enum_struct(l_token, code_iterator_p, context_stack_p);
		if (l_token._vocabulary != Vocabulary::_Undefined)
		{
			return l_token;
		}

		tokenize_class(l_token, code_iterator_p, context_stack_p);
		if (l_token._vocabulary != Vocabulary::_Undefined)
		{
			return l_token;
		}

		tokenize_struct(l_token, code_iterator_p, context_stack_p);
		if (l_token._vocabulary != Vocabulary::_Undefined)
		{
			return l_token;
		}

		tokenize_namespace(l_token, code_iterator_p, context_stack_p);
		if (l_token._vocabulary != Vocabulary::_Undefined)
		{
			return l_token;
		}


		tokenize_template_body(l_token, code_iterator_p, context_stack_p);



		// tokenize union/functions as undefined
		l_token._code.reserve(100);
		while (*code_iterator_p != '{')
		{
			if (*code_iterator_p == ';')
			{
				l_token._vocabulary = Vocabulary::_AnyDecl;
				return l_token;
			}
			l_token._code += *code_iterator_p;
			++code_iterator_p;
		}
		file_buffer_t l_brace_stack(framework::get_framework().get_memory_resource());
		token l_tmp = { ._code{ framework::get_framework().get_memory_resource()} };

		do
		{
			switch (*code_iterator_p)
			{
			case '(':
				if ((l_tmp._vocabulary != Vocabulary::_CharLiteral)
					&& (l_tmp._vocabulary != Vocabulary::_StringLiteral))
				{
					l_brace_stack.push_back('(');
				}
				break;

			case ')':
				if ((l_tmp._vocabulary != Vocabulary::_CharLiteral)
					&& (l_tmp._vocabulary != Vocabulary::_StringLiteral))
				{
					if (l_brace_stack.back() == '(')
					{
						l_brace_stack.pop_back();
					}
				}
				break;


			case '{':
				if ((l_tmp._vocabulary != Vocabulary::_CharLiteral)
					&& (l_tmp._vocabulary != Vocabulary::_StringLiteral))
				{
					l_brace_stack.push_back('{');
				}
				break;

			case '}':
				if ((l_tmp._vocabulary != Vocabulary::_CharLiteral)
					&& (l_tmp._vocabulary != Vocabulary::_StringLiteral))
				{
					if (l_brace_stack.back() == '{')
					{
						l_brace_stack.pop_back();
					}
				}
				break;

			default:
				l_tmp._vocabulary = Vocabulary::_Undefined;
				tokenize_string_literal(l_tmp, code_iterator_p, context_stack_p);
				switch (l_tmp._vocabulary)
				{
				case Vocabulary::_CharLiteral:
					l_token._code += *code_iterator_p;
					++code_iterator_p;
					continue;

				case Vocabulary::_StringLiteral:
					l_token._code += l_tmp._code;
					code_iterator_p += l_tmp._code.length();
					l_tmp._code.clear();
					continue;

				default:
					break;
				}
				break;
			}
			l_token._code += *code_iterator_p;
			++code_iterator_p;
			THROW_CPP_SYNTAX_ERROR(*code_iterator_p == '\0', "C++ Code Syntax Error C1075: missing '}' in class declaration, or found an explicit null terminator \0");
		} 
		while (l_brace_stack.size() > 0);
		l_token._code += *code_iterator_p;
		l_token._vocabulary = Vocabulary::_AnyDecl;

		return l_token; 
	}

	_FE_NODISCARD_ token tokenize_unidentifiable(typename file_buffer_t::const_pointer code_iterator_p, context_stack_t& context_stack_p)
	{
		token l_token = 
		{
			._vocabulary = Vocabulary::_Undefined,
			._code = file_buffer_t(framework::get_framework().get_memory_resource())
		};
	
		for (;(tokenize_identifiable(code_iterator_p, context_stack_p)._vocabulary == Vocabulary::_Undefined) &&
			(*code_iterator_p > ' '); ++code_iterator_p)
		{
			l_token._code += *code_iterator_p;
		}
		return l_token;
	}


	void tokenize_comment(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p) noexcept
	{
		thread_local static std::string tl_s_key_buffer;

		auto l_prefix_iterators = g_vocabulary.equal_prefix_range_ks(FE::iterator_cast<FE::ASCII*>(code_iterator_p), 2);

		for (auto it = l_prefix_iterators.first; it != l_prefix_iterators.second; ++it) // iterate all candidates.
		{
			it.key(tl_s_key_buffer); // populate the key buffer.
			switch (it.value()) // Check if it matches the Vocabulary enum value.
			{
			case Vocabulary::_CommentBegin:
				if (FE::algorithm::string::compare_ranged(	(FE::ASCII*)code_iterator_p, FE::algorithm::string::range{ 0, tl_s_key_buffer.length() },
																tl_s_key_buffer.c_str(), FE::algorithm::string::range{ 0, tl_s_key_buffer.length() }) == true)
				{
					context_stack_p.emplace_back(FHT::Context::_CommentBlock);
					out_token_p._vocabulary = it.value();
					out_token_p._code = reinterpret_cast<FE::UTF8*>(tl_s_key_buffer.c_str());
					return;
				}
				break;


			case Vocabulary::_CommentEnd:
				if (FE::algorithm::string::compare_ranged((FE::ASCII*)code_iterator_p, FE::algorithm::string::range{ 0, tl_s_key_buffer.length() },
					tl_s_key_buffer.c_str(), FE::algorithm::string::range{ 0, tl_s_key_buffer.length() }) == true)
				{
					if (context_stack_p.back() == FHT::Context::_CommentBlock)
					{
						context_stack_p.pop_back();
						out_token_p._vocabulary = it.value();
						out_token_p._code = reinterpret_cast<FE::UTF8*>(tl_s_key_buffer.c_str());
						return;
					}
				}
				break;


			case Vocabulary::_LineComment:
				if (FE::algorithm::string::compare_ranged((FE::ASCII*)code_iterator_p, FE::algorithm::string::range{ 0, tl_s_key_buffer.length() },
					tl_s_key_buffer.c_str(), FE::algorithm::string::range{ 0, tl_s_key_buffer.length() }) == true)
				{
					out_token_p._vocabulary = it.value();
					auto l_rng = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, '\n');

					if (l_rng != std::nullopt) _FE_LIKELY_
					{
						out_token_p._code.assign(code_iterator_p, l_rng->_begin);
					}
					else
					{
						out_token_p._code = code_iterator_p; // EOF
					}
					return;
				}
				break;


			default:
				if (context_stack_p.back() == FHT::Context::_CommentBlock)
				{
					goto MarkAsComment;
				}
				break;
			}
		}

		if (context_stack_p.back() == FHT::Context::_CommentBlock)
		{
		MarkAsComment:
			out_token_p._vocabulary = Vocabulary::_CommentBody;
			auto l_rng = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, '\n');

			if (l_rng != std::nullopt) _FE_LIKELY_
			{
				out_token_p._code.assign(code_iterator_p, l_rng->_begin);
			}
			else
			{
				out_token_p._code = code_iterator_p; // EOF
			}

			auto l_pos = out_token_p._code.find(u8"*/");
			if (l_pos != std::string::npos)
			{
				out_token_p._code.erase(l_pos, out_token_p._code.size() - l_pos);
			}
			return;
		}
	}

	void tokenize_preprocessor(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p) noexcept
	{
		switch (context_stack_p.back())
		{
		case FHT::Context::_StringLiteral:
			_FE_FALLTHROUGH_;
		case FHT::Context::_CharLiteral:
			return; // Preprocessor directives are not recognized inside string literals or char literals.

		default:
			break;
		}

		switch (*code_iterator_p)
		{
		case '#':
			if (context_stack_p.back() != FHT::Context::_Preprocessor)
			{
				context_stack_p.emplace_back(FHT::Context::_Preprocessor);
			}
			out_token_p._vocabulary = Vocabulary::_PreprocessorDirective;
			out_token_p._code = *code_iterator_p;
			break;


		case '\\':
			if (context_stack_p.back() == FHT::Context::_Preprocessor)
			{
				out_token_p._vocabulary = Vocabulary::_PreprocessorNextLine;
				out_token_p._code = *code_iterator_p;
			}
			break;


		default:
			if (context_stack_p.back() == FHT::Context::_Preprocessor)
			{
				out_token_p._vocabulary = Vocabulary::_Preprocessor;
				auto l_rng = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, '\n');

				if (l_rng != std::nullopt) _FE_LIKELY_
				{
					out_token_p._code.assign(code_iterator_p, l_rng->_begin);
				}
				else
				{
					out_token_p._code = code_iterator_p; // EOF
				}
				
				if (out_token_p._code.back() == '\\')
				{
					out_token_p._code.pop_back();
				}
				else
				{
					context_stack_p.pop_back();
				}
			}
			break;
		}
	}


	void tokenize_string_literal(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p)
	{
		thread_local static file_buffer_t tl_s_delimiter;

		// tokenize _TextLiteralPrefix
		switch (*code_iterator_p)
		{
		case 'L':
			_FE_FALLTHROUGH_;
		case 'u':
			_FE_FALLTHROUGH_;
		case 'U': 
			_FE_FALLTHROUGH_;

		case 'R':
			{
				auto l_quote = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, '\"');
				auto l_line_end = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, '\n');


				if ((l_quote == std::nullopt) || (l_line_end == std::nullopt)) // NAh!
				{
					return;
				}


				if (!(l_quote->_begin < l_line_end->_begin)) // doesn't the first " comes before \n in the current line?
				{
					break;
				}


				out_token_p._code.assign(code_iterator_p, l_quote->_begin); // copy until ".

				while (out_token_p._code.back() == ' ') // the first character is the case value. Purge all trailing spaces between the prefix and the quote.
				{
					out_token_p._code.pop_back();
				}


				switch (out_token_p._code.length()) // test the length of the purged string.
				{
				case 1:
					out_token_p._vocabulary = Vocabulary::_TextLiteralPrefix;

					if (out_token_p._code.back() == 'R') // is R
					{
						context_stack_p.push_back(FHT::Context::_RawTextLiteral);
						extract_raw_text_delimiter_from_the_left_quote(tl_s_delimiter, code_iterator_p);
						return;
					}
					tl_s_delimiter.clear();
					return;


				case 2:
					if (out_token_p._code.back() == '8') // is u8
					{
						out_token_p._vocabulary = Vocabulary::_TextLiteralPrefix;
						tl_s_delimiter.clear();
						return;
					}

					if (out_token_p._code.back() == 'R') // is LR, uR, or UR
					{
						out_token_p._vocabulary = Vocabulary::_TextLiteralPrefix;
						context_stack_p.push_back(FHT::Context::_RawTextLiteral);
						extract_raw_text_delimiter_from_the_left_quote(tl_s_delimiter, code_iterator_p);
						return;
					}

					tl_s_delimiter.clear();
					out_token_p._code.clear(); // is not a valid prefix.
					break;


				case 3:
					if (out_token_p._code == u8"u8R")
					{
						out_token_p._vocabulary = Vocabulary::_TextLiteralPrefix;
						context_stack_p.push_back(FHT::Context::_RawTextLiteral);
						extract_raw_text_delimiter_from_the_left_quote(tl_s_delimiter, code_iterator_p);
						return;
					}

					tl_s_delimiter.clear();
					out_token_p._code.clear(); // is not a valid prefix.
					break;
					

				default:
					tl_s_delimiter.clear();
					out_token_p._code.clear(); // is not a valid prefix.
					break;
				}
			}
			break;


		default:
			break; 
		}




		switch (*code_iterator_p)
		{
		case '\"':
			switch (context_stack_p.back())
			{
			case FHT::Context::_StringLiteral:
				if (code_iterator_p[-1] != '\\')
				{
					context_stack_p.pop_back(); // is accessible when "".
				}
				break;

			case FHT::Context::_RawTextLiteral:
				if (tl_s_delimiter.length() > 0)
				{
					auto l_pos = code_iterator_p - tl_s_delimiter.length();
					if (FE::algorithm::string::compare_ranged(
						l_pos,
						FE::algorithm::string::range{ ._begin = 0, ._end = tl_s_delimiter.length() },

						tl_s_delimiter.c_str(),
						FE::algorithm::string::range{ ._begin = 0, ._end = tl_s_delimiter.length() }
					) == true)
					{
						context_stack_p.pop_back(); // is accessible when R"delimiter()delimiter"
						tl_s_delimiter.clear();
						break;
					}
				}

				if (code_iterator_p[-1] == ')') // does not have any delimiters; is the previous character ')'?
				{
					context_stack_p.pop_back(); // is accessible when R"()"
					break;
				}
				break;

			default:
				context_stack_p.emplace_back(FHT::Context::_StringLiteral);
				break;
			}

			out_token_p._vocabulary = Vocabulary::_StringLiteral;
			out_token_p._code = *code_iterator_p;
			break;


		case '\'':
			if (context_stack_p.back() == FHT::Context::_CharLiteral)
			{
				if (code_iterator_p[-1] != '\\') // is accessible when '' or '\''.
				{
					context_stack_p.pop_back();
				}
			}
			else
			{
				context_stack_p.emplace_back(FHT::Context::_CharLiteral);
			}
			out_token_p._vocabulary = Vocabulary::_CharLiteral;
			out_token_p._code = *code_iterator_p;
			break;


		default:
			switch (context_stack_p.back())
			{
			case FHT::Context::_CharLiteral:
				out_token_p._vocabulary = Vocabulary::_CharLiteral;
				out_token_p._code = *code_iterator_p;
				break;

			case FHT::Context::_StringLiteral:
				_FE_FALLTHROUGH_;
			case FHT::Context::_RawTextLiteral:
				out_token_p._vocabulary = Vocabulary::_StringLiteral;
				{
					auto l_rng = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, '\"');

					THROW_CPP_SYNTAX_ERROR(l_rng == std::nullopt, "FHT C++ Code Syntax Error C2001: the string literal is incomplete.")
					out_token_p._code.assign(code_iterator_p, l_rng->_begin);
				}
				break;
			}
			break;
		}
	}

	void extract_raw_text_delimiter_from_the_left_quote(file_buffer_t& out_return_p, typename file_buffer_t::const_pointer code_iterator_p)
	{
		auto l_start = code_iterator_p;
		while (*l_start != '\"')
		{
			THROW_CPP_SYNTAX_ERROR(*l_start == '\0', "C++ code syntax Error C2001: the raw text literal delimiter is incomplete.");
			++l_start;
		}
		++l_start; // skip "

		auto l_end = l_start;
		while (*l_end != '(')
		{
			THROW_CPP_SYNTAX_ERROR(*l_end == '\0', "C++ code syntax Error C2001: the raw text literal delimiter is incomplete.");
			++l_end;
		}

		out_return_p.assign(l_start, l_end);
	}


	void tokenize_template(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p)
	{
		constexpr FE::UTF8* l_template_keyword = u8"template";
		FE::algorithm::string::range l_template = { 0, FE::algorithm::string::length(l_template_keyword) };
		if (FE::algorithm::string::compare_ranged<FE::UTF8>(code_iterator_p, l_template,
			l_template_keyword, l_template)
			== true)
		{
			auto l_code_line_end = FE::algorithm::string::find_the_first(code_iterator_p, u8';');
			THROW_CPP_SYNTAX_ERROR(l_code_line_end == std::nullopt, "C++ code syntax Error C2143: the template declaration is incomplete; ; is missing.");

			if (FE::algorithm::string::space_insensitive_contains(code_iterator_p, l_code_line_end->_begin, u8"template<") == false)
			{
				// is a template class forward declaration: template class BasicTemplateTypename<int>;
				out_token_p._vocabulary = Vocabulary::_ClassStructEnumForwardDeclaration;
				out_token_p._code.assign(code_iterator_p, l_code_line_end->_end);
				return;
			}

			out_token_p._vocabulary = Vocabulary::_Template;
			out_token_p._code = l_template_keyword;
			context_stack_p.emplace_back(FHT::Context::_Template);
			return;
		}


		switch (context_stack_p.back())
		{
		case FHT::Context::_Template:
			if (*code_iterator_p == '<')
			{
				out_token_p._vocabulary = Vocabulary::_BeginTemplateArgs;
				out_token_p._code = *code_iterator_p;
				context_stack_p.emplace_back(FHT::Context::_TemplateArgs);
				return;
			}
			break;


		case FHT::Context::_TemplateArgs:
			if (*code_iterator_p == '>')
			{
				out_token_p._vocabulary = Vocabulary::_EndTemplateArgs;
				out_token_p._code = *code_iterator_p;
				context_stack_p.pop_back();

				if (context_stack_p.size() >= 3)
				{
					auto l_right_before_back = context_stack_p.begin() + (context_stack_p.size() - 3);
					if (*l_right_before_back == FHT::Context::_Template) // is the C++ 17 nested template template argument: template <template <typename T> class C> class C {};
					{
						context_stack_p.pop_back(); // pop the template template arg
					}
				}
				return;
			}
			else
			{
				auto l_end_args = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, '>');
				auto l_nested_begin_args = FE::algorithm::string::find_the_first_within_range<FE::UTF8>(code_iterator_p, FE::algorithm::string::range{ 0, l_end_args->_begin }, '<');

				THROW_CPP_SYNTAX_ERROR(l_end_args == std::nullopt, "C++ code syntax Error C2988: the template argument list is incomplete.");

				if (l_nested_begin_args == std::nullopt) // not found '<'
				{
					// found '>'; copy until '>'
					out_token_p._vocabulary = Vocabulary::_TemplateArg;
					out_token_p._code.assign(code_iterator_p, l_end_args->_begin);
					return;
				}

				out_token_p._vocabulary = Vocabulary::_TemplateArg;
				out_token_p._code.assign(code_iterator_p, l_nested_begin_args->_end); // found '<'; copy until '<'
				return;
			}


		default:
			break;
		}
	}


	void tokenize_other(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, _FE_MAYBE_UNUSED_ FHT::context_stack_t& context_stack_p) noexcept
	{
		switch (*code_iterator_p)
		{
		case '{':
			out_token_p._vocabulary = Vocabulary::_LeftCurlyBracket;
			out_token_p._code = *code_iterator_p;
			break;

		case '}':
			if (context_stack_p.back() == FHT::Context::_EnumStruct)
			{
				context_stack_p.pop_back();
			}
			out_token_p._vocabulary = Vocabulary::_RightCurlyBracket;
			out_token_p._code = *code_iterator_p;
			break;


		case '(':
			out_token_p._vocabulary = Vocabulary::_LeftParen;
			out_token_p._code = *code_iterator_p;
			break;

		case ')':
			out_token_p._vocabulary = Vocabulary::_RightParen;
			out_token_p._code = *code_iterator_p;
			break;


		case '[':
			out_token_p._vocabulary = Vocabulary::_LeftBracket;
			out_token_p._code = *code_iterator_p;
			break;

		case ']':
			out_token_p._vocabulary = Vocabulary::_RightBracket;
			out_token_p._code = *code_iterator_p;
			break;


		case ';':
			out_token_p._vocabulary = Vocabulary::_Semicolon;
			out_token_p._code = *code_iterator_p;
			break;

		case ',':
			out_token_p._vocabulary = Vocabulary::_Comma;
			out_token_p._code = *code_iterator_p;
			break;

		case '=':
			out_token_p._vocabulary = Vocabulary::_AssignmentOperator;
			out_token_p._code = *code_iterator_p;
			break;

		case '.':
			_FE_FALLTHROUGH_;
		case '+':
			_FE_FALLTHROUGH_;
		case '-':
			_FE_FALLTHROUGH_;
		case '*':
			_FE_FALLTHROUGH_;
		case '/':
			_FE_FALLTHROUGH_;
		case '%':
			_FE_FALLTHROUGH_;
		case '&':
			_FE_FALLTHROUGH_;
		case '|':
			_FE_FALLTHROUGH_;
		case '^':
			_FE_FALLTHROUGH_;
		case '!':
			_FE_FALLTHROUGH_;
		case '<':
			_FE_FALLTHROUGH_;
		case '>':
			out_token_p._vocabulary = Vocabulary::_Operator;
			out_token_p._code = *code_iterator_p;
			break;

		default:
			if (FE::algorithm::string::find_the_first_within_range<var::UTF8>(code_iterator_p, FE::algorithm::string::range{ 0,2 }, u8"::")
				!= std::nullopt)
			{
				out_token_p._vocabulary = Vocabulary::_NamespaceConcatenator;
				out_token_p._code = u8"::";
			}
			else if (*code_iterator_p == ':')
			{
				out_token_p._vocabulary = Vocabulary::_Colon;
				out_token_p._code = *code_iterator_p;
			}
			break;
		}
	}


	void tokenize_reflection_macros(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p)
	{
		auto l_prefix_iterators = g_vocabulary.equal_prefix_range_ks("FE_", 3);
		thread_local static std::string tl_s_key_buffer;

		for (auto it = l_prefix_iterators.first; it != l_prefix_iterators.second; ++it) // iterate all candidates.
		{
			it.key(tl_s_key_buffer); // populate the key buffer.
			switch (it.value())
			{
			case Vocabulary::_FrogmanEngineClassReflectionMacro:
				_FE_FALLTHROUGH_;

			case Vocabulary::_FrogmanEngineStructReflectionMacro:
				_FE_FALLTHROUGH_;

			case Vocabulary::_FrogmanEngineEnumStructReflectionMacro:
				_FE_FALLTHROUGH_;

			case Vocabulary::_FrogmanEngineSystemMacro:
				if (FE::algorithm::string::space_insensitive_contains((FE::ASCII*)code_iterator_p, tl_s_key_buffer.length(), tl_s_key_buffer.c_str()) == true)
				{
					out_token_p._vocabulary = it.value();
					out_token_p._code = reinterpret_cast<FE::UTF8*>(tl_s_key_buffer.c_str());

					if (out_token_p._vocabulary == Vocabulary::_FrogmanEngineSystemMacro)
					{
						context_stack_p.push_back(FHT::Context::_FrogmanEngineSystemMacro);
					}
					return;
				}
			}
			
		}


		if (context_stack_p.back() == FHT::Context::_FrogmanEngineSystemMacro)
		{
			thread_local static var::int32 tl_s_arg_index = 0;
			auto l_comma = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, ',');
			out_token_p._code.assign(code_iterator_p, l_comma->_begin);

			switch (tl_s_arg_index)
			{
			case 0:
				THROW_CPP_SYNTAX_ERROR(l_comma == std::nullopt, "FHT C++ Error: the FE_SYSTEM macro is ill-formed.");
				out_token_p._vocabulary = Vocabulary::_FrogmanEngineSystemArgSysCallPhase;
				++tl_s_arg_index;
				return;

			case 1:
				THROW_CPP_SYNTAX_ERROR(l_comma == std::nullopt, "FHT C++ Error: the FE_SYSTEM macro is ill-formed.");
				out_token_p._vocabulary = Vocabulary::_FrogmanEngineSystemArgTargetComponentType;
				++tl_s_arg_index;
				return;

			case 2:
				l_comma = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, ')');
				THROW_CPP_SYNTAX_ERROR(l_comma == std::nullopt, "FHT C++ Error: the FE_SYSTEM macro is ill-formed.");
				out_token_p._code.assign(code_iterator_p, l_comma->_begin);
				out_token_p._vocabulary = Vocabulary::_FrogmanEngineSystemArgWorldTagEnumType;
				++tl_s_arg_index;
				return;

			default: // EOR; reset.
				tl_s_arg_index = 0;
				break;
			}
			context_stack_p.pop_back();
		}
	}


	void tokenize_class(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p)
	{
		auto l_class_keyword_end_pos = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, u8'\n');
		out_token_p._code.assign(code_iterator_p, l_class_keyword_end_pos->_begin);

		if (FE::algorithm::string::space_insensitive_contains(out_token_p._code.c_str(), out_token_p._code.length(), u8"class") == false)
		{
			out_token_p._code.clear();
			return;
		}


		code_iterator_p += out_token_p._code.length();
		tokenize_class_struct_enum_forward_decl_and_using_namespace(out_token_p, code_iterator_p);
		if (out_token_p._vocabulary != Vocabulary::_Undefined)
		{
			return; // return if the text is a forward declaration.
		}


		tokenize_template_body(out_token_p, code_iterator_p, context_stack_p);


		out_token_p._vocabulary = Vocabulary::_Class;

		// copy until '{'
		while (*code_iterator_p != '{')
		{
			out_token_p._code += *code_iterator_p;
			++code_iterator_p;
		}

		// THROW_CPP_SYNTAX_ERROR(*code_iterator_p != ';', "C++ Code Syntax Error C2143: missing ';' after class declaration");
	}

	void tokenize_struct(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p)
	{
		auto l_struct_keyword_end_pos = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, u8'\n');
		out_token_p._code.assign(code_iterator_p, l_struct_keyword_end_pos->_begin);

		if (FE::algorithm::string::space_insensitive_contains(out_token_p._code.c_str(), out_token_p._code.length(), u8"struct") == false)
		{
			out_token_p._code.clear();
			return;
		}


		code_iterator_p += out_token_p._code.length();
		tokenize_class_struct_enum_forward_decl_and_using_namespace(out_token_p, code_iterator_p);
		if (out_token_p._vocabulary != Vocabulary::_Undefined)
		{
			return; // return if the text is a forward declaration.
		}


		tokenize_template_body(out_token_p, code_iterator_p, context_stack_p);


		out_token_p._vocabulary = Vocabulary::_Struct;

		// copy until '{'
		while (*code_iterator_p != '{')
		{
			out_token_p._code += *code_iterator_p;
			++code_iterator_p;
		}

		//THROW_CPP_SYNTAX_ERROR(*code_iterator_p != ';', "C++ Code Syntax Error C2143: missing ';' after class declaration");
	}

	void tokenize_enum_struct(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p)
	{
		if ((context_stack_p.back() != FHT::Context::_EnumStruct) && (context_stack_p.back() != FHT::Context::_EnumStructFieldInitialValue))
		{
			auto l_enum_keyword_end_pos = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, u8'\n');
			out_token_p._code.assign(code_iterator_p, l_enum_keyword_end_pos->_end);
			if (FE::algorithm::string::space_insensitive_contains(out_token_p._code.c_str(), out_token_p._code.length(), u8"enum struct") == false)
			{
				auto l_struct_keyword_end_pos = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p + l_enum_keyword_end_pos->_end, u8'\n');
				out_token_p._code.assign(code_iterator_p, l_enum_keyword_end_pos->_end + l_struct_keyword_end_pos->_end);
				if (FE::algorithm::string::space_insensitive_contains(out_token_p._code.c_str(), out_token_p._code.length(), u8"enum struct") == false)
				{
					out_token_p._code.clear();
					return; // not an enum struct.
				}
			}


			THROW_CPP_SYNTAX_ERROR(context_stack_p.back() == FHT::Context::_Template, "C++ Code Syntax Error C3113: an 'enum' cannot be a template");


			code_iterator_p += out_token_p._code.length();
			tokenize_class_struct_enum_forward_decl_and_using_namespace(out_token_p, code_iterator_p);
			if (out_token_p._vocabulary != Vocabulary::_Undefined)
			{
				return; // return if the text is a forward declaration.
			}

			// copy until '{'
			while (*code_iterator_p != '{')
			{
				out_token_p._code += *code_iterator_p;
				++code_iterator_p;
			}

			out_token_p._vocabulary = Vocabulary::_EnumStruct;
			context_stack_p.emplace_back(FHT::Context::_EnumStruct);
			return;
		}
		
		
		while (*code_iterator_p != '}')
		{
			if (*code_iterator_p == ',')
			{
				out_token_p._vocabulary = Vocabulary::_EnumStructField;
				return;
			}

			if (*code_iterator_p == '=')
			{
				context_stack_p.emplace_back(FHT::Context::_EnumStructFieldInitialValue);
				out_token_p._vocabulary = Vocabulary::_EnumStructField;
				return;
			}

			if (context_stack_p.back() == FHT::Context::_EnumStructFieldInitialValue)
			{
				while (*code_iterator_p != '\n')
				{
					if (*code_iterator_p == ',')
					{
						break;
					}

					token l_tmp;
					tokenize_string_literal(l_tmp, code_iterator_p, context_stack_p);

					if (l_tmp._vocabulary != Vocabulary::_CharLiteral)
					{
						if (*code_iterator_p == '}')
						{
							context_stack_p.pop_back();
							out_token_p._code += *code_iterator_p;
							++code_iterator_p;
							break;
						}

						out_token_p._code += *code_iterator_p;
						++code_iterator_p;
						continue;
					}

					while (l_tmp._vocabulary == Vocabulary::_CharLiteral)
					{
						out_token_p._code += l_tmp._code;
						l_tmp._code.clear(); 
						l_tmp._vocabulary = Vocabulary::_Undefined;
						++code_iterator_p;
						tokenize_string_literal(l_tmp, code_iterator_p, context_stack_p);
					} 
				
					++code_iterator_p;
				}
				context_stack_p.pop_back();
				out_token_p._vocabulary = Vocabulary::_EnumStructFieldInitialValue;
				return;
			}

			if ((*code_iterator_p <= ' ') == false)
			{
				out_token_p._code += *code_iterator_p;
			}
			++code_iterator_p;
		}
		out_token_p._vocabulary = Vocabulary::_EnumStructField;

		context_stack_p.pop_back();
	}

	void tokenize_namespace(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p)
	{
		auto l_potential_namespace = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, u8'\n');
		out_token_p._code.assign(code_iterator_p, l_potential_namespace->_begin);


		constexpr FE::UTF8* l_end_namespace_keyword = u8"END_NAMESPACE";
		if (out_token_p._code.find(l_end_namespace_keyword) != std::string::npos) // found
		{
			out_token_p._vocabulary = Vocabulary::_EndNamespace;
			return;
		}


		constexpr FE::UTF8* l_begin_namespace_keyword = u8"BEGIN_NAMESPACE";
		if (out_token_p._code.find(l_begin_namespace_keyword) != std::string::npos) // found
		{
			if (out_token_p._code.find('(') == std::string::npos) 
			{
				/* 
				'BEGIN_NAMESPACE 
				(' if true.

				*/
				// copy until ')'
				while (*code_iterator_p != ')')
				{
					THROW_CPP_SYNTAX_ERROR(*code_iterator_p == '\0', "C++ Code Syntax Error C1057: unrecognizable BEGIN_NAMESPACE macro usage.");

					out_token_p._code += *code_iterator_p;
					++code_iterator_p;
				}
				out_token_p._code += *code_iterator_p;
				out_token_p._vocabulary = Vocabulary::_BeginNamespace;
				return;
			}


			if (out_token_p._code.find(')') == std::string::npos)
			{
				/* 
				'BEGIN_NAMESPACE(
				)' if true.

				*/
				// copy until ')'
				while (*code_iterator_p != ')')
				{
					THROW_CPP_SYNTAX_ERROR(*code_iterator_p == '\0', "C++ Code Syntax Error C1057: unrecognizable BEGIN_NAMESPACE macro usage.");

					out_token_p._code += *code_iterator_p;
					++code_iterator_p;
				}
				out_token_p._code += *code_iterator_p;
				out_token_p._vocabulary = Vocabulary::_BeginNamespace;
				return;
			}


			// 'BEGIN_NAMESPACE()' otherwise.
			THROW_CPP_SYNTAX_ERROR(out_token_p._code.find('(') == std::string::npos, "C++ Code Syntax Error C1057: unrecognizable BEGIN_NAMESPACE macro usage.");
			auto l_pos = out_token_p._code.find(')');
			THROW_CPP_SYNTAX_ERROR(l_pos == std::string::npos, "C++ Code Syntax Error C1057: unrecognizable BEGIN_NAMESPACE macro usage.");
			++l_pos; // point after )
			out_token_p._code.resize(l_pos);
			out_token_p._vocabulary = Vocabulary::_BeginNamespace;
			return;
		}


		if (FE::algorithm::string::space_insensitive_contains(out_token_p._code.c_str(), out_token_p._code.length(), u8"namespace") == false)
		{
			out_token_p._code.clear();
			return; // not a namespace.
		}

		THROW_CPP_SYNTAX_ERROR(context_stack_p.back() == FHT::Context::_Template, "C++ Code Syntax Error C2988: unrecognizable template declaration/definition");


		code_iterator_p += out_token_p._code.length();
		tokenize_class_struct_enum_forward_decl_and_using_namespace(out_token_p, code_iterator_p);
		if (out_token_p._vocabulary != Vocabulary::_Undefined)
		{
			return; // return if the text is a using statement.
		}


		// copy until '{'
		while (*code_iterator_p != '{')
		{
			out_token_p._code += *code_iterator_p;
			++code_iterator_p;
		}

		out_token_p._vocabulary = Vocabulary::_Namespace;
	}

	void tokenize_class_struct_enum_forward_decl_and_using_namespace(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p)
	{
		const auto l_begin = code_iterator_p;

		while (*code_iterator_p != '{')
		{
			if (*code_iterator_p == ';')
			{
				out_token_p._vocabulary = Vocabulary::_ClassStructEnumForwardDeclaration;
				out_token_p._code.append(l_begin, code_iterator_p - l_begin);
				out_token_p._code += *code_iterator_p;
				return;
			}

			++code_iterator_p;
		}
	}

	void tokenize_template_body(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p)
	{
		if (context_stack_p.back() == FHT::Context::_Template)
		{
			out_token_p._vocabulary = Vocabulary::_TemplateBody;
			out_token_p._code.reserve(100);

			while (*code_iterator_p != '{')
			{
				out_token_p._code += *code_iterator_p;
				++code_iterator_p;
			}

			file_buffer_t l_brace_stack(framework::get_framework().get_memory_resource());

			token l_tmp = { ._code{ framework::get_framework().get_memory_resource()} };

			do
			{
				switch (*code_iterator_p)
				{
				case '{':
					if ((l_tmp._vocabulary != Vocabulary::_CharLiteral)
						&& (l_tmp._vocabulary != Vocabulary::_StringLiteral))
					{
						l_brace_stack.push_back('{');
					}
					break;

				case '}':
					if ((l_tmp._vocabulary != Vocabulary::_CharLiteral)
						&& (l_tmp._vocabulary != Vocabulary::_StringLiteral))
					{
						l_brace_stack.pop_back();
					}
					break;

				default:
					l_tmp._vocabulary = Vocabulary::_Undefined;
					tokenize_string_literal(l_tmp, code_iterator_p, context_stack_p);
					switch (l_tmp._vocabulary)
					{
					case Vocabulary::_CharLiteral:
						out_token_p._code += *code_iterator_p;
						++code_iterator_p;
						continue;

					case Vocabulary::_StringLiteral:
						out_token_p._code += l_tmp._code;
						code_iterator_p += l_tmp._code.length();
						l_tmp._code.clear();
						continue;

					default:
						break;
					}
					break;
				}
				out_token_p._code += *code_iterator_p;
				++code_iterator_p;
				THROW_CPP_SYNTAX_ERROR(*code_iterator_p == '\0', "C++ Code Syntax Error C1075: missing '}' in class declaration, or found an explicit null terminator \0");
			} while (l_brace_stack.size() > 0);
			context_stack_p.pop_back(); // pop the template context.
			THROW_CPP_SYNTAX_ERROR(*code_iterator_p != ';', "C++ Code Syntax Error C2143: missing ';' after class declaration");
			out_token_p._code += *code_iterator_p;
			return;
		}
	}
}