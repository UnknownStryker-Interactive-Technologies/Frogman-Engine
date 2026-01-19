#ifndef _FE_HEADER_TOOL_VOCABULARY_HPP_
#define _FE_HEADER_TOOL_VOCABULARY_HPP_
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

// trie
#include <tsl/htrie_map.h>




enum struct Vocabulary : FE::uint32
{
	_Undefined, _EndOfCode,

	_LineComment, _LineEnd,
	_CommentBegin, _CommentEnd,
	_CommentBody,

	_BeginNamespace, _EndNamespace,
	_Namespace, _NamespaceConcatenator,

	_Template, _BeginTemplateArgs, _Typename, _TemplateArg, _EndTemplateArgs,

	_Class, _Struct, _Enum,

	_Colon, _Semicolon, _Comma,

	_BackSlash,

	_TextLiteralPrefix,
	_StringLiteral, _CharLiteral, _NumberValue,
	_Operator, _AssignmentOperator,

	_Virtual, _Override, _Final,

	_Private, _Protected, _Public,

	_Static, _ThreadLocal,

	_Const, _Volatile,
	_Reference, _Pointer,

	_Noexcept,
	_Constexpr, _Consteval, _Constinit,

	_LeftParen, _RightParen,
	_LeftBracket, _RightBracket,
	_LeftCurlyBracket, _RightCurlyBracket,

	_CallingConvention,

	_StaticAssert, _Using,

	_FrogmanEngineBaseClassReflectionMacro,
	_FrogmanEngineClassReflectionMacro,
	_FrogmanEngineStructReflectionMacro,
	_FrogmanEnginePropertyReflectionMacro,
	_FrogmanEngineStaticMethodReflectionMacro,
	_FrogmanEngineMethodReflectionMacro,
	_FrogmanEngineEnumStructReflectionMacro,
	_FrogmanEngineSystemAttributeMacro,

	_PreprocessorDirective, _PreprocessorNextLine,
	_Preprocessor,
};


const tsl::htrie_map<var::ASCII, Vocabulary> g_vocabulary =
{

	{ "//", Vocabulary::_LineComment }, { "\n", Vocabulary::_LineEnd },
	{ "/*", Vocabulary::_CommentBegin }, { "*/", Vocabulary::_CommentEnd },

	{ "BEGIN_NAMESPACE", Vocabulary::_BeginNamespace }, { "END_NAMESPACE", Vocabulary::_EndNamespace },
	{ "namespace", Vocabulary::_Namespace }, { "::", Vocabulary::_NamespaceConcatenator },

	{ "template", Vocabulary::_Template }, { "<", Vocabulary::_BeginTemplateArgs }, { "typename", Vocabulary::_Typename }, { ">", Vocabulary::_EndTemplateArgs },

	{ "class", Vocabulary::_Class }, { "struct", Vocabulary::_Struct }, { "enum", Vocabulary::_Enum },

	{ ":", Vocabulary::_Colon }, { ";", Vocabulary::_Semicolon }, { ",", Vocabulary::_Comma },

	{ "\"", Vocabulary::_StringLiteral }, { "\'", Vocabulary::_CharLiteral },

	{ "=", Vocabulary::_AssignmentOperator },

	{ "virtual", Vocabulary::_Virtual}, { "override", Vocabulary::_Override}, { "final", Vocabulary::_Final},

	{ "private", Vocabulary::_Private }, { "protected", Vocabulary::_Protected }, { "public", Vocabulary::_Public },

	{ "static", Vocabulary::_Static }, { "thread_local", Vocabulary::_ThreadLocal },

	{ "const", Vocabulary::_Const }, { "volatile", Vocabulary::_Volatile},
	{ "&", Vocabulary::_Reference }, { "*", Vocabulary::_Pointer },

	{ "noexcept", Vocabulary::_Noexcept },
	{ "constexpr", Vocabulary::_Constexpr }, { "consteval", Vocabulary::_Consteval }, { "constinit", Vocabulary::_Constinit },

	{ "(", Vocabulary::_LeftParen }, { ")", Vocabulary::_RightParen },
	{ "[", Vocabulary::_LeftBracket }, { "]", Vocabulary::_RightBracket },
	{ "{", Vocabulary::_LeftCurlyBracket }, { "}", Vocabulary::_RightCurlyBracket },

	{ "static_assert", Vocabulary::_StaticAssert }, { "using", Vocabulary::_Using },

	{ "FE_CLASS_HAS_A_BASE", Vocabulary::_FrogmanEngineBaseClassReflectionMacro },
	{ "FE_CLASS", Vocabulary::_FrogmanEngineClassReflectionMacro },
	{ "FE_STRUCT", Vocabulary::_FrogmanEngineStructReflectionMacro },
	{ "FE_PROPERTY", Vocabulary::_FrogmanEnginePropertyReflectionMacro },
	{ "FE_STATIC_METHOD", Vocabulary::_FrogmanEngineStaticMethodReflectionMacro },
	{ "FE_METHOD", Vocabulary::_FrogmanEngineMethodReflectionMacro },
	{ "FE_ENUM_STRUCT", Vocabulary::_FrogmanEngineEnumStructReflectionMacro },
	{ "FE_SYSTEM", Vocabulary::_FrogmanEngineSystemAttributeMacro }
};


#endif