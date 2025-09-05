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
#include <FE/framework/framework.hpp>

#include <vector>




BEGIN_NAMESPACE(FE)


ECS::ECS(std::pmr::memory_resource* resource) noexcept
	:	m_memory_resource(resource),
		m_archetype_table(),
		m_component_table(),
		m_system_table()
{
	m_archetype_table.reserve(1024);
	m_component_table.reserve(1024);
	m_system_table.reserve(1024);
}

ECS::ECS(FE::init& file_p, std::pmr::memory_resource* resource) noexcept
	:	m_memory_resource(resource),
		m_archetype_table(),
		m_component_table(),
		m_system_table()
{
	m_archetype_table.reserve(1024);
	m_component_table.reserve(1024);
	m_system_table.reserve(1024);
	(void)file_p;
}

system_view<system_base> ECS::find_system(FE::ASCII* const system_name_p) noexcept
{
	typename system_table::iterator l_probe_result = m_system_table.find(robin_hood::hash_bytes(system_name_p, std::strlen(system_name_p)));
	if (l_probe_result != m_system_table.end())
	{
		return l_probe_result->second;
	}
	return system_view<system_base>();
}

void ECS::serialize_entity(std::pmr::string& out_buffer, archetype_base* const entt_p) noexcept
{
	thread_local static std::pmr::string tl_s_temp_buffer = "serialize_component_";
	for (auto& [hash, component] : entt_p->m_component_view_table)
	{
		FE_LOG(component->get_typename().c_str());
		(out_buffer);
		(hash);
		(component);
		//FE::task_base* l_component_serializer = FE::framework::framework_base::get_framework().get_method_reflection().retrieve();
		
	}
}

END_NAMESPACE