// Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <memory>
#include <utility>

#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\SystemParseTest.hpp>




template <typename T, typename... Args>
T* construct(T* location_p, Args&&... args_p)
{
    new(location_p) T( ::std::forward<Args>(args_p)... );
    return location_p;
}


template <typename T>
void destruct(T* ptr_p)
{
    if (ptr_p == nullptr) return;
    ptr_p->~T();
}




void load_reflection_data()
{
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::cycle_a);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldDefaultEntityInitialization, &::Game::spawn_persistent_actors);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::refresh_transform_cache);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::cycle_b);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::rebuild_spatial_hash);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_PostPhysics, &::Game::write_back_transforms);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::prepare_animation_inputs);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldTick, &::Game::tick_weather);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::finalize_pre_entity_tick);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::system_a);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_EndPhysics, &::Game::resolve_contact_events);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::system_b);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::system_c);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::system_d);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::system_e);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldInitialization, &::Game::build_navmesh);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::MainMenu, FE::_WorldBegin, &::Game::fade_in_title_ui);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldBegin, &::Game::seed_encounter_rng);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreWorldTick, &::Game::stream_in_nearby_cells);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldTick, &::Game::tick_arena_hazards);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(EditorWorldTag::Viewport, FE::_WorldTick, &::Game::tick_editor_gizmos);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostWorldTick, &::Game::evict_far_cells);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::WorldSystemCallPhase::_EntityTick, &::Game::run_behaviour_trees);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostEntityTick, &::Game::apply_deferred_despawns);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreRenderQueueCommit, &::Game::cull_and_sort_draw_calls);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_PrePhysics, &::Game::sync_kinematic_bodies);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_StartPhysics, &::Game::begin_broadphase);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, g_physics_phase, &::Game::integrate_rigid_bodies);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostUpdateWork, &::Game::collect_profiling_counters);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostRenderQueueCommit, &::Game::submit_debug_lines);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldEnd, &::Game::tally_battle_results);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldDefaultEntityDeinitialization, &::Game::destroy_persistent_actors);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldDeinitialization, &::Game::release_navmesh);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_global_system(FE::_EngineInitialization, &::Game::boot_memory_arenas);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_global_system(FE::_GameInstanceInitialization, &::Game::load_player_profile);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_global_system(FE::_GameInstanceDeinitialization, &::Game::save_player_profile);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_global_system(FE::_PostGameInstanceTick, &::Game::flush_input_edge_flags);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_global_system(FE::_GameInstanceBegin, &::Game::play_intro_cinematic);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_global_system(FE::_PreGameInstanceTick, &::Game::poll_input_devices);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_global_system(FE::_GameInstanceTick, &::Game::advance_save_timer);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_global_system(FE::_GameInstanceEnd, &::Game::write_session_log);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_global_system(FE::_EngineDeinitialization, &::Game::teardown_memory_arenas);
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PreEntityTick,
        {
            {
                ._current_system = ::Game::cycle_a,
                ._runs_before = { ::Game::cycle_b },
                ._runs_after = { }
            },
            {
                ._current_system = ::Game::refresh_transform_cache,
                ._runs_before = { ::Game::rebuild_spatial_hash },
                ._runs_after = { }
            },
            {
                ._current_system = ::Game::cycle_b,
                ._runs_before = { ::Game::cycle_a },
                ._runs_after = { }
            },
            {
                ._current_system = ::Game::rebuild_spatial_hash,
                ._runs_before = { ::Game::finalize_pre_entity_tick },
                ._runs_after = { }
            },
            {
                ._current_system = ::Game::prepare_animation_inputs,
                ._runs_before = { },
                ._runs_after = { ::Game::refresh_transform_cache }
            },
            {
                ._current_system = ::Game::finalize_pre_entity_tick,
                ._runs_before = { },
                ._runs_after = { ::Game::prepare_animation_inputs }
            },
            {
                ._current_system = ::Game::system_a,
                ._runs_before = { ::Game::system_c, ::Game::system_d },
                ._runs_after = { }
            },
            {
                ._current_system = ::Game::system_b,
                ._runs_before = { ::Game::system_c, ::Game::system_d },
                ._runs_after = { }
            },
            {
                ._current_system = ::Game::system_e,
                ._runs_before = { },
                ._runs_after = { ::Game::system_a, ::Game::system_b }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_WorldDefaultEntityInitialization,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_WorldInitialization,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PostWorldTick,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_WorldTick,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PreWorldTick,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::WorldSystemCallPhase::_EntityTick,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PostEntityTick,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PreRenderQueueCommit,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PostUpdateWork,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PostRenderQueueCommit,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_WorldDeinitialization,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_WorldDefaultEntityDeinitialization,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_global_systems_topologically(FE::_GameInstanceDeinitialization,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_global_systems_topologically(FE::_EngineInitialization,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_global_systems_topologically(FE::_GameInstanceInitialization,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_global_systems_topologically(FE::_GameInstanceTick,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_global_systems_topologically(FE::_PostGameInstanceTick,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_global_systems_topologically(FE::_GameInstanceEnd,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_global_systems_topologically(FE::_GameInstanceBegin,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_global_systems_topologically(FE::_PreGameInstanceTick,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_global_systems_topologically(FE::_EngineDeinitialization,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_StartPhysics,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_PostPhysics,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_PrePhysics,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_EndPhysics,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_WorldEnd,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_WorldBegin,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, g_physics_phase,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_WorldTick,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::MainMenu, FE::_WorldBegin,
        {
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(EditorWorldTag::Viewport, FE::_WorldTick,
        {
        }
    );
}
