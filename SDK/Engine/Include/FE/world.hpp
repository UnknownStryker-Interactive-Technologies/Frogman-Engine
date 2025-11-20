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


class world;
CLASS_FORWARD_DECLARATION(internal::world, observer_base);
CLASS_FORWARD_DECLARATION(internal, game_processor);
CLASS_FORWARD_DECLARATION(internal, garbage_collector);


using world_tag = FE::uint64;


class world
{
	friend class internal::world::observer_base;

	using base_type = FE::archetype_base;

	world_tag m_world_tag;
	FE::framework::ECS m_ecs;
	FE::smart_ptr<FE::mode, FE::RefType::_Owner> m_game_mode;
	std::pmr::vector< FE::smart_ptr<FE::level, FE::RefType::_Owner> > m_levels;

public:
	world(	world_tag world_tag_p,
			framework::initializer_list&& initializer_list_p,
			FE::size component_type_count_hint_p) noexcept;
	~world() noexcept = default;

public:
	void spawn_entity() noexcept;

public:
	_FE_FORCE_INLINE_ world_tag get_world_tag() const noexcept { return m_world_tag; }

	_FE_FORCE_INLINE_ const FE::mode& get_game_mode() const noexcept { return *m_game_mode; }
	_FE_FORCE_INLINE_ FE::mode& get_game_mode() noexcept { return *m_game_mode; }

};


namespace internal::world
{
	class observer_base
	{
		FE::smart_ptr<FE::world, FE::RefType::_Observer> m_current_world;

	public:
		observer_base(FE::smart_ptr<FE::world, FE::RefType::_Observer> world_p) noexcept
			:	m_current_world(world_p)
		{}
		virtual ~observer_base() noexcept = default; 

		void notify_world_transition(FE::smart_ptr<FE::world, FE::RefType::_Observer> world_p) noexcept
		{
			m_current_world = world_p;
		} 
		
		_FE_FORCE_INLINE_ class FE::framework::ECS& get_ecs() noexcept
		{
			return m_current_world->m_ecs;
		}
	};
}


END_NAMESPACE
#endif