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
		m_system_table(),
		m_name_buffer(resource)
{
	m_archetype_table.reserve(1024);
	m_component_table.reserve(1024);
	m_system_table.reserve(1024);
}

ECS::ECS(FE::init& file_p, std::pmr::memory_resource* resource) noexcept
	:	m_memory_resource(resource),
		m_archetype_table(),
		m_component_table(),
		m_system_table(),
		m_name_buffer(resource)
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


serialized_entity ECS::serialize_entity(FE::entity<archetype_base> entt_p) noexcept
{
	constexpr FE::ASCII* l_function_prefix = "serialize_component_";
	robin_hood::unordered_map<std::pmr::string, std::pmr::string> l_serialized_components;

	for (auto& [hash, component] : entt_p->m_component_view_table)
	{
		m_name_buffer = component->get_typename();
		std::pmr::string::size_type l_pos = m_name_buffer.find("class");
		FE_ASSERT(l_pos != std::pmr::string::npos, "Assertion failed: the component type name must start with 'class'.");
		l_pos += std::strlen("class");
		m_name_buffer.erase(0, l_pos);

		for (l_pos = m_name_buffer.find(' '); l_pos != std::pmr::string::npos; l_pos = m_name_buffer.find(' '))
		{
			m_name_buffer.erase(m_name_buffer.begin() + l_pos);
		}
		m_name_buffer.insert(0, l_function_prefix);

		FE::task_base* l_component_serializer = FE::framework::framework_base::get_framework().get_method_reflection().retrieve(m_name_buffer);
		FE::arguments<std::pmr::string&, FE::component_base*, FE::ASCII*> l_arguments;
		l_serialized_components.emplace( std::pmr::string(component->get_typename(), m_memory_resource), std::pmr::string(m_memory_resource) );
		l_arguments._first = l_serialized_components[component->get_typename()];
		l_arguments._second = component.operator->();
		l_arguments._third = component->get_memory_layout_version().c_str();
		(*l_component_serializer)(nullptr, &l_arguments); // Boom! Magcic!
		/*
		* The first argument is a reference to the serialized component datavbuffer.\
		* The second argument is a pointer to the component instance.\
		* The third argument is the memory layout version of the component.
		*/
	}
	return l_serialized_components;
}
// const robin_hood::unordered_map<std::pmr::string, std::pmr::string>& does not compile
void ECS::deserialize_entity(serialized_entity& serialized_components_p, FE::entity<archetype_base> entt_p) noexcept
{
	constexpr FE::ASCII* l_function_prefix = "deserialize_component_";

	for (auto& [hash, component] : entt_p->m_component_view_table)
	{
		m_name_buffer = component->get_typename();
		std::pmr::string::size_type l_pos = m_name_buffer.find("class");
		FE_ASSERT(l_pos != std::pmr::string::npos, "Assertion failed: the component type name must start with 'class'.");
		l_pos += std::strlen("class");
		m_name_buffer.erase(0, l_pos);

		for (l_pos = m_name_buffer.find(' '); l_pos != std::pmr::string::npos; l_pos = m_name_buffer.find(' '))
		{
			m_name_buffer.erase(m_name_buffer.begin() + l_pos);
		}
		m_name_buffer.insert(0, l_function_prefix);

		_FE_MAYBE_UNUSED_ FE::task_base* l_component_deserializer = FE::framework::framework_base::get_framework().get_method_reflection().retrieve(m_name_buffer);

		auto l_probe_result = serialized_components_p.find(component->get_typename());
		FE_ASSERT(l_probe_result != serialized_components_p.end(), "Assertion failed: the serialized component data for the component type was not found.");

		FE::arguments<const std::pmr::string&, FE::component_base*, FE::ASCII*> l_arguments;
		l_arguments._first = l_probe_result->second;
		l_arguments._second = component.operator->();
		l_arguments._third = component->get_memory_layout_version().c_str();
		(*l_component_deserializer)(nullptr, &l_arguments);
		///*
		//* The first argument is a reference to the serialized component datavbuffer.\
		//* The second argument is a pointer to the component instance.\
		//* The third argument is the memory layout version of the component.
		//*/
	}
}

END_NAMESPACE