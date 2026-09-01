// Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <memory>

#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\app.hpp>




void load_reflection_data()
{
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_EngineInitialization, &::Game::boot_memory_arenas);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_GameInstanceInitialization, &::Game::load_player_profile);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldInitialization, &::Game::build_navmesh);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldDefaultEntityInitialization, &::Game::spawn_persistent_actors);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::MainMenu, FE::_GameInstanceBegin, &::Game::play_intro_cinematic);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::MainMenu, FE::_WorldBegin, &::Game::fade_in_title_ui);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldBegin, &::Game::seed_encounter_rng);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreGameInstanceTick, &::Game::poll_input_devices);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_GameInstanceTick, &::Game::advance_save_timer);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostGameInstanceTick, &::Game::flush_input_edge_flags);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreWorldTick, &::Game::stream_in_nearby_cells);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldTick, &::Game::tick_weather);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldTick, &::Game::tick_arena_hazards);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(EditorWorldTag::Viewport, FE::_WorldTick, &::Game::tick_editor_gizmos);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostWorldTick, &::Game::evict_far_cells);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::refresh_transform_cache);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreEntityTick, &::Game::rebuild_spatial_hash);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::SystemCallPhase::_EntityTick, &::Game::run_behaviour_trees);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostEntityTick, &::Game::apply_deferred_despawns);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_PrePhysics, &::Game::sync_kinematic_bodies);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_StartPhysics, &::Game::begin_broadphase);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, g_physics_phase, &::Game::integrate_rigid_bodies);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_EndPhysics, &::Game::resolve_contact_events);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_PostPhysics, &::Game::write_back_transforms);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostUpdateWork, &::Game::collect_profiling_counters);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PreRenderQueueCommit, &::Game::cull_and_sort_draw_calls);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_PostRenderQueueCommit, &::Game::submit_debug_lines);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::BattleArena, FE::_WorldEnd, &::Game::tally_battle_results);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::MainMenu, FE::_GameInstanceEnd, &::Game::write_session_log);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldDefaultEntityDeinitialization, &::Game::destroy_persistent_actors);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::OverWorld, FE::_WorldDeinitialization, &::Game::release_navmesh);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_GameInstanceDeinitialization, &::Game::save_player_profile);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_EngineDeinitialization, &::Game::teardown_memory_arenas);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Game::CombatDirector*(::Game::CombatDirector*)> >("construct ::Game::CombatDirector", &::std::construct_at<::Game::CombatDirector>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Game::CombatDirector*)> >("destruct ::Game::CombatDirector", &::std::destroy_at<::Game::CombatDirector>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Game::LoadingScreenDriver*(::Game::LoadingScreenDriver*)> >("construct ::Game::LoadingScreenDriver", &::std::construct_at<::Game::LoadingScreenDriver>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Game::LoadingScreenDriver*)> >("destruct ::Game::LoadingScreenDriver", &::std::destroy_at<::Game::LoadingScreenDriver>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Game::EditorInspector*(::Game::EditorInspector*)> >("construct ::Game::EditorInspector", &::std::construct_at<::Game::EditorInspector>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Game::EditorInspector*)> >("destruct ::Game::EditorInspector", &::std::destroy_at<::Game::EditorInspector>);
}
