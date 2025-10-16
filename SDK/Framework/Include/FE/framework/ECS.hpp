#ifndef _FE_FRAMEWORK_FRAMEWORK_ECS_HPP_
#define _FE_FRAMEWORK_FRAMEWORK_ECS_HPP_
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
#include <FE/prerequisites.h>

// E, C, and S
#include <FE/framework/archetype_base.hpp>
#include <FE/framework/component_base.hpp>
#include <FE/framework/system.hpp>

// ECS smart pointer
#include <FE/framework/smart_ptr.hxx>

#include <FE/framework/framework.hpp>

// FE::reflection::type_id<Archetype>()
#include <FE/framework/type_info.hpp>

// game memory pool
#include <FE/pool/memory_resource.hpp>

// ECS data structures
#include <FE/farray.hxx>
#include <FE/hash.hpp>
#include <forward_list>
#include <robin_hood.h>
#include <vector>

#include <boost/fiber/recursive_mutex.hpp>




BEGIN_NAMESPACE(FE::framework)


class processors;

using initializer = robin_hood::unordered_map<std::pmr::string, std::pmr::string>;
using initializer_list = robin_hood::unordered_map<std::pmr::string, initializer>;
using system_table_initializer_list = robin_hood::unordered_map<std::pmr::string, std::pmr::string>; // key: system function name, value: component type names; e.g. "TransformComponent,RenderComponent".


class ECS
{
	friend class processors;

	using archetype_table = robin_hood::unordered_map<std::pmr::string, archetype, FE::hash<std::pmr::string>>;
	using component_table = robin_hood::unordered_map<	std::size_t, // the robin hood hash map uses lighter hashing algorithm for integers, than objects.
														FE::pair<	FE::scalable_pool<FE::align_16bytes>,
																	std::pmr::forward_list<FE::internal::ECS::components>
																	>
														>;
	using system_table = robin_hood::unordered_map< std::pmr::string,
													FE::pair<FE::system, std::pmr::vector<std::size_t>> 
													>;

	std::pmr::memory_resource* m_memory_resource;
	FE::scalable_pool<FE::align_16bytes> m_archetype_pool;

	archetype_table m_archetype_table;
	component_table m_component_table;
	system_table m_system_table;

	framework::initializer_list m_archetype_default_entities;
	std::pmr::string m_buffer;
	boost::fibers::recursive_mutex m_fiber_lock;

public:
	ECS(framework::initializer_list& initializer_list_p, framework::system_table_initializer_list& system_table_initializer_p, std::pmr::memory_resource* resource_p) noexcept;
	~ECS() noexcept = default;

	ECS(const ECS&) noexcept = delete;
	ECS& operator=(const ECS&) noexcept = delete;

	
	template <class Archetype, typename ...Arguments>
	FE::entity<Archetype> instanciate_entity(FE::ASCII* const entity_name_p, Arguments&& ...arguments_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		FE::archetype l_alloc_result = FE::make_owner<Archetype>( &m_archetype_pool, std::forward<Arguments>(arguments_p)... );
		l_alloc_result->m_name = std::pmr::string( m_memory_resource );
		l_alloc_result->m_name.reserve( std::strlen(FE::framework::reflection::type_id<Archetype>().name()) + 1 + std::strlen(entity_name_p) );

		l_alloc_result->m_name = FE::framework::reflection::type_id<Archetype>().name();
		l_alloc_result->m_name += entity_name_p;
		std::pair<typename archetype_table::iterator, bool> l_result = m_archetype_table.emplace( l_alloc_result->m_name, std::move(l_alloc_result) );
		
		if (l_result.second == true) // The emplace() was successful. 
		{
			return FE::downcast_owner_to_observer<Archetype>(l_result.first->second);
		}

		return entity<Archetype>();
	}

	template <class Archetype>
	FE::entity<FE::archetype_base> instanciate_entity_from_initializer(FE::ASCII* const entity_name_p, FE::framework::initializer& serialized_entity_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		FE::entity<FE::archetype_base> l_entity = ECS::instanciate_entity<Archetype>(entity_name_p);
		if (l_entity.is_valid() == false)
		{
			return entity<FE::archetype_base>();
		}

		for (const auto& [component_identifier, serialized_component] : serialized_entity_p)
		{
			constexpr FE::ASCII* l_class = "class";
			constexpr FE::ASCII* l_struct = "struct";

			m_buffer = component_identifier;
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

			FE::task_base* l_component_adder = FE::framework::framework_base::get_framework().get_method_reflection().retrieve(m_buffer);
			FE_ASSERT(l_component_adder != nullptr, "Assertion failed: the component adder function is not found. The component type may not be registered.");

			FE::component_view<FE::component_base> l_handle;
			FE::arguments<FE::entity<FE::archetype_base>> l_arguments;
			l_arguments._first = l_entity;
			(*l_component_adder)(this, &l_handle, &l_arguments);
		}
		deserialize_entity(serialized_entity_p, l_entity);
		return l_entity;
	}

	template <class Archetype>
	FE::entity<FE::archetype_base> instanciate_default_entity(FE::ASCII* const entity_name_p) noexcept
	{
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		framework::initializer* l_default_values = ECS::get_archetype_default_entity<Archetype>();
		if (l_default_values == nullptr)
		{
			return FE::entity<FE::archetype_base>{};
		}
		return ECS::instanciate_entity_from_initializer<Archetype>(entity_name_p, *l_default_values);
	}

	void destruct_entity(FE::entity<archetype_base> entt_p) noexcept;


	template <class Archetype>
	_FE_FORCE_INLINE_ void set_archetype_default_entity(FE::framework::initializer& default_values_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		m_archetype_default_entities[ FE::framework::reflection::type_id<Archetype>().name() ] = default_values_p;
	}

	template <class Archetype>
	_FE_FORCE_INLINE_ FE::framework::initializer* const get_archetype_default_entity() noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		typename initializer_list::iterator l_probe_result = m_archetype_default_entities.find( FE::framework::reflection::type_id<Archetype>().name() );
		if (l_probe_result != m_archetype_default_entities.end())
		{
			return &(l_probe_result->second);
		}
		return nullptr;
	}


	template <class Archetype>
	FE::entity<Archetype> find_entity(FE::ASCII* const entity_name_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::archetype_base, Archetype>, "Static assertion failed: the template argument Archetype must be derived from FE::archetype_base.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		m_buffer.reserve( std::strlen( FE::framework::reflection::type_id<Archetype>().name() ) + std::strlen(entity_name_p) );
		m_buffer = FE::framework::reflection::type_id<Archetype>().name();
		m_buffer += entity_name_p;

		typename archetype_table::iterator l_probe_result = m_archetype_table.find(m_buffer);
		m_buffer.clear();
		if (l_probe_result != m_archetype_table.end())
		{
			return FE::downcast_owner_to_observer<Archetype>(l_probe_result->second);
		}

		return entity<Archetype>();
	}


	template <class Component, typename ...Arguments>
	FE::component_view<Component> add_component(FE::entity<archetype_base> entt_p, Arguments&& ...arguments_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");
		FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

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
			typename component_table::mapped_type& l_list_and_allocator = m_component_table[FE::framework::reflection::type_id<Component>().hash_code()];
			l_list_and_allocator._second = std::pmr::forward_list<FE::internal::ECS::components>(&l_list_and_allocator._first);
			l_list_and_allocator._second.emplace_front(); // allocate a component pool.
			l_probe_result = m_component_table.find(FE::framework::reflection::type_id<Component>().hash_code());
		}


		FE::component l_alloc_result;
		for (typename component_table::mapped_type::second_type::iterator components = l_probe_result->second._second.begin(); components != l_probe_result->second._second.end(); ++components)
		{
			if (components->get_size() == components->max_components) // true if the component pool is full.
			{
				continue;
			}

			l_alloc_result = FE::make_owner<Component>(&(l_probe_result->second._first), std::forward<Arguments>(arguments_p)...);
			l_alloc_result->m_metadata->_typename = FE::framework::reflection::type_id<Component>().name();
			l_alloc_result->m_metadata->_memory_layout_version = "default"; // modify the value when the memory layout of the component changes; this ensures correct auto serialization.
			FE::component_view<Component> l_view = FE::downcast_owner_to_observer<Component>(l_alloc_result);

			auto l_result = entt_p->m_component_view_table.emplace(FE::framework::reflection::type_id<Component>().hash_code(), l_view);
			if (l_result.second == false)
			{
				FE_LOG(FE::log::Severity::_Warning, "FE ECS: add_component<T>() failed due to a pre-existing duplicate component.");
				return component_view<Component>();
			}

			FE::size l_idx = components->add_component(std::move(l_alloc_result));
			l_view->m_metadata->_group = components;
			l_view->m_metadata->_index = l_idx;
			return l_view;
		}


		// could not find a component pool with free space, allocate a new component pool!
		if (l_alloc_result == nullptr) _FE_LIKELY_
		{
			// All components lists are full. Create a new one.
			l_probe_result->second._second.emplace_front();

			l_alloc_result = FE::make_owner<Component>(&(l_probe_result->second._first), std::forward<Arguments>(arguments_p)...);
			l_alloc_result->m_metadata->_typename = FE::framework::reflection::type_id<Component>().name();
			l_alloc_result->m_metadata->_memory_layout_version = "default"; // modify the value when the memory layout of the component changes; this ensures correct auto serialization.
			FE::component_view<Component> l_view = FE::downcast_owner_to_observer<Component>(l_alloc_result);

			auto l_result = entt_p->m_component_view_table.emplace(FE::framework::reflection::type_id<Component>().hash_code(), l_view);
			if (l_result.second == false)
			{
				FE_LOG(FE::log::Severity::_Warning, "FE ECS: add_component<T>() failed due to a pre-existing duplicate component.");
				return component_view<Component>();
			}

			FE::size l_idx = l_probe_result->second._second.front().add_component(std::move(l_alloc_result));
			l_view->m_metadata->_group = l_probe_result->second._second.begin();
			l_view->m_metadata->_index = l_idx;
			return l_view;
		}

		return component_view<Component>();
	}

	template <class Component>
	_FE_FORCE_INLINE_ FE::component_view<FE::component_base> instanciate_component(FE::entity<archetype_base> entt_p) noexcept
	{
		return add_component<Component>(entt_p);
	}

	template <class Component>
	void remove_component(FE::entity<archetype_base> entt_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");
		FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		typename archetype_base::component_view_table::iterator l_view_table_probe_result = entt_p->m_component_view_table.find(FE::framework::reflection::type_id<Component>().hash_code());

		if (l_view_table_probe_result == entt_p->m_component_view_table.end())
		{
			return; // The entity does not have this component.
		}

		typename component_table::iterator l_com_table_probe_result = m_component_table.find(FE::framework::reflection::type_id<Component>().hash_code());
		FE_ASSERT(l_com_table_probe_result != m_component_table.end(), "Assertion failed: the component table must have this component.");

		// Remove the component from the component table.
		l_view_table_probe_result->second->m_metadata->_group->remove_component(l_view_table_probe_result->second->m_metadata->_index);

		// Remove the component from the entity's component view table.
		entt_p->m_component_view_table.erase(l_view_table_probe_result);
	}


	void attatch_component(FE::entity<archetype_base> entt_p, const FE::component_view<component_base>& to_attatch_p) noexcept;

	template <class Component>
	FE::component_view<Component> detach_component(FE::entity<archetype_base> entt_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::component_base, Component>, "Static assertion failed: the template argument Component must be derived from FE::component_base.");
		FE_ASSERT(entt_p.is_valid() == true, "Assertion failed: the entity is not valid.");
		std::lock_guard<boost::fibers::recursive_mutex> l_lock(m_fiber_lock);

		typename FE::archetype_base::component_view_table::iterator l_probe_result = entt_p->m_component_view_table.find( FE::framework::reflection::type_id<Component>().hash_code() );
		FE_ASSERT(l_probe_result != entt_p->m_component_view_table.end(), "Assertion failed: the entity must have this component.");

		entt_p->m_component_view_table.erase(l_probe_result);
		return FE::downcast_observer<Component>(l_probe_result->second);
	}


	std::optional<FE::pair<FE::system, std::pmr::vector<std::size_t>>> find_system(FE::ASCII* const system_name_p) noexcept;


	FE::framework::initializer serialize_entity(FE::entity<archetype_base> entt_p) noexcept;
	void deserialize_entity(FE::framework::initializer& serialized_components_p, FE::entity<archetype_base> out_entt_p) noexcept;
};


END_NAMESPACE
#endif