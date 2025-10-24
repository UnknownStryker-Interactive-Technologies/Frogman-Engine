#ifndef _FE_FRAMEWORK_HXX_
#define _FE_FRAMEWORK_HXX_
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
#include <FE/prerequisites.hxx>
#include <FE/pair.hxx>

// std
#include <functional>


#ifdef CUSTOM_ENGINE
    #error Frogman Engine Prohibits macroizing the keyword "CUSTOM_ENGINE()".
#else                                                                                                                        // The name below does not follow the naming convention since it is considered hidden from users.
    #define CUSTOM_ENGINE(framework_class_name) static ::std::function<::FE::framework::framework_base* (FE::int32, FE::ASCII**)> CustomEngine = ::FE::framework::framework_base::allocate_framework( [](FE::int32 argc_p, FE::ASCII** argv_p) { return new framework_class_name(argc_p, argv_p); } );
#endif


#include <FE/framework/reflection.hxx>
#include <FE/framework/thread_id.hxx>


CLASS_FORWARD_DECLARATION(FE, memory_resource);
CLASS_FORWARD_DECLARATION(FE::framework, ECS);
CLASS_FORWARD_DECLARATION(FE::framework, processors);
CLASS_FORWARD_DECLARATION(FE::framework, processor);
CLASS_FORWARD_DECLARATION(FE::framework::internal::processors, fiber_stack_allocator);
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
along with reflection capabilities for methods, properties, and enums.
*/
class framework_base
{
	friend int ::main(FE::int32 argc_p, FE::ASCII** argv_p);
	friend class processor;
	friend class processors;
	friend class FE::framework::internal::processors::fiber_stack_allocator;
	
protected:
	program_options m_program_options;
	std::locale m_current_system_locale;

	std::unique_ptr<class FE::memory_resource[]> m_memory; // TLGPMP: Thread-Local General-Purpose Memory Pool

	reflection::method_registry m_method_reflection;
	reflection::property_registry m_property_reflection;
	reflection::enum_registry m_enum_reflection;

	std::unique_ptr<class framework::ECS> m_ecs;
	std::unique_ptr<class framework::processors> m_processors;

public:
	framework_base(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
	virtual ~framework_base() noexcept = default;

public:
	static framework_base& get_framework() noexcept;

public:
	static void request_restart() noexcept;
	static void cancel_restart() noexcept;

	const program_options& get_program_options() const noexcept;
	const std::locale& get_current_system_locale() const noexcept;

protected:
	std::pmr::memory_resource* get_memory_resource() noexcept;

public:
	reflection::method_registry& get_method_reflection() noexcept;
	reflection::property_registry& get_property_reflection() noexcept;
	reflection::enum_registry& get_enum_reflection() noexcept;

protected:
	class framework::ECS& get_ecs() noexcept;
	class framework::processors& get_processors() noexcept;

protected:
	virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) = 0;
	virtual FE::int32 run() = 0;
	virtual FE::int32 shutdown() = 0;

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


END_NAMESPACE
#endif