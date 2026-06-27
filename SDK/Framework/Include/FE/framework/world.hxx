#ifndef _FROGMAN_ENGINE_WORLD_HXX_
#define _FROGMAN_ENGINE_WORLD_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

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
#include <FE/mode.hpp>




CLASS_FORWARD_DECLARATION(FE::framework, game_processor);


BEGIN_NAMESPACE(FE)


using world_tag = var::int64;

struct area_of_operation
{
	std::pmr::string _target_gdk_version;
	world_tag _tag;
	var::float64 _gravity[3] = { 0.0, -9.81, 0.0 };
	var::size _component_type_count_hint;

	framework::initializer_list _initializer_list;
	// scene graph below!
	// ...
};


class world : public ::FE::archetype_base
{
	friend class ::FE::framework::game_processor;

	using base_type = ::FE::archetype_base;

	area_of_operation m_area_of_operation;
	framework::ECS m_ecs;
	FE::mode m_mode;

public:
	world(const area_of_operation& area_of_operation_p) noexcept;
	~world() noexcept = default;

public:
	template <typename T, typename... Arguments>
	FE::entity<T> spawn_entity(Arguments&&... args) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, T>, "Static assertion failed: the template argument T must be derived from FE::archetype_base.");
		return m_ecs.instanciate_entity<T>(std::forward<Arguments>(args)...);
	}

	_FE_FORCE_INLINE_ world_tag get_world_tag() const noexcept { return m_area_of_operation._tag; }
	_FE_FORCE_INLINE_ const FE::mode& get_mode() const noexcept { return m_mode; }
	_FE_FORCE_INLINE_ FE::mode& get_mode() noexcept { return m_mode; }
};


END_NAMESPACE
#endif