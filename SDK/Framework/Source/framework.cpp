/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#include <FE/framework.hxx>
#include <FE/framework/reflection/private/load_reflection_data.hxx>

#include <FE/algorithm/string.hxx>
#include <FE/algorithm/utility.hxx>
#include <FE/clock.hxx>
#include <FE/do_once.hxx>
#include <FE/fstream_guard.hxx>
#include <FE/log/logger.hxx>

// boost
#include <boost/stacktrace.hpp>

// std
#include <csignal>
#include <optional>
#include <filesystem>
#include <string>

#ifdef _FE_ON_WINDOWS_X86_64_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#undef WIN32_LEAN_AND_MEAN
#endif




BEGIN_NAMESPACE(FE::framework)


program_options::program_options(FE::int32 argc_p, FE::ASCII** argv_p) noexcept : m_max_concurrency{ "-max-concurrency=", std::thread::hardware_concurrency() >> 1 }
{
	for (var::int32 i = 0; i < argc_p; ++i)
	{
		if (algorithm::string::find_the_first<var::ASCII>(argv_p[i], m_max_concurrency._first) == std::nullopt)
		{
			continue;
		}

		std::optional<algorithm::string::range> l_range = algorithm::string::find_the_first<var::ASCII>(m_max_concurrency._first, '=');
		l_range->_begin = 0;

		if (algorithm::string::compare_ranged<var::ASCII>(argv_p[i], *l_range, m_max_concurrency._first, *l_range) == true)
		{
			algorithm::utility::uint_info l_uint_info = algorithm::utility::string_to_uint<var::ASCII>(argv_p[i] + l_range->_end);
			m_max_concurrency._second = static_cast<FE::uint32>(l_uint_info._value);

			if (l_uint_info._value < 6)
			{
				FE_LOG(FE::log::Severity::_Warning, "Warning, the option '${%s@0}${%u@1}' has no effect. The -max-concurrency must be greater than 6.\nThe value given to the option will be overriden with the default value '6'.", m_max_concurrency._first, &l_uint_info._value);
				m_max_concurrency._second = 6;
			}
			else if (l_uint_info._value > FE::int16_max)
			{
				FE_LOG(FE::log::Severity::_Warning, "Warning, the option '${%s@0}${%u@1}' has no effect. The number of thread must be less than (2^16) / 2.\nThe value given to the option will be overriden with the default value '6'.", m_max_concurrency._first, &l_uint_info._value);
				m_max_concurrency._second = 6;
			}
			break;
		}
	}

	if (m_max_concurrency._second < 6)
	{
		m_max_concurrency._second = 6;
		FE_LOG(FE::log::Severity::_Warning, "Performance Warning: the current system's CPU lacks logical hardware threads; Frogman Engine based games run slow on systems with CPU logical hardware threads less than six.");
	}
}

FE::uint32 program_options::get_max_concurrency() const noexcept
{
	return m_max_concurrency._second;
}

FE::ASCII* program_options::view_max_concurrency_option_title() const noexcept
{
	return m_max_concurrency._first;
}




static framework_base* s_framework = nullptr;
static RestartOrNot s_restart_or_not = RestartOrNot::_NoOperation;




framework_base::framework_base(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
	:	m_program_options(argc_p, argv_p), 
		m_current_system_locale(std::setlocale(LC_ALL, "")), 
		m_memory(std::make_unique<FE::memory_resource[]>( m_program_options.get_max_concurrency() )), 
		m_method_reflection(81920, get_memory_resource()), 
		m_property_reflection(81920, get_memory_resource()),
		m_enum_reflection(81920),
		m_ecs(),
		m_processors()
{
	std::locale::global(m_current_system_locale);
}




void framework_base::__load_reflection_data() noexcept
{
	load_reflection_data();
}

void framework_base::request_restart() noexcept
{
	s_restart_or_not = RestartOrNot::_HasToRestart;
}

void framework_base::cancel_restart() noexcept
{
	s_restart_or_not = RestartOrNot::_NoOperation;
}

framework_base& framework_base::get_framework() noexcept
{
	return *s_framework;
}

const program_options& framework_base::get_program_options() const noexcept
{
	return m_program_options;
}

const std::locale& framework_base::get_current_system_locale() const noexcept
{
	return m_current_system_locale;
}

std::pmr::memory_resource* framework_base::get_memory_resource() noexcept
{
	return &(m_memory[get_current_thread_id()]);
}

reflection::method_registry& framework_base::get_method_reflection() noexcept
{
	return m_method_reflection;
}

reflection::property_registry& framework_base::get_property_reflection() noexcept
{
	return m_property_reflection;
}

reflection::enum_registry& framework_base::get_enum_reflection() noexcept
{
	return m_enum_reflection;
}

framework::ECS& framework_base::get_ecs() noexcept
{
	return *m_ecs;
}

framework::processors& framework_base::get_processors() noexcept
{
	return *m_processors;
}




_FE_NORETURN_ void framework_base::__abnormal_shutdown_with_exit_code(int signal_p)
{
#ifdef _RELWITHDEBINFO_
	boost::stacktrace::stacktrace l_stack_trace_dumps;

	std::ofstream l_release_build_crash_report;
	{
		std::string l_dump_filename = "Crashed Thread Stack Trace Report from ";
		l_dump_filename += FE::clock::get_current_local_time();
		l_dump_filename += ".aar";
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




std::function<framework_base* (FE::int32, FE::ASCII**)>& framework_base::allocate_framework(std::function<framework_base* (FE::int32, FE::ASCII**)> script_p) noexcept
{
	static std::function<framework_base* (FE::int32, FE::ASCII**)> l_s_script = script_p;
	return l_s_script;
}


END_NAMESPACE




int main(FE::int32 argc_p, FE::ASCII** argv_p)
{
	var::int32 l_exit_code;

	std::signal(SIGTERM, FE::framework::framework_base::__abnormal_shutdown_with_exit_code);
	std::signal(SIGSEGV, FE::framework::framework_base::__abnormal_shutdown_with_exit_code);
	std::signal(SIGILL, FE::framework::framework_base::__abnormal_shutdown_with_exit_code);
	std::signal(SIGABRT, FE::framework::framework_base::__abnormal_shutdown_with_exit_code);
	std::signal(SIGFPE, FE::framework::framework_base::__abnormal_shutdown_with_exit_code);
	std::set_terminate([]() { FE::framework::framework_base::__abnormal_shutdown_with_exit_code(SIGTERM); });

	do
	{
		FE::framework::s_restart_or_not = FE::framework::RestartOrNot::_NoOperation;

		FE::framework::s_framework = FE::framework::framework_base::allocate_framework()(argc_p, argv_p);
		FE_EXIT_IF(FE::framework::s_framework == nullptr, FE::ErrorCode::_FatalMemoryError_1XX_NullPtr, "\nAn error from FE.Framework: An app pointer is a nullptr.\n");
		
		l_exit_code = FE::framework::s_framework->launch(argc_p, argv_p);

		if (l_exit_code != 0)
		{
			std::cerr << "\nAn error from FE.Framework: failed to set up an app.\n";
			delete FE::framework::s_framework;
			return l_exit_code;
		}

		l_exit_code = FE::framework::s_framework->run();

		if (l_exit_code != 0)
		{
			std::cerr << "\nAn error from FE.Framework: there was an error during the runtime.\n";
			delete FE::framework::s_framework;
			return l_exit_code;
		}

		l_exit_code = FE::framework::s_framework->shutdown();

		if (l_exit_code != 0)
		{
			std::cerr << "\nAn error from FE.Framework: unsuccessfully cleaned up an app.\n";
			delete FE::framework::s_framework;
			return l_exit_code;
		}

		delete FE::framework::s_framework;
	}
	while (FE::framework::s_restart_or_not == FE::framework::RestartOrNot::_HasToRestart);

	return l_exit_code;
}




#ifdef _FE_ON_WINDOWS_X86_64_

FE::int32 __count_command_line_arguments(FE::ASCII* cmd_line_p)
{
	var::int32 l_argc = 0;
	
	if ((*cmd_line_p != ' ') || (*cmd_line_p != '\0'))
	{
		++l_argc;
	}

	for (FE::ASCII* it = cmd_line_p; *it != '\0'; ++it)
	{
		if (*it == ';')
		{
			++l_argc;
		}
	}
	return l_argc;
}

int WINAPI WinMain(_FE_MAYBE_UNUSED_ HINSTANCE hInstance, _FE_MAYBE_UNUSED_ HINSTANCE hPrevInstance, LPSTR lpCmdLine, _FE_MAYBE_UNUSED_ int nShowCmd)
{
	var::int32 l_argc = __count_command_line_arguments(lpCmdLine);
	std::unique_ptr<FE::ASCII*[]> l_argv = std::make_unique<FE::ASCII*[]>( (l_argc == 0) ? 1 : l_argc );

	if (l_argc > 0)
	{
		l_argv[0] = lpCmdLine;
	}

	var::int32 l_index = 1;
	for (var::ASCII* it = lpCmdLine; *it != '\0';)
	{
		if (*it == ';')
		{
			*it = '\0';
			++it;

			if (*it != '\0')
			{
				l_argv[l_index] = it;
				++l_index;
			}
			continue;
		}
		++it;
	}
	return main(l_argc, l_argv.get());
}

#endif