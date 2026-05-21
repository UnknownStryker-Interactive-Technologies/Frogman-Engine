/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

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

#include <FE/framework/processors.hxx>

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
#include <ntsecapi.h>
#pragma comment(lib, "advapi32.lib")
#undef WIN32_LEAN_AND_MEAN
#endif




BEGIN_NAMESPACE(FE::framework)


bool __enable_lock_memory_privilege() noexcept
{
	HANDLE l_token;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &l_token))
	{
		FE_LOG(FE::log::Severity::_Warning, "\nAn error from FE.Framework: OpenProcessToken failed to acquire the current process's access token.\n");
		return false;
	}

	LUID l_luid;
	if (!LookupPrivilegeValue(nullptr, "SeLockMemoryPrivilege", &l_luid))
	{
		FE_LOG(FE::log::Severity::_Warning, "\nAn error from FE.Framework: LookupPrivilegeValue failed to retrieve the LUID for SeLockMemoryPrivilege.\n");
		CloseHandle(l_token);
		return false;
	}

	TOKEN_PRIVILEGES l_tp = { .PrivilegeCount = 1 };
	l_tp.Privileges[0].Luid = l_luid;
	l_tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(l_token, false, &l_tp, 0, nullptr, nullptr))
	{
		FE_LOG(FE::log::Severity::_Warning, "\nAn error from FE.Framework: AdjustTokenPrivileges failed to enable SeLockMemoryPrivilege.\n");
		CloseHandle(l_token);
		return false;
	}

	DWORD l_error = GetLastError();
	if (l_error != 0)
	{
		// 1300 = ERROR_NOT_ALL_ASSIGNED: privilege not in token
		FE_LOG(FE::log::Severity::_Warning, "\nAn error from FE.Framework: AdjustTokenPrivileges did not assign SeLockMemoryPrivilege. GetLastError returned ${%u@0}.\n", &l_error);
		CloseHandle(l_token);
		return false;
	}

	CloseHandle(l_token);
	return true;
}


bool __has_privilege() noexcept
{
	SIZE_T l_lpSize = GetLargePageMinimum();
	if (l_lpSize == 0) _FE_UNLIKELY_
	{
		FE_LOG(FE::log::Severity::_Warning, "\nAn error from FE.Framework: GetLargePageMinimum failed to retrieve the system's large page size. Large page support might not be available on this system.\n");
		return false;
	}

	if (__enable_lock_memory_privilege() == false)
	{
		FE_LOG(FE::log::Severity::_Warning, "\nAn error from FE.Framework: failed to enable SeLockMemoryPrivilege. Large page support might not be available on this system.\n");
		return false;
	}
	FE_LOG(FE::log::Severity::_Info, "\n__enable_lock_memory_privilege succeeded\n");

	void* l_ptr = VirtualAlloc(nullptr, l_lpSize, MEM_RESERVE | MEM_COMMIT | MEM_LARGE_PAGES, PAGE_READWRITE);
	if (l_ptr == nullptr)
	{
		_FE_MAYBE_UNUSED_ DWORD l_error = GetLastError();
		FE_LOG(FE::log::Severity::_Warning, "\nAn error from FE.Framework: VirtualAlloc failed to allocate large pages. GetLastError returned ${%u@0}.\n", &l_error);
		return false;
	}

	VirtualFree(l_ptr, 0, MEM_RELEASE);
	FE_LOG(FE::log::Severity::_Info, "\nVirtualAlloc with MEM_LARGE_PAGES succeeded\n");
	return true;
}


bool __try_enable_large_pages(FE::ASCII*& error_message_p) noexcept
{	
	HANDLE l_token = nullptr;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &l_token))
	{
		error_message_p = "OpenProcessToken Failed: Unable to acquire the current process's access token.";
		return false;
	}

	DWORD l_size = 0;
	GetTokenInformation(l_token, TokenUser, nullptr, 0, &l_size);
	if (l_size == 0)
	{
		error_message_p = "GetTokenInformation Failed: Unable to retrieve the required buffer size for the token information.";
		CloseHandle(l_token);
		return false;
	}

	PTOKEN_USER l_user = (PTOKEN_USER)LocalAlloc(LPTR, l_size);
	if (!l_user) _FE_UNLIKELY_
	{
		error_message_p = "LocalAlloc Failed";
		CloseHandle(l_token);
		return false;
	}

	if (!GetTokenInformation(l_token, TokenUser, l_user, l_size, &l_size))
	{
		error_message_p = "GetTokenInformation Failed: Unable to retrieve the token information.";
		LocalFree(l_user);
		CloseHandle(l_token);
		return false;
	}
	CloseHandle(l_token);

	LSA_UNICODE_STRING l_sys_name = {};
	LSA_OBJECT_ATTRIBUTES l_obj_attr = {.Length = sizeof(LSA_OBJECT_ATTRIBUTES)};
	LSA_HANDLE l_policy;

	NTSTATUS l_status = LsaOpenPolicy(&l_sys_name, &l_obj_attr, POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES, &l_policy);
	if (l_status != 0)
	{
		error_message_p = "LsaOpenPolicy Failed";
		LocalFree(l_user);
		return false;
	}

	FE::wchar l_se_lock_memory_privilege[] = L"SeLockMemoryPrivilege";
	auto l_length = algorithm::string::length(l_se_lock_memory_privilege);

	LSA_UNICODE_STRING l_privilege = 
	{
		.Length = (unsigned short)(l_length * 2),
		.MaximumLength = (unsigned short)((l_length + 1) * 2),
		.Buffer = (var::wchar*)l_se_lock_memory_privilege
	};

	l_status = LsaAddAccountRights(l_policy, l_user->User.Sid, &l_privilege, 1);
	LocalFree(l_user);

	if (l_status != 0)
	{
		error_message_p = "LsaAddAccountRights Failed";
		LsaClose(l_policy);
		return false;
	}
	LsaClose(l_policy);
	return true;
}




program_option::program_option(FE::int32 argc_p, FE::ASCII** argv_p) noexcept 
	: m_max_concurrency{ "-max-concurrency=", std::thread::hardware_concurrency() >> 1 },
	  m_enable_large_pages{ "-enable-large-pages", false }
{
	for (var::int32 i = 0; i < argc_p; ++i)
	{
		if (algorithm::string::find_the_first<var::ASCII>(argv_p[i], m_max_concurrency._first) != std::nullopt)
		{
			std::optional<algorithm::string::range> l_range = algorithm::string::find_the_first<var::ASCII>(m_max_concurrency._first, '=');
			l_range->_begin = 0;

			if (algorithm::string::compare_ranged<var::ASCII>(argv_p[i], *l_range, m_max_concurrency._first, *l_range) == true)
			{
				algorithm::utility::uint_info l_uint_info = algorithm::utility::string_to_uint<var::ASCII>(argv_p[i] + l_range->_end);
				m_max_concurrency._second = static_cast<FE::uint32>(l_uint_info._value);

				if (l_uint_info._value <= 6)
				{
					FE_LOG(FE::log::Severity::_Warning, "Warning, the option '${%s@0}${%u@1}' has no effect. The -max-concurrency must be greater than or equal to 6.\nThe value given to the option will be overridden with the default value '6'.", m_max_concurrency._first, &l_uint_info._value);
					m_max_concurrency._second = 6;
					continue;
				}
				
				if (l_uint_info._value >= 64)
				{
					FE_LOG(FE::log::Severity::_Warning, "Warning, the option '${%s@0}${%u@1}' has no effect. The number of thread must be less than or equal to 64.\nThe value given to the option will be overridden with the default value '6'.", m_max_concurrency._first, &l_uint_info._value);
					m_max_concurrency._second = 6;
					continue;
				}
			}
			continue;
		}


		if (algorithm::string::find_the_first<var::ASCII>(argv_p[i], m_enable_large_pages._first) != std::nullopt)
		{
			m_enable_large_pages._second = true;
			continue;
		}
	}


	if (m_max_concurrency._second < 6)
	{
		m_max_concurrency._second = 6;
		FE_LOG(FE::log::Severity::_Warning, "Performance Warning: the current system's CPU lacks logical hardware threads; Frogman Engine based games might run slow on systems with CPU logical hardware threads less than six.");
	}


	if (m_enable_large_pages._second == true)
	{

		if (__has_privilege() == false)
		{
			FE::ASCII* l_error_message = nullptr;
			FE::boolean l_does_require_reboot_and_was_successful = __try_enable_large_pages(l_error_message);
			FE_EXIT_IF(l_does_require_reboot_and_was_successful == true, 0, "Frogman Engine Runtime: the large page enablement was successful. Please reboot your system; ${%s@0}.", l_error_message);
			FE_EXIT_IF(l_does_require_reboot_and_was_successful == false, FE::ErrorCode::_FatalMemoryError_4XX_LargePageEnablementFailure, "Frogman Engine Runtime: failed to enable large pages but the option was specified. The error message is: '${%s@0}'", l_error_message);
		}
	}
}

FE::uint32 program_option::get_max_concurrency() const noexcept
{
	return m_max_concurrency._second;
}

FE::ASCII* program_option::view_max_concurrency_option_title() const noexcept
{
	return m_max_concurrency._first;
}

::FE::boolean program_option::is_large_pages_enabled() const noexcept
{
	return m_enable_large_pages._second;
}

::FE::ASCII* program_option::view_large_pages_option_title() const noexcept
{
	return m_enable_large_pages._first;
}




static framework_base* s_framework = nullptr;
static RestartOrNot s_restart_or_not = RestartOrNot::_NoOperation;



static std::pmr::memory_resource* s_TLGPMP_deleter = nullptr;
static std::pmr::memory_resource* s_LTLGPMP_deleter = nullptr;
framework_base::framework_base(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
	:	m_program_options(argc_p, argv_p), 
		m_current_system_locale(std::setlocale(LC_ALL, "")), 
	m_memory( new FE::memory_resource[m_program_options.get_max_concurrency()]{} ), // the new operator is overloaded to return CPU cache line size aligned memory.
	m_memory_large_pages(	(m_program_options.is_large_pages_enabled() == true) ? 
								(std::pmr::memory_resource*)new FE::large::memory_resource[m_program_options.get_max_concurrency()]{} 
							:	(std::pmr::memory_resource*)new FE::memory_resource[m_program_options.get_max_concurrency()]{} 
						),
		m_method_reflection(81920, get_large_memory_resource()), 
		m_property_reflection(81920, get_large_memory_resource()),
		m_enum_reflection(get_large_memory_resource(), 81920),
		m_ecs(),
		m_processors()
{
	std::locale::global(m_current_system_locale);
	s_TLGPMP_deleter = m_memory;
	s_LTLGPMP_deleter = m_memory_large_pages;
}

framework_base::~framework_base() noexcept
{
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

const program_option& framework_base::get_program_options() const noexcept
{
	return m_program_options;
}

const std::locale& framework_base::get_current_system_locale() const noexcept
{
	return m_current_system_locale;
}

std::pmr::memory_resource* framework_base::get_memory_resource() noexcept
{
	return ((FE::memory_resource*)m_memory) + get_current_thread_id();
}

std::pmr::memory_resource* framework_base::get_large_memory_resource() noexcept
{
	// compute once
	static FE::size l_size_of_page_element = (m_program_options.is_large_pages_enabled() == true) ? sizeof(FE::large::memory_resource) : sizeof(FE::memory_resource);

	return (std::pmr::memory_resource*)( ((var::byte*)m_memory_large_pages) + (l_size_of_page_element * get_current_thread_id()) );
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
			delete[] FE::framework::s_TLGPMP_deleter;
			delete[] FE::framework::s_LTLGPMP_deleter;
			return l_exit_code;
		}

		l_exit_code = FE::framework::s_framework->run();

		if (l_exit_code != 0)
		{
			std::cerr << "\nAn error from FE.Framework: there was an error during the runtime.\n";
			delete FE::framework::s_framework;
			delete[] FE::framework::s_TLGPMP_deleter;
			delete[] FE::framework::s_LTLGPMP_deleter;
			return l_exit_code;
		}

		l_exit_code = FE::framework::s_framework->shutdown();

		if (l_exit_code != 0)
		{
			std::cerr << "\nAn error from FE.Framework: unsuccessfully cleaned up an app.\n";
			delete FE::framework::s_framework;
			delete[] FE::framework::s_TLGPMP_deleter;
			delete[] FE::framework::s_LTLGPMP_deleter;
			return l_exit_code;
		}

		delete FE::framework::s_framework;
		delete[] FE::framework::s_TLGPMP_deleter;
		delete[] FE::framework::s_LTLGPMP_deleter;
	}
	while (FE::framework::s_restart_or_not == FE::framework::RestartOrNot::_HasToRestart);

	return l_exit_code;
}




#ifdef _FE_ON_WINDOWS_X86_64_

FE::int32 __count_command_line_arguments(FE::ASCII* cmd_line_p)
{
	var::int32 l_argc = 0;
	
	if ((*cmd_line_p != ' ') && (*cmd_line_p != '\0'))
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