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




enum struct FE_HeaderToolError : FE::int32
{
	_FatalInputError_InvalidDirectory = 1000,
};




class header_tool_engine : public FE::framework::framework_base
{
	using directory_list = std::vector<std::pmr::string>;
	directory_list m_directory_list;
	std::pmr::string m_copyright_notice;

	std::unique_ptr<std::thread[]> m_threads;
	std::unique_ptr<typename directory_list::value_type[]> m_mapped_files;

public:
	header_tool_engine(int argc_p, char** argv_p) noexcept : FE::framework::framework_base(argc_p, argv_p), m_copyright_notice(this->get_memory_resource())
	{
		this->m_threads = std::make_unique<std::thread[]>(this->m_program_options._max_concurrency._second);
		this->m_directory_list = __make_list(argc_p, argv_p);

		for (auto it = m_directory_list.cbegin(); it != m_directory_list.cend(); ++it)
		{
			FE_EXIT(it->find("/") == typename directory_list::value_type::npos, FE_HeaderToolError::_FatalInputError_InvalidDirectory, "Please check if the paths to files are correctly formatted.");
		}
		this->m_mapped_files = __map_files(this->m_directory_list);
	}

	~header_tool_engine() noexcept
	{

	}

	virtual int launch(int argc_p, char** argv_p) override
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
	directory_list __make_list(int argc_p, char** argv_p) noexcept
	{
		directory_list l_list;
		l_list.reserve(argc_p);

		for (int i = 0; i < argc_p; ++i)
		{
			if (argv_p[i] != this->m_program_options._max_concurrency._first)
			{
				l_list.emplace_back(this->get_memory_resource());
				l_list.back().assign(argv_p[i]);
			}
		}
		return l_list;
	}

	std::unique_ptr<typename directory_list::value_type[]> __map_files(const directory_list& file_list_p) noexcept
	{
		std::unique_ptr<typename directory_list::value_type[]> l_files = std::make_unique<typename directory_list::value_type[]>(file_list_p.size());

		size_t l_index = 0;
		for (auto& path_to_file : file_list_p)
		{
			std::fstream l_file_handler;
			l_file_handler.open(path_to_file, std::ios::in);
			l_file_handler >> l_files[l_index];
			++l_index;
			l_file_handler.close();
		}
		return l_files;
	}
};
CUSTOM_ENGINE(header_tool_engine);