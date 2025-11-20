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
#define _CRT_SECURE_NO_WARNINGS
#include "header_tool_engine.hpp"
#include "error_code.hpp"

// std::mbstowcs
#include <cstdlib>




header_tool_engine::header_tool_engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
	:	FE::framework::framework_base(argc_p, argv_p),
		m_UTF8_with_BOM{ 0xEF, 0xBB, 0xBF },
		m_UTF8_locale("en_US.UTF-8"),
		m_header_tool_options(argc_p, argv_p),
		m_FHT_error_codes()
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
	__load_reflection_data();
	m_FHT_error_codes = FE::framework::framework_base::get_framework().get_enum_reflection().retrieve_enum_struct_metadata("::FrogmanEngineHeaderToolError");

	if (m_header_tool_options.is_fno_op_defined() == true)
	{
		std::cerr << "\n\nFrogman Engine Header Tool: No operation will be done. Exiting the program.\n\n";
		std::exit(0);
	}

	if (*(m_header_tool_options.get_path_to_copyright_notice()) != '\0')
	{
		m_copyright_notice = __read_copyright_notice(argc_p, argv_p);
	}

	m_header_file_list = __make_header_file_list(argc_p, argv_p);
	m_mapped_header_files = __map_header_files(m_header_file_list);

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
	tf::Executor l_executor(m_max_concurrency.get_max_concurrency() - 1); // exlude the main thread.
	var::int32 l_exit_code = 0;
	FE::uint64 l_number_of_files = m_mapped_header_files.size();
	std::mutex l_log_lock;

	if (m_header_tool_options.is_fno_copyright_notice_defined() == false)
	{
		for (var::uint64 i = 0; i < l_number_of_files; ++i)
		{
			l_taskflow.emplace
			(
				[i, &l_exit_code, &l_log_lock, this]
				{
					file_buffer_t& l_file = m_mapped_header_files[i];

					// Check the presence of the given copy right notice.
					FE::boolean l_result = algorithm::string::space_insensitive_contains(algorithm::string::skip_BOM(l_file.c_str()), l_file.size(), algorithm::string::skip_BOM(m_copyright_notice.c_str()));
					directory_t& l_path = m_header_file_list[i];

					if (l_result == false) // The given copy right notice is not found.
					{
						std::lock_guard<std::mutex> l_guard(l_log_lock);
						std::cerr << "Frogman Engine Header Tool WARNING:\n\tThe file has no copy of the specified copyright notice.\n";
						std::wcerr << L"\033[33mSkipping the header file at: " << l_path.c_str() << "\033[0m\n\n";
						l_exit_code = (int)FrogmanEngineHeaderToolError::_InputError_NoCopyRightNoticeIsGiven;
					}
				}
			);
		}

		// Now, run it.
		l_executor.run(l_taskflow).wait();
		// The number of threads can be scaled via the '-max-concurrency=n' option.

		if (0 != l_exit_code)
		{
			return l_exit_code;
		}
	}

	l_taskflow.clear();

	if (m_header_tool_options.is_fno_reflection_helper_defined() == false)
	{
		for (var::uint64 i = 0; i < l_number_of_files; ++i)
		{
			l_taskflow.emplace
			(
				[i, &l_exit_code, &l_log_lock, this]
				{
					file_buffer_t& l_file = m_mapped_header_files[i];
					directory_t& l_path = m_header_file_list[i];

					// tokenize the header file to get the tokens.
					auto l_tokens = __tokenize_header(l_file, l_path);

					if (l_tokens == std::nullopt)
					{
						std::lock_guard<std::mutex> l_guard(l_log_lock);
						std::cerr << "Frogman Engine Header Tool Error:\n\tThe error code is " << m_FHT_error_codes->enum_to_string(FrogmanEngineHeaderToolError::_InputError_TokenizationFailure) << '\n';
						std::wcerr << L"\033[33mSkipping the header file at: " << l_path.c_str() << "\033[0m\n\n";
						l_exit_code = (int)FrogmanEngineHeaderToolError::_InputError_TokenizationFailure; 
						return;
					}
					
					if (__validate_parentheses(*l_tokens) != std::nullopt)
					{
						std::lock_guard<std::mutex> l_guard(l_log_lock);
						std::cerr << "Frogman Engine Header Tool Error:\n\tThe error code is " << m_FHT_error_codes->enum_to_string(FrogmanEngineHeaderToolError::_InputError_IncorrectCppSyntax) << '\n';
						std::wcerr << L"\033[33mSkipping the header file at: " << l_path.c_str() << "\033[0m\n\n";
						l_exit_code = (int)FrogmanEngineHeaderToolError::_InputError_IncorrectCppSyntax; 
						return;
					}

					// removes /**/ and // comments.
					__purge_comments(*l_tokens); // throws if */ is missing.
					FE_ASSERT(__validate_parentheses(*l_tokens) == std::nullopt, "Assertion failed: __purge_comments might corrupted list");

					__purge_string_literals(*l_tokens); // removes the string literals ( quoted texts ).
					FE_ASSERT(__validate_parentheses(*l_tokens) == std::nullopt, "Assertion failed: __purge_string_literals might corrupted list");

					// removes the # preprocessor directives and its contents. It cannot remove the text after the \.
					__purge_preprocessor_directives(*l_tokens); // throws if 'text' after # is missing.
					FE_ASSERT(__validate_parentheses(*l_tokens) == std::nullopt, "Assertion failed: __purge_preprocessor_directives might corrupted list");

					std::erase_if(*l_tokens, [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; });
					FE_ASSERT(__validate_parentheses(*l_tokens) == std::nullopt, "Assertion failed: erase_if might corrupted list");

					//// for debugging purpose.
					//for (auto& v : *l_tokens)
					//{
					//	std::cout << reinterpret_cast<const char*>(v._code.c_str()) << "\n";
					//}
					//std::cout << "\n";

					header_file_root l_reflection_tree;
					try // The exceptions must be thrown if the input header files have C++ syntax errors.
					{
						l_reflection_tree = __try_build_reflection_tree(l_path, *l_tokens); // throws if C++ syntax is incorrect.
					}
					catch (const FE::pair<FrogmanEngineHeaderToolError, FE::ASCII*>& error_p)
					{
						std::lock_guard<std::mutex> l_guard(l_log_lock);
						std::cerr << error_p._second << '\n';
						std::wcerr << L"\033[33mSkipping the header file at: " << l_path.c_str() << "\033[0m\n\n";
						l_exit_code = (int)error_p._first;
						return; 
					}

					// generate the reflection metadata set. m_reflection_metadata_set is a concurrent vector. 
					m_reflection_metadata_set.push_back( __generate_reflection_metadata(l_reflection_tree) );
				}
			);
		}

		// Now, run it.
		l_executor.run(l_taskflow).wait();
		// The number of threads can be scaled via the '-max-concurrency=n' option.

		if (0 != l_exit_code)
		{
			return l_exit_code;
		}

		// generate the reflection code in the generated.cpp file.
		if (m_header_tool_options.is_fno_write_defined() == false)
		{
			__generate_reflection_code(m_reflection_metadata_set);
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

	if (l_BOM_validator.is_open() == false)
	{
		std::wcerr << "Frogman Engine Header Tool ERROR: the path '" << directory_p << "' is not a valid directory.\n";
		return false;
	}
	
	var::uint8 l_BOM[3];
	l_BOM_validator.read(reinterpret_cast<char*>(l_BOM), 3);
	return ((l_BOM[0] == m_UTF8_with_BOM[0]) && (l_BOM[1] == m_UTF8_with_BOM[1]) && (l_BOM[2] == m_UTF8_with_BOM[2]));
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

		if (__is_the_file_encoded_with_UTF8_BOM(path_to_file.c_str()) == false)
		{
			std::wcerr << "Frogman Engine Header Tool ERROR: the header file '" << path_to_file.c_str() << "' is not encoded in UTF-8 BOM.\n";
			continue;
		}

		std::basic_ifstream<var::UTF8> l_file_handler;
		l_file_handler.imbue(m_UTF8_locale);
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




