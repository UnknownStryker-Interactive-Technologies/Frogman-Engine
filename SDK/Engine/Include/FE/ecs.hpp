#ifndef _FROGMAN_ENGINE_ECS_HPP_
#define _FROGMAN_ENGINE_ECS_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
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
#include <FE/engine.hpp>




BEGIN_NAMESPACE(FE::ECS)


template <class Archetype, typename ...Arguments>
_FE_FORCE_INLINE_ static FE::entity<Archetype> create_entity(Arguments&& ...arguments_p) noexcept
{
	return FE::engine::get_engine().get_ecs().instanciate_entity(std::forward<Arguments&&>(arguments_p)...);
}

template <class Archetype>
_FE_FORCE_INLINE_ static FE::entity<FE::archetype_base> create_entity_from_initializer(const FE::framework::initializer& serialized_entity_p) noexcept
{
	return FE::engine::get_engine().get_ecs().instanciate_entity_from_initializer(serialized_entity_p);
}

template <class Archetype> // ADE: Archetype Default Entity
_FE_FORCE_INLINE_ static FE::entity<FE::archetype_base> create_ADE() noexcept
{
	return FE::engine::get_engine().get_ecs().instanciate_archetype_default_entity();
}

template <class Archetype>
_FE_FORCE_INLINE_ static void set_ADE(FE::framework::initializer& default_values_p) noexcept
{
	FE::engine::get_engine().get_ecs().set_archetype_default_entity(default_values_p);
}

template <class Archetype>
_FE_FORCE_INLINE_ static FE::framework::initializer* const get_ADE_initializer() noexcept
{
	return FE::engine::get_engine().get_ecs().get_archetype_default_entity();
}


END_NAMESPACE
#endif