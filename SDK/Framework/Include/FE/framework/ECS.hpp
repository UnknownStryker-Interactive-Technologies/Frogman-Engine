#ifndef _FE_FRAMEWORK_ECS_HPP_
#define _FE_FRAMEWORK_ECS_HPP_
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

#include <array>
#include <list>
#include <memory> // std::shared_ptr, and std::weak_ptr
#include <memory_resource> // std::pmr::polymorphic_allocator
#include <utility> // std::forward
#include <vector>

// robin_hood::unordered_map
#include <robin_hood.h>




BEGIN_NAMESPACE(FE::framework)


class framework_base;
class archetype_base;
class component_base;
class system_base;


class ECS
{
public:
    template <class Archetype>
    using entity = std::weak_ptr<Archetype>;

    template <class Archetype>
    static entity<Archetype> nullntt;

private:
    std::pmr::vector<FE::scalable_pool<FE::PoolPageCapacity::_256MB>> m_arena;
    robin_hood::unordered_map<std::pmr::string, std::shared_ptr<archetype_base>> m_entity_table;
    robin_hood::unordered_map<archetype_base*, robin_hood::unordered_map<std::pmr::string, std::shared_ptr<component_base>>> m_component_table;
	robin_hood::unordered_map<std::pmr::string, std::shared_ptr<system_base>> m_system_table;

public:
	ECS() noexcept;
    ECS(FE::size max_concurrency_p) noexcept;
	~ECS() noexcept;


	template <class Archetype, typename ...Arguments>
    _FE_FORCE_INLINE_ entity<Archetype> create_entity(FE::ASCII* tag_p, Arguments&&... arguments_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, Archetype>), "Static assertion failed: the 'class Archetype' is not an archetype.");

        std::pmr::string l_identifier(tag_p, FE::framework::framework_base::get_engine().get_memory_resource());
        if (this->m_entity_table.find(l_identifier)->first == l_identifier)
        {
			FE_LOG("Warning: the entity tag is already in use.");
			return ECS::nullntt<Archetype>;
        }

        std::shared_ptr<Archetype> l_infant = std::allocate_shared<Archetype>(get_arena(), std::forward<Arguments>(arguments_p)...);
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
    _FE_FORCE_INLINE_ entity<Archetype> get_entity(FE::ASCII* tag_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, Archetype>), "Static assertion failed: the 'class Archetype' is not an archetype.");

        std::pmr::string l_identifier(tag_p, FE::framework::framework_base::get_engine().get_memory_resource());
        for (auto it = m_entity_table.find(l_identifier), end = m_entity_table.end(); it != end; ++it)
        {
            if (it->first == l_identifier)
            {
                return entity<Archetype>(it->second);
            }
        }
        return ECS::nullntt<Archetype>;
    }

	std::pmr::vector< FE::pair<std::pmr::string, std::weak_ptr<archetype_base> >> list_known_entities() noexcept;


    template <class Archetype, class Component>
    void request_component_attachment(entity<Archetype> entity_p) noexcept
    {
		FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, Archetype>), "Static assertion failed: the 'class Archetype' is not an archetype.");
		FE_STATIC_ASSERT((std::is_base_of_v<component_base, Component>), "Static assertion failed: the 'class Component' is not a component.");

    }

    template <class Archetype, class Component>
    void request_component_detachment(entity<Archetype> entity_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, Archetype>), "Static assertion failed: the 'class Archetype' is not an archetype.");
        FE_STATIC_ASSERT((std::is_base_of_v<component_base, Component>), "Static assertion failed: the 'class Component' is not a component.");

    }

    template <class Archetype, class Component>
    void request_component_retrieval(entity<Archetype> entity_p) noexcept
    {
        FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, Archetype>), "Static assertion failed: the 'class Archetype' is not an archetype.");
        FE_STATIC_ASSERT((std::is_base_of_v<component_base, Component>), "Static assertion failed: the 'class Component' is not a component.");

    }

    std::pmr::vector< FE::pair<std::pmr::string, std::weak_ptr<component_base> >> list_known_components() noexcept;


    template <class System>
	void register_system() noexcept
	{
		FE_STATIC_ASSERT((std::is_base_of_v<system_base, System>), "Static assertion failed: the 'class System' is not a system.");
		// system table
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

    std::pmr::vector< std::pmr::string > list_known_systems() noexcept;

private:
	FE::scalable_pool<FE::PoolPageCapacity::_256MB>& get_arena() noexcept;
};


END_NAMESPACE
#endif