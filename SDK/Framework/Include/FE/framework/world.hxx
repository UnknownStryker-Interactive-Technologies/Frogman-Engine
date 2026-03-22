#ifndef _FROGMAN_ENGINE_WORLD_HXX_
#define _FROGMAN_ENGINE_WORLD_HXX_
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
#include <FE/mode.hpp>




CLASS_FORWARD_DECLARATION(FE::framework, game_processor);


BEGIN_NAMESPACE(FE)


using world_tag = ::FE::uint64;


class world : public ::FE::archetype_base
{
	friend class ::FE::framework::game_processor;


	using base_type = ::FE::archetype_base;


	framework::ECS m_ecs;
	world_tag m_world_tag;
	FE::mode m_mode;

public:
	world(	world_tag world_tag_p,
			framework::initializer_list&& initializer_list_p,
			::FE::size component_type_count_hint_p) noexcept;
	~world() noexcept = default;

public:
	void spawn_entity() noexcept;


	_FE_FORCE_INLINE_ world_tag get_world_tag() const noexcept { return m_world_tag; }
	_FE_FORCE_INLINE_ const FE::mode& get_mode() const noexcept { return m_mode; }
	_FE_FORCE_INLINE_ FE::mode& get_mode() noexcept { return m_mode; }
};


END_NAMESPACE
#endif