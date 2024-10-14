#ifndef _FE_FRAMEWORK_HPP_
#define _FE_FRAMEWORK_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>

// std
#include <functional>

#ifdef CREATE_A_FROGMAN_ENGINE_APP
	#error Frogman Engine Prohibits macroizing the keyword "CREATE_A_FROGMAN_ENGINE_APP()".
#else                                                                                                              // The name below does not follow the class naming convention since it is considered hidden from users.
	#define CREATE_A_FROGMAN_ENGINE_APP() static ::std::function<::FE::framework::application_base* (int, char**)> FrogmanEngineApplication = ::FE::framework::application_base::create_application( [](int argc_p, char** argv_p) { return new ::FE::framework::vulkan_application(argc_p, argv_p); } );
#endif

#ifdef CREATE_A_CUSTOM_APP
#error Frogman Engine Prohibits macroizing the keyword "CREATE_A_CUSTOM_APP()".
#else                                                                                                                        // The name below does not follow the class naming convention since it is considered hidden from users.
#define CREATE_A_CUSTOM_APP(application_class_name) static ::std::function<::FE::framework::application_base* (int, char**)> FrogmanEngineApplication = ::FE::framework::application_base::create_application( [](int argc_p, char** argv_p) { return new application_class_name(argc_p, argv_p); } );
#endif

#include <FE/framework/input_manager.hpp>
#include <FE/framework/platform_information.h>
#include <FE/framework/task_scheduler.hpp>
#include <FE/framework/vulkan_renderer.hpp>




int main(int argc_p, char** argv_p);


BEGIN_NAMESPACE(FE::framework)


struct program_options
{

};

void read_program_options(program_options& out_options_p) noexcept;


struct runtime_configurations
{

};

void read_runtime_configurations(runtime_configurations& out_configs_p) noexcept;


enum struct RESTART_OR_NOT : uint8
{
	_NO_OPERATION = 0,
	_HAS_TO_RESTART = 1,
};


class application_base
{
	friend int ::main(int argc_p, char** argv_p);

	static application_base* s_app;

protected:
	virtual int launch(int argc_p, char** argv_p) = 0;
	virtual int run() = 0;
	virtual int shutdown() = 0;

public:
	static RESTART_OR_NOT s_restart_or_not;

	application_base() = default;
	virtual ~application_base() = default;

	static std::function<application_base* (int, char**)> create_application(std::function<application_base* (int, char**)> script_p = [](int, char**) { return nullptr; }) noexcept;

private:
	_FE_NORETURN_ static void __abnormal_shutdown_with_exit_code(int32 signal_p);

	application_base(const application_base&) = delete;
	application_base(application_base&&) = delete;
	application_base& operator=(const application_base&) = delete;
	application_base& operator=(application_base&&) = delete;
};


class vulkan_application : public application_base
{
protected:
	platform_information m_platform_info;
	program_options m_program_options;
	runtime_configurations m_runtime_configurations;

	input_manager m_input_manager;
	game_thread m_task_scheduler;
	vulkan_renderer m_renderer;

public:
	vulkan_application(int argc_p, char** argv_p);
	~vulkan_application() = default;

private:
	virtual int launch(int argc_p, char** argv_p) override;
	virtual int run() override;
	virtual int shutdown() override;
};


END_NAMESPACE
#endif