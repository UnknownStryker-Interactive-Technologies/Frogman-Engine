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

// ECS Archetype
#include <FE/framework/archetype_base.hpp>
#include <FE/framework/component_base.hpp>
#include <FE/framework/system_base.hpp>

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


template <class T>
using entity = FE::smart_ptr<T, FE::RefType::_Observer>;

using archetype = FE::smart_ptr<FE::archetype_base, FE::RefType::_Owner>;

using component = FE::smart_ptr<FE::component_base, FE::RefType::_Owner>;

using system = FE::smart_ptr<FE::system_base, FE::RefType::_Owner>;


class components
{
public:
	_FE_MAYBE_UNUSED_ static constexpr FE::size max_components = 1024;

private:
	component m_components[max_components]; // use FE::exclude_if()
	var::size m_current_size = 0;

public:
	void add_component(component&& comp_p) noexcept
	{
		FE_ASSERT(m_current_size < max_components);

		m_components[m_current_size] = std::move(comp_p);
		++m_current_size;
	}

	_FE_FORCE_INLINE_ var::size get_size() { return m_current_size; }
};


class ECS
{
	using archetype_table = robin_hood::unordered_map<std::pmr::string, archetype>;
	using component_table = robin_hood::unordered_map<	std::pmr::string, 
														FE::pair<	std::pmr::forward_list<components>, 
																	FE::scalable_pool<FE::PoolPageCapacity::_256KiB, FE::align_8bytes>
																	>
														>;
	using system_table = robin_hood::unordered_map<std::pmr::string, system>;

	FE::memory_resource m_game_memory_resource;

	archetype_table m_archetype_table;
	component_table m_component_table;
	system_table m_system_table;

public:
	ECS() noexcept 
		:	m_game_memory_resource(),
			m_archetype_table(),
			m_component_table(),
			m_system_table()
	{
		m_archetype_table.reserve(1024);
		m_component_table.reserve(1024);
		m_system_table.reserve(1024);
	}

	~ECS() noexcept = default;

	template <class Archetype, typename ...Arguments>
	entity<Archetype> instanciate_entity(FE::ASCII* const entity_name_p, Arguments&& ...arguments_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: T must be derived from FE::archetype_base.");

		std::pmr::string l_entity_name( FE::framework::reflection::type_id<Archetype>().name(), &m_game_memory_resource );
		l_entity_name += " ";
		l_entity_name += entity_name_p;
		FE::archetype l_alloc_result = FE::make_owner<Archetype>( &m_game_memory_resource, std::forward<Arguments>(arguments_p)... );
		l_alloc_result->m_name = std::pmr::string( l_entity_name, &m_game_memory_resource );
		entity<Archetype> l_entity = FE::down_cast_owner_to_observer<Archetype>( l_alloc_result );

		std::pair<typename archetype_table::iterator, bool> l_result = m_archetype_table.emplace( std::move(l_entity_name), std::move(l_alloc_result) );
		
		if (l_result.second == true) // The emplace() was successful. 
		{
			return l_entity;
		}

		return entity<Archetype>();
	}

	template <class Archetype>
	void destruct_entity(const entity<Archetype>& entt_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: T must be derived from FE::archetype_base.");

		for (typename archetype_table::iterator probe_result = m_archetype_table.find(entt_p->get_name()); probe_result != m_archetype_table.end(); ++probe_result)
		{
			if (probe_result->first == entt_p->get_name())
			{
				m_archetype_table.erase(probe_result);
				return;
			}
		}
	}

	template <class Component, class Archetype, typename ...Arguments>
	component_view<Component> add_component(entity<Archetype>& entt_p, FE::ASCII* const component_name_p, Arguments&& ...arguments_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: T must be derived from FE::component_base.");
		
		std::pmr::string l_typename( FE::framework::reflection::type_id<Component>().name(), &m_game_memory_resource );

		typename component_table::iterator l_probe_result = m_component_table.find(l_typename);
		if (l_probe_result == m_component_table.end())
		{
			/*               ( m_game_memory_resource )
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
			typename component_table::iterator l_list_and_allocator = m_component_table.emplace( l_typename, typename component_table::mapped_type{} ).first;
			l_list_and_allocator->second._second = typename component_table::mapped_type::second_type{ &m_game_memory_resource }; // Derive the memory resource from m_game_memory_resource.
			l_list_and_allocator->second._first = typename component_table::mapped_type::first_type{ &(l_list_and_allocator->second._second) };
			l_list_and_allocator->second._first.emplace_front();
			l_probe_result = l_list_and_allocator;
		}

		FE::component l_alloc_result;
		for (auto& components : l_probe_result->second._first)
		{
			if (components.get_size() == components.max_components)
			{
				continue;
			}
			l_alloc_result = FE::make_owner<Component>(&(l_probe_result->second._second), std::forward<Arguments>(arguments_p)...);
			FE::component_view<Component> l_view = FE::down_cast_owner_to_observer<Component>(l_alloc_result);
			auto l_result = entt_p->m_component_view_table.emplace(component_name_p, l_view);
			if (l_result.second == false)
			{
				FE_LOG("FE ECS: add_component<T>() failed due to a pre-existing duplicate component.");
				return component_view<Component>();
			}
			components.add_component( std::move(l_alloc_result) );
			return l_view;
		}

		if (l_alloc_result == nullptr) _FE_LIKELY_
		{
			// All components lists are full. Create a new one.
			l_probe_result->second._first.emplace_front();
			l_alloc_result = FE::make_owner<Component>(&(l_probe_result->second._second), std::forward<Arguments>(arguments_p)...);
			FE::component_view<Component> l_view = FE::down_cast_owner_to_observer<Component>(l_alloc_result);
			auto l_result = entt_p->m_component_view_table.emplace(component_name_p, l_view);
			if (l_result.second == false)
			{
				FE_LOG("FE ECS: add_component<T>() failed due to a pre-existing duplicate component.");
				return component_view<Component>();
			}
			l_probe_result->second._first.front().add_component( std::move(l_alloc_result) );
			return l_view;
		}

		return component_view<Component>();
	}

	template <class Component, class Archetype>
	void destruct_component(entity<Archetype>& entt_p, FE::ASCII* const component_name_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: T must be derived from FE::component_base.");
		
		auto l_probe_result = entt_p->m_component_view_table.find(component_name_p);
		if (l_probe_result != entt_p->m_component_view_table.end())
		{
			entt_p->m_component_view_table.erase(l_probe_result);
		}
	}
};


END_NAMESPACE
#endif