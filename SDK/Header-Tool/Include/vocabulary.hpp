#ifndef _FE_HEADER_TOOL_VOCABULARY_HPP_
#define _FE_HEADER_TOOL_VOCABULARY_HPP_
/*
Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

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

	_AnyDecl,

	_LineComment, _LineEnd,
	_CommentBegin, _CommentEnd,
	_CommentBody,

	_BeginNamespace, _EndNamespace,
	_Namespace, _NamespaceConcatenator,

	_Template, _BeginTemplateArgs, _Typename, _TemplateArg, _EndTemplateArgs,
	_TemplateBody,

	_Class, _Struct, _EnumStruct,
	_ClassStructEnumForwardDeclaration,
	_FieldType, _FieldIdentifier, _EnumStructField, _EnumStructFieldInitialValue,

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

	_CallingConvention, _FnReturnType, _FnIdentifier, _FnParameterList, _FnBody,

	_StaticAssert,

	_FrogmanEngineBaseClassReflectionMacro,
	_FrogmanEngineClassReflectionMacro,
	_FrogmanEngineStructReflectionMacro,
	_FrogmanEnginePropertyReflectionMacro,
	_FrogmanEngineStaticMethodReflectionMacro,
	_FrogmanEngineMethodReflectionMacro,
	_FrogmanEngineEnumStructReflectionMacro,
	_FrogmanEngineSystemMacro, _FrogmanEngineSystemArgSysCallPhase, _FrogmanEngineSystemArgTargetComponentType, _FrogmanEngineSystemArgWorldTagEnumType,

	_PreprocessorDirective, _PreprocessorNextLine,
	_Preprocessor,
};


const tsl::htrie_map<var::ASCII, Vocabulary> g_vocabulary =
{
	{"//", Vocabulary::_LineComment},
	{"/*", Vocabulary::_CommentBegin}, {"*/", Vocabulary::_CommentEnd},

	{ "BEGIN_NAMESPACE", Vocabulary::_BeginNamespace }, { "END_NAMESPACE", Vocabulary::_EndNamespace },
	{ "::", Vocabulary::_NamespaceConcatenator },

	{ "virtual", Vocabulary::_Virtual}, { "override", Vocabulary::_Override}, { "final", Vocabulary::_Final},

	{ "private", Vocabulary::_Private }, { "protected", Vocabulary::_Protected }, { "public", Vocabulary::_Public },

	{ "static", Vocabulary::_Static }, { "thread_local", Vocabulary::_ThreadLocal },

	{ "const", Vocabulary::_Const }, { "volatile", Vocabulary::_Volatile},

	{ "noexcept", Vocabulary::_Noexcept },
	{ "constexpr", Vocabulary::_Constexpr }, { "consteval", Vocabulary::_Consteval }, { "constinit", Vocabulary::_Constinit },

	{ "static_assert", Vocabulary::_StaticAssert },

	{ "FE_CLASS", Vocabulary::_FrogmanEngineClassReflectionMacro },
	{ "FE_STRUCT", Vocabulary::_FrogmanEngineStructReflectionMacro },
	{ "FE_ENUM_STRUCT", Vocabulary::_FrogmanEngineEnumStructReflectionMacro },
	{ "FE_SYSTEM", Vocabulary::_FrogmanEngineSystemMacro }
};


#endif