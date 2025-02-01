#ifndef _FE_ARCHETYPE_BASE_HPP_
#define _FE_ARCHETYPE_BASE_HPP_
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
#include <FE/ECS.hpp>




BEGIN_NAMESPACE(FE)


class ECS;
class engine;


class archetype_base
{
public:
	archetype_base() noexcept = default;
	virtual ~archetype_base() noexcept = default;

protected:
	virtual void on_construction() = 0;
	virtual void on_destruction() = 0;

public:
	template <class Component, class Archetype>
	_FE_FORCE_INLINE_ component_ptr<Component> attach_component_to(Archetype* const this_p) noexcept
	{
		return game_engine::get_game_engine().get_entity_component_system().request_component_attachment<Archetype, Component>(this_p);
	}

	template <class Component, class Archetype>
	_FE_FORCE_INLINE_ void detach_component_of(Archetype* const this_p) noexcept
	{
		game_engine::get_game_engine().get_entity_component_system().request_component_detachment<Archetype, Component>(this_p);
	}

	template <class Component, class Archetype>
	_FE_FORCE_INLINE_ void get_component_of(Archetype* const this_p) noexcept
	{
		game_engine::get_game_engine().get_entity_component_system().request_component_retrieval<Archetype, Component>(this_p);
	}
};


END_NAMESPACE
#endif