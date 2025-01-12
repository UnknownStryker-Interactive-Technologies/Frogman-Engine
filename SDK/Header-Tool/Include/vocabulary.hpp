#ifndef _FE_HEADER_TOOL_VOCABULARY_HPP_
#define _FE_HEADER_TOOL_VOCABULARY_HPP_
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
#include <FE/prerequisites.h>

// trie
#include <tsl/htrie_map.h>




enum struct Vocabulary : FE::uint16
{
	_Undefined, _EndOfCode,
	_Identifier,
	_LineComment, _LineEnd,
	_CommentBegin, _CommentEnd,
	_BeginNamespace, _EndNamespace,
	_Namespace, _NamespaceConcatenator,
	_Template, _BeginTemplateArgs, _Typename, _EndTemplateArgs,
	_Class, _Struct, _Enum,
	_Colon, _Semicolon, _Comma,

	_StringLiteral, _CharLiteral,
	_Access, _Operator,
	_AssignmentOperator,
	_LogicalAnd,
	_BitShiftOperator,
	_BitShiftAssignmentOperator, _BitwiseAndAssignmentOperator, _BitwiseOrAssignmentOperator, _BitwiseXorAssignmentOperator,
	_AddAssignmentOperator, _SubAssignmentOperator, _MulAssignmentOperator, _DivAssignmentOperator, _ModAssignmentOperator,
	_IsEqualTo, _IsNotEqualTo, _IsGreaterThanOrEqualTo, _IsLessThanOrEqualTo,

	_Virtual, _Override, _Final,
	_Private, _Protected, _Public,
	_Static, _ThreadLocal,
	_Const,
	_Reference, _Pointer,
	_Noexcept,
	_Constexpr, _Consteval, _Constinit,
	_LeftParen, _RightParen,
	_LeftBracket, _RightBracket,
	_LeftCurlyBracket, _RightCurlyBracket,
	_CallingConvention,

	_FrogmanEngineBaseClassReflectionMacro,
	_FrogmanEngineClassReflectionMacro,
	_FrogmanEngineStructReflectionMacro,
	_FrogmanEngineStaticPropertyReflectionMacro,
	_FrogmanEnginePropertyReflectionMacro,
	_FrogmanEngineStaticMethodReflectionMacro,
	_FrogmanEngineMethodReflectionMacro,
};


const tsl::htrie_map<char, Vocabulary> g_vocabulary =
{

	{ "//", Vocabulary::_LineComment }, { "\n", Vocabulary::_LineEnd },
	{ "/*", Vocabulary::_CommentBegin }, { "*/", Vocabulary::_CommentEnd },
	{ "BEGIN_NAMESPACE", Vocabulary::_BeginNamespace }, { "END_NAMESPACE", Vocabulary::_EndNamespace },
	{ "namespace", Vocabulary::_Namespace }, { "::", Vocabulary::_NamespaceConcatenator },
	{ "template", Vocabulary::_Template }, { "<", Vocabulary::_BeginTemplateArgs }, { ">", Vocabulary::_EndTemplateArgs },
	{ "class", Vocabulary::_Class }, { "struct", Vocabulary::_Struct }, { "enum", Vocabulary::_Enum },
	{ ":", Vocabulary::_Colon }, { ";", Vocabulary::_Semicolon }, { ",", Vocabulary::_Comma },

	{ "\"", Vocabulary::_StringLiteral }, { "\'", Vocabulary::_StringLiteral },
	{ "->", Vocabulary::_Access },
	{ "=", Vocabulary::_AssignmentOperator },
	{ "&&", Vocabulary::_LogicalAnd },
	{ ">>", Vocabulary::_BitShiftOperator }, { "<<", Vocabulary::_BitShiftOperator },
	{ ">>=", Vocabulary::_BitShiftAssignmentOperator }, { "<<=", Vocabulary::_BitShiftAssignmentOperator },
	{ "&=", Vocabulary::_BitwiseAndAssignmentOperator }, { "|=", Vocabulary::_BitwiseOrAssignmentOperator }, { "^=", Vocabulary::_BitwiseXorAssignmentOperator },
	{ "+=", Vocabulary::_AddAssignmentOperator }, { "-=", Vocabulary::_SubAssignmentOperator }, { "*=", Vocabulary::_MulAssignmentOperator }, { "/=", Vocabulary::_DivAssignmentOperator }, { "%=", Vocabulary::_ModAssignmentOperator },
	{ "==", Vocabulary::_IsEqualTo }, { "!=", Vocabulary::_IsNotEqualTo }, { ">=", Vocabulary::_IsGreaterThanOrEqualTo }, { "<=", Vocabulary::_IsLessThanOrEqualTo },

	{"virtual", Vocabulary::_Virtual}, {"override", Vocabulary::_Override}, {"final", Vocabulary::_Final},
	{ "private", Vocabulary::_Private }, { "public", Vocabulary::_Public }, { "protected", Vocabulary::_Protected },
	{ "static", Vocabulary::_Static }, { "thread_local", Vocabulary::_ThreadLocal },
	{ "const", Vocabulary::_Const },
	{ "&", Vocabulary::_Reference }, { "*", Vocabulary::_Pointer },
	{ "noexcept", Vocabulary::_Noexcept },
	{ "(", Vocabulary::_LeftParen }, { ")", Vocabulary::_RightParen },
	{ "[", Vocabulary::_LeftBracket }, { "]", Vocabulary::_RightBracket },
	{ "{", Vocabulary::_LeftCurlyBracket }, { "}", Vocabulary::_RightCurlyBracket },
	{ "FE_CLASS_HAS_A_BASE", Vocabulary::_FrogmanEngineBaseClassReflectionMacro },
	{ "FE_CLASS", Vocabulary::_FrogmanEngineClassReflectionMacro },
	{ "FE_STRUCT", Vocabulary::_FrogmanEngineStructReflectionMacro },
	{ "FE_PROPERTY", Vocabulary::_FrogmanEnginePropertyReflectionMacro },
	{ "FE_STATIC_METHOD", Vocabulary::_FrogmanEngineStaticMethodReflectionMacro },
	{ "FE_METHOD", Vocabulary::_FrogmanEngineMethodReflectionMacro },
};


#endif