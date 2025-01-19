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
#define _CRT_SECURE_NO_WARNINGS
#include "header_tool_engine.hpp"




_FE_NODISCARD_ file_buffer_t header_tool_engine::__read_copyright_notice(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
{
	for (var::int32 i = 0; i < argc_p; ++i)
	{
		if (FE::algorithm::string::find_the_first(argv_p[i], this->m_header_tool_options.get_path_to_copyright_notice()))
		{
			var::wchar l_wide_directory[_ALLOWED_DIRECTORY_LENGTH_];
			FE::size l_length = FE::algorithm::string::length(argv_p[i]);
			std::mbstowcs(l_wide_directory, this->m_header_tool_options.get_path_to_copyright_notice(), l_length);
			l_wide_directory[l_length] = L'\0';
			FE_EXIT(__is_the_file_encoded_with_UTF8_BOM(l_wide_directory) == false, FrogmanEngineHeaderToolError::_Fatal_InputError_TargetFileIsNotEncodedIn_UTF8_BOM, "Frogman Engine Header Tool ERROR: the license text file '${%s@0}' is not encoded in UTF-8 BOM.", this->m_header_tool_options.get_path_to_copyright_notice());

			std::basic_ifstream<var::UTF8> l_file_handler;
			l_file_handler.imbue(this->m_UTF8_locale);

			l_file_handler.open(this->m_header_tool_options.get_path_to_copyright_notice());
			FE_EXIT(l_file_handler.is_open() == false, FrogmanEngineHeaderToolError::_InputError_NoCopyRightNoticeIsGiven, "Frogman Engine Header Tool ERROR: the program option '${%s@0}' is not defined but no license text file is given. The given path is '${%s@1}'", this->m_header_tool_options.view_fno_copyright_notice_option_title(), this->m_header_tool_options.get_path_to_copyright_notice());

			file_buffer_t l_copyright_notice(std::istreambuf_iterator<var::UTF8>(l_file_handler), std::istreambuf_iterator<var::UTF8>(), this->get_memory_resource());
			l_file_handler.close();
			return l_copyright_notice;
		}
	}
	return file_buffer_t();
}