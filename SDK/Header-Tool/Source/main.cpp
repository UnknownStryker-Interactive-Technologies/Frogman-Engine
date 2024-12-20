/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// FE.Framework
#include <FE/framework/framework.hpp>

// string algorithm
#include <FE/algorithm/string.hxx>




enum struct FrogmanEngineHeaderToolError : FE::int32
{
	_CmdInputError_NoProgramOptionsAreGiven = 1000,
	_CmdInputError_FilesAreNotGiven = 1001,
	_Error_FailedToOpenFile = 1002,
	_Error_NoCopyRightNoticeIsGiven = 1003,
	_Error_TargetFileIsNotEncodedIn_UTF8_BOM = 1004
};


class program_options
{
	FE::pair<FE::tchar*, FE::tchar*> m_path_to_copyright_notice;

	FE::pair<FE::tchar*, var::boolean> m_fno_copyright_notice;
	FE::pair<FE::tchar*, var::boolean> m_fno_code_style_guide;
	FE::pair<FE::tchar*, var::boolean> m_fno_reflection_helper;
	FE::pair<FE::tchar*, var::boolean> m_fno_op;
	FE::pair<FE::tchar*, var::boolean> m_fno_write;

public:
	program_options(FE::int32 argc_p, FE::tchar** argv_p) noexcept 
		: m_path_to_copyright_notice{ "-path-to-copyright-notice=", "\0" },
		  m_fno_copyright_notice{ "-fno-copyright-notice", false }, 
		  m_fno_code_style_guide{ "-fno-code-style-guide", false }, 
		  m_fno_reflection_helper{ "-fno-reflection-helper", false },
		  m_fno_op{ "-fno-op", false }, m_fno_write{ "-fno-write", false }
	{
		using namespace FE;

		for (var::int32 i = 0; i < argc_p; ++i)
		{
			switch (algorithm::string::hash(argv_p[i], algorithm::string::length(argv_p[i])))
			{
			case algorithm::string::hash("-fno-copyright-notice", algorithm::string::length("-fno-copyright-notice")):
				this->m_fno_copyright_notice._second = true;
				break;

			case algorithm::string::hash("-fno-code-style-guide", algorithm::string::length("-fno-code-style-guide")):
				this->m_fno_code_style_guide._second = true;
				break;

			case algorithm::string::hash("-fno-reflection-helper", algorithm::string::length("-fno-reflection-helper")):
				this->m_fno_reflection_helper._second = true;
				break;

			case algorithm::string::hash("-fno-op", algorithm::string::length("-fno-op")):
				this->m_fno_op._second = true;
				break;

			case algorithm::string::hash("-fno-write", algorithm::string::length("-fno-write")):
				this->m_fno_write._second = true;
				break;

			default:
				if (algorithm::string::find_the_first(argv_p[i], this->m_path_to_copyright_notice._first) != std::nullopt)
				{
					this->m_path_to_copyright_notice._second = argv_p[i] + algorithm::string::find_the_first(argv_p[i], '=')->_end;
				}
				break;
			}
		}
	}
	~program_options() noexcept = default;


	FE::tchar* view_path_to_copyright_notice_option_title() const noexcept { return this->m_path_to_copyright_notice._first; }
	FE::tchar* get_path_to_copyright_notice() const noexcept { return this->m_path_to_copyright_notice._second; }

	FE::tchar* view_fno_copyright_notice_option_title() const noexcept { return this->m_fno_copyright_notice._first; }
	FE::boolean is_fno_copyright_notice_defined() const noexcept { return this->m_fno_copyright_notice._second; }

	FE::tchar* view_fno_code_style_guide_option_title() const noexcept { return this->m_fno_code_style_guide._first; }
	FE::boolean is_fno_code_style_guide_defined() const noexcept { return this->m_fno_code_style_guide._second; }

	FE::tchar* view_fno_reflection_helper_option_title() const noexcept { return this->m_fno_reflection_helper._first; }
	FE::boolean is_fno_reflection_helper_defined() const noexcept { return this->m_fno_reflection_helper._second; }

	FE::tchar* view_fno_op_option_title() const noexcept { return this->m_fno_op._first; }
	FE::boolean is_fno_op_defined() const noexcept { return this->m_fno_op._second; }

	FE::tchar* view_fno_write_option_title() const noexcept { return this->m_fno_write._first; }
	FE::boolean is_fno_write_defined() const noexcept { return this->m_fno_write._second; }
};

// -fno-code-style-guide -fno-reflection-helper -path-to-copyright-notice=C:/Users/leeho/OneDrive/문서/GitHub/Frogman-Engine/Template/CMake/../LICENSE.txt C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FE-HT-Test\HeaderWithCopyright.hpp;C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FE-HT-Test\HeaderWithoutCopyright.hpp
class header_tool_engine : public FE::framework::framework_base
{
	using directory_t = std::pmr::basic_string<var::tchar>;
	using file_buffer_t = std::pmr::basic_string<var::UTF8>;

	FE::uint8 m_UTF8_with_BOM[3];
	std::locale m_UTF8_locale;

	program_options m_header_tool_options;
	file_buffer_t m_copyright_notice;
	file_buffer_t m_code_style_guide;
	
	std::pmr::vector<directory_t> m_header_file_list;
	std::pmr::vector<file_buffer_t> m_mapped_header_files;

public:
	header_tool_engine(FE::int32 argc_p, FE::tchar** argv_p) noexcept
		: FE::framework::framework_base(argc_p, argv_p), m_UTF8_with_BOM{ 0xEF, 0xBB, 0xBF }, m_UTF8_locale("en_US.UTF-8"), m_header_tool_options(argc_p, argv_p),
		  m_copyright_notice(this->get_memory_resource()), m_code_style_guide(this->get_memory_resource()),
		  m_header_file_list(this->get_memory_resource()), m_mapped_header_files(this->get_memory_resource())
	{
		std::cout << "The given program options are: ";
		for (var::int32 i = 0; i < argc_p; ++i)
		{
			std::cout << argv_p[i] << ' ';
		}
		std::cout << '\n';
	}
	~header_tool_engine() noexcept override = default;

	virtual FE::int32 launch(FE::int32 argc_p, FE::tchar** argv_p) override
	{
		if (this->m_header_tool_options.is_fno_op_defined() == true)
		{
			std::cerr << "\n\nFrogman Engine Header Tool: No operation will be done. Exiting the program.\n\n";
			exit(0);
		}

		if (*(this->m_header_tool_options.get_path_to_copyright_notice()) != '\0')
		{
			this->m_copyright_notice = __read_copyright_notice(argc_p, argv_p);
		}

		if (this->m_header_tool_options.is_fno_code_style_guide_defined() == false)
		{
			this->m_code_style_guide = __read_code_style_guide(argc_p, argv_p);
		}

		this->m_header_file_list = __make_header_file_list(argc_p, argv_p);
		this->m_mapped_header_files = __map_header_files(this->m_header_file_list);

		return 0;
	}

	virtual FE::int32 run() override
	{
		/* Jobs to do:
		1. Check the presence of the given copyright notice.
		2. Check if the contents of the header files are conforming to the code style guide.
		3. Auto-generate the reflection macro function arguments.
		4. Report the issues found in the header files.

		Those jobs can be done in parallel by considering the header fiiles as jobs.
		*/

		tf::Taskflow l_taskflow;
		var::int32 l_exit_code = 0;
		if (this->m_header_tool_options.is_fno_copyright_notice_defined() == false)
		{
			FE::uint64 l_number_of_files = this->m_mapped_header_files.size();
			for (var::uint64 i = 0; i < l_number_of_files; ++i)
			{
				l_taskflow.emplace
				(
					[i, &l_exit_code, this]()
					{
						file_buffer_t& l_file = this->m_mapped_header_files[i];
						FE::boolean l_result = FE::algorithm::string::space_insensitive_contains(FE::algorithm::string::BOM_cast(l_file.c_str()), l_file.size(), FE::algorithm::string::BOM_cast(this->m_copyright_notice.c_str()));
						directory_t& l_path = this->m_header_file_list[i];
						if (l_result == false)
						{
							std::cerr << "\n\nFrogman Engine Header Tool WARNING:\n\tThe file located at '" << l_path.c_str() << "' has no copy of the specified copyright notice.\n\n";
							l_exit_code = -1;
						}
					}
				);
			}

			// Now, run it.
			get_engine().get_task_scheduler().access_executor().run(l_taskflow).wait();
			// The number of threads can be scaled via the '-max-concurrency=n' option.
		}

		return l_exit_code; // CMake or the current build system has to abort the compliation if the exit code is -1.
	}

	virtual FE::int32 shutdown() override
	{
		return 0;
	}

private:
	FE::boolean __is_the_file_encoded_with_UTF8_BOM(FE::tchar* directory_p) noexcept
	{
		std::basic_ifstream<var::ASCII> l_BOM_validator;
		l_BOM_validator.open(directory_p);
		FE_EXIT(l_BOM_validator.is_open() == false, FrogmanEngineHeaderToolError::_Error_NoCopyRightNoticeIsGiven, "Frogman Engine Header Tool ERROR: the path '${%s@0}' is not a valid directory.", directory_p);
		var::uint8 l_BOM[3];
		l_BOM_validator.read(reinterpret_cast<char*>(l_BOM), 3);
		return ((l_BOM[0] == this->m_UTF8_with_BOM[0]) && (l_BOM[1] == this->m_UTF8_with_BOM[1]) && (l_BOM[2] == this->m_UTF8_with_BOM[2]));
	}

	file_buffer_t __read_copyright_notice(FE::int32 argc_p, FE::tchar** argv_p) noexcept
	{
		for (var::int32 i = 0; i < argc_p; ++i)
		{
			if (FE::algorithm::string::find_the_first(argv_p[i], this->m_header_tool_options.get_path_to_copyright_notice()))
			{
				FE_EXIT(__is_the_file_encoded_with_UTF8_BOM(this->m_header_tool_options.get_path_to_copyright_notice()) == false, FrogmanEngineHeaderToolError::_Error_TargetFileIsNotEncodedIn_UTF8_BOM, "Frogman Engine Header Tool ERROR: the license text file '${%s@0}' is not encoded in UTF-8 BOM.", this->m_header_tool_options.get_path_to_copyright_notice());
				
				std::basic_ifstream<var::UTF8> l_file_handler;
				l_file_handler.imbue(this->m_UTF8_locale);

				l_file_handler.open(this->m_header_tool_options.get_path_to_copyright_notice());
				FE_EXIT(l_file_handler.is_open() == false, FrogmanEngineHeaderToolError::_Error_NoCopyRightNoticeIsGiven, "Frogman Engine Header Tool ERROR: the program option '${%s@0}' is not defined but no license text file is given. The given path is '${%s@1}'", this->m_header_tool_options.view_fno_copyright_notice_option_title(), this->m_header_tool_options.get_path_to_copyright_notice());
				
				file_buffer_t l_copyright_notice(std::istreambuf_iterator<var::UTF8>(l_file_handler), std::istreambuf_iterator<var::UTF8>(), this->get_memory_resource());
				l_file_handler.close();
				return l_copyright_notice;
			}
		}
		return file_buffer_t();
	}

	/* This is unavailable yet! */
	file_buffer_t __read_code_style_guide(FE::int32 argc_p, FE::tchar** argv_p) noexcept
	{
		(void)argc_p;
		(void)argv_p;
		return file_buffer_t();
	}

	std::pmr::vector<directory_t> __make_header_file_list(FE::int32 argc_p, FE::tchar** argv_p) noexcept
	{
		directory_t l_raw_directories(this->get_memory_resource());

		for (int i = 0; i < argc_p; ++i)
		{
			auto l_h = FE::algorithm::string::find_the_first<var::tchar>(argv_p[i], ".h");
			auto l_hpp = FE::algorithm::string::find_the_first<var::tchar>(argv_p[i], ".hpp");
			auto l_hxx = FE::algorithm::string::find_the_first<var::tchar>(argv_p[i], ".hxx");
			if ((l_h != std::nullopt) || 
				(l_hpp != std::nullopt) || 
				(l_hxx != std::nullopt))
			{
				l_raw_directories = argv_p[i];

				var::uint64 l_number_of_files = FE::algorithm::string::count_chars<var::tchar>(l_raw_directories.c_str(), ';')._match_count;
				++l_number_of_files; // CMake does not put ';' to indicate the end of the last directory of the list. So, we need to add 1 to the count.

				std::pmr::vector<directory_t> l_list;
				l_list.reserve(l_number_of_files);
			
				FE::tchar* l_end_of_path = l_raw_directories.c_str();
				auto l_path_seperator = FE::algorithm::string::find_the_first<var::tchar>(l_end_of_path, ';');

				while (l_path_seperator != std::nullopt)
				{
					FE::uint64 l_path = l_end_of_path - l_raw_directories.c_str();
					l_list.emplace_back(l_raw_directories.substr(l_path, l_path_seperator->_begin));
					l_end_of_path += l_path_seperator->_end;
					l_path_seperator = FE::algorithm::string::find_the_first<var::tchar>(l_end_of_path, ';');
				}

				l_path_seperator = FE::algorithm::string::find_the_last<var::tchar>(l_raw_directories.c_str(), ';');

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

	std::pmr::vector<file_buffer_t> __map_header_files(const std::pmr::vector<directory_t>& file_list_p) noexcept
	{
		std::pmr::vector<file_buffer_t> l_files(this->get_memory_resource());
		l_files.reserve(file_list_p.size());

		for (auto& path_to_file : file_list_p)
		{
			auto l_h = path_to_file.find(".h");
			auto l_hpp = path_to_file.find(".hpp");
			auto l_hxx = path_to_file.find(".hxx");
			if ((l_h == std::string::npos) && (l_hpp == std::string::npos) && (l_hxx == std::string::npos))
			{
				continue;
			}
			FE_EXIT(__is_the_file_encoded_with_UTF8_BOM(path_to_file.c_str()) == false, FrogmanEngineHeaderToolError::_Error_TargetFileIsNotEncodedIn_UTF8_BOM, "Frogman Engine Header Tool ERROR: the header file '${%s@0}' is not encoded in UTF-8 BOM.", path_to_file.c_str());
			
			std::basic_ifstream<var::UTF8> l_file_handler;
			l_file_handler.imbue(this->m_UTF8_locale);
			l_file_handler.open(path_to_file.c_str());
			FE_EXIT(l_file_handler.is_open() == false, FrogmanEngineHeaderToolError::_Error_FailedToOpenFile, "Frogman Engine Header Tool ERROR: failed to open a file. The given path is '${%s@0}'.", path_to_file.c_str());
			
			l_files.emplace_back(std::istreambuf_iterator<var::UTF8>(l_file_handler), std::istreambuf_iterator<var::UTF8>());
			l_file_handler.close();
		}
		return l_files;
	}
};
CUSTOM_ENGINE(header_tool_engine);