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
#include <FE/framework/framework.hpp>
#include <FE/algorithm/string.hxx>
#include <FE/algorithm/utility.hxx>
#include <FE/clock.hpp>
#include <FE/do_once.hxx>
#include <FE/fqueue.hxx>
#include <FE/fstream_guard.hxx>
#include <FE/log/logger.hpp>

// FE.framework
#include <FE/framework/reflection/reflection.h>

// boost
#include <boost/stacktrace.hpp>

// std
#include <csignal>
#include <optional>
#include <string>
#include <mutex>

// Windows
#include <processthreadsapi.h>




BEGIN_NAMESPACE(FE::framework)

framework_base* framework_base::s_framework = nullptr;
RestartOrNot framework_base::s_restart_or_not = RestartOrNot::_NoOperation;

FE::framework::reflection_system* reflection_system::s_instance = nullptr;


std::function<framework_base* (int, char**)>& framework_base::__allocate_framework(std::function<framework_base* (int, char**)> script_p) noexcept
{
	static std::function<framework_base* (int, char**)> l_s_script = script_p;
	return l_s_script;
}

_FE_NORETURN_ void framework_base::__abnormal_shutdown_with_exit_code(int32 signal_p)
{
#ifdef _RELWITHDEBINFO_
	boost::stacktrace::stacktrace l_stack_trace_dumps;

	std::ofstream l_release_build_crash_report;
	{
		std::string l_dump_filename = "Crashed Thread Stack Trace Report from ";
		l_dump_filename += FE::clock::get_current_local_time();
		l_dump_filename += ".txt";
		FE::ofstream_guard l_release_build_crash_report_guard(l_release_build_crash_report);
		l_release_build_crash_report_guard.get_stream().open(l_dump_filename.c_str());
		l_release_build_crash_report << "Compilation Date: " << " " << __DATE__ << " - " << __TIME__ << "\n\n";
		l_release_build_crash_report << "\n-------------------------------------------------- BEGIN STACK TRACE RECORD --------------------------------------------------\n\n";

		l_release_build_crash_report << boost::stacktrace::to_string(l_stack_trace_dumps).data() << '\n';

		l_release_build_crash_report << "\n-------------------------------------------------- END OF STACK TRACE RECORD --------------------------------------------------\n";

	}
#endif
	//FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, FE::framework::framework_base::s_framework->shutdown(); FE::framework::framework_base::s_framework->__shutdown_main());
	std::exit(signal_p);
}




namespace internal::thread_id
{
	static FE::fqueue<var::uint8, 255> s_thread_ids;
	static std::uint8_t s_next_thread_id = 0;
	static std::mutex s_lock;

	class __generator
	{
		var::uint8 m_thread_id;

	public:
		__generator() noexcept
		{
			std::lock_guard<std::mutex> l_lock(s_lock);
			if (s_thread_ids.is_empty() == false)
			{
				this->m_thread_id = s_thread_ids.pop();
				return;
			}
			FE_ASSERT(s_next_thread_id < 255);
			this->m_thread_id = s_next_thread_id;
			++s_next_thread_id;
		}

		~__generator() noexcept
		{
			std::lock_guard<std::mutex> l_lock(s_lock);
			s_thread_ids.push(this->m_thread_id);
		}

		FE::uint8 get_id() const noexcept
		{
			return this->m_thread_id;
		}
	};
}

FE::uint8 get_current_thread_id() noexcept
{
	thread_local static internal::thread_id::__generator tl_s_id_generator;
	return tl_s_id_generator.get_id();
}




game_engine::game_engine(int argc_p, char** argv_p) : framework_base(argc_p, argv_p)
{
	for (var::int32 i = 0; i < argc_p; ++i)
	{
		std::optional<algorithm::string::range> l_range = algorithm::string::find_the_first(this->m_program_options._max_concurrency._first, '=');
		l_range->_begin = 0;

		if (algorithm::string::compare_ranged(argv_p[i], *l_range, this->m_program_options._max_concurrency._first, *l_range) == true)
		{
			algorithm::utility::uint_info l_uint_info = algorithm::utility::string_to_uint(argv_p[i] + l_range->_end);
			this->m_program_options._max_concurrency._second = l_uint_info._value;
			
			if (l_uint_info._value < 3)
			{
				FE_LOG("Warning, the option '${%s@0}${%u@1}' has no effect. The number of thread must be greater than 3.\nThe value given to the option will be overriden with the default value '4'.", this->m_program_options._max_concurrency._first, &l_uint_info._value);
				this->m_program_options._max_concurrency._second = 4;
			}
			else if (l_uint_info._value > 254)
			{
				FE_LOG("Warning, the option '${%s@0}${%u@1}' has no effect. The number of thread must be less than 255.\nThe value given to the option will be overriden with the default value '4'.", this->m_program_options._max_concurrency._first, &l_uint_info._value);
				this->m_program_options._max_concurrency._second = 4;
			}
			break;
		}
	}
	this->m_memory = FE::make_unique<FE::scalable_pool<2 * FE::one_gb, FE::align_CPU_L1_cache_line>[]>(this->m_program_options._max_concurrency._second);

}

game_engine::~game_engine()
{

}


int game_engine::launch(_FE_MAYBE_UNUSED_ int argc_p, _FE_MAYBE_UNUSED_ char** argv_p)
{

	return 0;
}

int game_engine::run()
{

	return 0;
}

int game_engine::shutdown()
{
	return 0;
}


END_NAMESPACE




int main(int argc_p, char** argv_p)
{
	int l_exit_code;

	std::signal(SIGTERM, FE::framework::framework_base::__abnormal_shutdown_with_exit_code);
	std::signal(SIGSEGV, FE::framework::framework_base::__abnormal_shutdown_with_exit_code);
	std::signal(SIGILL, FE::framework::framework_base::__abnormal_shutdown_with_exit_code);
	std::signal(SIGABRT, FE::framework::framework_base::__abnormal_shutdown_with_exit_code);
	std::signal(SIGFPE, FE::framework::framework_base::__abnormal_shutdown_with_exit_code);
	std::set_terminate([]() { FE::framework::framework_base::__abnormal_shutdown_with_exit_code(SIGTERM); });

	do
	{
		FE::framework::framework_base::s_restart_or_not = FE::framework::RestartOrNot::_NoOperation;

		FE::framework::framework_base::s_framework = FE::framework::framework_base::__allocate_framework()(argc_p, argv_p);
		FE_EXIT(FE::framework::framework_base::s_framework == nullptr, FE::ErrorCode::_FATAL_MEMORY_ERROR_1XX_NULLPTR, "Assertion Failure: An app pointer is nullptr.");
		
		l_exit_code = FE::framework::framework_base::s_framework->launch(argc_p, argv_p);
		FE_EXIT(l_exit_code != 0, l_exit_code, "Failed to set up an app.");

		l_exit_code = FE::framework::framework_base::s_framework->run();
		FE_EXIT(l_exit_code != 0, l_exit_code, "There was an error during the runtime.");

		FE::framework::framework_base::s_framework->shutdown();
		FE_EXIT(l_exit_code != 0, l_exit_code, "Unsuccessfully cleaned up an app.");

		delete FE::framework::framework_base::s_framework;
	}
	while (FE::framework::framework_base::s_restart_or_not == FE::framework::RestartOrNot::_HasToRestart);

	return l_exit_code;
}

