#ifndef _FE_FRAMEWORK_FRAMEWORK_ECS_HPP_
#define _FE_FRAMEWORK_FRAMEWORK_ECS_HPP_
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
#include <FE/prerequisites.h>

// E, C, and S
#include <FE/framework/archetype_base.hpp>
#include <FE/framework/component_base.hpp>
#include <FE/framework/system_base.hpp>

#include <FE/framework/file.hpp>

// ECS smart pointer
#include <FE/framework/smart_ptr.hxx>

// FE::reflection::type_id<Archetype>()
#include <FE/framework/type_info.hpp>

// game memory pool
#include <FE/pool/memory_resource.hpp>

// ECS data structures
#include <FE/farray.hxx>
#include <forward_list>
#include <robin_hood.h>
#include <vector>




BEGIN_NAMESPACE(FE)


using serialized_entity = robin_hood::unordered_map<std::pmr::string, std::pmr::string>;

class ECS
{
	using archetype_table = robin_hood::unordered_map<std::pmr::string, archetype>;
	using component_table = robin_hood::unordered_map<	std::size_t, // the robin hood hash map uses lighter hashing algorithm for integers, than objects.
														FE::pair<	std::pmr::forward_list<components>, 
																	FE::scalable_pool<FE::PoolPageCapacity::_256KiB, FE::align_8bytes>
																	>
														>;
	using system_table = robin_hood::unordered_map<std::size_t, system>;

	std::pmr::memory_resource* m_memory_resource;

	archetype_table m_archetype_table;
	component_table m_component_table;
	system_table m_system_table;

public:
	ECS(std::pmr::memory_resource* resource) noexcept;
	ECS(FE::init& file_p, std::pmr::memory_resource* resource) noexcept;
	~ECS() noexcept = default;

	ECS(const ECS&) noexcept = delete;
	ECS& operator=(const ECS&) noexcept = delete;

	
	template <class Archetype, typename ...Arguments>
	entity<Archetype> instanciate_entity(FE::ASCII* const entity_name_p, Arguments&& ...arguments_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");

		FE::archetype l_alloc_result = FE::make_owner<Archetype>( m_memory_resource, std::forward<Arguments>(arguments_p)... );
		l_alloc_result->m_name = std::pmr::string( m_memory_resource );
		l_alloc_result->m_name.reserve( std::strlen(FE::framework::reflection::type_id<Archetype>().name()) + 1 + std::strlen(entity_name_p) );

		l_alloc_result->m_name = FE::framework::reflection::type_id<Archetype>().name();
		l_alloc_result->m_name += " ";
		l_alloc_result->m_name += entity_name_p;
		std::pair<typename archetype_table::iterator, bool> l_result = m_archetype_table.emplace( l_alloc_result->m_name, std::move(l_alloc_result) );
		
		if (l_result.second == true) // The emplace() was successful. 
		{
			return FE::down_cast_owner_to_observer<Archetype>(l_result.first->second);
		}

		return entity<Archetype>();
	}

	template <class Archetype>
	void destruct_entity(const entity<Archetype>& entt_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		typename archetype_table::iterator l_probe_result = m_archetype_table.find(entt_p->get_name());

		if (l_probe_result->first == entt_p->get_name())
		{
			m_archetype_table.erase(l_probe_result);
			return;
		}
	}

	std::pmr::string m_name_buffer;

	template <class Archetype>
	entity<Archetype> find_entity(FE::ASCII* const entity_name_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		m_name_buffer.reserve( std::strlen( FE::framework::reflection::type_id<Archetype>().name() ) + 1 + std::strlen(entity_name_p) );
		m_name_buffer = FE::framework::reflection::type_id<Archetype>().name();
		m_name_buffer += " ";
		m_name_buffer += entity_name_p;

		typename archetype_table::iterator l_probe_result = m_archetype_table.find(m_name_buffer);
		m_name_buffer.clear();
		if (l_probe_result != m_archetype_table.end())
		{
			return FE::down_cast_owner_to_observer<Archetype>(l_probe_result->second);
		}

		return entity<Archetype>();
	}


	template <class Component, typename ...Arguments>
	component_view<Component> add_component(archetype_base* const entt_p, Arguments&& ...arguments_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");

		typename component_table::iterator l_probe_result = m_component_table.find(FE::framework::reflection::type_id<Component>().hash_code());
		if (l_probe_result == m_component_table.end())
		{
			/*               ( m_memory_resource )
			*----------------------------------------------------------*
			*                    ( component pool )                    *
			*   *--------------------------------------------------*   *
			*   *                                                  *   *
			*   *                                                  *   *   
			*   *                                                  *   *
			*   *                                                  *   *
			*   *                                                  *   *
			*   *--------------------------------------------------*   *
			*                                                          *
			*----------------------------------------------------------*
			*/
			typename component_table::iterator l_list_and_allocator = m_component_table.emplace(FE::framework::reflection::type_id<Component>().hash_code(), typename component_table::mapped_type(std::pmr::forward_list<FE::components>(m_memory_resource), m_memory_resource)).first;
			l_list_and_allocator->second._first.emplace_front(); // allocate a component pool.
			l_probe_result = l_list_and_allocator;
		}


		FE::component l_alloc_result;
		for (typename component_table::mapped_type::first_type::iterator components = l_probe_result->second._first.begin(); components != l_probe_result->second._first.end(); ++components)
		{
			if (components->get_size() == components->max_components) // true if the component pool is full.
			{
				continue;
			}
			
			l_alloc_result = FE::make_owner<Component>(&(l_probe_result->second._second), std::forward<Arguments>(arguments_p)...);
			l_alloc_result->m_identifier._typename = std::pmr::string(FE::framework::reflection::type_id<Component>().name(), m_memory_resource);
			l_alloc_result->m_identifier._memory_layout_version = std::pmr::string("default", m_memory_resource); // modify the value when the memory layout of the component changes; this ensures correct auto serialization.
			FE::component_view<Component> l_view = FE::down_cast_owner_to_observer<Component>(l_alloc_result);
			
			auto l_result = entt_p->m_component_view_table.emplace(FE::framework::reflection::type_id<Component>().hash_code(), l_view);
			if (l_result.second == false)
			{
				FE_LOG("FE ECS: add_component<T>() failed due to a pre-existing duplicate component.");
				return component_view<Component>();
			}
			
			FE::size l_idx = components->add_component( std::move(l_alloc_result) );
			l_view->m_identifier._group = components;
			l_view->m_identifier._index = l_idx;
			return l_view;
		}


		// could not find a component pool with free space, allocate a new component pool!
		if (l_alloc_result == nullptr) _FE_LIKELY_
		{
			// All components lists are full. Create a new one.
			l_probe_result->second._first.emplace_front();

			l_alloc_result = FE::make_owner<Component>(&(l_probe_result->second._second), std::forward<Arguments>(arguments_p)...);
			l_alloc_result->m_identifier._typename = std::pmr::string(FE::framework::reflection::type_id<Component>().name(), m_memory_resource);
			l_alloc_result->m_identifier._memory_layout_version = std::pmr::string("default", m_memory_resource); // modify the value when the memory layout of the component changes; this ensures correct auto serialization.
			FE::component_view<Component> l_view = FE::down_cast_owner_to_observer<Component>(l_alloc_result);
			
			auto l_result = entt_p->m_component_view_table.emplace(FE::framework::reflection::type_id<Component>().hash_code(), l_view);
			if (l_result.second == false)
			{
				FE_LOG("FE ECS: add_component<T>() failed due to a pre-existing duplicate component.");
				return component_view<Component>();
			}
			
			FE::size l_idx = l_probe_result->second._first.front().add_component( std::move(l_alloc_result) );
			l_view->m_identifier._group = l_probe_result->second._first.begin();
			l_view->m_identifier._index = l_idx;
			return l_view;
		}

		return component_view<Component>();
	}

	template <class Component, class Archetype, typename ...Arguments>
	_FE_FORCE_INLINE_ component_view<Component> add_component(FE::entity<Archetype> entt_p, Arguments&& ...arguments_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		return add_component<Component>(entt_p.operator->(), std::forward<Arguments>(arguments_p)...);
	}

	template <class Component>
	void remove_component(archetype_base* const entt_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");
		
		typename archetype_base::component_view_table::iterator l_view_table_probe_result = entt_p->m_component_view_table.find(FE::framework::reflection::type_id<Component>().hash_code());

		if (l_view_table_probe_result == entt_p->m_component_view_table.end())
		{
			return; // The entity does not have this component.
		}

		typename component_table::iterator l_com_table_probe_result = m_component_table.find(FE::framework::reflection::type_id<Component>().hash_code());
		FE_ASSERT(l_com_table_probe_result != m_component_table.end(), "Assertion failed: the component table must have this component.");

		// Remove the component from the component table.
		l_view_table_probe_result->second->m_identifier._group->remove_component(l_view_table_probe_result->second->m_identifier._index);

		// Remove the component from the entity's component view table.
		entt_p->m_component_view_table.erase(l_view_table_probe_result);
	}

	template <class Component, class Archetype>
	_FE_FORCE_INLINE_ void remove_component(FE::entity<Archetype> entt_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		remove_component<Component>(entt_p.operator->());
	}


	template <class System>
	system_view<System> register_system() noexcept
	{
		static_assert(std::is_base_of_v<FE::system_base, System>, "Static assertion failed: the template argument System must be derived from FE::system_base.");
		typename system_table::iterator l_probe_result = m_system_table.find(FE::framework::reflection::type_id<System>().hash_code());
		if (l_probe_result != m_system_table.end())
		{
			return FE::down_cast_owner_to_observer<System>(l_probe_result->second);
		}
		FE::system l_alloc_result = FE::make_owner<System>(m_memory_resource);
		l_alloc_result->m_typename = std::pmr::string(FE::framework::reflection::type_id<System>().name(), m_memory_resource);
		std::pair<typename system_table::iterator, bool> l_result = m_system_table.emplace(FE::framework::reflection::type_id<System>().hash_code(), std::move(l_alloc_result));
		
		if (l_result.second == true) // The emplace() was successful. 
		{
			return FE::down_cast_owner_to_observer<System>(l_alloc_result);
		}
		return system_view<System>();
	}

	template <class System>
	system_view<System> find_system() noexcept
	{
		static_assert(std::is_base_of_v<FE::system_base, System>, "Static assertion failed: the template argument System must be derived from FE::system_base.");
		typename system_table::iterator l_probe_result = m_system_table.find(FE::framework::reflection::type_id<System>().hash_code());
		if (l_probe_result != m_system_table.end())
		{
			return FE::down_cast_owner_to_observer<System>(l_probe_result->second);
		}
		return system_view<System>();
	}

	system_view<system_base> find_system(FE::ASCII* const system_name_p) noexcept;


	serialized_entity serialize_entity(FE::entity<archetype_base> entt_p) noexcept;
	void deserialize_entity(serialized_entity& serialized_components_p, FE::entity<archetype_base> entt_p) noexcept;
};


END_NAMESPACE
#endif