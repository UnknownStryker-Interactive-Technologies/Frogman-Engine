/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#include <FE/framework/ECS.hxx>
#include <FE/framework.hxx>

#include <FE/framework/mutex.hpp> // fiber lock

#include <vector>




BEGIN_NAMESPACE(FE::framework)


ECS::ECS(FE::size component_type_count_hint_p) noexcept
	:	m_gc_root(),

		m_memory_resource(),
		m_archetype_pool(),

		m_entity_list(),
		m_component_table(),

		m_archetype_default_entities(),
		m_buffer(),
		m_fiber_lock()
{
	m_component_table.reserve(component_type_count_hint_p * 2);
}


void ECS::initialize(framework::initializer_list&& initializer_list_p) noexcept
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
}


void ECS::attatch_component(FE::entity<archetype_base> entt_p, const ::FE::component_view<component_base>& to_attatch_p) noexcept
{
	FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
	FE_ASSERT(to_attatch_p.is_valid() == true, "Assertion failed: the component to attatch is not valid.");


	const std::size_t l_hash_code = CityHash64(to_attatch_p->m_metadata->_typename, std::strlen(to_attatch_p->m_metadata->_typename));

	std::lock_guard<FE::mutex> l_lock(m_fiber_lock);

	entt_p->m_component_view_table[l_hash_code] = to_attatch_p;
}


initializer ECS::serialize_entity(FE::entity<archetype_base> entt_p, FE::ASCII* const entity_memory_layout_version) noexcept
{
	constexpr FE::ASCII* l_function_prefix = "serialize_component_";
	constexpr FE::ASCII* l_class = "class";
	constexpr FE::ASCII* l_struct = "struct";

	initializer l_serialized_components(&m_memory_resource);


	for (auto& [hash, component] : entt_p->m_component_view_table)
	{
		m_fiber_lock.lock();

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
		l_arguments._third = entity_memory_layout_version;

		m_fiber_lock.unlock();

		(*l_component_serializer)(nullptr, &l_arguments); // Boom! Magic!
		/*
		* The first argument is a reference to the serialized component data buffer.
		* The second argument is a pointer to the component instance.
		* The third argument is the memory layout version of the component.
		*/
	}
	return std::move(l_serialized_components);
}

void ECS::deserialize_entity(const initializer& serialized_components_p, FE::entity<archetype_base> out_entt_p, FE::ASCII* const entity_memory_layout_version) noexcept
{
	std::lock_guard<FE::mutex> l_lock(m_fiber_lock);

	constexpr FE::ASCII* l_function_prefix = "deserialize_component_";
	constexpr FE::ASCII* l_class = "class";
	constexpr FE::ASCII* l_struct = "struct";


	for (auto& [hash, component] : out_entt_p->m_component_view_table)
	{
		m_fiber_lock.lock();

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
		l_arguments._third = entity_memory_layout_version;

		m_fiber_lock.unlock();

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


archetype_base::archetype_base(framework::ECS& host_p) noexcept
	:	m_component_view_table(),
		m_host(&host_p),
		m_memory_layout_version("v0.0.0"),
		m_group(),
		m_index()
{
	//m_component_view_table.try_reserve(128);

}

archetype_base::~archetype_base() noexcept
{}


archetype_base::archetype_base(framework::ECS& host_p, const FE::framework::initializer& other_p) noexcept
	:	m_component_view_table(),
		m_host(&host_p)
{
	deserialize_entity(other_p);
}

archetype_base& archetype_base::operator=(const FE::framework::initializer& other_p) noexcept
{
	deserialize_entity(other_p);
	return *this;
}


void FE::archetype_base::attatch_component(const FE::component_view<component_base>& to_attatch_p) noexcept
{
	FE_ASSERT(m_host != nullptr, "Assertion failed: the archetype's host ECS is null.");
	FE::entity<FE::archetype_base> l_self;
	FE::internal::smart_ptr::metadata<FE::archetype_base> l_forged_metadata{};
	l_forged_metadata._data = this;
	l_self.m_ptr.store(&l_forged_metadata, std::memory_order_relaxed);

	m_host->attatch_component(l_self, to_attatch_p);
	l_self.m_ptr.store(nullptr, std::memory_order_relaxed);
}


FE::framework::initializer FE::archetype_base::serialize_entity() noexcept
{
	FE_ASSERT(m_host != nullptr, "Assertion failed: the archetype's host ECS is null.");
	FE::entity<FE::archetype_base> l_self;
	FE::internal::smart_ptr::metadata<FE::archetype_base> l_forged_metadata{};
	l_forged_metadata._data = this;
	l_self.m_ptr.store(&l_forged_metadata, std::memory_order_relaxed);

	auto l_initializer = m_host->serialize_entity(l_self, m_memory_layout_version);
	l_self.m_ptr.store(nullptr, std::memory_order_relaxed);
	return l_initializer;
}

void FE::archetype_base::deserialize_entity(const FE::framework::initializer& serialized_components_p) noexcept
{
	FE_ASSERT(m_host != nullptr, "Assertion failed: the archetype's host ECS is null.");
	FE::entity<FE::archetype_base> l_self;
	FE::internal::smart_ptr::metadata<FE::archetype_base> l_forged_metadata{};
	l_forged_metadata._data = this;
	l_self.m_ptr.store(&l_forged_metadata, std::memory_order_relaxed);

	m_host->deserialize_entity(serialized_components_p, l_self, m_memory_layout_version);
	l_self.m_ptr.store(nullptr, std::memory_order_relaxed);
}

FE::memory_resource* archetype_base::__get_ecs_memory_resource() noexcept { return &m_host->m_memory_resource; }




component_base::component_base(component_base&& other_p) noexcept
	:	m_metadata(std::move(other_p.m_metadata))
{}

FE::ASCII* component_base::get_typename() const noexcept { return m_metadata->_typename; }




internal::ECS::gc_metadata::gc_metadata() noexcept
	:	_member_components(),
		_garbage_class(::FE::internal::GarbageClass::_NotAGarbage)
{
}

END_NAMESPACE