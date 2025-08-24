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

// game memory pool
#include <FE/pool/memory_resource.hpp>

// FE::reflection::type_id<T>()
#include <FE/framework/type_info.hpp>

// ECS smart pointer
#include <FE/framework/smart_ptr.hxx>

// ECS data structures
#include <forward_list>
#include <robin_hood.h>
#include <vector>




BEGIN_NAMESPACE(FE)


template <class T>
using entity = FE::smart_ptr<T, FE::RefType::_Observer>;

using archetype = FE::smart_ptr<FE::archetype_base, FE::RefType::_Owner>;

using component = FE::smart_ptr<FE::component_base, FE::RefType::_Owner>;

using system = FE::smart_ptr<FE::system_base, FE::RefType::_Owner>;


struct components
{
	_FE_MAYBE_UNUSED_ static constexpr FE::size max_components = 1024;

	std::array<component, max_components> _components; // use FE::exclude_if()
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

	template <class T, typename ...Arguments>
	entity<T> instanciate_entity(FE::ASCII* const entity_name_p, Arguments&& ...arguments_p)
	{
		static_assert(std::is_base_of_v<FE::archetype_base, T>, "Static assertion failed: T must be derived from FE::archetype_base");

		std::pmr::string l_entity_name(&m_game_memory_resource);
		l_entity_name = FE::framework::reflection::type_id<T>().name();
		l_entity_name += " ";
		l_entity_name += entity_name_p;
		FE::archetype l_alloc_result = std::allocate_shared< T, std::pmr::polymorphic_allocator<T> >(&m_game_memory_resource, std::forward<Arguments>(arguments_p)...);
		entity<T> l_entity = l_alloc_result;

		std::pair<typename archetype_table::iterator, bool> l_result = m_archetype_table.emplace( std::move(l_entity_name), std::move(l_alloc_result) );
		
		if (l_result.second == true)
		{
			// Entity with the same name and the same type already exists, return null entity.
			return l_entity;
		}

		return entity<T>();
	}
};


END_NAMESPACE
#endif