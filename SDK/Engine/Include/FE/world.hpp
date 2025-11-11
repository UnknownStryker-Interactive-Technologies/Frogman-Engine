#ifndef _FROGMAN_ENGINE_WORLD_HPP_
#define _FROGMAN_ENGINE_WORLD_HPP_
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
#include <FE/framework/ECS.hxx>
#include <FE/controller.hpp>
#include <FE/level.hpp>
#include <FE/mode.hpp>




BEGIN_NAMESPACE(FE)

//// Read the .froggy file from m_froggy and deserialize the m_entity_list and the m_system_list.
//m_ecs = std::make_unique<framework::ECS>();
//m_ecs->initialize(m_entity_list, m_system_list);
class world : public FE::archetype_base
{
	using base_type = FE::archetype_base;

	FE::framework::ECS m_ecs;
	std::pmr::vector< FE::smart_ptr<FE::level, FE::RefType::_Owner> > m_levels;
	FE::smart_ptr<FE::mode, FE::RefType::_Observer> m_game_mode;
	FE::smart_ptr<FE::controller, FE::RefType::_Observer> m_controller;

public:
	world(	framework::ECS& engine_ecs_p, FE::size max_entities_p,
			framework::initializer_list& initializer_list_p, framework::system_table_initializer_list& system_table_initializer_p,
			FE::size component_type_count_hint_p = 1000, FE::size system_count_hint_p = 1000) noexcept;
	~world() noexcept = default;
};


END_NAMESPACE
#endif