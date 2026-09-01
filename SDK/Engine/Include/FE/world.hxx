#ifndef _FROGMAN_ENGINE_WORLD_HPP_
#define _FROGMAN_ENGINE_WORLD_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

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
#include <FE/prerequisites.hxx>
#include <FE/memory.hpp>
#include <FE/queue.hxx>

#include <FE/game.hxx>
#include <FE/entity.hpp>

#include <absl/container/flat_hash_map.h>




BEGIN_NAMESPACE(FE)

class engine;

class world final
{
public:
    class auth
    {
        friend class FE::engine;

        constexpr auth() noexcept {};
        constexpr ~auth() noexcept {};

        constexpr auth(auth&&) noexcept {};
        constexpr auth(const auth&) noexcept {};
    };

private:
    using registry = entt::basic_registry<FE::entity, std::pmr::polymorphic_allocator<FE::entity>>;
	using system = void(*)(class ::FE::world&);

    registry m_registry;
    world_tag_t m_world_tag;
	var::float64 m_delta_time;
	FE::queue<system> m_deferred_ecs_syscall_queue;

	absl::flat_hash_map<std::pmr::string, FE::entity, 
        absl::DefaultHashContainerHash<std::pmr::string>, 
        absl::DefaultHashContainerEq<std::pmr::string>, 
        std::pmr::polymorphic_allocator<std::pair<const std::pmr::string, FE::entity>>> m_tagged_entities;

public:
    world(world_tag_t world_tag_p) noexcept;
    ~world() noexcept;

    world(world&& other_p) noexcept;
    world& operator=(world&& other_p) noexcept;

    world(const world&) = delete;
    world& operator=(const world&) = delete;

public:
    _FE_FORCE_INLINE_ FE::float64 delta_milliseconds() const noexcept { return m_delta_time; }
	constexpr FE::float64 fixed_physics_delta_milliseconds() const noexcept { return 1000.0 / 60.0; }

	void set_delta_time(const auth&, FE::float64 delta_time_p) noexcept;
    registry& get_registry(const auth&) noexcept;

    FE::entity spawn_entity(FE::ASCII* const tag_p = NULL) noexcept;
    std::optional<FE::entity> find_entity(FE::ASCII* const tag_p) const noexcept;
    void despawn_entity(FE::entity entity_p) noexcept;
    FE::boolean is_valid(FE::entity entity_p) const noexcept;

    template <typename T, typename... Arguments>
    T& add_component(FE::entity entity_p, Arguments&&... arguments_p) noexcept
    {
        FE_ASSERT( m_registry.all_of<T>( entity_p ) == false, "Component of type T already exists on the entity.");
        return m_registry.emplace(entity_p, std::forward<Arguments>(arguments_p)...);
    }

    template <typename T>
    T& get_component(FE::entity entity_p) noexcept
    {
        FE_ASSERT(m_registry.all_of<T>(entity_p) == true, "The component type is not found on the entity.");
        return m_registry.get<T>(entity_p);
    }
    template <typename... ComponentTypes>
    auto get_components(FE::entity entity_p) noexcept
    {
        FE_ASSERT(m_registry.all_of<ComponentTypes...>(entity_p) == true, "The listed component types are not found on the entity.");
        return m_registry.get<ComponentTypes...>(entity_p);
    }

    template <typename... ComponentTypes>
    auto get_components_view() noexcept
    {
		return m_registry.view<ComponentTypes...>();
    }

    template <typename T>
    void remove_component(FE::entity entity_p) noexcept
    {
        FE_ASSERT(m_registry.all_of<T>(entity_p) == true, "The component type is not found on the entity.");
		m_registry.remove<T>(entity_p);
    }
    template <typename... ComponentTypes>
    void remove_components(FE::entity entity_p) noexcept
    {
        FE_ASSERT(m_registry.all_of<ComponentTypes...>(entity_p) == true, "The listed component types are not found on the entity.");
		m_registry.remove<ComponentTypes...>(entity_p);
    }

    template <typename T>  
    FE::boolean has_component(FE::entity entity_p) const noexcept
    {
        return m_registry.all_of<T>( entity_p );
    }
    template <typename... ComponentTypes>
    FE::boolean has_components(FE::entity entity_p) const noexcept
    {
        return m_registry.all_of<ComponentTypes...>( entity_p );
    }

	_FE_FORCE_INLINE_ world_tag_t get_world_tag() const noexcept { return m_world_tag; }

    //template <typename... ComponentTypes>
    //std::pmr::string serialize_entity(FE::entity entity_p) const noexcept
    //{
    //}
    //template <typename... ComponentTypes>
    //FE::entity deserialize_entity(const std::pmr::string& serialized_entity_p) noexcept
    //{
    //}

    static void create_world(world_tag_t world_tag_p) noexcept;
	static void enter_world(world_tag_t world_tag_p) noexcept;
};


END_NAMESPACE
#endif