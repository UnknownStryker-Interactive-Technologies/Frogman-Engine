#ifndef _FE_FRAMEWORK_HPP_
#define _FE_FRAMEWORK_HPP_
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
#include <FE/prerequisites.h>
#include <FE/pair.hxx>

// std
#include <functional>


#ifdef FROGMAN_ENGINE
	#error Frogman Engine Prohibits macroizing the keyword "FROGMAN_ENGINE()".
#else                                                                                                              // The name below does not follow the naming convention since it is considered hidden from users.
	#define FROGMAN_ENGINE() static ::std::function<::FE::framework::framework_base* (FE::int32, FE::tchar**)> FrogmanEngine = ::FE::framework::framework_base::allocate_framework( [](FE::int32 argc_p, FE::tchar** argv_p) { return new ::FE::framework::game_engine(argc_p, argv_p); } );
#endif

#ifdef CUSTOM_ENGINE
    #error Frogman Engine Prohibits macroizing the keyword "CUSTOM_ENGINE()".
#else                                                                                                                        // The name below does not follow the naming convention since it is considered hidden from users.
    #define CUSTOM_ENGINE(framework_class_name) static ::std::function<::FE::framework::framework_base* (FE::int32, FE::tchar**)> CustomEngine = ::FE::framework::framework_base::allocate_framework( [](FE::int32 argc_p, FE::tchar** argv_p) { return new framework_class_name(argc_p, argv_p); } );
#endif
#include <FE/pool/memory_resource.hpp>

#include <FE/framework/managed.hpp>
#include <FE/framework/reflection.hpp>
#include <FE/framework/task.hpp>

// boost::function
#include <boost/functional.hpp>




int _tmain(int argc_p, FE::tchar** argv_p);




BEGIN_NAMESPACE(FE::framework)


enum struct RestartOrNot : uint8
{
	_NoOperation = 0,
	_HasToRestart = 1,
};

class program_options
{
	FE::pair<FE::tchar*, var::uint32> m_max_concurrency;

public:
	program_options(FE::int32 argc_p, FE::tchar** argv_p) noexcept;
	~program_options() noexcept = default;

	FE::uint32 get_max_concurrency() const noexcept;
	FE::tchar* view_max_concurrency_option_title() const noexcept;
};


class framework_base
{
	friend int ::_tmain(int argc_p, FE::tchar** argv_p);

	static RestartOrNot s_restart_or_not;
	static framework_base* s_framework;
	
protected:
	program_options m_program_options;
	std::unique_ptr<FE::scalable_pool_resource<FE::PoolPageCapacity::_Max>[]> m_memory;
	framework::managed m_reference_manager;
	reflection::method m_method_reflection;
	reflection::property m_property_reflection;
	framework::task_scheduler m_cpu;

public:
	framework_base(FE::int32 argc_p, FE::tchar** argv_p) noexcept;
	virtual ~framework_base() noexcept;

	static void request_restart() noexcept;
	static framework_base& get_engine() noexcept;

	std::pmr::memory_resource* get_memory_resource() noexcept;
	framework::managed& access_reference_manager() noexcept;
	reflection::method& access_method_reflection() noexcept;
	reflection::property& access_property_reflection() noexcept;
	framework::task_scheduler& access_task_scheduler() noexcept;

protected:
	virtual FE::int32 launch(FE::int32 argc_p, FE::tchar** argv_p);
	virtual FE::int32 run();
	virtual FE::int32 shutdown();

public:
	_FE_NORETURN_ static void __abnormal_shutdown_with_exit_code(int signal_p);

	// Do not call this function directly.
	static std::function<framework_base* (FE::int32, FE::tchar**)>& allocate_framework(std::function<framework_base* (FE::int32, FE::tchar**)> script_p = [](FE::int32, FE::tchar**) { return nullptr; }) noexcept;

	framework_base(const framework_base&) = delete;
	framework_base(framework_base&&) = delete;
	framework_base& operator=(const framework_base&) = delete;
	framework_base& operator=(framework_base&&) = delete;
};


class game_instance;

class game_engine : public framework_base
{
	std::unique_ptr<game_instance> m_game_instance;
	//d3d11_renderer m_renderer;

public:
	game_engine(FE::int32 argc_p, FE::tchar** argv_p);
	~game_engine();

private:
	virtual FE::int32 launch(FE::int32 argc_p, FE::tchar** argv_p) override;
	virtual FE::int32 run() override;
	virtual FE::int32 shutdown() override;
};


END_NAMESPACE
#endif