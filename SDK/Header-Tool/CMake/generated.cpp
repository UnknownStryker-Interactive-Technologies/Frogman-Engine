// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.h> 
#include <FE/framework/reflection.hpp> 
#include <FE/framework/framework.hpp> 
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Header-Tool\Include\error_code.hpp>

void load_reflection_data()
{
    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FrogmanEngineHeaderToolError >("::FrogmanEngineHeaderToolError",
    {
        { ::FrogmanEngineHeaderToolError::_FatalCmdInputError_NoProgramOptionsAreGiven, "_FatalCmdInputError_NoProgramOptionsAreGiven" },
        { ::FrogmanEngineHeaderToolError::_FatalCmdInputError_NoFilesAreGiven, "_FatalCmdInputError_NoFilesAreGiven" },
        { ::FrogmanEngineHeaderToolError::_FatalCmdInputError_InvalidPathToCMakeProject, "_FatalCmdInputError_InvalidPathToCMakeProject" },
        { ::FrogmanEngineHeaderToolError::_FatalError_FailedToOpenFile, "_FatalError_FailedToOpenFile" },
        { ::FrogmanEngineHeaderToolError::_InputError_NoCopyRightNoticeIsGiven, "_InputError_NoCopyRightNoticeIsGiven" },
        { ::FrogmanEngineHeaderToolError::_Fatal_InputError_TargetFileNotEncodedWithUTF8_BOM, "_Fatal_InputError_TargetFileNotEncodedWithUTF8_BOM" },
        { ::FrogmanEngineHeaderToolError::_InputError_IncorrectCppSyntax, "_InputError_IncorrectCppSyntax" },
        { ::FrogmanEngineHeaderToolError::_InputError_ParsingFailure, "_InputError_ParsingFailure" }
    });
}
