// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/framework.hpp>
#include <FE/framework/reflection/function_table.hpp>
#include <FE/framework/private/internal_functions.h>
#include <FE/core/fstream_guard.hxx>
#include <FE/core/fstring.hxx>
#include <FE/core/log/logger.hpp>
#include <FE/core/pool_allocator.hxx>

// boost
#include <boost/stacktrace.hpp>

// std
#include <csignal>

#define _DUMP_FILE_NAME_LENGTH_ 128




BEGIN_NAMESPACE(FE::framework)

application* application::s_app = nullptr;
RESTART_OR_NOT application::s_restart_or_not = RESTART_OR_NOT::_NO_OPERATION;

application::initializer_t application::create_application(initializer_t script_p) noexcept
{
	static application::initializer_t l_s_script = script_p;
	return l_s_script;
}


void application::__set_up_main() noexcept
{
	std::signal(SIGTERM, __abnormal_shutdown_with_exit_code);
	std::signal(SIGSEGV, __abnormal_shutdown_with_exit_code);
	std::signal(SIGILL, __abnormal_shutdown_with_exit_code);
	std::signal(SIGABRT, __abnormal_shutdown_with_exit_code);
	std::signal(SIGFPE, __abnormal_shutdown_with_exit_code);
	std::set_terminate([]() { __abnormal_shutdown_with_exit_code(SIGTERM); });
	FE::pool_allocator_base<FE::SIMD_auto_alignment>::create_pool_allocator_resource(1);
	FE::framework::function_table::create_function_table();
}

void application::__shutdown_main() noexcept
{
	delete FE::framework::application::s_app;
	FE::framework::function_table::destroy_function_table();
	FE::pool_allocator_base<FE::SIMD_auto_alignment>::destroy_pool_allocator_resource();
}

_NORETURN_ void application::__abnormal_shutdown_with_exit_code(int32 signal_p)
{
#ifdef _RELEASE_
	boost::stacktrace::stacktrace l_stack_trace_dumps;

	std::ofstream l_release_build_crash_report;
	{
		FE::fstring<_DUMP_FILE_NAME_LENGTH_> l_dump_filename = "Crashed Thread Stack Trace Report from ";
		l_dump_filename += internal::get_current_local_time();
		l_dump_filename += ".txt";
		FE::ofstream_guard l_release_build_crash_report_guard(l_release_build_crash_report, l_dump_filename.c_str());
		l_release_build_crash_report << "Compilation Date: " << " " << __DATE__ << " - " << __TIME__ << "\n\n";
		l_release_build_crash_report << "\n-------------------------------------------------- BEGIN STACK TRACE RECORD --------------------------------------------------\n\n";

		l_release_build_crash_report << boost::stacktrace::to_string(l_stack_trace_dumps).data() << '\n';

		l_release_build_crash_report << "\n-------------------------------------------------- END OF STACK TRACE RECORD --------------------------------------------------\n";

	}
#endif

	FE::framework::application::s_app->clean_up();
	FE::framework::application::__shutdown_main();

	std::exit(signal_p);
}

END_NAMESPACE




int main(int argc_p, char** argv_p)
{
	int l_exit_code;

	do
	{
		FE::framework::application::s_restart_or_not = FE::framework::RESTART_OR_NOT::_NO_OPERATION;

		FE::framework::application::__set_up_main();

		FE::framework::application::initializer_t l_get_app_address = FE::framework::application::create_application();
		FE::framework::application::s_app = l_get_app_address();
		FE_ASSERT(FE::framework::application::s_app == nullptr, "Assertion Failure: ${%s@0} is nullptr.", TO_STRING(FE::framework::application::s_options._application_pointer));

		FE_EXPECT(FE::framework::application::s_app->set_up(argc_p, argv_p), _FE_SUCCESS_, "Failed to set up an app.");
		l_exit_code = FE::framework::application::s_app->run(argc_p, argv_p);
		FE::framework::application::s_app->clean_up();

		FE::framework::application::__shutdown_main();
	}
	while (FE::framework::application::s_restart_or_not == FE::framework::RESTART_OR_NOT::_HAS_TO_RESTART);

	return l_exit_code;
}