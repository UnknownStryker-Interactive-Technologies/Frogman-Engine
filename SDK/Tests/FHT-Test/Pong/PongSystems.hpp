/*
Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

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
#ifndef _PONG_SYSTEMS_HPP_
#define _PONG_SYSTEMS_HPP_

#include <FE/prerequisites.hxx>
#include <FE/framework/reflection.hxx>

#include <FE/world.hxx>

#include "PongComponents.hpp"
#include "PongInput.hpp"
#include "PongWorld.hpp"


// Every function below matches FE::world's system signature, void(*)(FE::world&).
// FE_SYSTEM binds one to a call phase and a world tag; the engine walks the phases
// in the order of FE::SystemCallPhase, so the declaration order here is the run order.
namespace pong
{
	// ------------------------------------------------------------------ boot

	FE_SYSTEM(FE::_EngineInitialization, FEWorldTag::Boot);
	void reserve_court_arenas(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_GameInstanceInitialization, FEWorldTag::Boot);
	void read_match_settings(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_GameInstanceInitialization, FEWorldTag::Boot);
	void install_input_contexts(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_WorldInitialization, FEWorldTag::Boot);
	void create_gameplay_worlds(::FE::world& world_p) noexcept;


	// ------------------------------------------------------------------ title menu

	FE_SYSTEM(FE::_WorldInitialization, FEWorldTag::TitleMenu);
	void spawn_title_entities(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_WorldBegin, FEWorldTag::TitleMenu);
	void fade_in_title_banner(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_WorldTick, FEWorldTag::TitleMenu);
	void tick_attract_rally(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_WorldTick, FEWorldTag::TitleMenu);
	void poll_menu_selection(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_WorldEnd, FEWorldTag::TitleMenu);
	void despawn_title_entities(::FE::world& world_p) noexcept;


	// ------------------------------------------------------------------ court setup

	// Spawns the ball, both paddles, the net and the match singleton, tagging each
	// with the entity tags from PongWorld.hpp.
	FE_SYSTEM(FE::_WorldInitialization, FEWorldTag::Court);
	void spawn_court_entities(::FE::world& world_p) noexcept;

	// Attaches transform_2d, velocity_2d and the collider components to the entities
	// spawned above, plus cpu_brain on whichever paddle the CPU drives.
	FE_SYSTEM(FE::_WorldDefaultEntityInitialization, FEWorldTag::Court);
	void attach_court_components(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_WorldBegin, FEWorldTag::Court);
	void reset_score_and_clock(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_WorldBegin, FEWorldTag::Court);
	void queue_first_serve(::FE::world& world_p) noexcept;


	// ------------------------------------------------------------------ input

	// Drains raw_input_frame, which the keyboard callbacks in PongInput.hpp fill.
	FE_SYSTEM(FE::_PreGameInstanceTick, FEWorldTag::Court);
	void drain_raw_input_frame(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_GameInstanceTick, FEWorldTag::Court);
	void resolve_pause_request(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_PostGameInstanceTick, FEWorldTag::Court);
	void clear_input_edges(::FE::world& world_p) noexcept;


	// ------------------------------------------------------------------ gameplay tick

	FE_SYSTEM(FE::_PreWorldTick, FEWorldTag::Court);
	void tick_serve_countdown(::FE::world& world_p) noexcept;

	// View over paddle, player_intent and velocity_2d.
	FE_SYSTEM(FE::_WorldTick, FEWorldTag::Court);
	void drive_player_paddle(::FE::world& world_p) noexcept;

	// View over paddle, cpu_brain and velocity_2d.
	FE_SYSTEM(FE::_WorldTick, FEWorldTag::Court);
	void drive_cpu_paddle(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_PostWorldTick, FEWorldTag::Court);
	void clamp_paddles_to_court(::FE::world& world_p) noexcept;


	// ------------------------------------------------------------------ physics

	// Copies transform_2d into previous_transform_2d so the sweep has both ends.
	FE_SYSTEM(FE::_PrePhysics, FEWorldTag::Court);
	void cache_previous_transforms(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_StartPhysics, FEWorldTag::Court);
	void integrate_velocities(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_Physics, FEWorldTag::Court);
	void sweep_ball_against_walls(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_Physics, FEWorldTag::Court);
	void sweep_ball_against_paddles(::FE::world& world_p) noexcept;

	// Consumes bounce_event, mirrors the velocity and removes the component.
	FE_SYSTEM(FE::_EndPhysics, FEWorldTag::Court);
	void apply_bounce_events(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_PostPhysics, FEWorldTag::Court);
	void detect_goal_line_crossing(::FE::world& world_p) noexcept;


	// ------------------------------------------------------------------ scoring

	FE_SYSTEM(FE::_PreEntityTick, FEWorldTag::Court);
	void advance_rally_state(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_EntityTick, FEWorldTag::Court);
	void award_pending_point(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_PostEntityTick, FEWorldTag::Court);
	void recentre_ball_after_point(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_PostEntityTick, FEWorldTag::Court);
	void enter_result_world_on_match_point(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_PostUpdateWork, FEWorldTag::Court);
	void accumulate_match_report(::FE::world& world_p) noexcept;


	// ------------------------------------------------------------------ presentation

	// Builds the draw queue from a view over transform_2d and sprite_quad.
	FE_SYSTEM(FE::_PreRenderQueueCommit, FEWorldTag::Court);
	void build_sprite_draw_queue(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_PreRenderQueueCommit, FEWorldTag::Court);
	void animate_net_strip(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_PostRenderQueueCommit, FEWorldTag::Court);
	void submit_score_board(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_PostRenderQueueCommit, FEWorldTag::Court);
	void submit_bounce_audio(::FE::world& world_p) noexcept;


	// ------------------------------------------------------------------ result screen

	FE_SYSTEM(FE::_WorldBegin, FEWorldTag::MatchResult);
	void show_winner_banner(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_WorldTick, FEWorldTag::MatchResult);
	void poll_rematch_request(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_WorldEnd, FEWorldTag::MatchResult);
	void despawn_result_entities(::FE::world& world_p) noexcept;


	// ------------------------------------------------------------------ teardown

	FE_SYSTEM(FE::_WorldDefaultEntityDeinitialization, FEWorldTag::Court);
	void detach_court_components(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_WorldDeinitialization, FEWorldTag::Court);
	void despawn_court_entities(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_GameInstanceEnd, FEWorldTag::MatchResult);
	void write_match_history(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_GameInstanceDeinitialization, FEWorldTag::Boot);
	void save_match_settings(::FE::world& world_p) noexcept;

	FE_SYSTEM(FE::_EngineDeinitialization, FEWorldTag::Boot);
	void release_court_arenas(::FE::world& world_p) noexcept;
}


#endif // _PONG_SYSTEMS_HPP_
