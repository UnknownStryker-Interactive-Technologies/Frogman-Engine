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


END_NAMESPACE