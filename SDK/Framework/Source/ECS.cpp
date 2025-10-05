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
		m_archetype_pool(),

		m_archetype_table(),
		m_component_table(),
		m_system_table(),
		m_buffer(resource)
{
	m_archetype_table.reserve(1024);
	m_component_table.reserve(1024);
	m_system_table.reserve(1024);
}

ECS::ECS(FE::initializer_list& initializer_list_p, std::pmr::memory_resource* resource) noexcept
	:	m_memory_resource(resource),
		m_archetype_pool(),

		m_archetype_table(),
		m_component_table(),
		m_system_table(),
		m_archetype_default_entities(initializer_list_p),
		m_buffer(resource)
{
	m_archetype_table.reserve(1024);
	m_component_table.reserve(1024);
	m_system_table.reserve(1024);

	for (auto& [archetype_identifier, archetype_default_entity] : m_archetype_default_entities)
	{
		constexpr FE::ASCII* l_class = "class";
		constexpr FE::ASCII* l_struct = "struct";

		m_buffer = archetype_identifier;
		std::pmr::string::size_type l_pos = m_buffer.find(l_class);
		if (l_pos == std::pmr::string::npos)
		{
			l_pos = m_buffer.find(l_struct);
			FE_ASSERT(l_pos != std::pmr::string::npos, "Assertion failed: the component type name must start with 'class' or 'struct'.");
			l_pos += std::strlen(l_struct);
		}
		else
		{
			l_pos += std::strlen(l_class);
		}
		m_buffer.erase(0, l_pos);

		for (l_pos = m_buffer.find(' '); l_pos != std::pmr::string::npos; l_pos = m_buffer.find(' '))
		{
			m_buffer.erase(m_buffer.begin() + l_pos);
		}
		m_buffer.insert(0, "::");


		FE::task_base* l_entity_creator = FE::framework::framework_base::get_framework().get_method_reflection().retrieve(m_buffer);
		FE_ASSERT(l_entity_creator != nullptr, "Assertion failed: the entity creator function is not found. The archetype type may not be registered.");

		FE::arguments<FE::ASCII*, FE::initializer&> l_arguments{ "", archetype_default_entity};
		FE::entity<FE::archetype_base> l_entity;
		(*l_entity_creator)(this, &l_entity, &l_arguments); // Boom! Magcic!
		FE_ASSERT(l_entity.is_valid() == true, "Assertion failed: the entity could not be instanciated from the initializer list.");
	}
}

ECS::~ECS() noexcept
{
}


void ECS::destruct_entity(FE::entity<archetype_base> entt_p) noexcept
{
	FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
	typename archetype_table::iterator l_probe_result = m_archetype_table.find(entt_p->get_name());

	if (l_probe_result->first == entt_p->get_name())
	{
		m_archetype_table.erase(l_probe_result);
		return;
	}
}


void ECS::attatch_component(FE::entity<archetype_base> entt_p, const FE::component_view<component_base>& to_attatch_p) noexcept
{
	FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
	FE_ASSERT(to_attatch_p.is_valid() == true, "Assertion failed: the component to attatch is not valid.");

	_FE_MAYBE_UNUSED_ auto l_result = entt_p->m_component_view_table.emplace( robin_hood::hash_bytes( to_attatch_p->m_identifier._typename.c_str(), to_attatch_p->m_identifier._typename.length() ), to_attatch_p );
	FE_ASSERT(l_result.second == true, "Assertion failed: the component to attatch already exists in the entity.");
}


system ECS::find_system(FE::ASCII* const system_name_p) noexcept
{
	FE::task_base* l_system = FE::framework::framework_base::get_framework().get_method_reflection().retrieve(system_name_p);
	return (l_system == nullptr) ? nullptr : l_system->try_get_as_system();
}


initializer ECS::serialize_entity(FE::entity<archetype_base> entt_p) noexcept
{
	constexpr FE::ASCII* l_function_prefix = "serialize_component_";
	constexpr FE::ASCII* l_class = "class";
	constexpr FE::ASCII* l_struct = "struct";

	robin_hood::unordered_map<std::pmr::string, std::pmr::string> l_serialized_components;

	for (auto& [hash, component] : entt_p->m_component_view_table)
	{
		m_buffer = component->get_typename();
		std::pmr::string::size_type l_pos = m_buffer.find(l_class);
		if (l_pos == std::pmr::string::npos)
		{
			l_pos = m_buffer.find(l_struct);
			FE_ASSERT(l_pos != std::pmr::string::npos, "Assertion failed: the component type name must start with 'class' or 'struct'.");
			l_pos += std::strlen(l_struct);
		}
		else
		{
			l_pos += std::strlen(l_class);
		}
		m_buffer.erase(0, l_pos);

		for (l_pos = m_buffer.find(' '); l_pos != std::pmr::string::npos; l_pos = m_buffer.find(' '))
		{
			m_buffer.erase(m_buffer.begin() + l_pos);
		}
		m_buffer.insert(0, l_function_prefix);

		FE::task_base* l_component_serializer = FE::framework::framework_base::get_framework().get_method_reflection().retrieve(m_buffer);
		FE_ASSERT(l_component_serializer != nullptr, "Assertion failed: the component serializer function is not found. The component type may not be registered.");

		FE::arguments<std::pmr::string&, FE::component_base*, FE::ASCII*> l_arguments;
		l_serialized_components.emplace( std::pmr::string(component->get_typename(), m_memory_resource), std::pmr::string(m_memory_resource) );
		l_arguments._first = l_serialized_components[component->get_typename()];
		l_arguments._second = component.operator->();
		l_arguments._third = component->get_memory_layout_version().c_str();
		(*l_component_serializer)(nullptr, &l_arguments); // Boom! Magcic!
		/*
		* The first argument is a reference to the serialized component datavbuffer.
		* The second argument is a pointer to the component instance.
		* The third argument is the memory layout version of the component.
		*/
	}
	return l_serialized_components;
}
// const robin_hood::unordered_map<std::pmr::string, std::pmr::string>& does not compile
void ECS::deserialize_entity(initializer& serialized_components_p, FE::entity<archetype_base> out_entt_p) noexcept
{
	constexpr FE::ASCII* l_function_prefix = "deserialize_component_";
	constexpr FE::ASCII* l_class = "class";
	constexpr FE::ASCII* l_struct = "struct";

	for (auto& [hash, component] : out_entt_p->m_component_view_table)
	{
		m_buffer = component->get_typename();
		std::pmr::string::size_type l_pos = m_buffer.find(l_class);
		if (l_pos == std::pmr::string::npos)
		{
			l_pos = m_buffer.find(l_struct);
			FE_ASSERT(l_pos != std::pmr::string::npos, "Assertion failed: the component type name must start with 'class' or 'struct'.");
			l_pos += std::strlen(l_struct);
		}
		else
		{
			l_pos += std::strlen(l_class);
		}
		m_buffer.erase(0, l_pos);

		for (l_pos = m_buffer.find(' '); l_pos != std::pmr::string::npos; l_pos = m_buffer.find(' '))
		{
			m_buffer.erase(m_buffer.begin() + l_pos);
		}
		m_buffer.insert(0, l_function_prefix);

		FE::task_base* l_component_deserializer = FE::framework::framework_base::get_framework().get_method_reflection().retrieve(m_buffer);
		FE_ASSERT(l_component_deserializer != nullptr, "Assertion failed: the component deserializer function is not found. The component type may not be registered.");

		auto l_probe_result = serialized_components_p.find(component->get_typename());
		FE_ASSERT(l_probe_result != serialized_components_p.end(), "Assertion failed: the serialized component data for the component type was not found.");

		FE::arguments<const std::pmr::string&, FE::component_base*, FE::ASCII*> l_arguments;
		l_arguments._first = l_probe_result->second;
		l_arguments._second = component.operator->();
		l_arguments._third = component->get_memory_layout_version().c_str();
		(*l_component_deserializer)(nullptr, &l_arguments);
		/*
		* The first argument is a reference to the serialized component datavbuffer.
		* The second argument is a pointer to the component instance.
		* The third argument is the memory layout version of the component.
		*/
	}
}

END_NAMESPACE