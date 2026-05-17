#ifndef _FE_HEADER_TOOL_FILE_IO_HPP_
#define _FE_HEADER_TOOL_FILE_IO_HPP_
/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#define _CRT_SECURE_NO_WARNINGS
#include "token.hpp"
#include "error_code.hpp"

#include <vector> // std::pmr::vector




namespace FHT::file_io
{
	extern constinit FE::uint8 UTF8_BOM[3];

	FE::boolean is_file_encoded_with_UTF8_BOM(FE::wchar* directory_p) noexcept;

	_FE_NODISCARD_ std::pmr::vector<directory_t> make_header_file_list(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;

	_FE_NODISCARD_ std::pmr::vector<file_buffer_t> map_header_files(const std::pmr::vector<directory_t>& file_list_p) noexcept;
	
	FE::boolean is_encoded_with_UTF8_BOM(FE::UTF8* const buffer_entry_p) noexcept;

	_FE_NODISCARD_ file_buffer_t read_copyright_notice(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
}
#endif