#ifndef _FE_HEADER_TOOL_HPP_
#define _FE_HEADER_TOOL_HPP_
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

#include "reflection_tree.hpp"
#include "reflexcode_generator.hpp"
#include "program_options.hpp"
#include "token.hpp"

// FE.Framework
#include <FE/framework.hxx>

// FE.Core string algorithm
#include <FE/algorithm/string.hxx>

// std
#include <algorithm> // std::find_if
#include <fstream> // std::basic_ifstream
#include <list> // std::pmr::pmr::list
#include <string> // std::pmr::basic_string
#include <string_view> // std::basic_string_view
#include <vector> // std::pmr::vector

// Microsoft Parallel Patterns Library. This header is specific to the Microsoft Visual Studio.
//#include <concurrent_vector.h>
#include <FE/concurrent_vector.hxx>


using framework = FE::framework::framework_base;


/*
* Test arguments:
-fno-copyright-notice -path-to-project=C:\Users\leeho\Documents\GitHub\Frogman-Engine\SDK\Header-Tool\CMake C:\Users\leeho\Documents\GitHub\Frogman-Engine\SDK\Core\Include\FE\list.hxx
*/

/*
The header_tool class is a specialized tool for enforcing copyright notice embedment and generating reflection meta data registry code
while utilizing parallel task execution for efficiency.
*/


class header_tool : public FE::framework::framework_base
{
	program_options m_header_tool_options;
	file_buffer_t m_copyright_notice;
	
	std::pmr::vector<directory_t> m_header_file_list;
	std::pmr::vector<file_buffer_t> m_mapped_header_files;
	FE::framework::reflection::enum_metadata* m_FHT_error_codes;
	FHT::reflexcode_generator::metadata_set_t m_metadata_set;

public:
	header_tool(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
	~header_tool() noexcept override = default;

	virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) override;
	virtual FE::int32 run() override;
	virtual FE::int32 shutdown() override;
		
public:
	_FE_FORCE_INLINE_ static program_options& get_program_options() noexcept { return static_cast<header_tool&>( get_framework() ).m_header_tool_options; }
};


#endif