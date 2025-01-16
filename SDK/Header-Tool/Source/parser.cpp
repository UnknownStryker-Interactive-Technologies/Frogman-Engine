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
#include "header_tool_engine.hpp"




_FE_NODISCARD_ std::optional<std::pmr::vector<token>> header_tool_engine::__parse_header(const file_buffer_t& file_p) noexcept
{
	std::pmr::vector<token> l_list(get_memory_resource());
	l_list.reserve(2048);

	if (10 > file_p.size())
	{
		return std::nullopt;
	}

	auto l_end = file_p.c_str() + file_p.size();
	token l_token{ Vocabulary::_Undefined, file_buffer_t(get_memory_resource()) };

	for (auto iterator = FE::algorithm::string::skip_BOM(file_p.c_str()); iterator < l_end;)
	{
		switch (*iterator)
		{
		case ' ':
			_FE_FALLTHROUGH_;
		case '\t':
			_FE_FALLTHROUGH_;
		case '\r':
			++iterator;
			continue;

		default:
			break;
		}

		l_token = __tokenize(iterator);

		if (l_token._vocabulary == Vocabulary::_Undefined)
		{
			l_token = __tokenize_undefined(iterator);
			iterator += l_token._code.size();
			l_list.push_back(std::move(l_token));
			continue;
		}

		iterator += l_token._code.size();
		l_list.push_back(std::move(l_token)); // push_back the defined vocab.
	}

	l_list.emplace_back(Vocabulary::_EndOfCode, u8"\0");
	return std::make_optional<std::pmr::vector<token>>(l_list);
}

void header_tool_engine::__purge_comments(std::pmr::vector<token>& out_list_p) noexcept
{
	const auto l_is_comment_begin = [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_CommentBegin; };
	const auto l_is_comment_end = [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_CommentEnd; };

	auto l_comment_begin = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_comment_begin);
	auto l_comment_end = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_comment_end);

	while ((l_comment_begin != out_list_p.end()) && (l_comment_end != out_list_p.end()))
	{
		out_list_p.erase(l_comment_begin, l_comment_end + 1 /* + 1 includes the last element to be deleted. */);
		l_comment_begin = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_comment_begin);
		l_comment_end = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_comment_end);
	}


	const auto l_is_line_comment = [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineComment; };
	const auto l_is_line_end = [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; };

	l_comment_begin = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_line_comment);
	l_comment_end = std::find_if(l_comment_begin, out_list_p.end(), l_is_line_end);

	while ((l_comment_begin != out_list_p.end()) && (l_comment_end != out_list_p.end()))
	{
		out_list_p.erase(l_comment_begin, l_comment_end);
		l_comment_begin = std::find_if(out_list_p.begin(), out_list_p.end(), l_is_line_comment);
		l_comment_end = std::find_if(l_comment_begin, out_list_p.end(), l_is_line_end);
	}
}

_FE_NODISCARD_ token header_tool_engine::__tokenize(typename file_buffer_t::const_pointer code_iterator_p) noexcept
{
	token l_token{ Vocabulary::_Undefined, file_buffer_t(get_memory_resource()) };

	// The top priority is filtering out the comments.
	l_token = __tokenize_comment(code_iterator_p);
	if (l_token._vocabulary != Vocabulary::_Undefined)
	{
		return l_token;
	}

	l_token = __tokenize_reflection_relevant(code_iterator_p);
	if (l_token._vocabulary != Vocabulary::_Undefined)
	{
		return l_token;
	}

	l_token = __tokenize_operator(code_iterator_p);
	if (l_token._vocabulary != Vocabulary::_Undefined)
	{
		return l_token;
	}

	auto l_char = g_vocabulary.equal_prefix_range_ks(FE::iterator_cast<FE::ASCII*>(code_iterator_p), 1);

	for (auto it = l_char.first; it != l_char.second; ++it)
	{
		switch (it.value())
		{
		case Vocabulary::_Override:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "override") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"override";
				return l_token;
			}
			break;

		case Vocabulary::_Final:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "final") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"final";
				return l_token;
			}
			break;

		case Vocabulary::_CharLiteral:
			l_token._vocabulary = Vocabulary::_CharLiteral;
			l_token._code = '\'';
			return l_token;

		case Vocabulary::_StringLiteral:
			l_token._vocabulary = Vocabulary::_StringLiteral;
			l_token._code = '\"';
			return l_token;

		default:
			break;
		}
	}

	return l_token;
}

_FE_NODISCARD_ token header_tool_engine::__tokenize_undefined(typename file_buffer_t::const_pointer code_iterator_p) noexcept
{
	token l_token{ Vocabulary::_Undefined, file_buffer_t(get_memory_resource()) };

	while (__tokenize(code_iterator_p)._vocabulary == Vocabulary::_Undefined)
	{
		switch (*code_iterator_p)
		{
		case ' ':
			_FE_FALLTHROUGH_;
		case '\t':
			_FE_FALLTHROUGH_;
		case '\r':
			_FE_FALLTHROUGH_;
		case '\0':
			_FE_FALLTHROUGH_;
		case '\n':
			return l_token;

		default:
			break;
		}
		l_token._code += *code_iterator_p;
		++code_iterator_p;
	}

	return l_token;
}

_FE_NODISCARD_ token header_tool_engine::__tokenize_comment(typename file_buffer_t::const_pointer code_iterator_p) noexcept
{
	token l_token{ Vocabulary::_Undefined, file_buffer_t(get_memory_resource()) };

	// Check if the string is a comment.
	auto l_char = g_vocabulary.equal_prefix_range_ks(FE::iterator_cast<FE::ASCII*>(code_iterator_p), 1);

	for (auto it = l_char.first; it != l_char.second; ++it)
	{
		switch (it.value())
		{
		case Vocabulary::_CommentBegin:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "/*") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"/*";
				return l_token;
			}
			break;

		case Vocabulary::_CommentEnd:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "*/") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"*/";
				return l_token;
			}
			break;

		case Vocabulary::_LineComment:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "//") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"//";
				return l_token;
			}
			break;

		case Vocabulary::_LineEnd:
			l_token._vocabulary = it.value();
			l_token._code = u8'\n';
			return l_token;

		default:
			break;
		}
	}

	return l_token;
}

_FE_NODISCARD_ token header_tool_engine::__tokenize_reflection_relevant(typename file_buffer_t::const_pointer code_iterator_p) noexcept
{
	token l_token{ Vocabulary::_Undefined, file_buffer_t(get_memory_resource()) };

	auto l_char = g_vocabulary.equal_prefix_range_ks(FE::iterator_cast<FE::ASCII*>(code_iterator_p), 1);

	for (auto it = l_char.first; it != l_char.second; ++it)
	{
		switch (it.value())
		{
		case Vocabulary::_BeginNamespace:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "BEGIN_NAMESPACFE") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"BEGIN_NAMESPACE";
				return l_token;
			}
			break;

		case Vocabulary::_EndNamespace:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "END_NAMESPACE") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"END_NAMESPACE";
				return l_token;
			}
			break;

		case Vocabulary::_Namespace:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "namespace") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"namespace";
				return l_token;
			}
			break;

		case Vocabulary::_NamespaceConcatenator:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "::") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"::";
				return l_token;
			}
			break;

		case Vocabulary::_Template:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "template") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"template";
				return l_token;
			}
			break;

		case Vocabulary::_Class:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "class") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"class";
				return l_token;
			}
			break;

		case Vocabulary::_Struct:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "struct") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"struct";
				return l_token;
			}
			break;

		case Vocabulary::_Enum:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "enum") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"enum";
				return l_token;
			}
			break;

		case Vocabulary::_Private:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "private") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"private";
				return l_token;
			}
			break;

		case Vocabulary::_Protected:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "protected") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"protected";
				return l_token;
			}
			break;

		case Vocabulary::_Public:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "public") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"public";
				return l_token;
			}
			break;

		case Vocabulary::_Static:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "static") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"static";
				return l_token;
			}
			break;

		case Vocabulary::_ThreadLocal:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "thread_local") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"thread_local";
				return l_token;
			}
			break;

		case Vocabulary::_Virtual:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "virtual") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"virtual";
				return l_token;
			}
			break;

		case Vocabulary::_Const:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "const") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"const";
				return l_token;
			}
			break;

		case Vocabulary::_Noexcept:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "noexcept") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"noexcept";
				return l_token;
			}
			break;

		case Vocabulary::_FrogmanEngineBaseClassReflectionMacro:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "FE_CLASS_HAS_A_BASE") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"FE_CLASS_HAS_A_BASE";
				return l_token;
			}
			break;

		case Vocabulary::_FrogmanEngineClassReflectionMacro:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "FE_CLASS") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"FE_CLASS";
				return l_token;
			}
			break;

		case Vocabulary::_FrogmanEngineStructReflectionMacro:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "FE_STRUCT") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"FE_STRUCT";
				return l_token;
			}
			break;

		case Vocabulary::_FrogmanEnginePropertyReflectionMacro:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "FE_PROPERTY") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"FE_PROPERTY";
				return l_token;
			}
			break;

		case Vocabulary::_FrogmanEngineStaticMethodReflectionMacro:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "FE_STATIC_METHOD") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"FE_STATIC_METHOD";
				return l_token;
			}
			break;

		case Vocabulary::_FrogmanEngineMethodReflectionMacro:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "FE_METHOD") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"FE_METHOD";
				return l_token;
			}
			break;

		case Vocabulary::_Semicolon:
			l_token._vocabulary = it.value();
			l_token._code = u8";";
			return l_token;

		case Vocabulary::_Comma:
			l_token._vocabulary = it.value();
			l_token._code = u8",";
			return l_token;

		case Vocabulary::_AssignmentOperator:
			l_token._vocabulary = it.value();
			l_token._code = u8"=";
			return l_token;

		case Vocabulary::_Pointer:
			l_token._vocabulary = it.value();
			l_token._code = u8"*";
			return l_token;

		case Vocabulary::_LeftParen:
			l_token._vocabulary = it.value();
			l_token._code = u8"(";
			return l_token;

		case Vocabulary::_RightParen:
			l_token._vocabulary = it.value();
			l_token._code = u8")";
			return l_token;

		case Vocabulary::_LeftBracket:
			l_token._vocabulary = it.value();
			l_token._code = u8"[";
			return l_token;

		case Vocabulary::_RightBracket:
			l_token._vocabulary = it.value();
			l_token._code = u8"]";
			return l_token;

		case Vocabulary::_LeftCurlyBracket:
			l_token._vocabulary = it.value();
			l_token._code = u8"{";
			return l_token;

		case Vocabulary::_RightCurlyBracket:
			l_token._vocabulary = it.value();
			l_token._code = u8"}";
			return l_token;

		default:
			break;
		}
	}
	return l_token;
}

_FE_NODISCARD_ token header_tool_engine::__tokenize_operator(typename file_buffer_t::const_pointer code_iterator_p) noexcept
{
	token l_token{ Vocabulary::_Undefined, file_buffer_t(get_memory_resource()) };

	auto l_char = g_vocabulary.equal_prefix_range_ks(FE::iterator_cast<FE::ASCII*>(code_iterator_p), 1);

	for (auto it = l_char.first; it != l_char.second; ++it)
	{
		switch (it.value())
		{
		case Vocabulary::_Access:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "->") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"->";
				return l_token;
			}
			break;

		case Vocabulary::_AddAssignmentOperator:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "+=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"+=";
				return l_token;
			}
			break;

		case Vocabulary::_SubAssignmentOperator:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "-=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"-=";
				return l_token;
			}
			break;

		case Vocabulary::_MulAssignmentOperator:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "*=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"*=";
				return l_token;
			}
			break;

		case Vocabulary::_DivAssignmentOperator:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "/=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"/=";
				return l_token;
			}
			break;

		case Vocabulary::_ModAssignmentOperator:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "%=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"%=";
				return l_token;
			}
			break;

		case Vocabulary::_BitwiseAndAssignmentOperator:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "&=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"&=";
				return l_token;
			}
			break;

		case Vocabulary::_BitwiseOrAssignmentOperator:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "|=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"|=";
				return l_token;
			}
			break;

		case Vocabulary::_BitwiseXorAssignmentOperator:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "^=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"^=";
				return l_token;
			}
			break;

		case Vocabulary::_BitShiftAssignmentOperator:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "<<=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"<<=";
				return l_token;
			}
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, ">>=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8">>=";
				return l_token;
			}
			break;

		case Vocabulary::_LogicalAnd:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "&&") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"&&";
				return l_token;
			}
			break;

		case Vocabulary::_IsEqualTo:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "==") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"==";
				return l_token;
			}
			break;

		case Vocabulary::_IsNotEqualTo:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "!=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"!=";
				return l_token;
			}
			break;

		case Vocabulary::_IsLessThanOrEqualTo:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "<=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"<=";
				return l_token;
			}
			break;

		case Vocabulary::_IsGreaterThanOrEqualTo:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, ">=") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8">=";
				return l_token;
			}
			break;

		case Vocabulary::_BitShiftOperator:
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, "<<") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8"<<";
				return l_token;
			}
			if (__verify_if_subject_is_equal_to_key(code_iterator_p, ">>") == true)
			{
				l_token._vocabulary = it.value();
				l_token._code = u8">>";
				return l_token;
			}
			break;

		default:
			break;
		}
	}

	switch (*code_iterator_p)
	{
	case '<':
		l_token._vocabulary = Vocabulary::_BeginTemplateArgs;
		l_token._code = u8"<";
		return l_token;

	case '>':
		l_token._vocabulary = Vocabulary::_EndTemplateArgs;
		l_token._code = u8">";
		return l_token;

	case ':':
		l_token._vocabulary = Vocabulary::_Colon;
		l_token._code = u8":";
		return l_token;

	case '&':
		l_token._vocabulary = Vocabulary::_Reference;
		l_token._code = u8"&";
		return l_token;

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
	case '~':
		_FE_FALLTHROUGH_;
	case '?':
		_FE_FALLTHROUGH_;
	case '.':
		l_token._vocabulary = Vocabulary::_Operator;
		l_token._code = *FE::iterator_cast<FE::UTF8*>(code_iterator_p);
		return l_token;

	default:
		break;
	}

	return l_token;
}
