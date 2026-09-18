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




enum struct Vocabulary : FE::uint16
{
	_Undefined, _EndOfCode,

	_Identifier,
	_AnyDecl, _Attribute,

	_LineComment, _LineEnd,
	_CommentBegin, _CommentEnd,
	_CommentBody,

	_BeginNamespace, _EndNamespace,
	_Namespace, _NamespaceConcatenator, _NamespaceIdentifier,

	_Template, _BeginTemplateArgs, _Typename, _TemplateArg, _EndTemplateArgs,
	_TemplateBody,

	_EnumStruct, _StructKeywordOfEnumStruct, _EnumStructIdentifier, _EnumStructExtension,
	_EnumStructField, _EnumStructFieldValue,
	_Struct, _StructIdentifier,
	_Class, _ClassIdentifier, _BaseClassIdentifier,
	_ClassStructEnumMethodForwardDeclaration,

	_Colon, _Semicolon, _Comma,

	_BackSlash,

	_TextLiteralPrefix,
	_StringLiteral, _CharLiteral, _NumberValue,
	_Operator, _AssignmentOperator, _Asterisk, _BitwiseNot,

	_Virtual, _Override, _Final, _This,

	_Private, _Protected, _Public,

	_Static, _ThreadLocal,

	_Mutable, _Extern, _Friend, _Using, _Typedef, _Alignas,

	_Const, _Volatile,
	_Reference, _Pointer,

	_Noexcept,
	_Constexpr, _Consteval, _Constinit,

	_Inline, _ForceInline, _FrogmanEngineForceInline,

	_LeftParen, _RightParen,
	_LeftBracket, _RightBracket,
	_LeftCurlyBracket, _RightCurlyBracket,

	//_CallingConvention, _FnReturnType, _FnIdentifier, _FnParameterList, _FnBody,
	_Macro,

	_StaticAssert,

	_FrogmanEngineBaseClassReflectionMacro,

	_FrogmanEnginePropertyReflectionMacro,
	_FrogmanEngineStaticMethodReflectionMacro,
	_FrogmanEngineMethodReflectionMacro,

	_FrogmanEngineClassReflectionMacro,
	_FrogmanEngineStructReflectionMacro,
	_FrogmanEngineEnumStructReflectionMacro,
	_FrogmanEngineSystemMacro, _FrogmanEngineSystemArgSysCallPhase, _FrogmanEngineSystemArgWorldTagEnum,
	_FrogmanEngineEnableSerialization, _FrogmanHeaderToolGeneratedReflectionMacro,

	_PreprocessorDirective, _PreprocessorNextLine,
	_Preprocessor,
};


const tsl::htrie_map<var::ASCII, Vocabulary> g_vocabulary =
{
	{"//", Vocabulary::_LineComment},
	{"/*", Vocabulary::_CommentBegin}, {"*/", Vocabulary::_CommentEnd},

	{ "namespace", Vocabulary::_Namespace },
	{ "BEGIN_NAMESPACE", Vocabulary::_BeginNamespace }, { "END_NAMESPACE", Vocabulary::_EndNamespace },
	{ "::", Vocabulary::_NamespaceConcatenator },

	{ "virtual", Vocabulary::_Virtual}, { "override", Vocabulary::_Override}, { "final", Vocabulary::_Final}, { "this", Vocabulary::_This },

	{ "private", Vocabulary::_Private }, { "protected", Vocabulary::_Protected }, { "public", Vocabulary::_Public },

	{ "static", Vocabulary::_Static }, { "thread_local", Vocabulary::_ThreadLocal },

	{ "mutable", Vocabulary::_Mutable }, { "extern", Vocabulary::_Extern }, { "friend", Vocabulary::_Friend }, { "using", Vocabulary::_Using }, { "typedef", Vocabulary::_Typedef }, { "alignas", Vocabulary::_Alignas },

	{ "const", Vocabulary::_Const }, { "volatile", Vocabulary::_Volatile},

	{ "noexcept", Vocabulary::_Noexcept },
	{ "constexpr", Vocabulary::_Constexpr }, { "consteval", Vocabulary::_Consteval }, { "constinit", Vocabulary::_Constinit },
	{ "inline", Vocabulary::_Inline }, { "__forceinline", Vocabulary::_ForceInline }, { "_FE_FORCE_INLINE_", Vocabulary::_FrogmanEngineForceInline },

	{ "static_assert", Vocabulary::_StaticAssert },

	{ "FE_CLASS_HAS_A_BASE", Vocabulary::_FrogmanEngineBaseClassReflectionMacro },
	{ "FE_ENABLE_SERIALIZATION", Vocabulary::_FrogmanEngineEnableSerialization },

	{ "FE_PROPERTY", Vocabulary::_FrogmanEnginePropertyReflectionMacro },
	{ "FE_STATIC_METHOD", Vocabulary::_FrogmanEngineStaticMethodReflectionMacro },
	{ "FE_METHOD", Vocabulary::_FrogmanEngineMethodReflectionMacro },

	{ "FE_CLASS", Vocabulary::_FrogmanEngineClassReflectionMacro },
	{ "FE_STRUCT", Vocabulary::_FrogmanEngineStructReflectionMacro },
	{ "FE_ENUM_STRUCT", Vocabulary::_FrogmanEngineEnumStructReflectionMacro },
	{ "FE_SYSTEM", Vocabulary::_FrogmanEngineSystemMacro }
};


#endif