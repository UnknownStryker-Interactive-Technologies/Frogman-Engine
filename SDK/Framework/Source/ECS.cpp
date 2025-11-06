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
#include <FE/framework/ECS.hxx>
#include <FE/framework.hxx>

#include <vector>




BEGIN_NAMESPACE(FE::framework)


ECS::ECS(FE::size max_entities_p, FE::size component_type_count_hint_p, FE::size system_count_hint_p) noexcept
	:	m_memory_resource(),
		m_archetype_pool(),

		m_archetype_table(),
		m_component_table(),
		m_system_table(),
		m_archetype_default_entities(),
		m_buffer(),
		m_fiber_lock(),
		m_max_entities(max_entities_p)
{
	m_archetype_table.reserve(m_max_entities);
	m_component_table.reserve(component_type_count_hint_p);
	m_system_table.reserve(system_count_hint_p);
}


void ECS::initialize(framework::initializer_list& initializer_list_p, framework::system_table_initializer_list& system_table_initializer_p) noexcept
{
	m_archetype_default_entities = std::move(initializer_list_p);
	m_buffer = std::pmr::string(&m_memory_resource);

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

		FE::arguments<FE::ASCII*, framework::initializer&> l_arguments{ "", archetype_default_entity };
		FE::entity<FE::archetype_base> l_entity;
		(*l_entity_creator)(this, &l_entity, &l_arguments); // Boom! Magcic!
		FE_ASSERT(l_entity.is_valid() == true, "Assertion failed: the entity could not be instanciated from the initializer list.");
	}


	for (auto& [system_name, component_type_names] : system_table_initializer_p)
	{
		m_buffer.clear();
		m_buffer.reserve(component_type_names.length());

		FE::task_base* l_task = framework::framework_base::get_framework().get_method_reflection().retrieve(system_name);
		FE_ASSERT(l_task != nullptr, "Assertion failed: the system function is not found. The system may not be registered.");

		FE::system l_system = l_task->try_get_as_system();
		FE_ASSERT(l_system != nullptr, "Assertion failed: the system function signature is invalid. The system function must have the signature: void system_name(FE::component_base* const).");

		FE::pair<FE::system, std::pmr::vector<std::size_t>> l_value{ l_system, std::pmr::vector<std::size_t>(&m_memory_resource) };
		l_value._second.reserve(32);
		std::pmr::string::size_type l_name_pos = 0;
		std::pmr::string::size_type l_next_name_pos = component_type_names.find(',', l_name_pos);
		std::pmr::string::size_type l_name_length = 0;

		while (l_next_name_pos != std::pmr::string::npos)
		{
			l_name_length = l_next_name_pos - l_name_pos;
			m_buffer.resize(l_name_length + 1); // +1 for the null terminator
			component_type_names.copy(m_buffer.data(), l_name_length, l_name_pos);
			l_value._second.emplace_back(robin_hood::hash_bytes(m_buffer.c_str(), m_buffer.length()));
			l_name_pos = l_next_name_pos + 1;
			l_next_name_pos = component_type_names.find(',', l_name_pos);
		}

		_FE_MAYBE_UNUSED_ auto l_result = m_system_table.emplace(std::pmr::string(system_name, &m_memory_resource), l_value);
		FE_ASSERT(l_result.second == true, "Assertion failed: the system was already registered.");
	}
}

void ECS::destruct_entity(FE::entity<archetype_base> entt_p) noexcept
{
	FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
	std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

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
	std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

	_FE_MAYBE_UNUSED_ auto l_result = entt_p->m_component_view_table.emplace( robin_hood::hash_bytes( to_attatch_p->m_metadata->_typename, std::strlen(to_attatch_p->m_metadata->_typename) ), to_attatch_p );
	FE_ASSERT(l_result.second == true, "Assertion failed: the component to attatch already exists in the entity.");
}


std::optional< FE::pair<FE::system, std::pmr::vector<std::size_t>> > ECS::find_system(FE::ASCII* const system_name_p) noexcept
{
	m_buffer = system_name_p;
	auto l_result = m_system_table.find(m_buffer);
	if (l_result == m_system_table.end())
	{
		return std::nullopt;
	}
	return l_result->second;
}


initializer ECS::serialize_entity(FE::entity<archetype_base> entt_p) noexcept
{
	constexpr FE::ASCII* l_function_prefix = "serialize_component_";
	constexpr FE::ASCII* l_class = "class";
	constexpr FE::ASCII* l_struct = "struct";

	initializer l_serialized_components;

	std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

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
		_FE_MAYBE_UNUSED_ auto l_result = l_serialized_components.emplace( std::pmr::string(component->get_typename(), &m_memory_resource), std::pmr::string(&m_memory_resource) );
		FE_ASSERT(l_result.second == true, "Assertion failed: the component type was already serialized. This should never happen.");
		l_arguments._first = l_serialized_components[component->get_typename()];
		l_arguments._second = component.operator->();
		l_arguments._third = component->get_memory_layout_version();
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

	std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

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
		l_arguments._third = component->get_memory_layout_version();
		(*l_component_deserializer)(nullptr, &l_arguments);
		/*
		* The first argument is a reference to the serialized component datavbuffer.
		* The second argument is a pointer to the component instance.
		* The third argument is the memory layout version of the component.
		*/
	}
}


END_NAMESPACE




BEGIN_NAMESPACE(FE)


archetype_base::archetype_base() noexcept
	:	m_component_view_table(),
		m_name()
{
}

archetype_base::~archetype_base() noexcept
{
	for (auto& pair : m_component_view_table)
	{
		pair.second->m_metadata->_group->remove_component(pair.second->m_metadata->_index);
	}
}




component_base::component_base() noexcept
	: m_metadata()
{
}

FE::ASCII* component_base::get_typename() const noexcept { return m_metadata->_typename; }
FE::ASCII* component_base::get_memory_layout_version() const noexcept { return m_metadata->_memory_layout_version; }


internal::ECS::gc_metadata::gc_metadata() noexcept
	: _member_components(),
	_member_entities(),
	_is_circular_reference(false)
{
}


internal::ECS::component_metadata::component_metadata() noexcept
	: m_gc_metadata(),
	_group(),
	_index(0),
	_typename(nullptr),
	_memory_layout_version("default")
{
}


END_NAMESPACE