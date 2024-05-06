#ifndef _FE_FRAMEWORK_HPP_
#define _FE_FRAMEWORK_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <functional>
#define _CREATE_AN_APP_ new




int main(int argc_p, char** argv_p);




BEGIN_NAMESPACE(FE::framework)

enum struct RESTART_OR_NOT : uint8
{
	_NO_OPERATION = 0,
	_HAS_TO_RESTART = 1,
};

class application
{
	friend int ::main(int argc_p, char** argv_p);

	static application* s_app;

protected:
	virtual bool set_up(int argc_p, char** argv_p) = 0;
	virtual int run(int argc_p, char** argv_p) = 0;
	virtual void clean_up() = 0;

public:
	using initializer_t = std::function<application* (void)>;

	static RESTART_OR_NOT s_restart_or_not;

	application() = default;
	virtual ~application() {};

	static initializer_t create_application(initializer_t script_p = []() { return nullptr; }) noexcept;

private:
	static void __set_up_main() noexcept;
	static void __shutdown_main() noexcept;
	_NORETURN_ static void __abnormal_shutdown_with_exit_code(int32 signal_p);

	application(const application&) = delete;
	application(application&&) = delete;
	application& operator=(const application&) = delete;
	application& operator=(application&&) = delete;
};


END_NAMESPACE
#endif