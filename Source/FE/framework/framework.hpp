#ifndef _FE_FRAMEWORK_HPP_
#define _FE_FRAMEWORK_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <functional>

#ifdef CONFIGURE_A_FROGMAN_ENGINE_PROJECT
	#error Frogman Engine Prohibits macroizing the keyword "CONFIGURE_A_FROGMAN_ENGINE_PROJECT()".
#else
	#define CONFIGURE_A_FROGMAN_ENGINE_PROJECT() FE::framework::application_base::initializer_t g_init = FE::framework::application_base::create_application( []() {
	#ifdef END_OF_THE_FROGMAN_ENGINE_PROJECT_CONFIGURATION
		#error Frogman Engine Prohibits macroizing the keyword "END_OF_THE_FROGMAN_ENGINE_PROJECT_CONFIGURATION()".
	#else
		#define END_OF_THE_FROGMAN_ENGINE_PROJECT_CONFIGURATION() } );
	#endif
#endif




int main(int argc_p, char** argv_p);




BEGIN_NAMESPACE(FE::framework)

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
	virtual bool set_up(int argc_p, char** argv_p) = 0;
	virtual int run(int argc_p, char** argv_p) = 0;
	virtual void clean_up() = 0;

public:
	using initializer_t = std::function<application_base* (void)>;

	static RESTART_OR_NOT s_restart_or_not;

	application_base() = default;
	virtual ~application_base() {};

	static initializer_t create_application(initializer_t script_p = []() { return nullptr; }) noexcept;

private:
	static void __set_up_main() noexcept;
	static void __shutdown_main() noexcept;
	_NORETURN_ static void __abnormal_shutdown_with_exit_code(int32 signal_p);

	application_base(const application_base&) = delete;
	application_base(application_base&&) = delete;
	application_base& operator=(const application_base&) = delete;
	application_base& operator=(application_base&&) = delete;
};


END_NAMESPACE
#endif