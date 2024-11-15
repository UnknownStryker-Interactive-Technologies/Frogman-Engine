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

#include <FE/pool/scalable_pool_allocator.hxx>

// std
#include <functional>


#ifdef FROGMAN_ENGINE
	#error Frogman Engine Prohibits macroizing the keyword "FROGMAN_ENGINE()".
#else                                                                                                              // The name below does not follow the naming convention since it is considered hidden from users.
	#define FROGMAN_ENGINE() static ::std::function<::FE::framework::framework_base* (int, char**)> FrogmanEngine = ::FE::framework::framework_base::__allocate_framework( [](int argc_p, char** argv_p) { return new ::FE::framework::game_engine(argc_p, argv_p); } );
#endif

#ifdef CUSTOM_ENGINE
    #error Frogman Engine Prohibits macroizing the keyword "CUSTOM_ENGINE()".
#else                                                                                                                        // The name below does not follow the naming convention since it is considered hidden from users.
    #define CUSTOM_ENGINE(framework_class_name) static ::std::function<::FE::framework::framework_base* (int, char**)> CustomEngine = ::FE::framework::framework_base::__allocate_framework( [](int argc_p, char** argv_p) { return new framework_class_name(argc_p, argv_p); } );
#endif
#include <FE/framework/reflection/reflection.h>
#include <FE/framework/platform_information.h>
#include <FE/framework/game_instance.hpp>
#include <FE/framework/vulkan_renderer.hpp>
#include <FE/framework/managed/unique_ptr.hxx>




int main(int argc_p, char** argv_p);




BEGIN_NAMESPACE(FE::framework)


enum struct RestartOrNot : uint8
{
	_NoOperation = 0,
	_HasToRestart = 1,
};


class framework_base
{
	friend int ::main(int argc_p, char** argv_p);

	static framework_base* s_framework;

protected:
	virtual int launch(_FE_MAYBE_UNUSED_ int argc_p, _FE_MAYBE_UNUSED_ char** argv_p) { return 0; };
	virtual int run() { return 0; };
	virtual int shutdown() { return 0; };

public:
	static RestartOrNot s_restart_or_not;

	framework_base(_FE_MAYBE_UNUSED_ int argc_p, _FE_MAYBE_UNUSED_ char** argv_p) noexcept { reflection_system::initialize(8192); };
	virtual ~framework_base() noexcept { reflection_system::shutdown(); };

	static std::function<framework_base* (int, char**)>& __allocate_framework(std::function<framework_base* (int, char**)> script_p = [](int, char**) { return nullptr; }) noexcept;

private:
	_FE_NORETURN_ static void __abnormal_shutdown_with_exit_code(int32 signal_p);

	framework_base(const framework_base&) = delete;
	framework_base(framework_base&&) = delete;
	framework_base& operator=(const framework_base&) = delete;
	framework_base& operator=(framework_base&&) = delete;
};




FE::uint8 get_current_thread_id() noexcept;




struct program_options
{
	FE::pair<const char*, var::uint32> _max_concurrency = { "-max-concurrency=", 4 };
};




class game_engine : public framework_base
{
	program_options m_program_options;
	FE::unique_ptr<FE::scalable_pool<2* FE::one_gb, FE::align_CPU_L1_cache_line>[]> m_memory;
	game_instance m_game_instance;
	//vulkan_renderer m_renderer;

public:

	game_engine(int argc_p, char** argv_p);
	~game_engine();

	template<typename T>
	FE::scalable_pool_allocator<T, FE::size_in_bytes <2 * FE::one_gb>, FE::align_CPU_L1_cache_line> get_allocator() noexcept
	{
		FE::scalable_pool_allocator<T, FE::size_in_bytes <2 * FE::one_gb>, FE::align_CPU_L1_cache_line> l_allocator = this->m_memory[get_current_thread_id()];
		return l_allocator;
	}

private:
	virtual int launch(int argc_p, char** argv_p) override;
	virtual int run() override;
	virtual int shutdown() override;
};


END_NAMESPACE
#endif