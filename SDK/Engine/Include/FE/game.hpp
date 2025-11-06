#ifndef _FROGMAN_ENGINE_GAME_HPP_
#define _FROGMAN_ENGINE_GAME_HPP_
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
#include <FE/world.hpp>




BEGIN_NAMESPACE(FE)


class game : public FE::archetype_base
{
	framework::initializer_list m_entity_list;
	framework::system_table_initializer_list m_system_list;

	std::pmr::vector< FE::smart_ptr<FE::world, FE::RefType::_Owner> > m_world_list;
	FE::smart_ptr<FE::world, FE::RefType::_Observer> m_active_world;

	// player list, current player

public:
	game() noexcept;
	virtual ~game() noexcept override = default;
};


END_NAMESPACE
#endif