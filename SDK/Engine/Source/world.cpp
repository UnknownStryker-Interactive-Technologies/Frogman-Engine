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
#include <FE/world.hpp>
#include <FE/framework/reflection.hxx>
#include <FE/engine.hpp>




FE::world::world(	world_tag world_tag_p,
					FE::framework::initializer_list&& initializer_list_p,
					FE::size component_type_count_hint_p) noexcept
	:	m_world_tag(world_tag_p),
		m_ecs(component_type_count_hint_p),
		m_game_mode( FE::make_owner<FE::mode>(	FE::engine::get_engine().get_memory_resource(), 
												m_ecs, FE::ControllerType::_KeyboardAndMouse 
												) 
		),
		m_levels( FE::engine::get_engine().get_memory_resource() )
{
	m_ecs.initialize(std::move(initializer_list_p));
	// call the decryption method to decrypt the world if the functor is not null.
}

void FE::world::spawn_entity() noexcept
{
}
