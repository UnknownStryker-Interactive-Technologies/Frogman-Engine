#ifndef _FE_HEADER_TOOL_PARSER_HPP_
#define _FE_HEADER_TOOL_PARSER_HPP_
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




enum struct ScopeContext
{
    _Global,
	_Namespace,
	_Class,
	_Struct,
	_EnumStruct,
	_Function,
};

// migrate parser related code from header_tool_engine.hpp to here in the future.


#endif
