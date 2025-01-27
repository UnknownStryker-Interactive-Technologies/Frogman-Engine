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
    : m_upstream_arena(),
	  m_entity_arena(&m_upstream_arena),
	  m_component_arena(&m_upstream_arena),
	  m_system_arena(&m_upstream_arena),
	  m_entity_table(), m_component_table(), m_system_table()
{
	constexpr FE::size l_max_entities = 1048576;
	m_entity_table.reserve(l_max_entities);
	m_component_table.reserve(l_max_entities);
	m_system_table.reserve(l_max_entities);
}

ECS::~ECS() noexcept
{
}


std::pmr::vector< FE::pair<std::string_view, std::weak_ptr<archetype_base> >> ECS::list_known_entities() noexcept
{
	std::pmr::vector< FE::pair<std::string_view, std::weak_ptr<archetype_base> >> l_list(FE::framework::framework_base::get_engine().get_memory_resource());
	l_list.reserve( m_entity_table.size() );
	for (auto entity = m_entity_table.begin(); entity != m_entity_table.end(); ++entity)
	{
		l_list.emplace_back(entity->first, entity->second);
	}
	return l_list;
}

std::pmr::vector<FE::pair<std::string_view, std::weak_ptr<component_base>>> ECS::list_known_components() noexcept
{
	std::pmr::vector<FE::pair<std::string_view, std::weak_ptr<component_base>>> l_list(FE::framework::framework_base::get_engine().get_memory_resource());
	l_list.reserve( m_component_table.size() );
	for (auto archetype = m_component_table.begin(); archetype != m_component_table.end(); ++archetype)
	{
		for (auto component = archetype->second.begin(); component != archetype->second.end(); ++component)
		{
			l_list.emplace_back(component->first, component->second);
		}
	}
	return l_list;
}

std::pmr::vector<std::string_view> ECS::list_known_systems() noexcept
{
	std::pmr::vector<std::string_view> l_list(FE::framework::framework_base::get_engine().get_memory_resource());
	l_list.reserve( m_system_table.size() );
	for (auto system = m_system_table.begin(); system != m_system_table.end(); ++system)
	{
		l_list.emplace_back(system->first);
	}
	return l_list;
}


END_NAMESPACE