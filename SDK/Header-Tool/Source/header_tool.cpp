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
#include "header_tool.hpp"
#include "error_code.hpp"
#include "file_io.hpp"
#include "parser.hpp"
#include "reflexcode_generator.hpp"
#include "symbol_counter.hpp"
#include "tokenizer.hpp"
#include "vocabulary.hpp"

// std::mbstowcs
#include <cstdlib>




header_tool::header_tool(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
	:	FE::framework::framework_base(argc_p, argv_p),
		m_header_tool_options(argc_p, argv_p),
		m_FHT_error_codes()
{
	m_current_system_locale = std::locale::global(std::locale("en_US.UTF-8"));
	std::cout << "Frogman Engine Header Tool: the given program options are - ";
	for (var::int32 i = 0; i < argc_p; ++i)
	{
		std::cout << argv_p[i] << ' ';
	}
	std::cout << '\n';

	std::cout << "Frogman Engine Header Tool: the current locale is " << std::locale().name() << '\n';
}


FE::int32 header_tool::launch(FE::int32 argc_p, FE::ASCII** argv_p)
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
		m_copyright_notice = FHT::file_io::read_copyright_notice(argc_p, argv_p);
	}

	m_header_file_list = FHT::file_io::make_header_file_list(argc_p, argv_p);
	m_mapped_header_files = FHT::file_io::map_header_files(m_header_file_list);

	return 0;
}

FE::int32 header_tool::run()
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
					auto l_tokens = FHT::tokenizer::tokenize_header(l_file, l_path);

					if (l_tokens == std::nullopt)
					{
						std::lock_guard<std::mutex> l_guard(l_log_lock);						// self-reflection: the FHT enabling the enum reflection for itself! It actually works!
						std::cerr << "Frogman Engine Header Tool Error:\n\tThe error code is " << m_FHT_error_codes->enum_to_string(FrogmanEngineHeaderToolError::_InputError_TokenizationFailure) << '\n';
						std::wcerr << L"\033[33mSkipping the header file at: " << l_path.c_str() << "\033[0m\n\n";
						l_exit_code = (int)FrogmanEngineHeaderToolError::_InputError_TokenizationFailure; 
						return;
					}
					
					if (FHT::symbol_counter::validate_parentheses(*l_tokens) != std::nullopt)
					{
						std::lock_guard<std::mutex> l_guard(l_log_lock);
						std::cerr << "Frogman Engine Header Tool Error:\n\tThe error code is " << m_FHT_error_codes->enum_to_string(FrogmanEngineHeaderToolError::_InputError_IncorrectCppSyntax) << '\n';
						std::wcerr << L"\033[33mSkipping the header file at: " << l_path.c_str() << "\033[0m\n\n";
						l_exit_code = (int)FrogmanEngineHeaderToolError::_InputError_IncorrectCppSyntax; 
						return;
					}

					// removes /**/ and // comments.
					FHT::tokenizer::purge_comments(*l_tokens); // throws if */ is missing.
					FE_ASSERT(FHT::symbol_counter::validate_parentheses(*l_tokens) == std::nullopt, "Assertion failed: __purge_comments might corrupted list");

					FHT::tokenizer::purge_string_literals(*l_tokens); // removes the string literals ( quoted texts ).
					FE_ASSERT(FHT::symbol_counter::validate_parentheses(*l_tokens) == std::nullopt, "Assertion failed: __purge_string_literals might corrupted list");

					// removes the # preprocessor directives and its contents. It cannot remove the text after the \.
					FHT::tokenizer::purge_preprocessor_directives(*l_tokens); // throws if 'text' after # is missing.
					FE_ASSERT(FHT::symbol_counter::validate_parentheses(*l_tokens) == std::nullopt, "Assertion failed: __purge_preprocessor_directives might corrupted list");

					std::erase_if(*l_tokens, [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; });
					FE_ASSERT(FHT::symbol_counter::validate_parentheses(*l_tokens) == std::nullopt, "Assertion failed: erase_if might corrupted list");

					//// for debugging purpose.
					//for (auto& v : *l_tokens)
					//{
					//	std::cout << reinterpret_cast<const char*>(v._code.c_str()) << "\n";
					//}
					//std::cout << "\n";

					header_file_root l_reflection_tree;
					try // The exceptions must be thrown if the input header files have C++ syntax errors.
					{
						l_reflection_tree = FHT::parser::try_build_reflection_tree(l_path, *l_tokens); // throws if C++ syntax is incorrect.
					}
					catch (const FE::pair<FrogmanEngineHeaderToolError, FE::ASCII*>& error_p)
					{
						std::lock_guard<std::mutex> l_guard(l_log_lock);
						std::cerr << error_p._second << '\n';
						std::wcerr << L"\033[33mSkipping the header file at: " << l_path.c_str() << "\033[0m\n\n";
						l_exit_code = (int)error_p._first;
						return; 
					}

					// generate the reflection metadata set. m_metadata_set is a concurrent vector. 
					m_metadata_set.push_back(FHT::reflexcode_generator::generate_metadata(l_reflection_tree) );
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
			FHT::reflexcode_generator::generate_reflection_code(m_metadata_set);
		}
	}

	return l_exit_code; // CMake or the current build system has to abort the compliation if the exit code is -1.
}

FE::int32 header_tool::shutdown()
{
	return 0;
}