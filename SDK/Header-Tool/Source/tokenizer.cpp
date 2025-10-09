/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "header_tool_engine.hpp"




_FE_NODISCARD_ std::optional<std::pmr::list<token>> header_tool_engine::__tokenize_header(const file_buffer_t& file_p, const directory_t& path_p) noexcept
{
	if (file_p.empty() == true)
	{
		return std::nullopt;
	}

	std::pmr::list<token> l_list(get_memory_resource());

	auto l_end = file_p.c_str() + file_p.size();
	token l_token;
	var::uint32 l_line_number = 1;
	for (FE::UTF8* iterator = FE::algorithm::string::skip_BOM(file_p.c_str()); iterator < l_end;)
	{
		if (*iterator <= ' ')
		{
			if (*iterator == '\n')
			{
				l_token._vocabulary = Vocabulary::_LineEnd;
				l_token._line_number = l_line_number; // Set the line number for the token.
			    l_token._code = file_buffer_t(get_memory_resource());
				l_token._code = *iterator;
				l_token._header_file_path = path_p.c_str(); // Set the header file path for the token.
				l_list.push_back( std::move(l_token) );
				++l_line_number; // Increment the line number.
			}
			++iterator;
			continue;
		}

		l_token = __tokenize_identifiable(iterator);
		l_token._line_number = l_line_number; 
		l_token._header_file_path = path_p.c_str();

		if (l_token._vocabulary == Vocabulary::_Undefined)
		{
			//l_list.emplace_back(Vocabulary::_ContractedSpace, l_line_number, u8" ", path_p.c_str());
			l_token = __tokenize_unidentifiable(iterator);
			l_token._line_number = l_line_number;
			l_token._header_file_path = path_p.c_str();
			iterator += l_token._code.size();
			l_list.push_back( std::move(l_token) );
			continue;
		}

		iterator += l_token._code.size();
		l_list.push_back( std::move(l_token) ); // push_back the defined vocab.
	}

	l_list.emplace_back( Vocabulary::_EndOfCode, l_line_number, u8"\0" );
	return l_list;
}

// const char* p = "/* text */", f = "//text"; the 'text' is recognized as comments by FHT are purged from the token list.
void header_tool_engine::__purge_comments(std::pmr::list<token>& out_list_p) noexcept
{
	const auto l_is_comment_begin = [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_CommentBegin; };
	const auto l_is_comment_end = [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_CommentEnd; };

	auto l_comment_begin = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_comment_begin);
	auto l_comment_end = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_comment_end);

	while ((l_comment_begin != out_list_p.end()) && (l_comment_end != out_list_p.end()))
	{
		//FE_ASSERT(l_comment_begin <= l_comment_end, "Assertion failure: the token iterators are transposed.");
		out_list_p.erase(l_comment_begin, std::next(l_comment_end, 1) /* + 1 includes the last element to be deleted. */);
		l_comment_begin = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_comment_begin);
		l_comment_end = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_comment_end);
	}

	const auto l_is_line_comment = [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineComment; };
	const auto l_is_line_end = [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; };

	l_comment_begin = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_line_comment);
	l_comment_end = std::find_if(l_comment_begin, out_list_p.end(), l_is_line_end);

	while ((l_comment_begin != out_list_p.end()) && (l_comment_end != out_list_p.end()))
	{
		//FE_ASSERT(l_comment_begin <= l_comment_end, "Assertion failure: the token iterators are transposed.");
		out_list_p.erase(l_comment_begin, l_comment_end);
		l_comment_begin = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_line_comment);
		l_comment_end = std::find_if(l_comment_begin, out_list_p.end(), l_is_line_end);
	}
}

void header_tool_engine::__purge_string_literals(std::pmr::list<token>& out_list_p) noexcept
{
	using iterator = typename std::pmr::list<token>::iterator;

	const auto l_is_string_literal = [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_StringLiteral; };
	iterator l_begin = out_list_p.begin();
	iterator l_end = out_list_p.begin();;

	while (true)
	{
		l_begin = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_string_literal);
		if (l_begin == out_list_p.end())
		{
			break;
		}

		l_end = std::find_if(std::next(l_begin), out_list_p.end(), l_is_string_literal);
		if (l_end == out_list_p.end())
		{
			break; 
		}

		while ( std::prev(l_end)->_code == u8"\\" )
		{
			l_end = std::find_if(std::next(l_end), out_list_p.end(), l_is_string_literal);
			if (l_end == out_list_p.end())
			{
				break; 
			}
		}
		out_list_p.erase(l_begin, l_end);
		out_list_p.erase(l_end); // erase the last string literal.
	}
}

void header_tool_engine::__purge_preprocessor_directives(std::pmr::list<token>& out_list_p)
{
	using range = std::pair< typename std::pmr::list<token>::iterator, typename std::pmr::list<token>::iterator >;
	std::pmr::vector<range> l_ranges(get_memory_resource());
	
	for (auto it = out_list_p.begin(); it != out_list_p.end(); ++it)
	{
		if (it->_vocabulary == Vocabulary::_PreprocessorDirective)
		{
			//std::cout << "Removing the preprocessor directive:\n";
			range l_range;
			l_range.first = it; // Start of the preprocessor directive.

			auto l_line_end_indicator = std::find_if( l_range.first, out_list_p.end(), [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; } );
			auto l_preprocessor_nextline_indicator = std::find_if( l_range.first, out_list_p.end(), [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_PreprocessorNextLine; } );

			if ( l_preprocessor_nextline_indicator != out_list_p.end())
			{
				var::ptrdiff l_line_end_indicator_offset = std::distance(l_range.first, l_line_end_indicator);
				var::ptrdiff l_preprocessor_nextline_indicator_offset = std::distance(l_range.first, l_preprocessor_nextline_indicator);

				// while "#directive \ \n" is true.
				while (l_preprocessor_nextline_indicator_offset < l_line_end_indicator_offset)
				{
					// conttinue searching for the next preprocessor directive.
					l_line_end_indicator = std::find_if( std::next(l_line_end_indicator, 1), out_list_p.end(), [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; } );
					l_preprocessor_nextline_indicator = std::find_if( std::next(l_preprocessor_nextline_indicator, 1), out_list_p.end(), [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_PreprocessorNextLine; } );

					if (l_preprocessor_nextline_indicator == out_list_p.end())
					{
						break; // No more preprocessor directives.
					}

					l_line_end_indicator_offset = std::distance(l_range.first, l_line_end_indicator);
					l_preprocessor_nextline_indicator_offset = std::distance(l_range.first, l_preprocessor_nextline_indicator);
				}
			}

			l_range.second = l_line_end_indicator; // End of the preprocessor directive.
			l_ranges.push_back(l_range);
			it = l_line_end_indicator; // Move the iterator to the end of the preprocessor directive.

			//// for debugging purpose.
			//while (l_range.first != l_range.second)
			//{
			//	std::cout << reinterpret_cast<const char*>(l_range.first->_code.c_str()) << "\n";
			//	++l_range.first;
			//}
			//std::cout << "\n";
		}

		if (it == out_list_p.end())
		{
			break;
		}
	}
	
	for (range& l_range : l_ranges)
	{
		if (l_range.first != out_list_p.end() && l_range.second != out_list_p.end())
		{
			out_list_p.erase(l_range.first, l_range.second);
		}
	}
} 

token header_tool_engine::__tokenize_identifiable(typename file_buffer_t::const_pointer code_iterator_p) noexcept
{
	token l_token;
	l_token._vocabulary = Vocabulary::_Undefined;
	l_token._code = file_buffer_t(u8"\0", get_memory_resource());

	// The top priority is filtering out the comments.
	__tokenize_comment(l_token, code_iterator_p);
	if (l_token._vocabulary != Vocabulary::_Undefined)
	{
		return l_token;
	}

	__tokenize_operator(l_token, code_iterator_p);
	if (l_token._vocabulary != Vocabulary::_Undefined)
	{
		return l_token;
	}

	auto l_prefix_iterator_range = g_vocabulary.equal_prefix_range_ks(FE::iterator_cast<FE::ASCII*>(code_iterator_p), 1);
	thread_local static std::string tl_s_key_buffer;

	for (auto it = l_prefix_iterator_range.first; it != l_prefix_iterator_range.second; ++it)
	{
		it.key(tl_s_key_buffer);
		switch (it.value())
		{
		// letter-non-contigous keywords.
		case Vocabulary::_Override:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Final:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Constexpr:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Consteval:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Constinit:
			_FE_FALLTHROUGH_;
		case Vocabulary::_EndNamespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Namespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Class:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Struct:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Enum:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Static:
			_FE_FALLTHROUGH_;
		case Vocabulary::_ThreadLocal:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Virtual:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Using:
			_FE_FALLTHROUGH_;
		case Vocabulary::_StaticAssert:
			if (__verify_key_equivalence(code_iterator_p, tl_s_key_buffer.c_str()) == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = reinterpret_cast<FE::UTF8*>(tl_s_key_buffer.c_str());
				return l_token;
			}
			break;

		case Vocabulary::_CallingConvention: /* __cdecl __stdcall __fastcall __thiscall __vectorcall */
			{
				constexpr static std::array<FE::ASCII*, 5> l_calling_conventions = 
				{
					"__cdecl", "__stdcall", "__fastcall", "__thiscall", "__vectorcall"
				};

				for (FE::ASCII* calling_convention : l_calling_conventions)
				{
					if (__verify_key_equivalence(code_iterator_p, calling_convention) == true)
					{
						l_token._vocabulary = it.value();
						l_token._code = reinterpret_cast<FE::UTF8*>(calling_convention);
						return l_token;
					}
				}
			}
			break;

		// letter-contigous keywords.
		case Vocabulary::_BeginNamespace:
			_FE_FALLTHROUGH_;
		case Vocabulary::_NamespaceConcatenator:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Template:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Private:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Protected:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Public:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Const:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Volatile:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Noexcept:
			_FE_FALLTHROUGH_;

		case Vocabulary::_FrogmanEngineBaseClassReflectionMacro:
			_FE_FALLTHROUGH_;
		case Vocabulary::_FrogmanEnginePropertyReflectionMacro:
			_FE_FALLTHROUGH_;
		case Vocabulary::_FrogmanEngineStaticMethodReflectionMacro:
			_FE_FALLTHROUGH_;
		case Vocabulary::_FrogmanEngineMethodReflectionMacro:
			_FE_FALLTHROUGH_;
		case Vocabulary::_FrogmanEngineEnumStructReflectionMacro:
			if (FE::algorithm::string::compare_ranged( (FE::ASCII*)code_iterator_p, FE::algorithm::string::range{ 0, tl_s_key_buffer.length() },
													   tl_s_key_buffer.c_str(), FE::algorithm::string::range{ 0, tl_s_key_buffer.length() } ) == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = reinterpret_cast<FE::UTF8*>(tl_s_key_buffer.c_str());
				return l_token;
			}
			break;

		default:
			break;
		}
	}


	for (auto it = l_prefix_iterator_range.first; it != l_prefix_iterator_range.second; ++it)
	{
		it.key(tl_s_key_buffer);
		switch (it.value())
		{
			// Miscellaneous keywords.
		case Vocabulary::_Semicolon:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Colon:
			_FE_FALLTHROUGH_;
		case Vocabulary::_Comma:
			_FE_FALLTHROUGH_;
		case Vocabulary::_CharLiteral:
			_FE_FALLTHROUGH_;
		case Vocabulary::_StringLiteral:
			_FE_FALLTHROUGH_;
		case Vocabulary::_PreprocessorNextLine:
			_FE_FALLTHROUGH_;
		case Vocabulary::_PreprocessorDirective:
			l_token._vocabulary = it.value();
			l_token._code = static_cast<FE::UTF8>(*tl_s_key_buffer.c_str());
			return l_token;

		default:
			break;
		}
	}

	return l_token; // Vocabulary::_Undefined
} 

token header_tool_engine::__tokenize_unidentifiable(typename file_buffer_t::const_pointer code_iterator_p) noexcept
{
	token l_token; 
	l_token._vocabulary = Vocabulary::_Undefined;
	l_token._code = file_buffer_t(get_memory_resource());

	while ((__tokenize_identifiable(code_iterator_p)._vocabulary == Vocabulary::_Undefined) &&
		   (*code_iterator_p > ' '))
	{
		l_token._code += *code_iterator_p;
		++code_iterator_p;
	}
	return l_token;
}

void header_tool_engine::__tokenize_comment(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p) noexcept
{
	// Check if the string is a comment.
	auto l_prefix_iterator_range = g_vocabulary.equal_prefix_range_ks(FE::iterator_cast<FE::ASCII*>(code_iterator_p), 1);
	thread_local static std::string tl_s_key_buffer;

	for (auto it = l_prefix_iterator_range.first; it != l_prefix_iterator_range.second; ++it)
	{
		it.key(tl_s_key_buffer);
		switch (it.value())
		{
		case Vocabulary::_CommentBegin:
			_FE_FALLTHROUGH_;
		case Vocabulary::_CommentEnd:
			_FE_FALLTHROUGH_;
		case Vocabulary::_LineComment:
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
}

void header_tool_engine::__tokenize_operator(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p) noexcept
{
	auto l_prefix_iterator_range = g_vocabulary.equal_prefix_range_ks(FE::iterator_cast<FE::ASCII*>(code_iterator_p), 1);
	thread_local static std::string tl_s_key_buffer;

	for (auto it = l_prefix_iterator_range.first; it != l_prefix_iterator_range.second; ++it)
	{
		it.key(tl_s_key_buffer);
		switch (it.value())
		{
		case Vocabulary::_Access:
			_FE_FALLTHROUGH_;
		case Vocabulary::_AddAssignmentOperator:
			_FE_FALLTHROUGH_;
		case Vocabulary::_SubAssignmentOperator:
			_FE_FALLTHROUGH_;
		case Vocabulary::_MulAssignmentOperator:
			_FE_FALLTHROUGH_;
		case Vocabulary::_DivAssignmentOperator:
			_FE_FALLTHROUGH_;
		case Vocabulary::_ModAssignmentOperator:
			_FE_FALLTHROUGH_;
		case Vocabulary::_BitwiseAndAssignmentOperator:
			_FE_FALLTHROUGH_;
		case Vocabulary::_BitwiseOrAssignmentOperator:
			_FE_FALLTHROUGH_;
		case Vocabulary::_BitwiseXorAssignmentOperator:
			_FE_FALLTHROUGH_;
		case Vocabulary::_BitShiftAssignmentOperator:
			_FE_FALLTHROUGH_;
		case Vocabulary::_LogicalAnd:
			_FE_FALLTHROUGH_;
		case Vocabulary::_IsEqualTo:
			_FE_FALLTHROUGH_;
		case Vocabulary::_IsNotEqualTo:
			_FE_FALLTHROUGH_;
		case Vocabulary::_IsLessThanOrEqualTo:
			_FE_FALLTHROUGH_;
		case Vocabulary::_IsGreaterThanOrEqualTo:
			_FE_FALLTHROUGH_;
		case Vocabulary::_BitShiftOperator:
			_FE_FALLTHROUGH_;
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

_FE_NODISCARD_ FE::boolean header_tool_engine::__verify_key_equivalence(typename file_buffer_t::const_pointer subject_p, FE::ASCII* key_p) noexcept
{
	static_assert(std::is_same_v<typename file_buffer_t::value_type, var::UTF8>, "static assertion failure, the header files must be encoded in UTF8.");

	FE::uint64 l_length = FE::algorithm::string::length(key_p);

	if ((subject_p[-1] <= ' ') && (subject_p[l_length] <= ' ') ||
		(subject_p[-1] == m_UTF8_with_BOM[2]) && (subject_p[l_length] <= ' '))
	{
		if (FE::algorithm::string::compare_ranged((FE::ASCII*)subject_p, FE::algorithm::string::range{ 0, l_length },
			key_p, FE::algorithm::string::range{ 0, l_length }) == true)
		{
			return true;
		}
	}
	return false;
}