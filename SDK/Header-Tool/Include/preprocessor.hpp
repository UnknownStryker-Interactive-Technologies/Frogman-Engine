#ifndef _FE_HEADER_TOOL_PREPROCESSOR_HPP_
#define _FE_HEADER_TOOL_PREPROCESSOR_HPP_
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
#include "program_options.hpp"
#include "token.hpp"

#include <list>
#include <memory_resource>




namespace FHT::preprocessor
{
    void preprocess_macros(token& out_token_p, typename file_buffer_t::const_pointer code_iterator_p, const program_options::defined_macros& macro_table_p) noexcept;
}

#endif