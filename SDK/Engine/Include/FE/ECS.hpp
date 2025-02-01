#ifndef _FE_ECS_HPP_
#define _FE_ECS_HPP_
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
#include <FE/pair.hxx>
#include <FE/pool/block_pool.hxx>
#include <FE/pool/scalable_pool.hxx>

#include <FE/framework/reflection.hpp>

#include <array>
#include <list>
#include <memory> // std::shared_ptr, and std::weak_ptr
#include <memory_resource> // std::pmr::polymorphic_allocator
#include <utility> // std::forward
#include <vector>

// robin_hood::unordered_map
#include <robin_hood.h>




BEGIN_NAMESPACE(FE)


class game_engine;
class archetype_base;
class component_base;
class system_base;

template <class Archetype>
using entity = std::weak_ptr<Archetype>;

template <class Component>
using component_ptr = std::weak_ptr<Component>;

template <class System>
using system = FE::pair<std::shared_ptr<System>, std::pmr::vector< std::weak_ptr<component_base> >>;


class ECS
{
public:
    template <class Archetype>
    static std::shared_ptr<Archetype> nullntt;

    template <class Component>
    static std::shared_ptr<Component> nullcom;

	using upstream_arena_t = FE::scalable_pool<FE::PoolPageCapacity::_Max>;
	using entity_arena_t = FE::scalable_pool<FE::PoolPageCapacity::_64MB>;
	using component_arena_t = FE::scalable_pool<FE::PoolPageCapacity::_256MB>;
    using component_pool_t = FE::scalable_pool<FE::PoolPageCapacity::_256KB>;
	using system_arena_t = FE::scalable_pool<FE::PoolPageCapacity::_16MB>;

private:
    upstream_arena_t m_upstream_arena;
    entity_arena_t m_entity_arena;
    component_arena_t m_component_arena;
	robin_hood::unordered_map<std::pmr::string, component_pool_t> m_component_pool;
    system_arena_t m_system_arena;

    robin_hood::unordered_map<std::pmr::string, std::shared_ptr<archetype_base>> m_entity_table;
    robin_hood::unordered_map<archetype_base*, 
                              robin_hood::unordered_map<std::pmr::string, 
                                                        std::shared_ptr<component_base>>> m_component_table;
	robin_hood::unordered_map<std::pmr::string, system<system_base>> m_system_table;

public:
	ECS() noexcept;
	~ECS() noexcept;


	template <class Archetype, typename ...Arguments>
    _FE_FORCE_INLINE_ entity<Archetype> create_entity(FE::ASCII* const tag_p, Arguments&&... arguments_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, Archetype>), "Static assertion failed: the 'class Archetype' is not an archetype.");

        std::pmr::string l_identifier(tag_p, FE::game_engine::get_game_engine().get_memory_resource());
        if (this->m_entity_table.find(l_identifier)->first == l_identifier)
        {
			FE_LOG("Warning, create_entity() failed: the entity tag '${%s@0}' is already in use.", tag_p);
			return ECS::nullntt<Archetype>;
        }

        std::shared_ptr<Archetype> l_infant = std::allocate_shared<Archetype, std::pmr::polymorphic_allocator<Archetype>>(&m_entity_arena, std::forward<Arguments>(arguments_p)...);
        this->m_entity_table.emplace(std::move(l_identifier), l_infant);
		return l_infant;
    }

    template <class Archetype>
    _FE_FORCE_INLINE_ void destroy_entity(const entity<Archetype>& entity_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, Archetype>), "Static assertion failed: the 'class Archetype' is not an archetype.");
		entity_p.lock().reset();
    }

    template <class Archetype>
    _FE_FORCE_INLINE_ entity<Archetype> get_entity(FE::ASCII* const tag_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, Archetype>), "Static assertion failed: the 'class Archetype' is not an archetype.");

        std::pmr::string l_identifier(tag_p, FE::game_engine::get_game_engine().get_memory_resource());
        for (auto it = m_entity_table.find(l_identifier), end = m_entity_table.end(); it != end; ++it)
        {
            if (it->first == l_identifier)
            {
                return entity<Archetype>(it->second);
            }
        }
        return ECS::nullntt<Archetype>;
    }

	std::pmr::vector< FE::pair<std::string_view, std::weak_ptr<archetype_base> >> list_known_entities() noexcept;


    template <class Archetype, class Component>
    component_ptr<Component> request_component_attachment(Archetype* const entity_p) noexcept
    {
		FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, Archetype>), "Static assertion failed: the 'class Archetype' is not an archetype.");
		FE_STATIC_ASSERT((std::is_base_of_v<component_base, Component>), "Static assertion failed: the 'class Component' is not a component.");

        if (nullptr == entity_p)
        {
			FE_LOG("Warning, request_component_attachment() failed: the entity is not valid.");
			return ECS::nullcom<Component>;
        }

        for (auto archetype_iter = m_component_table.find(entity_p); archetype_iter != m_component_table.end(); ++archetype_iter)
        {
			if (archetype_iter->first == entity_p)
			{
                std::pmr::string l_identifier(framework::reflection::type_id<Component>().name(), FE::game_engine::get_game_engine().get_memory_resource());
                
				auto l_component_pool_iter = this->m_component_pool.find(l_identifier);
                if (l_component_pool_iter == m_component_pool.end())
                {
                    l_component_pool_iter = this->m_component_pool.emplace(l_identifier, &m_component_arena).first;
                }

                auto l_component_it = archetype_iter->second.find(l_identifier);
                if (l_component_it == archetype_iter->second.end())
                {
                    std::shared_ptr<Component> l_infant = std::allocate_shared<Component, std::pmr::polymorphic_allocator<Component>>(&(l_component_pool_iter->second));
                    archetype_iter->second.emplace(std::move(l_identifier), l_infant);
                    return l_infant;
                }

                FE_LOG("Warning, request_component_attachment() failed: the component already exists within the archetype named '${%s@0}.'", framework::reflection::type_id<Archetype>().name());
                return ECS::nullcom<Component>;
			}
        }

        FE_LOG("Warning, request_component_attachment() failed: the component already exists within the archetype named '${%s@0}.'", framework::reflection::type_id<Archetype>().name());
        return ECS::nullcom<Component>;
    }

    template <class Archetype, class Component>
    void request_component_detachment(Archetype* const entity_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, Archetype>), "Static assertion failed: the 'class Archetype' is not an archetype.");
        FE_STATIC_ASSERT((std::is_base_of_v<component_base, Component>), "Static assertion failed: the 'class Component' is not a component.");

        if (nullptr == entity_p)
        {
            FE_LOG("Warning, request_component_detachment() failed: the entity is not valid.");
            return;
        }

        for (auto archetype_iter = m_component_table.find(entity_p); archetype_iter != m_component_table.end(); ++archetype_iter)
        {
            if (archetype_iter->first == entity_p)
            {
                std::pmr::string l_identifier(framework::reflection::type_id<Component>().name(), FE::game_engine::get_game_engine().get_memory_resource());
                auto l_component_it = archetype_iter->second.find(l_identifier);

                if (l_component_it != archetype_iter->second.end())
                {
					archetype_iter->second.erase(l_identifier);
                    return;
                }

                FE_LOG("Warning, request_component_detachment() failed: the component does not exist within the archetype named '${%s@0}.'", framework::reflection::type_id<Archetype>().name());
                return;
            }
        }

        FE_LOG("Warning, request_component_detachment() failed: the component does not exist within the archetype named '${%s@0}.'", framework::reflection::type_id<Archetype>().name());
    }

    template <class Archetype, class Component>
    component_ptr<Component> request_component_retrieval(Archetype* const entity_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, Archetype>), "Static assertion failed: the 'class Archetype' is not an archetype.");
        FE_STATIC_ASSERT((std::is_base_of_v<component_base, Component>), "Static assertion failed: the 'class Component' is not a component.");

        if (nullptr == entity_p)
        {
            FE_LOG("Warning, request_component_retrieval() failed: the entity is not valid.");
            return ECS::nullcom<Component>;
        }

        for (auto archetype_iter = m_component_table.find(entity_p); archetype_iter != m_component_table.end(); ++archetype_iter)
        {
            if (archetype_iter->first == entity_p)
            {
                std::pmr::string l_identifier(framework::reflection::type_id<Component>().name(), FE::game_engine::get_game_engine().get_memory_resource());
                auto l_component_iter = archetype_iter->second.find(l_identifier);

                if (l_component_iter != archetype_iter->second.end())
                {
                    return l_component_iter->second;
                }

                FE_LOG("Warning, request_component_retrieval() failed: the component does not exist within the archetype named '${%s@0}.'", framework::reflection::type_id<Archetype>().name());
                return ECS::nullcom<Component>;
            }
        }

        FE_LOG("Warning, request_component_retrieval() failed: the component does not exist within the archetype named '${%s@0}.'", framework::reflection::type_id<Archetype>().name());
        return ECS::nullcom<Component>;
    }

    std::pmr::vector< FE::pair<std::string_view, std::weak_ptr<component_base> >> list_known_components() noexcept;


    template <class System>
	void register_system() noexcept
	{
		FE_STATIC_ASSERT((std::is_base_of_v<system_base, System>), "Static assertion failed: the 'class System' is not a system.");
		
        std::pmr::string l_identifier(framework::reflection::type_id<System>().name(), FE::game_engine::get_game_engine().get_memory_resource());
        if (this->m_entity_table.find(l_identifier)->first == l_identifier)
        {
            FE_LOG("Warning, register_system() failed: the system named '${%s@0}' already exists.", l_identifier.c_str());
            return;
        }

        std::shared_ptr<System> l_infant = std::allocate_shared<System, std::pmr::polymorphic_allocator<System>>(&m_system_arena);
        this->m_entity_table.emplace(std::move(l_identifier), std::move(l_infant));
	}

    template <class System>
    std::optional<system<System>> request_system() noexcept
    {
		FE_STATIC_ASSERT((std::is_base_of_v<system_base, System>), "Static assertion failed: the 'class System' is not a system.");
		// system table

		return std::nullopt;
    }

	template <class Dependancy, class System>
    FE::boolean is_system_dependant_on() noexcept
    {
        FE_STATIC_ASSERT((std::is_base_of_v<system_base, System>), "Static assertion failed: the 'class System' is not a system.");
        FE_STATIC_ASSERT((std::is_base_of_v<system_base, Dependancy>), "Static assertion failed: the 'class Dependancy' is not a system.");
        // system table
    }

	template <class System, class Dependancy>
	void build_system_dependancy_on() noexcept
	{
		FE_STATIC_ASSERT((std::is_base_of_v<system_base, System>), "Static assertion failed: the 'class System' is not a system.");
		FE_STATIC_ASSERT((std::is_base_of_v<system_base, Dependancy>), "Static assertion failed: the 'class Dependancy' is not a system.");
		// system table
	}

	template <class System, class Dependancy>
	void remove_system_dependancy_from() noexcept
	{
		FE_STATIC_ASSERT((std::is_base_of_v<system_base, System>), "Static assertion failed: the 'class System' is not a system.");
		FE_STATIC_ASSERT((std::is_base_of_v<system_base, Dependancy>), "Static assertion failed: the 'class Dependancy' is not a system.");
		// system table
	}

    std::pmr::vector< std::string_view > list_known_systems() noexcept;
};


template <class Archetype>
std::shared_ptr<Archetype> ECS::nullntt{ nullptr };

template <class Component>
std::shared_ptr<Component> ECS::nullcom{ nullptr };


END_NAMESPACE
#endif