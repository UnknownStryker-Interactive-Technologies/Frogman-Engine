// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/framework.hpp>
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
#include <string>

#define _DUMP_FILE_NAME_LENGTH_ 128




BEGIN_NAMESPACE(FE::framework)


void read_program_options(_FE_MAYBE_UNUSED_ program_options& out_options_p) noexcept
{
}

void read_runtime_configurations(_FE_MAYBE_UNUSED_ runtime_configurations& out_configs_p) noexcept
{
}


application_base* application_base::s_app = nullptr;
RESTART_OR_NOT application_base::s_restart_or_not = RESTART_OR_NOT::_NO_OPERATION;

std::function<application_base* (int, char**)> application_base::create_application(std::function<application_base* (int, char**)> script_p) noexcept
{
	static std::function<application_base* (int, char**)> l_s_script = script_p;
	return l_s_script;
}

_FE_NORETURN_ void application_base::__abnormal_shutdown_with_exit_code(int32 signal_p)
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
	//FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, FE::framework::application_base::s_app->clean_up(); FE::framework::application_base::s_app->__shutdown_main());
	std::exit(signal_p);
}




vulkan_application::vulkan_application(_FE_MAYBE_UNUSED_ int argc_p, _FE_MAYBE_UNUSED_ char** argv_p)
{
}

int vulkan_application::launch(_FE_MAYBE_UNUSED_ int argc_p, _FE_MAYBE_UNUSED_ char** argv_p)
{
	return 0;
}

int vulkan_application::run()
{
	this->m_task_scheduler.run();
	this->m_renderer.run();
	this->m_input_manager.run();
	return 0;
}

int vulkan_application::shutdown()
{
	this->m_input_manager.shutdown();
	this->m_renderer.shutdown();
	this->m_task_scheduler.shutdown();
	return 0;
}


END_NAMESPACE




int main(int argc_p, char** argv_p)
{
	int l_exit_code;

	do
	{
		FE::framework::application_base::s_restart_or_not = FE::framework::RESTART_OR_NOT::_NO_OPERATION;

		std::signal(SIGTERM, FE::framework::application_base::__abnormal_shutdown_with_exit_code);
		std::signal(SIGSEGV, FE::framework::application_base::__abnormal_shutdown_with_exit_code);
		std::signal(SIGILL, FE::framework::application_base::__abnormal_shutdown_with_exit_code);
		std::signal(SIGABRT, FE::framework::application_base::__abnormal_shutdown_with_exit_code);
		std::signal(SIGFPE, FE::framework::application_base::__abnormal_shutdown_with_exit_code);
		std::set_terminate([]() { FE::framework::application_base::__abnormal_shutdown_with_exit_code(SIGTERM); });

		FE::framework::application_base::s_app = FE::framework::application_base::create_application()(argc_p, argv_p);
		FE_EXIT(FE::framework::application_base::s_app == nullptr, FE::ERROR_CODE::_FATAL_MEMORY_ERROR_1XX_NULLPTR, "Assertion Failure: An app pointer is nullptr.");
		
		FE::framework::reflection::initialize();

		l_exit_code = FE::framework::application_base::s_app->launch(argc_p, argv_p);
		FE_EXIT(l_exit_code != 0, l_exit_code, "Failed to set up an app.");

		l_exit_code = FE::framework::application_base::s_app->run();
		FE_EXIT(l_exit_code != 0, l_exit_code, "There was an error during the runtime.");

		FE::framework::application_base::s_app->shutdown();
		FE_EXIT(l_exit_code != 0, l_exit_code, "Unsuccessfully cleaned up an app.");

		FE::framework::reflection::clean_up();

		delete FE::framework::application_base::s_app;
	}
	while (FE::framework::application_base::s_restart_or_not == FE::framework::RESTART_OR_NOT::_HAS_TO_RESTART);

	return l_exit_code;
}
