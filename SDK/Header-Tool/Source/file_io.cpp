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
#include "file_io.hpp"
#include "header_tool.hpp"

#include <FE/algorithm/string.hxx>

// std::mbstowcs
#include <cstdlib>




namespace FHT::file_io
{
	constinit FE::uint8 UTF8_BOM[3] = { 0xEF, 0xBB, 0xBF };

	FE::boolean is_file_encoded_with_UTF8_BOM(FE::wchar* directory_p) noexcept
	{
		std::basic_ifstream<var::ASCII> l_BOM_validator;
		l_BOM_validator.open(directory_p);

		if (l_BOM_validator.is_open() == false)
		{
			std::wcerr << "Frogman Engine Header Tool ERROR: the path '" << directory_p << "' is not a valid directory.\n";
			return false;
		}

		var::uint8 l_BOM[3];
		l_BOM_validator.read(reinterpret_cast<char*>(l_BOM), 3);
		return ((l_BOM[0] == UTF8_BOM[0]) && (l_BOM[1] == UTF8_BOM[1]) && (l_BOM[2] == UTF8_BOM[2]));
	}

	_FE_NODISCARD_ std::pmr::vector<directory_t> make_header_file_list(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
	{
		directory_t l_raw_directories(::framework::get_framework().get_memory_resource());

		for (int i = 0; i < argc_p; ++i)
		{
			auto l_h = FE::algorithm::string::find_the_first<var::ASCII>(argv_p[i], ".h");
			auto l_hpp = FE::algorithm::string::find_the_first<var::ASCII>(argv_p[i], ".hpp");
			auto l_hxx = FE::algorithm::string::find_the_first<var::ASCII>(argv_p[i], ".hxx");
			if ((l_h != std::nullopt) ||
				(l_hpp != std::nullopt) ||
				(l_hxx != std::nullopt))
			{
				FE::int64 l_directory_length = FE::algorithm::string::length(argv_p[i]);
				l_raw_directories.resize(l_directory_length + 1);
				std::mbstowcs(l_raw_directories.data(), argv_p[i], l_directory_length);
				l_raw_directories = l_raw_directories.c_str();

				var::uint64 l_number_of_files = FE::algorithm::string::count_chars<var::wchar>(l_raw_directories.c_str(), L';')._match_count;
				++l_number_of_files; // CMake does not put ';' to indicate the end of the last directory of the list. So, we need to add 1 to the count.

				std::pmr::vector<directory_t> l_list;
				l_list.reserve(l_number_of_files);

				FE::wchar* l_end_of_path = l_raw_directories.c_str();
				auto l_path_seperator = FE::algorithm::string::find_the_first<var::wchar>(l_end_of_path, L';');

				while (l_path_seperator != std::nullopt)
				{
					FE::uint64 l_path = l_end_of_path - l_raw_directories.c_str();
					l_list.emplace_back(l_raw_directories.substr(l_path, l_path_seperator->_begin));
					l_end_of_path += l_path_seperator->_end;
					l_path_seperator = FE::algorithm::string::find_the_first<var::wchar>(l_end_of_path, L';');
				}

				l_path_seperator = FE::algorithm::string::find_the_last<var::wchar>(l_raw_directories.c_str(), L';');

				if (l_path_seperator != std::nullopt)
				{
					l_list.emplace_back(l_raw_directories.substr(l_path_seperator->_end, l_raw_directories.length() - l_path_seperator->_end));
					return l_list;
				}

				l_list.emplace_back(l_raw_directories.substr(0, l_raw_directories.length()));
				return l_list;
			}
		}

		return std::pmr::vector<directory_t>();
	}

	_FE_NODISCARD_ std::pmr::vector<file_buffer_t> map_header_files(const std::pmr::vector<directory_t>& file_list_p) noexcept
	{
		std::pmr::vector<file_buffer_t> l_files(framework::get_framework().get_memory_resource());
		l_files.reserve(file_list_p.size());

		for (auto& path_to_file : file_list_p)
		{
			auto l_h = path_to_file.find(L".h");
			auto l_hpp = path_to_file.find(L".hpp");
			auto l_hxx = path_to_file.find(L".hxx");
			if ((l_h == std::string::npos) && (l_hpp == std::string::npos) && (l_hxx == std::string::npos))
			{
				continue;
			}

			if (is_file_encoded_with_UTF8_BOM(path_to_file.c_str()) == false)
			{
				std::wcerr << "Frogman Engine Header Tool ERROR: the header file '" << path_to_file.c_str() << "' is not encoded in UTF-8 BOM.\n";
				continue;
			}

			std::basic_ifstream<var::UTF8> l_file_handler;
			l_file_handler.imbue(framework::get_framework().get_current_system_locale());
			l_file_handler.open(path_to_file.c_str());

			if (l_file_handler.is_open() == false)
			{
				std::wcerr << "Frogman Engine Header Tool ERROR: the header file '" << path_to_file.c_str() << "' is not encoded in UTF-8 BOM.\n";
				continue;
			}

			l_files.emplace_back(std::istreambuf_iterator<var::UTF8>(l_file_handler), std::istreambuf_iterator<var::UTF8>());
			l_file_handler.close();
		}
		return l_files;
	}

	FE::boolean is_encoded_with_UTF8_BOM(FE::UTF8* const buffer_entry_p) noexcept
	{
		return ((buffer_entry_p[0] == UTF8_BOM[0]) && (buffer_entry_p[1] == UTF8_BOM[1]) && (buffer_entry_p[2] == UTF8_BOM[2]));
	}

	_FE_NODISCARD_ file_buffer_t read_copyright_notice(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
	{
		for (var::int32 i = 0; i < argc_p; ++i)
		{
			if (FE::algorithm::string::find_the_first<FE::ASCII>(argv_p[i], ::header_tool::get_program_options().get_path_to_copyright_notice()))
			{
				var::wchar l_wide_directory[_ALLOWED_DIRECTORY_LENGTH_];
				FE::size l_length = FE::algorithm::string::length(argv_p[i]);
				std::mbstowcs(l_wide_directory, ::header_tool::get_program_options().get_path_to_copyright_notice(), l_length);
				l_wide_directory[l_length] = L'\0';
				//FE_EXIT_IF(__is_file_encoded_with_UTF8_BOM(l_wide_directory) == false, FrogmanEngineHeaderToolError::_Fatal_InputError_TargetFileNotEncodedWithUTF8_BOM, "Frogman Engine Header Tool ERROR: the license text file '${%s@0}' is not encoded in UTF-8 BOM.", m_header_tool_options.get_path_to_copyright_notice());

				std::basic_ifstream<var::UTF8> l_file_handler;
				l_file_handler.imbue(framework::get_framework().get_current_system_locale());

				l_file_handler.open(::header_tool::get_program_options().get_path_to_copyright_notice());
				//FE_EXIT_IF(l_file_handler.is_open() == false, FrogmanEngineHeaderToolError::_InputError_NoCopyRightNoticeIsGiven, "Frogman Engine Header Tool ERROR: the program option '${%s@0}' is not defined but no license text file is given. The given path is '${%s@1}'", m_header_tool_options.view_fno_copyright_notice_option_title(), m_header_tool_options.get_path_to_copyright_notice());

				file_buffer_t l_copyright_notice(std::istreambuf_iterator<var::UTF8>(l_file_handler), std::istreambuf_iterator<var::UTF8>(), ::framework::get_framework().get_memory_resource());
				l_file_handler.close();
				return l_copyright_notice;
			}
		}
		return file_buffer_t();
	}
}