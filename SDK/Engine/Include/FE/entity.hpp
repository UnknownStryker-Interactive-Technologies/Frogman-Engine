#ifndef _FROGMAN_ENGINE_ENTITY_HPP_
#define _FROGMAN_ENGINE_ENTITY_HPP_
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
#include <entt/entt.hpp>




BEGIN_NAMESPACE(FE)

class world;

namespace internal
{
    enum struct entity : var::uint64 {};
}

class entity final
{
	friend class world;

    internal::entity m_handle;

public:
    entity() noexcept;
    explicit entity(internal::entity handle_p) noexcept;
	entity& operator=(internal::entity handle_p) noexcept;

	~entity() noexcept = default;

    entity(const entity& other_p) noexcept;
    entity& operator=(const entity& other_p) noexcept;

    entity(entity&& other_p) noexcept;
    entity& operator=(entity&& other_p) noexcept;

    FE::boolean operator==(const entity& other_p) const noexcept;
    explicit operator bool() const noexcept;
};

END_NAMESPACE
#endif