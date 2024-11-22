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



// sample data - C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Framework\Include\FE\framework\super_object_base.hpp
class header_tool_engine : public FE::framework::framework_base
{
	using string_type = std::pmr::basic_string<var::tchar>;
	using directory_list = std::pmr::vector<string_type>;

	std::pmr::vector<std::thread> m_threads;
	string_type m_copyright_notice;
	
	directory_list m_directory_list;
	std::pmr::vector<string_type> m_mapped_files;

public:
	header_tool_engine(int argc_p, FE::tchar** argv_p) noexcept 
		: FE::framework::framework_base(argc_p, argv_p), m_threads(this->get_memory_resource()), m_copyright_notice(this->get_memory_resource()), m_directory_list(this->get_memory_resource()), m_mapped_files(this->get_memory_resource())
	{
		this->m_threads.resize(this->m_program_options._max_concurrency._second);
		this->m_directory_list = __make_directory_list(argc_p, argv_p);
		this->m_mapped_files = __map_files(this->m_directory_list);
	}
	~header_tool_engine() noexcept override = default;

	virtual int launch(int argc_p, FE::tchar** argv_p) override
	{
		(void)argc_p;
		(void)argv_p;
		// launch threads
		for (var::size i = 0; i < this->m_program_options._max_concurrency._second; ++i)
		{
			this->m_threads[i] = std::thread(&header_tool_engine::__process_files, this);
		}

		for (var::size i = 0; i < this->m_program_options._max_concurrency._second; ++i)
		{
			this->m_threads[i].join();
		}
		return 0;
	}

	virtual int run() override
	{
		// report errors
		return 0;
	}

	virtual int shutdown() override
	{
		// write to files
		return 0;
	}

	static void __process_files(header_tool_engine* this_p) noexcept
	{
		// process files
		(void)this_p;
	}

private:
	directory_list __make_directory_list(int argc_p, FE::tchar** argv_p) noexcept
	{
		string_type l_raw_directories(this->get_memory_resource());

		for (int i = 0; i < argc_p; ++i)
		{
			auto l_h = FE::algorithm::string::find_the_first<var::tchar>(argv_p[i], ".h");
			auto l_hpp = FE::algorithm::string::find_the_first<var::tchar>(argv_p[i], ".hpp");
			auto l_hxx = FE::algorithm::string::find_the_first<var::tchar>(argv_p[i], ".hxx");
			if (	(argv_p[i] != this->m_program_options._max_concurrency._first) &&
					((l_h != std::nullopt) || 
					(l_hpp != std::nullopt) || 
					(l_hxx != std::nullopt))
				)
			{
				l_raw_directories = argv_p[i];

				var::count_t l_number_of_files = FE::algorithm::string::count_chars<var::tchar>(l_raw_directories.c_str(), ';')._match_count;
				++l_number_of_files; // CMake does not put ';' to indicate the end of the last directory of the list. So, we need to add 1 to the count.

				directory_list l_list;
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

		return directory_list();
	}

	std::pmr::vector<string_type> __map_files(const directory_list& file_list_p) noexcept
	{
		std::pmr::vector<string_type> l_files(file_list_p.size(), this->get_memory_resource());

		size_t l_index = 0;
		for (auto& path_to_file : file_list_p)
		{
			auto l_h = path_to_file.find(".h");
			auto l_hpp = path_to_file.find(".hpp");
			auto l_hxx = path_to_file.find(".hxx");
			if ((l_h == std::string::npos) && (l_hpp == std::string::npos) && (l_hxx == std::string::npos))
			{
				continue;
			}

			std::basic_fstream<var::tchar> l_file_handler;
			l_file_handler.open(path_to_file.c_str(), std::ios::in | std::ios::binary);
			FE_EXIT(l_file_handler.is_open() == false, FrogmanEngineHeaderToolError::_Error_FailedToOpenFile, "Frogman Engine Header Tool: Failed to open a file");
			//l_file_handler >> l_files[l_index];
			++l_index;
			l_file_handler.close();
		}
		return l_files;
	}
};
CUSTOM_ENGINE(header_tool_engine);