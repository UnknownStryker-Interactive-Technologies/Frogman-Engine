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




enum struct FrogmanEngineHeaderToolError : FE::int32
{
	_CmdInputError_NoProgramOptionsAreGiven = 1000,
	_CmdInputError_NoFilesAreGiven = 1001,
	_CmdInputError_InvalidPathToCMakeProject = 1002,
	_Error_FailedToOpenFile = 1003,
	_Error_NoCopyRightNoticeIsGiven = 1004,
	_Error_TargetFileIsNotEncodedIn_UTF8_BOM = 1005,
	_Error_IncorrectCppSyntex = 1006
};

#define REPORT_CPP_SYNTEX_ERROR(expr, msg) \
if (expr) _FE_UNLIKELY_ \
{ \
	::std::cerr << msg; \
	::std::exit((int)FrogmanEngineHeaderToolError::_Error_IncorrectCppSyntex); \
}


#endif