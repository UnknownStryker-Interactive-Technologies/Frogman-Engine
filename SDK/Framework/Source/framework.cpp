// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/framework.hpp>
#include <FE/algorithm/string.hxx>
#include <FE/algorithm/utility.hxx>
#include <FE/clock.hpp>
#include <FE/do_once.hxx>
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




BEGIN_NAMESPACE(FE::framework)

framework_base* framework_base::s_framework = nullptr;
RESTART_OR_NOT framework_base::s_restart_or_not = RESTART_OR_NOT::_NO_OPERATION;

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




game_engine::game_engine(int argc_p, char** argv_p)
{
	if (argv_p != nullptr)
	{
		this->m_program_options._exe_directory = *argv_p;
	}

	for (var::int32 i = 1; i < argc_p; ++i)
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
			break;
		}
	}
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
		FE::framework::framework_base::s_restart_or_not = FE::framework::RESTART_OR_NOT::_NO_OPERATION;

		FE::framework::reflection::function::__initialize();
		FE::framework::reflection::property::__initialize();

		FE::framework::framework_base::s_framework = FE::framework::framework_base::__allocate_framework()(argc_p, argv_p);
		FE_EXIT(FE::framework::framework_base::s_framework == nullptr, FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR, "Assertion Failure: An app pointer is nullptr.");

		l_exit_code = FE::framework::framework_base::s_framework->launch(argc_p, argv_p);
		FE_EXIT(l_exit_code != 0, l_exit_code, "Failed to set up an app.");

		l_exit_code = FE::framework::framework_base::s_framework->run();
		FE_EXIT(l_exit_code != 0, l_exit_code, "There was an error during the runtime.");

		FE::framework::framework_base::s_framework->shutdown();
		FE_EXIT(l_exit_code != 0, l_exit_code, "Unsuccessfully cleaned up an app.");

		delete FE::framework::framework_base::s_framework;

		FE::framework::reflection::property::__shutdown();
		FE::framework::reflection::function::__shutdown();
	}
	while (FE::framework::framework_base::s_restart_or_not == FE::framework::RESTART_OR_NOT::_HAS_TO_RESTART);

	return l_exit_code;
}
