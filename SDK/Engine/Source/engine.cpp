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
#include <FE/engine.hpp>
#include <FE/ECS.hpp>
#include <FE/game_instance_base.hpp>

#include <FE/framework/reflection/private/load_reflection_data.h>

#ifdef _FE_ON_WINDOWS_X86_64_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif




extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x00000001;
}




BEGIN_NAMESPACE(FE)


game_engine::game_engine(FE::int32 argc_p, FE::ASCII** argv_p)
	: framework_base(argc_p, argv_p), /* Exclude main thread and the render thread from counting the number of the task scheduler threads. */
	m_entity_component_system(std::allocate_shared<FE::ECS, std::pmr::polymorphic_allocator<FE::ECS>>(this->get_memory_resource())),
	m_game_instance()
{

}

game_engine::~game_engine()
{

}


game_engine& game_engine::get_game_engine() noexcept
{
	static game_engine& l_s_game_engine = dynamic_cast<game_engine&>(framework_base::get_framework());
	return l_s_game_engine;
}

FE::ECS& game_engine::get_entity_component_system() noexcept
{
	return *(this->m_entity_component_system);
}


FE::int32 game_engine::launch(_FE_MAYBE_UNUSED_ FE::int32 argc_p, _FE_MAYBE_UNUSED_ FE::ASCII** argv_p)
{
	__load_reflection_data();
	this->m_game_instance = std::allocate_shared<game_instance_base, std::pmr::polymorphic_allocator<game_instance_base>>(this->get_memory_resource());
	return 0;
}

FE::int32 game_engine::run()
{

	return 0;
}

FE::int32 game_engine::shutdown()
{
	return 0;
}


END_NAMESPACE