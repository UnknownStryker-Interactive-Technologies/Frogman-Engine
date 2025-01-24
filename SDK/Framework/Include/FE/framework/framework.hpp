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
	#define FROGMAN_ENGINE() static ::std::function<::FE::framework::framework_base* (FE::int32, FE::ASCII**)> FrogmanEngine = ::FE::framework::framework_base::allocate_framework( [](FE::int32 argc_p, FE::ASCII** argv_p) { return new ::FE::framework::game_framework_base(argc_p, argv_p); } );
#endif

#ifdef CUSTOM_ENGINE
    #error Frogman Engine Prohibits macroizing the keyword "CUSTOM_ENGINE()".
#else                                                                                                                        // The name below does not follow the naming convention since it is considered hidden from users.
    #define CUSTOM_ENGINE(framework_class_name) static ::std::function<::FE::framework::framework_base* (FE::int32, FE::ASCII**)> CustomEngine = ::FE::framework::framework_base::allocate_framework( [](FE::int32 argc_p, FE::ASCII** argv_p) { return new framework_class_name(argc_p, argv_p); } );
#endif
#include <FE/pool/memory_resource.hpp>

#include <FE/framework/ECS.hpp>
#include <FE/framework/reflection.hpp>
#include <FE/framework/task.hpp>

// boost::function
#include <boost/functional.hpp>




int main(FE::int32 argc_p, FE::ASCII** argv_p);




BEGIN_NAMESPACE(FE::framework)


enum struct RestartOrNot : uint8
{
	_NoOperation = 0,
	_HasToRestart = 1,
};

class program_options
{
	FE::pair<FE::ASCII*, var::uint32> m_max_concurrency;

public:
	program_options(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
	~program_options() noexcept = default;

	FE::uint32 get_max_concurrency() const noexcept;
	FE::ASCII* view_max_concurrency_option_title() const noexcept;
};


/*
The framework_base class serves as a foundational component for a framework
managing program options, memory resources, and task scheduling, while providing mechanisms for launching, running, and shutting down the framework
along with reflection capabilities for methods and properties.
*/
class framework_base
{
	friend int ::main(FE::int32 argc_p, FE::ASCII** argv_p);

	static RestartOrNot s_restart_or_not;
	static framework_base* s_framework;
	
protected:
	program_options m_program_options;
	std::locale m_current_system_locale;
	std::unique_ptr<class FE::memory_resource[]> m_memory;
	reflection::method_map m_method_reflection;
	reflection::property_map m_property_reflection;
	framework::task_scheduler m_cpu;

public:
	framework_base(FE::int32 argc_p, FE::ASCII** argv_p, FE::uint32 concurrency_decrement_value_p = 1) noexcept; // Exclude main thread from counting the number of the task scheduler threads.
	virtual ~framework_base() noexcept;

	static void request_restart() noexcept;
	static framework_base& get_engine() noexcept;

	std::pmr::memory_resource* get_memory_resource() noexcept;
	reflection::method_map& get_method_reflection() noexcept;
	reflection::property_map& get_property_reflection() noexcept;
	framework::task_scheduler& get_task_scheduler() noexcept;

protected:
	virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p);
	virtual FE::int32 run();
	virtual FE::int32 shutdown();

protected:
	void __load_reflection_data() noexcept;

public:
	_FE_NORETURN_ static void __abnormal_shutdown_with_exit_code(int signal_p);

	// Do not call this function directly.
	static std::function<framework_base* (FE::int32, FE::ASCII**)>& allocate_framework(std::function<framework_base* (FE::int32, FE::ASCII**)> script_p = [](FE::int32, FE::ASCII**) { return nullptr; }) noexcept;

	framework_base(const framework_base&) = delete;
	framework_base(framework_base&&) = delete;
	framework_base& operator=(const framework_base&) = delete;
	framework_base& operator=(framework_base&&) = delete;
};


class game_instance;

class game_framework_base : public framework_base
{
	std::unique_ptr<game_instance> m_game_instance;
	framework::ECS m_entity_component_system;

public:
	game_framework_base(FE::int32 argc_p, FE::ASCII** argv_p);
	~game_framework_base();

private:
	virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) override;
	virtual FE::int32 run() override;
	virtual FE::int32 shutdown() override;
};


END_NAMESPACE
#endif