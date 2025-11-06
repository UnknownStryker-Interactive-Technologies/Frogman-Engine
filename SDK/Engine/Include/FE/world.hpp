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
#include <FE/level.hpp>




BEGIN_NAMESPACE(FE)

//// Read the .froggy file from m_froggy and deserialize the m_entity_list and the m_system_list.
//m_ecs = std::make_unique<framework::ECS>();
//m_ecs->initialize(m_entity_list, m_system_list);
class world : public FE::archetype_base
{
	FE::framework::ECS m_ecs;
	std::pmr::vector< FE::smart_ptr<FE::level, FE::RefType::_Owner> > m_levels;

public:
	world() noexcept = default;
	virtual ~world() noexcept override = default;
};


END_NAMESPACE
#endif