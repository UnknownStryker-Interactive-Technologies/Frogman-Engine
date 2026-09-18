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
#ifndef _PONG_WORLD_HPP_
#define _PONG_WORLD_HPP_

#include <FE/prerequisites.hxx>
#include <FE/framework/reflection.hxx>


// Pong is played across four worlds. FE::world::create_world() is called once per
// tag at boot and FE::world::enter_world() drives the transitions between them.
FE_WORLD_TAG
{
	Boot = 0,
	TitleMenu = 1,
	Court = 2,
	MatchResult = 3
};


BEGIN_NAMESPACE(pong)


// Which half of the court a paddle defends.
FE_ENUM_STRUCT();
enum struct PaddleSide : var::uint8
{
	_Left = 0,
	_Right = 1
};


// The rally state machine. It lives on the match entity and the scoring systems
// are the only ones allowed to write it.
FE_ENUM_STRUCT();
enum struct RallyState : var::uint8
{
	_WaitingForServe = 0,
	_BallInPlay = 1,
	_PointScored = 2,
	_MatchOver = 3
};


// What the ball touched during the last fixed physics step.
FE_ENUM_STRUCT();
enum struct BounceSurface : var::uint8
{
	_None = 0,
	_TopWall = 1,
	_BottomWall = 2,
	_LeftPaddle = 3,
	_RightPaddle = 4
};


// The preset the CPU brain component is seeded from.
FE_ENUM_STRUCT();
enum struct OpponentSkill : var::uint8
{
	_Rookie = 0,
	_Amateur = 1,
	_Champion = 2
};


// The court is centred on the origin and every metric below is in world units.
constexpr FE::float32 court_half_width = 16.0f;
constexpr FE::float32 court_half_height = 9.0f;
constexpr FE::float32 paddle_inset_from_wall = 1.5f;
constexpr FE::float32 paddle_half_height = 1.75f;
constexpr FE::float32 paddle_half_width = 0.25f;
constexpr FE::float32 ball_radius = 0.3f;

// Speeds are world units per second. FE::world::delta_milliseconds() is in
// milliseconds, so every integrating system scales by delta * 0.001.
constexpr FE::float32 serve_speed = 12.0f;
constexpr FE::float32 speed_gain_per_hit = 1.05f;
constexpr FE::float32 max_ball_speed = 32.0f;
constexpr FE::float32 max_bounce_angle_radians = 1.047f;
constexpr FE::float32 milliseconds_to_seconds = 0.001f;

// A match is first to eleven with a two point margin.
constexpr FE::uint8 points_to_win = 11;
constexpr FE::uint8 winning_margin = 2;
constexpr FE::float32 serve_delay_seconds = 1.25f;


// Entity tags handed to FE::world::spawn_entity() so the systems can find the
// singletons again through FE::world::find_entity().
constexpr FE::ASCII* ball_entity_tag = "pong.ball";
constexpr FE::ASCII* left_paddle_entity_tag = "pong.paddle.left";
constexpr FE::ASCII* right_paddle_entity_tag = "pong.paddle.right";
constexpr FE::ASCII* match_entity_tag = "pong.match";
constexpr FE::ASCII* net_entity_tag = "pong.net";


END_NAMESPACE
#endif // _PONG_WORLD_HPP_
