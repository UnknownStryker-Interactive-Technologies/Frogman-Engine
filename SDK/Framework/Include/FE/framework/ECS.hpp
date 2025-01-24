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

#include <FE/framework/archetype_base.hpp>




BEGIN_NAMESPACE(FE::framework)


class ECS
{
public:
    template <typename T>
    using entity = std::weak_ptr<T>;

    template <typename T>
    static entity<T> nullntt;

private:
    std::pmr::vector<FE::scalable_pool<FE::PoolPageCapacity::_256MB>> m_archetype_arena;
    std::pmr::vector<FE::scalable_pool<FE::PoolPageCapacity::_256MB>> m_component_arena;
    robin_hood::unordered_map<std::pmr::string, std::shared_ptr<void>> m_entity_table;
    // system table

public:
	ECS() noexcept;
    ECS(FE::size max_concurrency_p) noexcept;
	~ECS() noexcept;

	template <typename T, typename ...Arguments>
    _FE_FORCE_INLINE_ entity<T> create_entity(FE::ASCII* const entity_tag_p, Arguments&&... arguments_p) noexcept
    {
        //FE_STATIC_ASSERT((std::is_base_of_v<archetype_base, T>), "Static assertion failed: the 'typename T' is not an archetype.");
        if (this->m_entity_table.find(entity_tag_p)->first == entity_tag_p)
        {
			FE_LOG("Warning: the entity tag is already in use.");
			return ECS::nullntt<T>;
        }

        std::shared_ptr<T> l_infant = std::allocate_shared<T>(get_archetype_arena(), std::forward<Arguments>(arguments_p)...);
        this->m_entity_table.emplace(entity_tag_p, l_infant);
		return l_infant;
    }

    template <typename T>
    _FE_FORCE_INLINE_ void destroy_entity(const entity<T>& entity_p) noexcept
    {
		entity_p.lock().reset();
    }

    template <typename T>
    entity<T> get_entity(FE::ASCII* const entity_tag_p) noexcept
    {
        for (auto it = m_entity_table.find(entity_tag_p), end = m_entity_table.end(); it != end; ++it)
        {
            if (it->first == entity_tag_p)
            {
                return entity<T>(it->second);
            }
        }
        return ECS::nullntt<T>;
    }

	// list_known_entities();

	// create component();
	// destroy component();
	// list_known_components();

	// register system<class System>();
    // is_system_dependant_on<class Dependancy, class System>();
	// set_system_dependancy_on<class System, class Dependancy>();
	// remove_system_dependancy_from<class System, class Dependancy>();
	// list_known_systems();

private:
	FE::scalable_pool<FE::PoolPageCapacity::_256MB>& get_archetype_arena() noexcept;
    FE::scalable_pool<FE::PoolPageCapacity::_256MB>& get_component_arena() noexcept;
};


END_NAMESPACE
#endif