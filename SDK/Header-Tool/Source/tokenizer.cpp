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
#include "scope_context.hpp"
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
		l_context_stack.emplace_back(FHT::ScopeContext::_Global);

		std::pmr::list<token> l_list{ framework::get_framework().get_memory_resource() };

		auto l_end = file_p.c_str() + file_p.size();
		var::uint32 l_line_number = 1;
		for (FE::UTF8* iterator = FE::algorithm::string::skip_BOM(file_p.c_str()); iterator < l_end;)
		{
			if (*iterator <= ' ')
			{
				++iterator;
				continue;
			}


			token l_token = tokenize_identifiable(iterator, l_context_stack);
			l_token._header_file_path = path_p.c_str();
			l_token._line_number = l_line_number;
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
						l_token._line_number = l_line_number;

						l_list.push_back(std::move(l_token));
						++l_line_number; // Increment the line number.
						++iterator; // move to the next.
					}
				}
				continue;
			}


			l_token = tokenize_unidentifiable(iterator, l_context_stack);
			l_token._header_file_path = path_p.c_str();
			l_token._line_number = l_line_number;
			iterator += l_token._code.size(); // move to the next.
			l_list.push_back(std::move(l_token));

			if (iterator < l_end) _FE_LIKELY_
			{
				if (*iterator == '\n')
				{
					l_token._vocabulary = Vocabulary::_LineEnd;
					l_token._code = file_buffer_t(1, *iterator, framework::get_framework().get_memory_resource());
					l_token._header_file_path = path_p.c_str();
					l_token._line_number = l_line_number;

					l_list.push_back(std::move(l_token));
					++l_line_number; // Increment the line number.
					++iterator; // move to the next.
				}
			}
			continue;
		}

		l_list.emplace_back(Vocabulary::_EndOfCode, l_line_number, u8"\0");
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

		tokenize_string_literal(l_token, code_iterator_p, context_stack_p);
		if (l_token._vocabulary != Vocabulary::_Undefined)
		{
			return l_token; // return if the text is a string literal.
		}

		//auto l_prefix_iterator_range = g_vocabulary.equal_prefix_range_ks(FE::iterator_cast<FE::ASCII*>(code_iterator_p), 1);
		//thread_local static std::string tl_s_key_buffer;

		//for (auto it = l_prefix_iterator_range.first; it != l_prefix_iterator_range.second; ++it)
		//{
		//	it.key(tl_s_key_buffer);
		//	switch (it.value())
		//	{
		//	case Vocabulary::_Namespace:
		//		context_stack_p.emplace_back(ScopeContext::_Namespace);
		//		break;

		//	case Vocabulary::_Class:
		//		context_stack_p.emplace_back(ScopeContext::_Class);
		//		break;

		//	case Vocabulary::_Struct:
		//		context_stack_p.emplace_back(ScopeContext::_Struct);
		//		break;

		//	case Vocabulary::_Enum:
		//		context_stack_p.emplace_back(ScopeContext::_EnumStruct);
		//		break;

		//	case Vocabulary::_Template:
		//		context_stack_p.emplace_back(ScopeContext::_TemplateArgs);

		//		break;

		//	case Vocabulary::_StringLiteral:
		//		context_stack_p.emplace_back(ScopeContext::_StringLiteral);

		//		break;

		//	case Vocabulary::_CharLiteral:
		//		context_stack_p.emplace_back(ScopeContext::_CharLiteral);

		//		break;

		//	case Vocabulary::_PreprocessorDirective:
		//		context_stack_p.emplace_back(ScopeContext::_Preprocessor);
		//		
		//		break;
		//	}
		//}

		//tokenize_operator(l_token, code_iterator_p, context_stack_p);
		//if (l_token._vocabulary != Vocabulary::_Undefined)
		//{
		//	return l_token;
		//}

		//for (auto it = l_prefix_iterator_range.first; it != l_prefix_iterator_range.second; ++it)
		//{
		//	it.key(tl_s_key_buffer);
		//	switch (it.value())
		//	{
		//		// letter-non-contigous keywords.
		//	case Vocabulary::_Override:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Final:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Constexpr:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Consteval:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Constinit:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_EndNamespace:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Namespace:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Class:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Struct:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Enum:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Static:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_ThreadLocal:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Virtual:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Using:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_StaticAssert:
		//		if (verify_key_equivalence(code_iterator_p, tl_s_key_buffer.c_str(), context_stack_p) == true)
		//		{
		//			l_token._vocabulary = it.value();
		//			l_token._code = reinterpret_cast<FE::UTF8*>(tl_s_key_buffer.c_str());
		//			return l_token;
		//		}
		//		break;

		//	case Vocabulary::_CallingConvention: /* __cdecl __stdcall __fastcall __thiscall __vectorcall */
		//	{
		//		constexpr static std::array<FE::ASCII*, 5> l_calling_conventions =
		//		{
		//			"__cdecl", "__stdcall", "__fastcall", "__thiscall", "__vectorcall"
		//		};

		//		for (FE::ASCII* calling_convention : l_calling_conventions)
		//		{
		//			if (verify_key_equivalence(code_iterator_p, calling_convention, context_stack_p) == true)
		//			{
		//				l_token._vocabulary = it.value();
		//				l_token._code = reinterpret_cast<FE::UTF8*>(calling_convention);
		//				return l_token;
		//			}
		//		}
		//	}
		//	break;

		//	// letter-contigous keywords.
		//	case Vocabulary::_BeginNamespace:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_NamespaceConcatenator:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Template:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Private:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Protected:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Public:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Const:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Volatile:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Noexcept:
		//		_FE_FALLTHROUGH_;

		//	case Vocabulary::_FrogmanEngineBaseClassReflectionMacro:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_FrogmanEnginePropertyReflectionMacro:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_FrogmanEngineStaticMethodReflectionMacro:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_FrogmanEngineMethodReflectionMacro:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_FrogmanEngineEnumStructReflectionMacro:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_FrogmanEngineSystemAttributeMacro:
		//		if (FE::algorithm::string::compare_ranged((FE::ASCII*)code_iterator_p, FE::algorithm::string::range{ 0, tl_s_key_buffer.length() },
		//			tl_s_key_buffer.c_str(), FE::algorithm::string::range{ 0, tl_s_key_buffer.length() }) == true)
		//		{
		//			l_token._vocabulary = it.value();
		//			l_token._code = reinterpret_cast<FE::UTF8*>(tl_s_key_buffer.c_str());
		//			return l_token;
		//		}
		//		break;

		//	default:
		//		break;
		//	}
		//}


		//for (auto it = l_prefix_iterator_range.first; it != l_prefix_iterator_range.second; ++it)
		//{
		//	it.key(tl_s_key_buffer);
		//	switch (it.value())
		//	{
		//		// Miscellaneous keywords.
		//	case Vocabulary::_Semicolon:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Colon:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_Comma:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_CharLiteral:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_StringLiteral:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_PreprocessorNextLine:
		//		_FE_FALLTHROUGH_;
		//	case Vocabulary::_PreprocessorDirective:
		//		l_token._vocabulary = it.value();
		//		l_token._code = static_cast<FE::UTF8>(*tl_s_key_buffer.c_str());
		//		return l_token;

		//	default:
		//		break;
		//	}
		//}

		return l_token; // Vocabulary::_Undefined
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

		auto l_prefix_iterators = g_vocabulary.equal_prefix_range_ks(FE::iterator_cast<FE::ASCII*>(code_iterator_p), 1);

		for (auto it = l_prefix_iterators.first; it != l_prefix_iterators.second; ++it) // iterate all candidates.
		{
			it.key(tl_s_key_buffer); // populate the key buffer.
			switch (it.value()) // Check if it matches the Vocabulary enum value.
			{
			case Vocabulary::_CommentBegin:
				if (FE::algorithm::string::compare_ranged(	(FE::ASCII*)code_iterator_p, FE::algorithm::string::range{ 0, tl_s_key_buffer.length() },
																tl_s_key_buffer.c_str(), FE::algorithm::string::range{ 0, tl_s_key_buffer.length() }) == true)
				{
					context_stack_p.emplace_back(FHT::ScopeContext::_CommentBlock);
					out_token_p._vocabulary = it.value();
					out_token_p._code = reinterpret_cast<FE::UTF8*>(tl_s_key_buffer.c_str());
					return;
				}
				break;


			case Vocabulary::_CommentEnd:
				if (FE::algorithm::string::compare_ranged((FE::ASCII*)code_iterator_p, FE::algorithm::string::range{ 0, tl_s_key_buffer.length() },
					tl_s_key_buffer.c_str(), FE::algorithm::string::range{ 0, tl_s_key_buffer.length() }) == true)
				{
					if (context_stack_p.back() == FHT::ScopeContext::_CommentBlock)
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
				if (context_stack_p.back() == FHT::ScopeContext::_CommentBlock)
				{
					goto MarkAsComment;
				}
				break;
			}
		}

		if (context_stack_p.back() == FHT::ScopeContext::_CommentBlock)
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
		case FHT::ScopeContext::_StringLiteral:
			_FE_FALLTHROUGH_;
		case FHT::ScopeContext::_CharLiteral:
			return; // Preprocessor directives are not recognized inside string literals or char literals.

		default:
			break;
		}

		switch (*code_iterator_p)
		{
		case '#':
			if (context_stack_p.back() != FHT::ScopeContext::_Preprocessor)
			{
				context_stack_p.emplace_back(FHT::ScopeContext::_Preprocessor);
			}
			out_token_p._vocabulary = Vocabulary::_PreprocessorDirective;
			out_token_p._code = *code_iterator_p;
			break;


		case '\\':
			if (context_stack_p.back() == FHT::ScopeContext::_Preprocessor)
			{
				out_token_p._vocabulary = Vocabulary::_PreprocessorNextLine;
				out_token_p._code = *code_iterator_p;
			}
			break;


		default:
			if (context_stack_p.back() == FHT::ScopeContext::_Preprocessor)
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
						context_stack_p.push_back(FHT::ScopeContext::_RawTextLiteral);
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
						context_stack_p.push_back(FHT::ScopeContext::_RawTextLiteral);
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
						context_stack_p.push_back(FHT::ScopeContext::_RawTextLiteral);
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
			case FHT::ScopeContext::_StringLiteral:
				if (code_iterator_p[-1] != '\\')
				{
					context_stack_p.pop_back(); // is accessible when "".
				}
				break;

			case FHT::ScopeContext::_RawTextLiteral:
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
				context_stack_p.emplace_back(FHT::ScopeContext::_StringLiteral);
				break;
			}

			out_token_p._vocabulary = Vocabulary::_StringLiteral;
			out_token_p._code = *code_iterator_p;
			break;


		case '\'':
			if (context_stack_p.back() == FHT::ScopeContext::_CharLiteral)
			{
				if (code_iterator_p[-1] != '\\') // is accessible when '' or '\''.
				{
					context_stack_p.pop_back();
				}
			}
			else
			{
				context_stack_p.emplace_back(FHT::ScopeContext::_CharLiteral);
			}
			out_token_p._vocabulary = Vocabulary::_CharLiteral;
			out_token_p._code = *code_iterator_p;
			break;


		default:
			switch (context_stack_p.back())
			{
			case FHT::ScopeContext::_CharLiteral:
				out_token_p._vocabulary = Vocabulary::_CharLiteral;
				out_token_p._code = *code_iterator_p;
				break;

			case FHT::ScopeContext::_StringLiteral:
				_FE_FALLTHROUGH_;
			case FHT::ScopeContext::_RawTextLiteral:
				out_token_p._vocabulary = Vocabulary::_StringLiteral;
				{
					auto l_rng = FE::algorithm::string::find_the_first<FE::UTF8>(code_iterator_p, '\"');

					THROW_CPP_SYNTAX_ERROR(l_rng == std::nullopt, "C++ code syntax Error C2001: the string literal is incomplete.")
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

	void tokenize_operator(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, FHT::context_stack_t& context_stack_p) noexcept
	{
		auto l_prefix_iterator_range = g_vocabulary.equal_prefix_range_ks(FE::iterator_cast<FE::ASCII*>(code_iterator_p), 1);
		thread_local static std::string tl_s_key_buffer;
		(void)context_stack_p;
		for (auto it = l_prefix_iterator_range.first; it != l_prefix_iterator_range.second; ++it)
		{
			it.key(tl_s_key_buffer);
			switch (it.value())
			{
			case Vocabulary::_LeftParen:
				_FE_FALLTHROUGH_;
			case Vocabulary::_RightParen:
				_FE_FALLTHROUGH_;
			case Vocabulary::_LeftBracket:
				_FE_FALLTHROUGH_;
			case Vocabulary::_RightBracket:
				_FE_FALLTHROUGH_;
			case Vocabulary::_LeftCurlyBracket:
				_FE_FALLTHROUGH_;
			case Vocabulary::_RightCurlyBracket:
				if (FE::algorithm::string::compare_ranged((FE::ASCII*)code_iterator_p, FE::algorithm::string::range{ 0, tl_s_key_buffer.length() },
					tl_s_key_buffer.c_str(), FE::algorithm::string::range{ 0, tl_s_key_buffer.length() }) == true)
				{
					out_token_p._vocabulary = it.value();
					out_token_p._code = reinterpret_cast<FE::UTF8*>(tl_s_key_buffer.c_str());
					return;
				}
				break;

			default:
				break;
			}
		}

		switch (*code_iterator_p)
		{
		case '=':
			out_token_p._vocabulary = Vocabulary::_AssignmentOperator;
			out_token_p._code = *code_iterator_p;
			return;

		case '*':
			out_token_p._vocabulary = Vocabulary::_Pointer;
			out_token_p._code = *code_iterator_p;
			return;

		case '<':
			out_token_p._vocabulary = Vocabulary::_BeginTemplateArgs;
			out_token_p._code = *code_iterator_p;
			return;

		case '>':
			out_token_p._vocabulary = Vocabulary::_EndTemplateArgs;
			out_token_p._code = *code_iterator_p;
			return;

		case '&':
			out_token_p._vocabulary = Vocabulary::_Reference;
			out_token_p._code = *code_iterator_p;
			return;

		case '+':
			_FE_FALLTHROUGH_;
		case '-':
			_FE_FALLTHROUGH_;
		case '/':
			_FE_FALLTHROUGH_;
		case '%':
			_FE_FALLTHROUGH_;
		case '!':
			_FE_FALLTHROUGH_;
		case '|':
			_FE_FALLTHROUGH_;
		case '^':
			_FE_FALLTHROUGH_;
		case '?':
			_FE_FALLTHROUGH_;
		case '.':
			out_token_p._vocabulary = Vocabulary::_Operator;
			out_token_p._code = *code_iterator_p;
			return;

		default:
			break;
		}
	}

	_FE_NODISCARD_ FE::boolean verify_key_equivalence(typename file_buffer_t::const_pointer subject_p, FE::ASCII* key_p, FHT::context_stack_t& context_stack_p) noexcept
	{
		static_assert(std::is_same_v<typename file_buffer_t::value_type, var::UTF8>, "static assertion failure, the header files must be encoded in UTF8.");
		(void)context_stack_p;
		FE::uint64 l_length = FE::algorithm::string::length(key_p);

		if ((subject_p[-1] <= ' ') && (subject_p[l_length] <= ' ') ||
			(subject_p[-1] == FHT::file_io::UTF8_BOM[2]) && (subject_p[l_length] <= ' '))
		{
			if (FE::algorithm::string::compare_ranged((FE::ASCII*)subject_p, FE::algorithm::string::range{ 0, l_length },
				key_p, FE::algorithm::string::range{ 0, l_length }) == true)
			{
				return true;
			}
		}
		return false;
	}
}