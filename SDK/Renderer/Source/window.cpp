#include <FE/window.hxx>
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

#include <FE/blacklist_evaluator.hxx>
#include <FE/clock.hxx>
#include <FE/random.hxx>
#include <FE/renderer.hxx>
#include <FE/engine.hpp>
#include <FE/image.hpp>
#include <FE/input_contexts.hxx>
#include <FE/video_player.hpp>

#include <FE/game_processor.hxx>

#include <atomic>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_dx11.h>

#include <taskflow.hpp>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <stb_image.h> // for loading icons & images




BEGIN_NAMESPACE(FE)


static void key_callback(GLFWwindow* const window_p, FE::int32 key_p, FE::int32 scancode_p, FE::int32 action_p, FE::int32 mods_p) noexcept
{
	(window_p);
	var::boolean l_is_consumed = false;

	for (auto context : FE::engine::get_engine().get_game().get_current_input_contexts())
	{
		FE::input::keyboard& l_keyboard = context->second._keyboard;
		FE::input::KeyState l_current_key_state = static_cast<FE::input::KeyState>(action_p);
		l_keyboard._keyboard_state._current_mode = static_cast<FE::input::KeyMode>(mods_p);

		switch (static_cast<FE::input::Key>(key_p))
		{
		case FE::input::Key::_Space:
			l_keyboard._keyboard_state._key_space = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_space != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_space(FE::input::Key::_Space, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Apostrophe:
			l_keyboard._keyboard_state._key_apostrophe = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_apostrophe != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_apostrophe(FE::input::Key::_Apostrophe, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Comma:
			l_keyboard._keyboard_state._key_comma = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_comma != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_comma(FE::input::Key::_Comma, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Minus:
			l_keyboard._keyboard_state._key_minus = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_minus != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_minus(FE::input::Key::_Minus, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Period:
			l_keyboard._keyboard_state._key_period = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_period != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_period(FE::input::Key::_Period, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Slash:
			l_keyboard._keyboard_state._key_slash = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_slash != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_slash(FE::input::Key::_Slash, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_0:
			l_keyboard._keyboard_state._key_0 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_0 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_0(FE::input::Key::_0, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_1:
			l_keyboard._keyboard_state._key_1 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_1 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_1(FE::input::Key::_1, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_2:
			l_keyboard._keyboard_state._key_2 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_2 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_2(FE::input::Key::_2, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_3:
			l_keyboard._keyboard_state._key_3 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_3 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_3(FE::input::Key::_3, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_4:
			l_keyboard._keyboard_state._key_4 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_4 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_4(FE::input::Key::_4, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_5:
			l_keyboard._keyboard_state._key_5 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_5 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_5(FE::input::Key::_5, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_6:
			l_keyboard._keyboard_state._key_6 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_6 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_6(FE::input::Key::_6, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_7:
			l_keyboard._keyboard_state._key_7 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_7 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_7(FE::input::Key::_7, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_8:
			l_keyboard._keyboard_state._key_8 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_8 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_8(FE::input::Key::_8, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_9:
			l_keyboard._keyboard_state._key_9 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_9 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_9(FE::input::Key::_9, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Semicolon:
			l_keyboard._keyboard_state._key_semicolon = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_semicolon != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_semicolon(FE::input::Key::_Semicolon, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Equal:
			l_keyboard._keyboard_state._key_equal = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_equal != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_equal(FE::input::Key::_Equal, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_A:
			l_keyboard._keyboard_state._key_a = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_a != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_a(FE::input::Key::_A, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_B:
			l_keyboard._keyboard_state._key_b = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_b != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_b(FE::input::Key::_B, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_C:
			l_keyboard._keyboard_state._key_c = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_c != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_c(FE::input::Key::_C, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_D:
			l_keyboard._keyboard_state._key_d = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_d != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_d(FE::input::Key::_D, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_E:
			l_keyboard._keyboard_state._key_e = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_e != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_e(FE::input::Key::_E, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F:
			l_keyboard._keyboard_state._key_f = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f(FE::input::Key::_F, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_G:
			l_keyboard._keyboard_state._key_g = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_g != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_g(FE::input::Key::_G, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_H:
			l_keyboard._keyboard_state._key_h = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_h != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_h(FE::input::Key::_H, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_I:
			l_keyboard._keyboard_state._key_i = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_i != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_i(FE::input::Key::_I, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_J:
			l_keyboard._keyboard_state._key_j = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_j != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_j(FE::input::Key::_J, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_K:
			l_keyboard._keyboard_state._key_k = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_k != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_k(FE::input::Key::_K, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_L:
			l_keyboard._keyboard_state._key_l = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_l != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_l(FE::input::Key::_L, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_M:
			l_keyboard._keyboard_state._key_m = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_m != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_m(FE::input::Key::_M, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_N:
			l_keyboard._keyboard_state._key_n = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_n != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_n(FE::input::Key::_N, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_O:
			l_keyboard._keyboard_state._key_o = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_o != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_o(FE::input::Key::_O, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_P:
			l_keyboard._keyboard_state._key_p = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_p != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_p(FE::input::Key::_P, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Q:
			l_keyboard._keyboard_state._key_q = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_q != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_q(FE::input::Key::_Q, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_R:
			l_keyboard._keyboard_state._key_r = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_r != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_r(FE::input::Key::_R, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_S:
			l_keyboard._keyboard_state._key_s = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_s != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_s(FE::input::Key::_S, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_T:
			l_keyboard._keyboard_state._key_t = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_t != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_t(FE::input::Key::_T, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_U:
			l_keyboard._keyboard_state._key_u = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_u != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_u(FE::input::Key::_U, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_V:
			l_keyboard._keyboard_state._key_v = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_v != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_v(FE::input::Key::_V, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_W:
			l_keyboard._keyboard_state._key_w = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_w != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_w(FE::input::Key::_W, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_X:
			l_keyboard._keyboard_state._key_x = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_x != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_x(FE::input::Key::_X, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Y:
			l_keyboard._keyboard_state._key_y = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_y != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_y(FE::input::Key::_Y, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Z:
			l_keyboard._keyboard_state._key_z = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_z != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_z(FE::input::Key::_Z, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_LeftBracket:
			l_keyboard._keyboard_state._key_left_bracket = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_left_bracket != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_left_bracket(FE::input::Key::_LeftBracket, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Backslash:
			l_keyboard._keyboard_state._key_backslash = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_backslash != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_backslash(FE::input::Key::_Backslash, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_RightBracket:
			l_keyboard._keyboard_state._key_right_bracket = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_right_bracket != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_right_bracket(FE::input::Key::_RightBracket, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_GraveAccent:
			l_keyboard._keyboard_state._key_grave_accent = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_grave_accent != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_grave_accent(FE::input::Key::_GraveAccent, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_World1:
			l_keyboard._keyboard_state._key_world_1 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_world_1 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_world_1(FE::input::Key::_World1, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_World2:
			l_keyboard._keyboard_state._key_world_2 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_world_2 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_world_2(FE::input::Key::_World2, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Escape:
			l_keyboard._keyboard_state._key_escape = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_escape != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_escape(FE::input::Key::_Escape, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Enter:
			static_assert(sizeof(l_keyboard._keyboard_state._current_mode) == 1);
			l_keyboard._keyboard_state._key_enter = l_current_key_state;
			if ((l_is_consumed == false) &&
				((FE::int8)l_keyboard._keyboard_state._current_mode & (FE::int8)FE::input::KeyMode::_Alt) &&
				(l_current_key_state == FE::input::KeyState::_Pressed)
				)
			{
				// toggle fullscreen when ALT + ENTER is pressed
				FE::engine::get_engine().get_window().toggle_borderless_fullscreen();
				l_is_consumed = true;
				break;
			}

			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_enter != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_enter(FE::input::Key::_Enter, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Tab:
			l_keyboard._keyboard_state._key_tab = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_tab != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_tab(FE::input::Key::_Tab, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Backspace:
			l_keyboard._keyboard_state._key_backspace = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_backspace != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_backspace(FE::input::Key::_Backspace, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Insert:
			l_keyboard._keyboard_state._key_insert = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_insert != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_insert(FE::input::Key::_Insert, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Delete:
			l_keyboard._keyboard_state._key_delete = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_delete != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_delete(FE::input::Key::_Delete, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Right:
			l_keyboard._keyboard_state._key_right = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_right != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_right(FE::input::Key::_Right, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Left:
			l_keyboard._keyboard_state._key_left = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_left != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_left(FE::input::Key::_Left, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Down:
			l_keyboard._keyboard_state._key_down = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_down != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_down(FE::input::Key::_Down, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Up:
			l_keyboard._keyboard_state._key_up = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_up != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_up(FE::input::Key::_Up, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_PageUp:
			l_keyboard._keyboard_state._key_page_up = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_page_up != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_page_up(FE::input::Key::_PageUp, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_PageDown:
			l_keyboard._keyboard_state._key_page_down = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_page_down != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_page_down(FE::input::Key::_PageDown, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Home:
			l_keyboard._keyboard_state._key_home = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_home != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_home(FE::input::Key::_Home, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_End:
			l_keyboard._keyboard_state._key_end = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_end != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_end(FE::input::Key::_End, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_CapsLock:
			l_keyboard._keyboard_state._key_caps_lock = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_caps_lock != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_caps_lock(FE::input::Key::_CapsLock, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_ScrollLock:
			l_keyboard._keyboard_state._key_scroll_lock = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_scroll_lock != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_scroll_lock(FE::input::Key::_ScrollLock, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_NumLock:
			l_keyboard._keyboard_state._key_num_lock = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_num_lock != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_num_lock(FE::input::Key::_NumLock, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_PrintScreen:
			l_keyboard._keyboard_state._key_print_screen = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_print_screen != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_print_screen(FE::input::Key::_PrintScreen, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Pause:
			l_keyboard._keyboard_state._key_pause = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pause != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pause(FE::input::Key::_Pause, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F1:
			l_keyboard._keyboard_state._key_f1 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f1 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f1(FE::input::Key::_F1, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F2:
			l_keyboard._keyboard_state._key_f2 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f2 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f2(FE::input::Key::_F2, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F3:
			l_keyboard._keyboard_state._key_f3 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f3 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f3(FE::input::Key::_F3, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F4:
			l_keyboard._keyboard_state._key_f4 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f4 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f4(FE::input::Key::_F4, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F5:
			l_keyboard._keyboard_state._key_f5 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f5 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f5(FE::input::Key::_F5, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F6:
			l_keyboard._keyboard_state._key_f6 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f6 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f6(FE::input::Key::_F6, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F7:
			l_keyboard._keyboard_state._key_f7 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f7 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f7(FE::input::Key::_F7, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F8:
			l_keyboard._keyboard_state._key_f8 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f8 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f8(FE::input::Key::_F8, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F9:
			l_keyboard._keyboard_state._key_f9 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f9 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f9(FE::input::Key::_F9, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F10:
			l_keyboard._keyboard_state._key_f10 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f10 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f10(FE::input::Key::_F10, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F11:
			l_keyboard._keyboard_state._key_f11 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f11 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f11(FE::input::Key::_F11, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F12:
			l_keyboard._keyboard_state._key_f12 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f12 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f12(FE::input::Key::_F12, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F13:
			l_keyboard._keyboard_state._key_f13 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f13 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f13(FE::input::Key::_F13, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F14:
			l_keyboard._keyboard_state._key_f14 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f14 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f14(FE::input::Key::_F14, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F15:
			l_keyboard._keyboard_state._key_f15 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f15 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f15(FE::input::Key::_F15, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F16:
			l_keyboard._keyboard_state._key_f16 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f16 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f16(FE::input::Key::_F16, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F17:
			l_keyboard._keyboard_state._key_f17 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f17 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f17(FE::input::Key::_F17, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F18:
			l_keyboard._keyboard_state._key_f18 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f18 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f18(FE::input::Key::_F18, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F19:
			l_keyboard._keyboard_state._key_f19 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f19 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f19(FE::input::Key::_F19, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F20:
			l_keyboard._keyboard_state._key_f20 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f20 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f20(FE::input::Key::_F20, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F21:
			l_keyboard._keyboard_state._key_f21 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f21 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f21(FE::input::Key::_F21, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F22:
			l_keyboard._keyboard_state._key_f22 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f22 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f22(FE::input::Key::_F22, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F23:
			l_keyboard._keyboard_state._key_f23 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f23 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f23(FE::input::Key::_F23, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F24:
			l_keyboard._keyboard_state._key_f24 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f24 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f24(FE::input::Key::_F24, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_F25:
			l_keyboard._keyboard_state._key_f25 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_f25 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_f25(FE::input::Key::_F25, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Pad0:
			l_keyboard._keyboard_state._key_pad_0 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_0 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_0(FE::input::Key::_Pad0, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Pad1:
			l_keyboard._keyboard_state._key_pad_1 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_1 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_1(FE::input::Key::_Pad1, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Pad2:
			l_keyboard._keyboard_state._key_pad_2 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_2 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_2(FE::input::Key::_Pad2, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Pad3:
			l_keyboard._keyboard_state._key_pad_3 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_3 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_3(FE::input::Key::_Pad3, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Pad4:
			l_keyboard._keyboard_state._key_pad_4 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_4 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_4(FE::input::Key::_Pad4, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Pad5:
			l_keyboard._keyboard_state._key_pad_5 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_5 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_5(FE::input::Key::_Pad5, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Pad6:
			l_keyboard._keyboard_state._key_pad_6 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_6 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_6(FE::input::Key::_Pad6, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Pad7:
			l_keyboard._keyboard_state._key_pad_7 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_7 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_7(FE::input::Key::_Pad7, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Pad8:
			l_keyboard._keyboard_state._key_pad_8 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_8 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_8(FE::input::Key::_Pad8, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Pad9:
			l_keyboard._keyboard_state._key_pad_9 = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_9 != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_9(FE::input::Key::_Pad9, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_PadDecimal:
			l_keyboard._keyboard_state._key_pad_decimal = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_decimal != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_decimal(FE::input::Key::_PadDecimal, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_PadDivide:
			l_keyboard._keyboard_state._key_pad_divide = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_divide != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_divide(FE::input::Key::_PadDivide, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_PadMultiply:
			l_keyboard._keyboard_state._key_pad_multiply = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_multiply != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_multiply(FE::input::Key::_PadMultiply, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_PadSubtract:
			l_keyboard._keyboard_state._key_pad_subtract = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_subtract != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_subtract(FE::input::Key::_PadSubtract, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_PadAdd:
			l_keyboard._keyboard_state._key_pad_add = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_add != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_add(FE::input::Key::_PadAdd, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_PadEnter:
			l_keyboard._keyboard_state._key_pad_enter = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_enter != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_enter(FE::input::Key::_PadEnter, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_PadEqual:
			l_keyboard._keyboard_state._key_pad_equal = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_pad_equal != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_pad_equal(FE::input::Key::_PadEqual, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_LeftShift:
			l_keyboard._keyboard_state._key_left_shift = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_left_shift != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_left_shift(FE::input::Key::_LeftShift, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_LeftControl:
			l_keyboard._keyboard_state._key_left_control = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_left_control != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_left_control(FE::input::Key::_LeftControl, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_LeftAlt:
			l_keyboard._keyboard_state._key_left_alt = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_left_alt != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_left_alt(FE::input::Key::_LeftAlt, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_LeftSuper:
			l_keyboard._keyboard_state._key_left_super = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_left_super != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_left_super(FE::input::Key::_LeftSuper, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_RightShift:
			l_keyboard._keyboard_state._key_right_shift = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_right_shift != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_right_shift(FE::input::Key::_RightShift, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_RightControl:
			l_keyboard._keyboard_state._key_right_control = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_right_control != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_right_control(FE::input::Key::_RightControl, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_RightAlt:
			l_keyboard._keyboard_state._key_right_alt = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_right_alt != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_right_alt(FE::input::Key::_RightAlt, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_RightSuper:
			l_keyboard._keyboard_state._key_right_super = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_right_super != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_right_super(FE::input::Key::_RightSuper, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		case FE::input::Key::_Menu:
			l_keyboard._keyboard_state._key_menu = l_current_key_state;
			if ((l_is_consumed == false) && (l_keyboard._keyboard_callbacks._key_menu != &FE::input::__null_keyboard_callback_action))
			{
				l_keyboard._keyboard_callbacks._key_menu(FE::input::Key::_Menu, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
				l_is_consumed = true;
			}
			break;

		default:
			break;
		}
	}
}

static void mouse_button_callback(GLFWwindow* const window_p, FE::int32 button_p, FE::int32 action_p, FE::int32 mods_p) noexcept
{
	(window_p);
	var::boolean l_is_consumed = false;

	for (auto context : FE::engine::get_engine().get_game().get_current_input_contexts())
	{
		FE::input::mouse& l_mouse = context->second._mouse;
		FE::input::ButtonState l_current_button_state = static_cast<FE::input::ButtonState>(action_p);
		l_mouse._mouse_state._current_mode = static_cast<FE::input::KeyMode>(mods_p);

		switch (static_cast<FE::input::Button>(button_p))
		{
		case FE::input::Button::_Left:
			l_mouse._mouse_state._button_left = l_current_button_state;
			if ((l_is_consumed == false) && (l_mouse._mouse_callbacks._button_left != &FE::input::__null_mouse_callback_action))
			{
				l_mouse._mouse_callbacks._button_left(FE::input::Button::_Left, l_current_button_state, l_mouse._mouse_state._current_mode);
				l_is_consumed = true;
			}
			break;

		case FE::input::Button::_Right:
			l_mouse._mouse_state._button_right = l_current_button_state;
			if ((l_is_consumed == false) && (l_mouse._mouse_callbacks._button_right != &FE::input::__null_mouse_callback_action))
			{
				l_mouse._mouse_callbacks._button_right(FE::input::Button::_Right, l_current_button_state, l_mouse._mouse_state._current_mode);
				l_is_consumed = true;
			}
			break;

		case FE::input::Button::_Middle:
			l_mouse._mouse_state._button_middle = l_current_button_state;
			if ((l_is_consumed == false) && (l_mouse._mouse_callbacks._button_middle != &FE::input::__null_mouse_callback_action))
			{
				l_mouse._mouse_callbacks._button_middle(FE::input::Button::_Middle, l_current_button_state, l_mouse._mouse_state._current_mode);
				l_is_consumed = true;
			}
			break;

		case FE::input::Button::_4th:
			l_mouse._mouse_state._button_4th = l_current_button_state;
			if ((l_is_consumed == false) && (l_mouse._mouse_callbacks._button_4th != &FE::input::__null_mouse_callback_action))
			{
				l_mouse._mouse_callbacks._button_4th(FE::input::Button::_4th, l_current_button_state, l_mouse._mouse_state._current_mode);
				l_is_consumed = true;
			}
			break;

		case FE::input::Button::_5th:
			l_mouse._mouse_state._button_5th = l_current_button_state;
			if ((l_is_consumed == false) && (l_mouse._mouse_callbacks._button_5th != &FE::input::__null_mouse_callback_action))
			{
				l_mouse._mouse_callbacks._button_5th(FE::input::Button::_5th, l_current_button_state, l_mouse._mouse_state._current_mode);
				l_is_consumed = true;
			}
			break;

		case FE::input::Button::_6th:
			l_mouse._mouse_state._button_6th = l_current_button_state;
			if ((l_is_consumed == false) && (l_mouse._mouse_callbacks._button_6th != &FE::input::__null_mouse_callback_action))
			{
				l_mouse._mouse_callbacks._button_6th(FE::input::Button::_6th, l_current_button_state, l_mouse._mouse_state._current_mode);
				l_is_consumed = true;
			}
			break;

		case FE::input::Button::_7th:
			l_mouse._mouse_state._button_7th = l_current_button_state;
			if ((l_is_consumed == false) && (l_mouse._mouse_callbacks._button_7th != &FE::input::__null_mouse_callback_action))
			{
				l_mouse._mouse_callbacks._button_7th(FE::input::Button::_7th, l_current_button_state, l_mouse._mouse_state._current_mode);
				l_is_consumed = true;
			}
			break;

		case FE::input::Button::_8th:
			l_mouse._mouse_state._button_8th = l_current_button_state;
			if ((l_is_consumed == false) && (l_mouse._mouse_callbacks._button_8th != &FE::input::__null_mouse_callback_action))
			{
				l_mouse._mouse_callbacks._button_8th(FE::input::Button::_8th, l_current_button_state, l_mouse._mouse_state._current_mode);
				l_is_consumed = true;
			}
			break;

		default:
			break;
		}
	}
}

static void cursor_position_callback(GLFWwindow* const window_p, double x_p, double y_p) noexcept
{
	(window_p);
	var::boolean l_is_consumed = false;

	for (auto context : FE::engine::get_engine().get_game().get_current_input_contexts())
	{
		FE::input::mouse& l_mouse = context->second._mouse;

		l_mouse._mouse_state._cursor_coordinate_x = x_p;
		l_mouse._mouse_state._cursor_coordinate_y = y_p;

		if ((l_is_consumed == false) && (l_mouse._mouse_callbacks._cursor_position_callback != &FE::input::__null_cursor_position_callback_action))
		{
			l_mouse._mouse_callbacks._cursor_position_callback(x_p, y_p);
			l_is_consumed = true;
		}
	}
}

static void scroll_callback(GLFWwindow* const window_p, double x_offset_p, double y_offset_p) noexcept
{
	(window_p);
	var::boolean l_is_consumed = false;

	for (auto context : FE::engine::get_engine().get_game().get_current_input_contexts())
	{
		FE::input::mouse& l_mouse = context->second._mouse;

		l_mouse._mouse_state._scroll_x_offset = x_offset_p;
		l_mouse._mouse_state._scroll_y_offset = y_offset_p;

		if ((l_is_consumed == false) && (l_mouse._mouse_callbacks._scroll_callback != &FE::input::__null_scroll_callback_action))
		{
			l_mouse._mouse_callbacks._scroll_callback(x_offset_p, y_offset_p);
			l_is_consumed = true;
		}
	}
}

static void window_focus_callback(GLFWwindow* const window_p, FE::int32 focused_p) noexcept
{
	(window_p);
	if (focused_p == GLFW_FALSE)
	{
		for (auto context : FE::engine::get_engine().get_game().get_current_input_contexts())
		{
			context->second.flush_pressed_keys();
		}
	}
}


window::window(const window_config& window_config_p) noexcept
	:	m_window(),
		m_primary_monitor(),
		m_monitors(),
		m_monitor_count(),
		m_video_mode(),
		m_window_config(window_config_p),

		m_pending_resolution_change(),
	
		m_saved_window_x(0),
		m_saved_window_y(0),
		m_saved_window_width(0),
		m_saved_window_height(0)
{
	FE_EXIT_IF(glfwInit() == GLFW_FALSE, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: The GLFW Window initialization failed.");
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // We do not want to create an OpenGL context
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE); // the D3D has its own buffering system
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE); // Make window visible upon creation

	m_monitors = glfwGetMonitors(&m_monitor_count);
	FE_EXIT_IF(m_monitors == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: Could not detect monitors; the GLFW Monitors retrieval failed.");

	m_primary_monitor = m_monitors[0];
	FE_EXIT_IF(m_primary_monitor == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: Could not detect a monitor; the GLFW Primary Monitor retrieval failed.");

	m_video_mode = glfwGetVideoMode(m_primary_monitor);
	FE_EXIT_IF(m_video_mode == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: The GLFW Video Mode retrieval failed.");
	glfwWindowHint(GLFW_RED_BITS, m_video_mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, m_video_mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, m_video_mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, m_video_mode->refreshRate);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE); // Disable automatic content scaling based on monitor DPI


	if (m_window_config._is_fullscreen == true)
	{
		glfwWindowHint(GLFW_FLOATING, GLFW_TRUE); // Make window always on top
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Disable window decorations (title bar, borders, etc.)
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Do not allow window resizing
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE); // Start window maximized

		m_window = glfwCreateWindow(m_video_mode->width, m_video_mode->height, m_window_config._title.c_str(), m_primary_monitor, nullptr);
		// glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwWindowHint(GLFW_FLOATING, GLFW_FALSE); // Make window always not on top
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); // Enable window decorations (title bar, borders, etc.)
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // Allow window resizing
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE); // Start window not maximized

		m_window = glfwCreateWindow(m_video_mode->width, m_video_mode->height, m_window_config._title.c_str(), nullptr, nullptr);
	}


	FE_EXIT_IF(m_window == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_WindowCreationFailure, "Frogman Engine Renderer Initialization Failure: The GLFW Window creation failed.");
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE); // Enable sticky keys input mode; the value remains until retrieved.

	if (m_window_config._icon_images.empty() == false)
	{
		glfwSetWindowIcon(m_window, (int)m_window_config._icon_images.size(), m_window_config._icon_images.data());

		for (GLFWimage& image : m_window_config._icon_images)
		{
			if (image.pixels == nullptr)
			{
				continue;
			}

			stbi_image_free(image.pixels);
			image.pixels = nullptr;
		}
	}

	glfwSetWindowCloseCallback(m_window, &__on_window_close);
	glfwSetFramebufferSizeCallback(m_window, &__on_window_resize);

	glfwSetKeyCallback(m_window, &key_callback);
	glfwSetMouseButtonCallback(m_window, &mouse_button_callback);
	glfwSetCursorPosCallback(m_window, &cursor_position_callback);
	glfwSetScrollCallback(m_window, &scroll_callback);
	glfwSetWindowFocusCallback(m_window, &window_focus_callback);
}

window::~window() noexcept
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}


void FE::window::__on_window_close(GLFWwindow* window_p) noexcept
{
	FE::engine::get_engine().terminate_all_processors();
	glfwSetWindowShouldClose(window_p, GLFW_TRUE);
}

void window::__on_window_resize(_FE_MAYBE_UNUSED_ GLFWwindow* const window_p, FE::int32 new_width_p, FE::int32 new_height_p) noexcept
{
	if ((new_width_p <= 0) || (new_height_p <= 0))
	{
		return; // Ignore minimize events
	}

	FE::engine::get_engine().get_window().m_pending_resolution_change.store({ ._width = (FE::uint32)new_width_p, ._height = (FE::uint32)new_height_p },
		std::memory_order_release);
}

void window::toggle_borderless_fullscreen() noexcept
{
	if (m_window_config._is_fullscreen == false)
	{
		// Save windowed-mode state
		glfwGetWindowPos(m_window, &m_saved_window_x, &m_saved_window_y);
		glfwGetWindowSize(m_window, &m_saved_window_width, &m_saved_window_height);


		// Position to cover the target monitor
		int l_monitor_x = 0;
		int l_monitor_y = 0;
		glfwGetMonitorPos(m_primary_monitor, &l_monitor_x, &l_monitor_y);


		glfwSetWindowAttrib(m_window, GLFW_FLOATING, GLFW_TRUE);
		glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_FALSE);
		glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, GLFW_FALSE);
		glfwSetWindowAttrib(m_window, GLFW_MAXIMIZED, GLFW_TRUE);

		m_pending_resolution_change.store({ ._width = (FE::uint32)m_video_mode->width, ._height = (FE::uint32)m_video_mode->height },
			std::memory_order_release);

		glfwSetWindowMonitor(m_window, nullptr,
			l_monitor_x, l_monitor_y,
			m_video_mode->width, m_video_mode->height,
			m_video_mode->refreshRate);

		m_window_config._is_fullscreen = true;
		return;
	}

	glfwSetWindowAttrib(m_window, GLFW_FLOATING, GLFW_FALSE);
	glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_TRUE);
	glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, GLFW_TRUE);
	glfwSetWindowAttrib(m_window, GLFW_MAXIMIZED, GLFW_FALSE);

	m_pending_resolution_change.store({ ._width = (FE::uint32)m_saved_window_width, ._height = (FE::uint32)m_saved_window_height },
		std::memory_order_release);

	glfwSetWindowMonitor(m_window, nullptr,
		m_saved_window_x, m_saved_window_y,
		m_saved_window_width, m_saved_window_height,
		m_video_mode->refreshRate);

	m_window_config._is_fullscreen = false;
}


END_NAMESPACE