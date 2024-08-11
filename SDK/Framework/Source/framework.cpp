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

application_base* application_base::s_app = nullptr;
RESTART_OR_NOT application_base::s_restart_or_not = RESTART_OR_NOT::_NO_OPERATION;

application_base::initializer_t application_base::create_application(initializer_t script_p) noexcept
{
	static application_base::initializer_t l_s_script = script_p;
	return l_s_script;
}

void application_base::__set_up_main() noexcept
{
	std::signal(SIGTERM, __abnormal_shutdown_with_exit_code);
	std::signal(SIGSEGV, __abnormal_shutdown_with_exit_code);
	std::signal(SIGILL, __abnormal_shutdown_with_exit_code);
	std::signal(SIGABRT, __abnormal_shutdown_with_exit_code);
	std::signal(SIGFPE, __abnormal_shutdown_with_exit_code);
	std::set_terminate([]() { __abnormal_shutdown_with_exit_code(SIGTERM); });

	FE::framework::application_base::initializer_t l_get_app_address = FE::framework::application_base::create_application();
	FE::framework::application_base::s_app = l_get_app_address();
	FE_ASSERT(FE::framework::application_base::s_app == nullptr, "Assertion Failure: An app pointer is nullptr.");
	FE::framework::reflection::initialize();
}

void application_base::__shutdown_main() noexcept
{
	FE::framework::reflection::clean_up();
	delete FE::framework::application_base::s_app;
}

_NORETURN_ void application_base::__abnormal_shutdown_with_exit_code(int32 signal_p)
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
	FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, FE::framework::application_base::s_app->clean_up(); FE::framework::application_base::s_app->__shutdown_main());
	std::exit(signal_p);
}

END_NAMESPACE




int main(int argc_p, char** argv_p)
{
	int l_exit_code;

	do
	{
		FE::framework::application_base::s_restart_or_not = FE::framework::RESTART_OR_NOT::_NO_OPERATION;

		FE::framework::application_base::__set_up_main();

		FE_EXPECT(FE::framework::application_base::s_app->set_up(argc_p, argv_p), _FE_SUCCESS_, "Failed to set up an app.");
		l_exit_code = FE::framework::application_base::s_app->run(argc_p, argv_p);
		FE::framework::application_base::s_app->clean_up();

		FE::framework::application_base::__shutdown_main();
	}
	while (FE::framework::application_base::s_restart_or_not == FE::framework::RESTART_OR_NOT::_HAS_TO_RESTART);

	return l_exit_code;
}