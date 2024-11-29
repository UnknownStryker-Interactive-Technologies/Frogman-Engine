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




enum struct FrogmanEngineHeaderToolError : FE::int32
{
	_CmdInputError_FilesAreNotGiven = 1000,
	_Error_FailedToOpenFile = 1001
};


struct code_style_guide
{
	/*define struct*/
	FE::UTF8* _;
};


// sample data - C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Framework\Include\FE\framework\super_object_base.hpp
class header_tool_engine : public FE::framework::framework_base
{
	using directory_t = std::pmr::basic_string<var::tchar>;
	using file_buffer_t = std::pmr::basic_string<var::UTF8>;

	file_buffer_t m_copyright_notice;
	file_buffer_t m_code_style_guide;
	
	std::pmr::vector<directory_t> m_header_file_list;
	std::pmr::vector<file_buffer_t> m_mapped_header_files;

public:
	header_tool_engine(FE::int32 argc_p, FE::tchar** argv_p) noexcept
		: FE::framework::framework_base(argc_p, argv_p), m_copyright_notice(this->get_memory_resource()), m_code_style_guide(this->get_memory_resource()),
		  m_header_file_list(this->get_memory_resource()), m_mapped_header_files(this->get_memory_resource())
	{

	}
	~header_tool_engine() noexcept override = default;

	virtual FE::int32 launch(FE::int32 argc_p, FE::tchar** argv_p) override
	{
		this->m_copyright_notice = __read_copyright_notice(argc_p, argv_p);
		this->m_code_style_guide = __read_code_style_guide(argc_p, argv_p);

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

		// Define the tasks.
		tf::Taskflow l_taskflow;
		l_taskflow.emplace
		(
			[]()
			{
				std::cout << "Hi, World!";
			}
		);

		// Now, run it.
		get_engine().access_task_scheduler().access_executor()->run(l_taskflow).wait(); // The number of threads can be scaled via the '-max-concurrency=n' option.
		return 0;
	}

	virtual FE::int32 shutdown() override
	{
		return 0;
	}

private:
	file_buffer_t __read_copyright_notice(FE::int32 argc_p, FE::tchar** argv_p) noexcept
	{
		(void)argc_p;
		(void)argv_p;
		return file_buffer_t();
	}

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
			if (	(argv_p[i] != this->m_program_options.view_max_concurrency_option_title()) &&
					((l_h != std::nullopt) || 
					(l_hpp != std::nullopt) || 
					(l_hxx != std::nullopt))
				)
			{
				l_raw_directories = argv_p[i];

				var::count_t l_number_of_files = FE::algorithm::string::count_chars<var::tchar>(l_raw_directories.c_str(), ';')._match_count;
				++l_number_of_files; // CMake does not put ';' to indicate the end of the last directory of the list. So, we need to add 1 to the count.

				std::pmr::vector<directory_t> l_list;
				l_list.reserve(l_number_of_files);

				FE::tchar* l_iterator = l_raw_directories.c_str();
				auto l_result = FE::algorithm::string::find_the_first<var::tchar>(l_iterator, ';');

				while (l_result != std::nullopt)
				{
					FE::index_t l_index_of_it = l_iterator - l_raw_directories.c_str();
					l_list.emplace_back(l_raw_directories.substr(l_index_of_it, l_result->_begin));
					l_iterator += l_result->_end;
					l_result = FE::algorithm::string::find_the_first<var::tchar>(l_iterator, ';');
				}

				l_result = FE::algorithm::string::find_the_last<var::tchar>(l_raw_directories.c_str(), ';');

				if (l_result.has_value() == true)
				{
					l_list.emplace_back(l_raw_directories.substr(l_result->_end, l_raw_directories.length() - l_result->_end));

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

			std::basic_ifstream<var::UTF8> l_file_handler;
			l_file_handler.open(path_to_file.c_str(), std::ios::binary);
			FE_EXIT(l_file_handler.is_open() == false, FrogmanEngineHeaderToolError::_Error_FailedToOpenFile, "Frogman Engine Header Tool: Failed to open a file");
			l_files.emplace_back(std::istreambuf_iterator<var::UTF8>(l_file_handler), std::istreambuf_iterator<var::UTF8>());
			l_file_handler.close();
		}
		return l_files;
	}
};
CUSTOM_ENGINE(header_tool_engine);