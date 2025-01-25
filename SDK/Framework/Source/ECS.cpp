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
#include <FE/framework/ECS.hpp>

#include <FE/framework/framework.hpp> // FE::framework::framework_base::get_engine()
#include <FE/framework/task.hpp> // for get_current_thread_id();




BEGIN_NAMESPACE(FE::framework)


ECS::ECS() noexcept
{
}

ECS::~ECS() noexcept
{
}

ECS::ECS(FE::size max_concurrency_p) noexcept
    : m_arena(max_concurrency_p, FE::framework::framework_base::get_engine().get_memory_resource()),
	  m_entity_table(1048576), m_component_table(1048576), m_system_table(1048576)
{
}


std::pmr::vector< FE::pair<std::pmr::string, std::weak_ptr<archetype_base> >> ECS::list_known_entities() noexcept
{
	return std::pmr::vector< FE::pair<std::pmr::string, std::weak_ptr<archetype_base> >>();
}

std::pmr::vector<FE::pair<std::pmr::string, std::weak_ptr<component_base>>> ECS::list_known_components() noexcept
{
	return std::pmr::vector<FE::pair<std::pmr::string, std::weak_ptr<component_base>>>();
}

std::pmr::vector<std::pmr::string> ECS::list_known_systems() noexcept
{
	return std::pmr::vector<std::pmr::string>();
}


FE::scalable_pool<FE::PoolPageCapacity::_256MB>& ECS::get_arena() noexcept
{
	return this->m_arena[get_current_thread_id()];
}


END_NAMESPACE