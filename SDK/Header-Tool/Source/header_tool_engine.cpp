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

// std::mbstowcs
#include <cstdlib>




header_tool_engine::header_tool_engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
	: FE::framework::framework_base(argc_p, argv_p),
	  m_UTF8_with_BOM{ 0xEF, 0xBB, 0xBF },
	  m_UTF8_locale("en_US.UTF-8"),
	  m_header_tool_options(argc_p, argv_p)
{
	std::cout << "Frogman Engine Header Tool: the given program options are - ";
	for (var::int32 i = 0; i < argc_p; ++i)
	{
		std::cout << argv_p[i] << ' ';
	}
	std::cout << '\n';

	std::cout << "Frogman Engine Header Tool: the current locale is " << std::locale().name() << '\n';
}


FE::int32 header_tool_engine::launch(FE::int32 argc_p, FE::ASCII** argv_p)
{
	this->m_code_style_guide = file_buffer_t(this->get_memory_resource());
	this->m_reflection_metadata_set = reflection_metadata_set_t(this->get_memory_resource());

	if (this->m_header_tool_options.is_fno_op_defined() == true)
	{
		std::cerr << "\n\nFrogman Engine Header Tool: No operation will be done. Exiting the program.\n\n";
		std::exit(0);
	}

	if (*(this->m_header_tool_options.get_path_to_copyright_notice()) != '\0')
	{
		this->m_copyright_notice = __read_copyright_notice(argc_p, argv_p);
	}

	this->m_header_file_list = __make_header_file_list(argc_p, argv_p);
	this->m_mapped_header_files = __map_header_files(this->m_header_file_list);

	return 0;
}

FE::int32 header_tool_engine::run()
{
	using namespace FE;
	/* Jobs to do:
	1. Check the presence of the given copyright notice.

	Those jobs can be done in parallel by considering the header fiiles as jobs.
	*/

	tf::Taskflow l_taskflow;
	var::int32 l_exit_code = 0;
	FE::uint64 l_number_of_files = this->m_mapped_header_files.size();
	std::mutex l_log_lock;

	if (this->m_header_tool_options.is_fno_copyright_notice_defined() == false)
	{
		for (var::uint64 i = 0; i < l_number_of_files; ++i)
		{
			l_taskflow.emplace
			(
				[i, &l_exit_code, &l_log_lock, this]
				{
					file_buffer_t& l_file = this->m_mapped_header_files[i];

					// Check the presence of the given copy right notice.
					FE::boolean l_result = algorithm::string::space_insensitive_contains(algorithm::string::skip_BOM(l_file.c_str()), l_file.size(), algorithm::string::skip_BOM(this->m_copyright_notice.c_str()));
					directory_t& l_path = this->m_header_file_list[i];

					if (l_result == false) // The given copy right notice is not found.
					{
						std::lock_guard<std::mutex> l_guard(l_log_lock);
						std::wcerr << L"\n\nFrogman Engine Header Tool WARNING:\n\tThe file located at '" << l_path.c_str() << L"' has no copy of the specified copyright notice.\n\n";
						l_exit_code = -1; // Reserve the error report.
					}
				}
			);
		}

		// Now, run it.
		get_task_scheduler().access_executor().run(l_taskflow).wait();
		// The number of threads can be scaled via the '-max-concurrency=n' option.

		if (0 != l_exit_code)
		{
			return l_exit_code;
		}
	}

	l_taskflow.clear();

	if (this->m_header_tool_options.is_fno_reflection_helper_defined() == false)
	{
		for (var::uint64 i = 0; i < l_number_of_files; ++i)
		{
			l_taskflow.emplace
			(
				[i, &l_exit_code, &l_log_lock, this]
				{
					file_buffer_t& l_file = this->m_mapped_header_files[i];
					directory_t& l_path = this->m_header_file_list[i];

					// parse the header file to get the tokens.
					auto l_tokens = __parse_header(l_file);
					if (l_tokens == std::nullopt)
					{
						std::lock_guard<std::mutex> l_guard(l_log_lock);
						std::wcerr << L"\n\nFrogman Engine Header Tool Error:\n\tUnable to parse the file located at '" << l_path.c_str();
						l_exit_code = -2;
						return;
					}

					try // The exceptions must be thrown if the input header files have C++ syntex errors.
					{
						// literally removes /**/ and // comments.
						__purge_comments(*l_tokens); // throws if */ is missing.

						// removes the # preprocessor directives and its contents. It cannot remove the text after the \.
						__purge_preprocessor_directives(*l_tokens); // throws if 'text' after # is missing.
					}
					catch (const FE::pair<FrogmanEngineHeaderToolError, FE::ASCII*>& error_p)
					{
						std::wcerr << "\n\nFrogman Engine Header Tool: failed to parse the header file. Skipping the file located at '" << l_path.c_str() << "'.\n\n";
						std::wcerr << error_p._second;
						l_exit_code = (int)error_p._first;
						return;
					}

					algorithm::utility::cherry_pick_if<algorithm::utility::IsolationVector::_Right>(l_tokens->begin(), l_tokens->end(), [](const token& token_p) { return token_p._vocabulary == Vocabulary::_LineEnd; });

					header_file_root l_reflection_tree;
					try // The exceptions must be thrown if the input header files have C++ syntex errors.
					{
						l_reflection_tree = __build_reflection_tree(l_path, *l_tokens); // throws if C++ syntex is incorrect.
					}
					catch (const FE::pair<FrogmanEngineHeaderToolError, FE::ASCII*>& error_p)
					{
						std::wcerr << "\n\nFrogman Engine Header Tool: failed to parse the header file. Skipping the file located at '" << l_path.c_str() << "'.\n\n";
						std::wcerr << error_p._second;
						l_exit_code = (int)error_p._first;
						return; 
					}

					// generate the reflection metadata set. m_reflection_metadata_set is a concurrent vector. 
					this->m_reflection_metadata_set.push_back( __generate_reflection_metadata(l_reflection_tree) );
				}
			);
		}

		// Now, run it.
		get_task_scheduler().access_executor().run(l_taskflow).wait();
		// The number of threads can be scaled via the '-max-concurrency=n' option.

		if (0 != l_exit_code)
		{
			return l_exit_code;
		}

		// generate the reflection code in the generated.cpp file.
		if (this->m_header_tool_options.is_fno_write_defined() == false)
		{
			__generate_reflection_code(this->m_reflection_metadata_set);
		}
	}

	return l_exit_code; // CMake or the current build system has to abort the compliation if the exit code is -1.
}

FE::int32 header_tool_engine::shutdown()
{
	return 0;
}

FE::boolean header_tool_engine::__is_the_file_encoded_with_UTF8_BOM(FE::wchar* directory_p) const noexcept
{
	std::basic_ifstream<var::ASCII> l_BOM_validator;
	l_BOM_validator.open(directory_p);
	FE_EXIT(l_BOM_validator.is_open() == false, FrogmanEngineHeaderToolError::_InputError_NoCopyRightNoticeIsGiven, "Frogman Engine Header Tool ERROR: the path '${%s@0}' is not a valid directory.", directory_p);
	var::uint8 l_BOM[3];
	l_BOM_validator.read(reinterpret_cast<char*>(l_BOM), 3);
	return ((l_BOM[0] == this->m_UTF8_with_BOM[0]) && (l_BOM[1] == this->m_UTF8_with_BOM[1]) && (l_BOM[2] == this->m_UTF8_with_BOM[2]));
}

std::pmr::vector<directory_t> header_tool_engine::__make_header_file_list(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
{
	directory_t l_raw_directories(get_memory_resource());

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

std::pmr::vector<file_buffer_t> header_tool_engine::__map_header_files(const std::pmr::vector<directory_t>& file_list_p) noexcept
{
	std::pmr::vector<file_buffer_t> l_files(get_memory_resource());
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
		FE_EXIT(__is_the_file_encoded_with_UTF8_BOM(path_to_file.c_str()) == false, FrogmanEngineHeaderToolError::_Fatal_InputError_TargetFileIsNotEncodedIn_UTF8_BOM, "Frogman Engine Header Tool ERROR: the header file '${%s@0}' is not encoded in UTF-8 BOM.", path_to_file.c_str());

		std::basic_ifstream<var::UTF8> l_file_handler;
		l_file_handler.imbue(this->m_UTF8_locale);
		l_file_handler.open(path_to_file.c_str());
		FE_EXIT(l_file_handler.is_open() == false, FrogmanEngineHeaderToolError::_FatalError_FailedToOpenFile, "Frogman Engine Header Tool ERROR: failed to open a file. The given path is '${%s@0}'.", path_to_file.c_str());

		l_files.emplace_back(std::istreambuf_iterator<var::UTF8>(l_file_handler), std::istreambuf_iterator<var::UTF8>());
		l_file_handler.close();
	}
	return l_files;
}




