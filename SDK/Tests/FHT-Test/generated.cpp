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
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreGameInstanceTick, &::Game::poll_input_devices);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldTick, &::Game::tick_weather);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::finalize_pre_entity_tick);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::system_a);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_EndPhysics, &::Game::resolve_contact_events);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::system_b);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::system_c);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_GameInstanceDeinitialization, &::Game::save_player_profile);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::system_d);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::system_e);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_EngineInitialization, &::Game::boot_memory_arenas);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostWorldTick, &::Game::evict_far_cells);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_GameInstanceInitialization, &::Game::load_player_profile);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldInitialization, &::Game::build_navmesh);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::MainMenu, FE::_GameInstanceBegin, &::Game::play_intro_cinematic);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::MainMenu, FE::_WorldBegin, &::Game::fade_in_title_ui);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldBegin, &::Game::seed_encounter_rng);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_GameInstanceTick, &::Game::advance_save_timer);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostGameInstanceTick, &::Game::flush_input_edge_flags);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreWorldTick, &::Game::stream_in_nearby_cells);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldTick, &::Game::tick_arena_hazards);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_EngineDeinitialization, &::Game::teardown_memory_arenas);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(EditorWorldTag::Viewport, FE::_WorldTick, &::Game::tick_editor_gizmos);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::SystemCallPhase::_EntityTick, &::Game::run_behaviour_trees);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostEntityTick, &::Game::apply_deferred_despawns);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreRenderQueueCommit, &::Game::cull_and_sort_draw_calls);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_PrePhysics, &::Game::sync_kinematic_bodies);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_StartPhysics, &::Game::begin_broadphase);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, g_physics_phase, &::Game::integrate_rigid_bodies);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostUpdateWork, &::Game::collect_profiling_counters);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostRenderQueueCommit, &::Game::submit_debug_lines);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldEnd, &::Game::tally_battle_results);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::MainMenu, FE::_GameInstanceEnd, &::Game::write_session_log);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldDefaultEntityDeinitialization, &::Game::destroy_persistent_actors);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldDeinitialization, &::Game::release_navmesh);
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PreEntityTick,
        {
            {
                ._current_system = ::Game::cycle_a,
                ._runs_before = { ::Game::cycle_b },
                ._runs_after = { }
            },
            {
                ._current_system = ::Game::refresh_transform_cache,
                ._runs_before = { },
                ._runs_after = { }
            },
            {
                ._current_system = ::Game::cycle_b,
                ._runs_before = { ::Game::cycle_a },
                ._runs_after = { }
            },
            {
                ._current_system = ::Game::rebuild_spatial_hash,
                ._runs_before = { },
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
                ._current_system = ::Game::system_c,
                ._runs_before = { },
                ._runs_after = { }
            },
            {
                ._current_system = ::Game::system_d,
                ._runs_before = { },
                ._runs_after = { }
            },
            {
                ._current_system = ::Game::system_e,
                ._runs_before = { },
                ._runs_after = { ::Game::system_a, ::Game::system_b }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PostGameInstanceTick,
        {
            {
                ._current_system = ::Game::flush_input_edge_flags,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_GameInstanceTick,
        {
            {
                ._current_system = ::Game::advance_save_timer,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_WorldDefaultEntityInitialization,
        {
            {
                ._current_system = ::Game::spawn_persistent_actors,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_WorldInitialization,
        {
            {
                ._current_system = ::Game::build_navmesh,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PreRenderQueueCommit,
        {
            {
                ._current_system = ::Game::cull_and_sort_draw_calls,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PreGameInstanceTick,
        {
            {
                ._current_system = ::Game::poll_input_devices,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PostWorldTick,
        {
            {
                ._current_system = ::Game::evict_far_cells,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_WorldTick,
        {
            {
                ._current_system = ::Game::tick_weather,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PreWorldTick,
        {
            {
                ._current_system = ::Game::stream_in_nearby_cells,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::SystemCallPhase::_EntityTick,
        {
            {
                ._current_system = ::Game::run_behaviour_trees,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PostEntityTick,
        {
            {
                ._current_system = ::Game::apply_deferred_despawns,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PostUpdateWork,
        {
            {
                ._current_system = ::Game::collect_profiling_counters,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_PostRenderQueueCommit,
        {
            {
                ._current_system = ::Game::submit_debug_lines,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_WorldDeinitialization,
        {
            {
                ._current_system = ::Game::release_navmesh,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::OverWorld, FE::_WorldDefaultEntityDeinitialization,
        {
            {
                ._current_system = ::Game::destroy_persistent_actors,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_StartPhysics,
        {
            {
                ._current_system = ::Game::begin_broadphase,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_PostPhysics,
        {
            {
                ._current_system = ::Game::write_back_transforms,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_PrePhysics,
        {
            {
                ._current_system = ::Game::sync_kinematic_bodies,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_EndPhysics,
        {
            {
                ._current_system = ::Game::resolve_contact_events,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_WorldEnd,
        {
            {
                ._current_system = ::Game::tally_battle_results,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_WorldBegin,
        {
            {
                ._current_system = ::Game::seed_encounter_rng,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, g_physics_phase,
        {
            {
                ._current_system = ::Game::integrate_rigid_bodies,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::BattleArena, FE::_WorldTick,
        {
            {
                ._current_system = ::Game::tick_arena_hazards,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(EditorWorldTag::Viewport, FE::_WorldTick,
        {
            {
                ._current_system = ::Game::tick_editor_gizmos,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::Boot, FE::_EngineInitialization,
        {
            {
                ._current_system = ::Game::boot_memory_arenas,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::Boot, FE::_GameInstanceDeinitialization,
        {
            {
                ._current_system = ::Game::save_player_profile,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::Boot, FE::_EngineDeinitialization,
        {
            {
                ._current_system = ::Game::teardown_memory_arenas,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::Boot, FE::_GameInstanceInitialization,
        {
            {
                ._current_system = ::Game::load_player_profile,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::MainMenu, FE::_GameInstanceEnd,
        {
            {
                ._current_system = ::Game::write_session_log,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::MainMenu, FE::_GameInstanceBegin,
        {
            {
                ._current_system = ::Game::play_intro_cinematic,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
    ::FE::framework::framework_base::get_framework().get_method_reflection().sort_systems_topologically(FEWorldTag::MainMenu, FE::_WorldBegin,
        {
            {
                ._current_system = ::Game::fade_in_title_ui,
                ._runs_before = { },
                ._runs_after = { }
            }
        }
    );
}
