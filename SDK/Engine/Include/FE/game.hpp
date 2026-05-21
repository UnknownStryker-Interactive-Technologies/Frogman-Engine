#ifndef _FROGMAN_ENGINE_GAME_HPP_
#define _FROGMAN_ENGINE_GAME_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/framework/ECS.hxx>
#include <FE/framework/world.hxx>

#include <absl/container/flat_hash_map.h>

#include <boost/filesystem/path.hpp>




BEGIN_NAMESPACE(FE)


class game : public FE::archetype_base
{
	using base_type = FE::archetype_base;

	using world_list = absl::flat_hash_map<world_tag, FE::smart_ptr<FE::world, FE::RefType::_Owner>,
		FE::hash<world_tag>,
		std::equal_to<world_tag>,
		std::pmr::polymorphic_allocator < std::pair<const world_tag, FE::smart_ptr<FE::world, FE::RefType::_Owner>> >>;
	
	world_list m_world_list;

	FE::smart_ptr<FE::world, FE::RefType::_Observer> m_current_world;
	// player list, current player

public:
	game(framework::ECS& engine_ecs_p) noexcept;
	virtual ~game() noexcept override = default;

	FE::smart_ptr<FE::world, FE::RefType::_Observer> get_current_world() const noexcept { return m_current_world; }
	FE::smart_ptr<FE::world, FE::RefType::_Observer> get_current_world() noexcept { return m_current_world; }
	FE::smart_ptr<FE::world, FE::RefType::_Observer> create_world(const std::pmr::string& path_to_world_file_p) noexcept;
};


END_NAMESPACE
#endif