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
#else                                                                                                              // The name below does not follow the class naming convention since it is considered hidden from users.
	#define FROGMAN_ENGINE() static ::std::function<::FE::framework::framework_base* (int, char**)> FrogmanEngine = ::FE::framework::framework_base::__allocate_framework( [](int argc_p, char** argv_p) { return new ::FE::framework::game_engine(argc_p, argv_p); } );
#endif

#ifdef CUSTOM_ENGINE
    #error Frogman Engine Prohibits macroizing the keyword "CUSTOM_ENGINE()".
#else                                                                                                                        // The name below does not follow the class naming convention since it is considered hidden from users.
    #define CUSTOM_ENGINE(framework_class_name) static ::std::function<::FE::framework::framework_base* (int, char**)> CustomEngine = ::FE::framework::framework_base::__allocate_framework( [](int argc_p, char** argv_p) { return new framework_class_name(argc_p, argv_p); } );
#endif

#include <FE/framework/platform_information.h>
#include <FE/framework/game_instance.hpp>
#include <FE/framework/vulkan_renderer.hpp>




int main(int argc_p, char** argv_p);




BEGIN_NAMESPACE(FE::framework)


enum struct RESTART_OR_NOT : uint8
{
	_NO_OPERATION = 0,
	_HAS_TO_RESTART = 1,
};


class framework_base
{
	friend int ::main(int argc_p, char** argv_p);

	static framework_base* s_framework;

protected:
	virtual int launch(int argc_p, char** argv_p) = 0;
	virtual int run() = 0;
	virtual int shutdown() = 0;

public:
	static RESTART_OR_NOT s_restart_or_not;

	framework_base() = default;
	virtual ~framework_base() = default;

	static std::function<framework_base* (int, char**)>& __allocate_framework(std::function<framework_base* (int, char**)> script_p = [](int, char**) { return nullptr; }) noexcept;

private:
	_FE_NORETURN_ static void __abnormal_shutdown_with_exit_code(int32 signal_p);

	framework_base(const framework_base&) = delete;
	framework_base(framework_base&&) = delete;
	framework_base& operator=(const framework_base&) = delete;
	framework_base& operator=(framework_base&&) = delete;
};


struct program_options
{
	const char* _exe_directory = nullptr;
	FE::pair<const char*, var::uint32> _max_concurrency = { "-max-concurrency=", 4 };
};


class game_engine : public framework_base
{
protected:
	program_options m_program_options;
	game_instance m_game_instance;

	vulkan_renderer m_renderer;

public:
	game_engine(int argc_p, char** argv_p);
	~game_engine() = default;

private:
	virtual int launch(int argc_p, char** argv_p) override;
	virtual int run() override;
	virtual int shutdown() override;
};


END_NAMESPACE
#endif