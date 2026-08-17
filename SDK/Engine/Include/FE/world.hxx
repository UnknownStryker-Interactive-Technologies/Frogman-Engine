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
    using registry = entt::basic_registry<internal::entity, std::pmr::polymorphic_allocator<internal::entity>>;

    registry m_registry;
    world_tag_t m_world_tag;
	var::float64 m_delta_time;
	FE::queue<void(*)(class ::FE::world&), std::pmr::polymorphic_allocator<void(*)(class ::FE::world&)>> m_deferred_ecs_syscall_queue;

public:
    world(world_tag_t world_tag_p) noexcept;
    ~world() noexcept;

    world(const world&) = delete;
    world& operator=(const world&) = delete;

public:
    _FE_FORCE_INLINE_ FE::float64 delta_milliseconds() const noexcept { return m_delta_time; }
	constexpr FE::float64 fixed_physics_delta_milliseconds() const noexcept { return 1000.0 / 60.0; }

	void __set_delta_time(const auth&, FE::float64 delta_time_p) noexcept;
    registry& __get_registry(const auth&) noexcept;

    FE::entity spawn_entity();
    void despawn_entity(FE::entity entity_p);
    bool is_valid(FE::entity entity_p) const noexcept;

    template <typename T, typename... Arguments>
    T& add_component(FE::entity entity_p, Arguments&&... arguments_p) noexcept
    {

    }

    template <typename T>
    T& get_component(FE::entity entity_p) noexcept
    {

    }
    template <typename... T>
    auto get_components(FE::entity entity_p) noexcept
    {

    }
    template <typename... T>
    auto get_components() noexcept
    {

    }

    template <typename T>
    void remove_component(FE::entity entity_p) noexcept
    {

    }
    template <typename... T>
    void remove_components(FE::entity entity_p) noexcept
    {

    }

    template <typename T>  
    FE::boolean has_component(FE::entity entity_p) const noexcept
    {

    }
    template <typename... T>
    FE::boolean has_components(FE::entity entity_p) const noexcept
    {

    }

	world_tag_t get_world_tag() const noexcept;

    static void create_world(world_tag_t world_tag_p) noexcept;
	static void enter_world(world_tag_t world_tag_p) noexcept;
};


END_NAMESPACE
#endif