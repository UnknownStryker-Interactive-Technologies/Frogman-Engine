#ifndef _FROGMAN_ENGINE_ECS_HPP_
#define _FROGMAN_ENGINE_ECS_HPP_
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
#include <FE/engine.hxx>




BEGIN_NAMESPACE(FE)


class ecs
{
public:

	template <class Archetype, typename ...Arguments>
	_FE_FORCE_INLINE_ static FE::entity<Archetype> create_entity(FE::ASCII* const name_p, Arguments&& ...arguments_p) noexcept
	{
		return FE::engine::__get_engine().get_ecs().instanciate_entity(name_p, std::forward<Arguments&&>(arguments_p)...);
	}

	template <class Archetype>
	_FE_FORCE_INLINE_ static FE::entity<FE::archetype_base> create_entity_from_initializer(FE::ASCII* const name_p, FE::framework::initializer& serialized_entity_p) noexcept
	{
		return FE::engine::__get_engine().get_ecs().instanciate_entity_from_initializer(name_p, serialized_entity_p);
	}

	template <class Archetype> // ADE: Archetype Default Entity
	_FE_FORCE_INLINE_ static FE::entity<FE::archetype_base> create_ADE(FE::ASCII* const name_p) noexcept
	{
		return FE::engine::__get_engine().get_ecs().instanciate_archetype_default_entity(name_p);
	}

	_FE_FORCE_INLINE_ static void destroy_entity(FE::entity<archetype_base> entity_p) noexcept
	{
		FE::engine::__get_engine().get_ecs().destruct_entity(entity_p);
	}

	template <class Archetype>
	_FE_FORCE_INLINE_ static void set_ADE(FE::framework::initializer& default_values_p) noexcept
	{
		FE::engine::__get_engine().get_ecs().set_archetype_default_entity(default_values_p);
	}

	template <class Archetype>
	_FE_FORCE_INLINE_ static FE::framework::initializer* const get_ADE_initializer() noexcept
	{
		return FE::engine::__get_engine().get_ecs().get_archetype_default_entity();
	}

	template <class Archetype>
	_FE_FORCE_INLINE_ static FE::entity<Archetype> find_entity(FE::ASCII* const name_p) noexcept
	{
		return FE::engine::__get_engine().get_ecs().find_entity<Archetype>(name_p);
	}

	template <class Component, typename ...Arguments>
	_FE_FORCE_INLINE_ static FE::component_view<Component> add_component(FE::entity<archetype_base> entity_p, Arguments&& ...arguments_p) noexcept
	{
		return FE::engine::__get_engine().get_ecs().add_component<Component>(entity_p, std::forward<Arguments&&>(arguments_p)...);
	}

	template <class Component>
	_FE_FORCE_INLINE_ static FE::component_view<FE::component_base> create_component(FE::entity<archetype_base> entity_p) noexcept
	{
		return FE::engine::__get_engine().get_ecs().instanciate_component<Component>(entity_p);
	}

	template <class Component>
	_FE_FORCE_INLINE_ static void destroy_component(FE::entity<archetype_base> entity_p) noexcept
	{
		FE::engine::__get_engine().get_ecs().remove_component(entity_p)
	}

	_FE_FORCE_INLINE_ static void attatch_component(FE::entity<archetype_base> entity_p, const FE::component_view<component_base>& to_attatch_p) noexcept
	{
		FE::engine::__get_engine().get_ecs().attatch_component(entity_p, to_attatch_p);
	}

	template <class Component>
	_FE_FORCE_INLINE_ static FE::component_view<Component> detach_component(FE::entity<archetype_base> entity_p) noexcept
	{
		return FE::engine::__get_engine().get_ecs().detach_component<Component>(entity_p);
	}

	_FE_FORCE_INLINE_ static std::optional<FE::pair<FE::system, std::pmr::vector<std::size_t>>> find_system(FE::ASCII* const name_p) noexcept
	{
		return FE::engine::__get_engine().get_ecs().find_system(name_p);
	}

	_FE_FORCE_INLINE_ static FE::framework::initializer serialize_entity(FE::entity<archetype_base> entity_p) noexcept
	{
		return FE::engine::__get_engine().get_ecs().serialize_entity(entity_p);
	}

	_FE_FORCE_INLINE_ static void deserialize_entity(FE::framework::initializer& serialized_components_p, FE::entity<archetype_base> out_entity_p) noexcept
	{
		FE::engine::__get_engine().get_ecs().deserialize_entity(serialized_components_p, out_entity_p);
	}

public:
	ecs() = delete;
	~ecs() = delete;
};


END_NAMESPACE
#endif