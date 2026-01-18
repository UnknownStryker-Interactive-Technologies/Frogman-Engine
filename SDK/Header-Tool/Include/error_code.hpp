#ifndef _FE_HEADER_TOOL_ERROR_CODE_HPP_
#define _FE_HEADER_TOOL_ERROR_CODE_HPP_
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
#include <FE/pair.hxx>

#include <FE/framework/reflection.hxx>




FE_ENUM_STRUCT();
enum struct FrogmanEngineHeaderToolError : FE::int32
{
	_EmptyHeaderFile = 999,
	_FatalCmdInputError_NoProgramOptionsAreGiven = 1000,
	_FatalCmdInputError_NoFilesAreGiven = 1001,
	_FatalCmdInputError_InvalidPathToCMakeProject = 1002,
	_FatalError_FailedToOpenFile = 1003,
	_InputError_NoCopyRightNoticeIsGiven = 1004,
	_Fatal_InputError_TargetFileNotEncodedWithUTF8_BOM = 1005,
	_InputError_IncorrectCppSyntax = 1006,
	_InputError_ParsingFailure = 1007,
	_InputError_TokenizationFailure = 1008
};

#define THROW_CPP_SYNTAX_ERROR(expr, ...) \
if (expr) _FE_UNLIKELY_ \
{ \
	throw ::FE::pair<FrogmanEngineHeaderToolError, ::FE::ASCII*>{FrogmanEngineHeaderToolError::_InputError_IncorrectCppSyntax, ::FE::log::buffered_string_formatter({ __VA_ARGS__ })}; \
}


#endif