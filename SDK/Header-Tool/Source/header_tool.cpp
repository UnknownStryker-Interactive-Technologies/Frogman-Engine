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

					try
					{
						//---------------- Throwable Methods Below This Line ----------------//
						// throws if the header file is ill-formed.
						auto l_tokens = FHT::tokenizer::tokenize_header(l_file, l_path);	// tokenize the header file to get the tokens.


						//---------------- Noexcept Methods Below This Line ----------------//
						// removes /**/ and // comments.
						FHT::tokenizer::purge_comments(l_tokens);

						// removes the # preprocessor directives and its contents.
						FHT::tokenizer::purge_preprocessor(l_tokens); 

						FHT::tokenizer::purge_string_literals_and_backslashes(l_tokens); // removes the \, characters, and strings.

						FHT::tokenizer::purge_template(l_tokens); // removes the template declarations.

						FHT::tokenizer::purge_forward_declaration(l_tokens); // removes the forward declarations.

						
						std::erase_if(l_tokens, [](const token& token_p) -> FE::boolean { return token_p._vocabulary == Vocabulary::_LineEnd; }); // It is a bug if this function throws.
						
						header_file_root l_reflection_tree;
						l_reflection_tree = FHT::parser::try_build_reflection_tree(l_path, l_tokens); // Parse the header; throws if the C++ header file is ill-formed.
												

						//---------------- Noexcept Methods Below This Line ----------------//
						// generate and add the reflection metadata to the in-house concurrent vector. 
						m_metadata_set.push_back( FHT::reflexcode_generator::generate_metadata(l_reflection_tree) );
					}
					catch (const FE::pair<FrogmanEngineHeaderToolError, FE::ASCII*>& error_p) // The exceptions must be thrown if the input header files have C++ syntax errors.
					{
						std::lock_guard<std::mutex> l_guard(l_log_lock);
						std::cerr << error_p._second << '\n';
						std::wcerr << L"\033[33mSkipping the header file at: " << l_path.c_str() << "\033[0m\n\n";
						l_exit_code = (int)error_p._first;
						return; 
					}
					
					//// for debugging purpose.
					//for (auto& v : *l_tokens)
					//{
					//	std::cout << reinterpret_cast<const char*>(v._code.c_str()) << "\n";
					//}
					//std::cout << "\n";
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
			FHT::reflexcode_generator::generate_reflexcode(m_metadata_set);
		}
	}

	return l_exit_code; // CMake or the current build system has to abort the compliation if the exit code is -1.
}

FE::int32 header_tool::shutdown()
{
	return 0;
}