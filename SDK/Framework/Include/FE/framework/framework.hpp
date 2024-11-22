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

#include <FE/pool/memory_resource.hpp>

// std
#include <functional>


#ifdef FROGMAN_ENGINE
	#error Frogman Engine Prohibits macroizing the keyword "FROGMAN_ENGINE()".
#else                                                                                                              // The name below does not follow the naming convention since it is considered hidden from users.
	#define FROGMAN_ENGINE() static ::std::function<::FE::framework::framework_base* (int, FE::tchar**)> FrogmanEngine = ::FE::framework::framework_base::__allocate_framework( [](int argc_p, FE::tchar** argv_p) { return new ::FE::framework::game_engine(argc_p, argv_p); } );
#endif

#ifdef CUSTOM_ENGINE
    #error Frogman Engine Prohibits macroizing the keyword "CUSTOM_ENGINE()".
#else                                                                                                                        // The name below does not follow the naming convention since it is considered hidden from users.
    #define CUSTOM_ENGINE(framework_class_name) static ::std::function<::FE::framework::framework_base* (int, FE::tchar**)> CustomEngine = ::FE::framework::framework_base::__allocate_framework( [](int argc_p, FE::tchar** argv_p) { return new framework_class_name(argc_p, argv_p); } );
#endif
#include <FE/framework/reflection.hpp>
#include <FE/framework/game_instance.hpp>




int _tmain(int argc_p, FE::tchar** argv_p);




BEGIN_NAMESPACE(FE::framework)


enum struct RestartOrNot : uint8
{
	_NoOperation = 0,
	_HasToRestart = 1,
};

struct program_options
{
	FE::pair<const FE::tchar*, var::uint32> _max_concurrency = { (FE::tchar*)"-max-concurrency=", 4 };
};



class framework_base
{
	friend int ::main(int argc_p, FE::tchar** argv_p);

	static framework_base* s_framework;

protected:
	static RestartOrNot s_restart_or_not;

	program_options m_program_options;
	std::unique_ptr<FE::scalable_pool_resource[]> m_memory;

	virtual int launch(_FE_MAYBE_UNUSED_ int argc_p, _FE_MAYBE_UNUSED_ FE::tchar** argv_p) { return 0; };
	virtual int run() { return 0; };
	virtual int shutdown() { return 0; };

public:
	framework_base(_FE_MAYBE_UNUSED_ int argc_p, _FE_MAYBE_UNUSED_ FE::tchar** argv_p) noexcept;
	virtual ~framework_base() noexcept;

	static void request_restart() noexcept;
	std::pmr::memory_resource* get_memory_resource() noexcept;
	static std::function<framework_base* (int, FE::tchar**)>& __allocate_framework(std::function<framework_base* (int, FE::tchar**)> script_p = [](int, FE::tchar**) { return nullptr; }) noexcept;

private:
	_FE_NORETURN_ static void __abnormal_shutdown_with_exit_code(int32 signal_p);

	framework_base(const framework_base&) = delete;
	framework_base(framework_base&&) = delete;
	framework_base& operator=(const framework_base&) = delete;
	framework_base& operator=(framework_base&&) = delete;
};




class game_engine : public framework_base
{
	game_instance m_game_instance;
	//d3d11_renderer m_renderer;

public:
	game_engine(int argc_p, FE::tchar** argv_p);
	~game_engine();

private:
	virtual int launch(int argc_p, FE::tchar** argv_p) override;
	virtual int run() override;
	virtual int shutdown() override;
};


END_NAMESPACE
#endif