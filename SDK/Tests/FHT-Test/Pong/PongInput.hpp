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
#ifndef _PONG_INPUT_HPP_
#define _PONG_INPUT_HPP_

#include <FE/prerequisites.hxx>
#include <FE/framework/reflection.hxx>

#include <FE/game.hxx>
#include <FE/input_contexts.hxx>

#include "PongComponents.hpp"
#include "PongWorld.hpp"


BEGIN_NAMESPACE(pong)


// The two input contexts FE::game keeps for this project. The court context is
// pushed on entering FEWorldTag::Court and popped again on leaving it.
constexpr ::FE::input::context_id menu_context_id = 1;
constexpr ::FE::input::context_id court_context_id = 2;


// Keyboard callbacks. They match FE::input::keyboard_callbacks::action, so they
// have to stay free functions with exactly this signature; they only stage the
// key state and the systems in PongSystems.hpp turn it into player_intent.
void on_paddle_up_key(::FE::input::Key key_p, ::FE::input::KeyState state_p, ::FE::input::KeyMode mode_p, FE::int32 scancode_p) noexcept;
void on_paddle_down_key(::FE::input::Key key_p, ::FE::input::KeyState state_p, ::FE::input::KeyMode mode_p, FE::int32 scancode_p) noexcept;
void on_serve_key(::FE::input::Key key_p, ::FE::input::KeyState state_p, ::FE::input::KeyMode mode_p, FE::int32 scancode_p) noexcept;
void on_pause_key(::FE::input::Key key_p, ::FE::input::KeyState state_p, ::FE::input::KeyMode mode_p, FE::int32 scancode_p) noexcept;
void on_menu_confirm_key(::FE::input::Key key_p, ::FE::input::KeyState state_p, ::FE::input::KeyMode mode_p, FE::int32 scancode_p) noexcept;
void on_menu_cancel_key(::FE::input::Key key_p, ::FE::input::KeyState state_p, ::FE::input::KeyMode mode_p, FE::int32 scancode_p) noexcept;

// Mouse callbacks. The court only uses the cursor for the practice mode where the
// left paddle follows the pointer instead of the keyboard.
void on_cursor_moved(FE::float64 x_p, FE::float64 y_p) noexcept;
void on_menu_click(::FE::input::Button button_p, ::FE::input::ButtonState state_p, ::FE::input::KeyMode mode_p) noexcept;


// Owns the two FE::input::context registrations for the whole run. It is not an
// entity and holds no component data, so it stays a plain class rather than a
// component; the boot systems create one and hand it the FE::game instance.
FE_CLASS();
class input_bindings
{
	FHT_GENERATED();
public:
	input_bindings() noexcept;
	~input_bindings() noexcept;

	// Registers both contexts on the game and wires the callbacks above into them.
	void install(::FE::game& game_p) noexcept;

	// FE::game keeps a stack of active contexts; these two push and pop ours.
	void enter_court(::FE::game& game_p) noexcept;
	void leave_court(::FE::game& game_p) noexcept;

	// The court context uses the cursor only while practice mode is on.
	void set_practice_mode(var::boolean is_enabled_p) noexcept;

	var::boolean is_installed() const noexcept;
	var::boolean is_practice_mode() const noexcept;

private:
	::FE::input::context_id m_menu_context;
	::FE::input::context_id m_court_context;
	var::boolean m_is_installed;
	var::boolean m_is_practice_mode;
};


// The staging buffer the callbacks write and the input system drains once a frame.
// It is a component so the court world owns it rather than a global.
FE_STRUCT();
struct raw_input_frame
{
	FHT_GENERATED();
	var::boolean _up_held = false;
	var::boolean _down_held = false;
	var::boolean _serve_edge = false;
	var::boolean _pause_edge = false;
	var::float64 _cursor_y = 0.0;
};


END_NAMESPACE
#endif // _PONG_INPUT_HPP_
