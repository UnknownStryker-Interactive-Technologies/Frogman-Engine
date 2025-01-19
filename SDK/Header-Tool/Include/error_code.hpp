#ifndef _FE_HEADER_TOOL_ERROR_CODE_HPP_
#define _FE_HEADER_TOOL_ERROR_CODE_HPP_
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
#include <FE/pair.hxx>




enum struct FrogmanEngineHeaderToolError : FE::int32
{
	_FatalCmdInputError_NoProgramOptionsAreGiven = 1000,
	_FatalCmdInputError_NoFilesAreGiven = 1001,
	_FatalCmdInputError_InvalidPathToCMakeProject = 1002,
	_FatalError_FailedToOpenFile = 1003,
	_InputError_NoCopyRightNoticeIsGiven = 1004,
	_Fatal_InputError_TargetFileIsNotEncodedIn_UTF8_BOM = 1005,
	_InputError_IncorrectCppSyntex = 1006,
	_InputError_ParsingFailure = 1007
};

#define THROW_CPP_SYNTEX_ERROR(expr, msg) \
if (expr) _FE_UNLIKELY_ \
{ \
	throw ::FE::pair<FrogmanEngineHeaderToolError, ::FE::ASCII*>{FrogmanEngineHeaderToolError::_InputError_IncorrectCppSyntex, msg}; \
}


#endif