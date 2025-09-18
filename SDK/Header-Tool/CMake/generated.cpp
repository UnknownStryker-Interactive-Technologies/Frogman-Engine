// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.h> 
#include <FE/framework/framework.hpp> 
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Header-Tool\Include\error_code.hpp>
#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Core\Include\FE\clock.hpp>



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
        { ::FrogmanEngineHeaderToolError::_InputError_ParsingFailure, "_InputError_ParsingFailure" },
        { ::FrogmanEngineHeaderToolError::_InputError_TokenizationFailure, "_InputError_TokenizationFailure" }
    });
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::clock*(::FE::clock*)> >("construct ::FE::clock", &::std::construct_at<::FE::clock>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::clock*)> >("destruct ::FE::clock", &::std::destroy_at<::FE::clock>);
}
