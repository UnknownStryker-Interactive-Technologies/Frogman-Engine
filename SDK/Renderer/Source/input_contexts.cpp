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
#include <FE/input_contexts.hxx>
#include <FE/pair.hxx>




BEGIN_NAMESPACE(FE::input)


void __null_keyboard_callback_action(_FE_MAYBE_UNUSED_ Key key_p, _FE_MAYBE_UNUSED_ KeyState state_p, _FE_MAYBE_UNUSED_ KeyMode mode_p, _FE_MAYBE_UNUSED_ FE::int32 scancode_p) noexcept
{
}


keyboard_callbacks::keyboard_callbacks() noexcept
    :   _key_space(&__null_keyboard_callback_action), _key_apostrophe(&__null_keyboard_callback_action), _key_comma(&__null_keyboard_callback_action), _key_minus(&__null_keyboard_callback_action), _key_period(&__null_keyboard_callback_action), _key_slash(&__null_keyboard_callback_action),
        _key_0(&__null_keyboard_callback_action), _key_1(&__null_keyboard_callback_action), _key_2(&__null_keyboard_callback_action), _key_3(&__null_keyboard_callback_action), _key_4(&__null_keyboard_callback_action), _key_5(&__null_keyboard_callback_action), _key_6(&__null_keyboard_callback_action), _key_7(&__null_keyboard_callback_action), _key_8(&__null_keyboard_callback_action), _key_9(&__null_keyboard_callback_action),
        _key_semicolon(&__null_keyboard_callback_action), _key_equal(&__null_keyboard_callback_action),

        _key_a(&__null_keyboard_callback_action), _key_b(&__null_keyboard_callback_action), _key_c(&__null_keyboard_callback_action), _key_d(&__null_keyboard_callback_action), _key_e(&__null_keyboard_callback_action), _key_f(&__null_keyboard_callback_action), _key_g(&__null_keyboard_callback_action), _key_h(&__null_keyboard_callback_action), _key_i(&__null_keyboard_callback_action), _key_j(&__null_keyboard_callback_action), _key_k(&__null_keyboard_callback_action), _key_l(&__null_keyboard_callback_action), _key_m(&__null_keyboard_callback_action),
        _key_n(&__null_keyboard_callback_action), _key_o(&__null_keyboard_callback_action), _key_p(&__null_keyboard_callback_action), _key_q(&__null_keyboard_callback_action), _key_r(&__null_keyboard_callback_action), _key_s(&__null_keyboard_callback_action), _key_t(&__null_keyboard_callback_action), _key_u(&__null_keyboard_callback_action), _key_v(&__null_keyboard_callback_action), _key_w(&__null_keyboard_callback_action), _key_x(&__null_keyboard_callback_action), _key_y(&__null_keyboard_callback_action), _key_z(&__null_keyboard_callback_action),

        _key_left_bracket(&__null_keyboard_callback_action), _key_backslash(&__null_keyboard_callback_action), _key_right_bracket(&__null_keyboard_callback_action), _key_grave_accent(&__null_keyboard_callback_action),
        _key_world_1(&__null_keyboard_callback_action), _key_world_2(&__null_keyboard_callback_action),
        _key_escape(&__null_keyboard_callback_action), _key_enter(&__null_keyboard_callback_action), _key_tab(&__null_keyboard_callback_action), _key_backspace(&__null_keyboard_callback_action), _key_insert(&__null_keyboard_callback_action), _key_delete(&__null_keyboard_callback_action),
        _key_right(&__null_keyboard_callback_action), _key_left(&__null_keyboard_callback_action), _key_down(&__null_keyboard_callback_action), _key_up(&__null_keyboard_callback_action),
        _key_page_up(&__null_keyboard_callback_action), _key_page_down(&__null_keyboard_callback_action),
        _key_home(&__null_keyboard_callback_action), _key_end(&__null_keyboard_callback_action),
        _key_caps_lock(&__null_keyboard_callback_action), _key_scroll_lock(&__null_keyboard_callback_action), _key_num_lock(&__null_keyboard_callback_action),
        _key_print_screen(&__null_keyboard_callback_action), _key_pause(&__null_keyboard_callback_action),
        _key_f1(&__null_keyboard_callback_action), _key_f2(&__null_keyboard_callback_action), _key_f3(&__null_keyboard_callback_action), _key_f4(&__null_keyboard_callback_action), _key_f5(&__null_keyboard_callback_action), _key_f6(&__null_keyboard_callback_action), _key_f7(&__null_keyboard_callback_action), _key_f8(&__null_keyboard_callback_action), _key_f9(&__null_keyboard_callback_action), _key_f10(&__null_keyboard_callback_action),
        _key_f11(&__null_keyboard_callback_action), _key_f12(&__null_keyboard_callback_action), _key_f13(&__null_keyboard_callback_action), _key_f14(&__null_keyboard_callback_action), _key_f15(&__null_keyboard_callback_action), _key_f16(&__null_keyboard_callback_action), _key_f17(&__null_keyboard_callback_action), _key_f18(&__null_keyboard_callback_action), _key_f19(&__null_keyboard_callback_action), _key_f20(&__null_keyboard_callback_action),
        _key_f21(&__null_keyboard_callback_action), _key_f22(&__null_keyboard_callback_action), _key_f23(&__null_keyboard_callback_action), _key_f24(&__null_keyboard_callback_action), _key_f25(&__null_keyboard_callback_action),

        _key_pad_0(&__null_keyboard_callback_action), _key_pad_1(&__null_keyboard_callback_action), _key_pad_2(&__null_keyboard_callback_action), _key_pad_3(&__null_keyboard_callback_action), _key_pad_4(&__null_keyboard_callback_action), _key_pad_5(&__null_keyboard_callback_action), _key_pad_6(&__null_keyboard_callback_action), _key_pad_7(&__null_keyboard_callback_action), _key_pad_8(&__null_keyboard_callback_action), _key_pad_9(&__null_keyboard_callback_action),
        _key_pad_decimal(&__null_keyboard_callback_action), _key_pad_divide(&__null_keyboard_callback_action), _key_pad_multiply(&__null_keyboard_callback_action), _key_pad_subtract(&__null_keyboard_callback_action), _key_pad_add(&__null_keyboard_callback_action), _key_pad_enter(&__null_keyboard_callback_action), _key_pad_equal(&__null_keyboard_callback_action),

        _key_left_shift(&__null_keyboard_callback_action), _key_left_control(&__null_keyboard_callback_action), _key_left_alt(&__null_keyboard_callback_action), _key_left_super(&__null_keyboard_callback_action),
        _key_right_shift(&__null_keyboard_callback_action), _key_right_control(&__null_keyboard_callback_action), _key_right_alt(&__null_keyboard_callback_action), _key_right_super(&__null_keyboard_callback_action),

        _key_menu(&__null_keyboard_callback_action)
{
}




void __null_mouse_callback_action(_FE_MAYBE_UNUSED_ Button button_p, _FE_MAYBE_UNUSED_ ButtonState state_p, _FE_MAYBE_UNUSED_ KeyMode mode_p) noexcept
{
}

void __null_cursor_position_callback_action(_FE_MAYBE_UNUSED_ FE::float64 x_p, _FE_MAYBE_UNUSED_ FE::float64 y_p) noexcept
{
}

void __null_scroll_callback_action(_FE_MAYBE_UNUSED_ FE::float64 x_offset_p, _FE_MAYBE_UNUSED_ FE::float64 y_offset_p) noexcept
{
}

// Sorry, forgive me...
void FE::input::context::flush_pressed_keys() noexcept
{
	const FE::input::keyboard_state l_keyboard = _keyboard._keyboard_state;
	static_assert(sizeof(FE::input::keyboard_state) > 32);
	std::memset(&_keyboard._keyboard_state, 0, 32);

	if (l_keyboard._key_space != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_space(FE::input::Key::_Space, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_apostrophe != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_apostrophe(FE::input::Key::_Apostrophe, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_comma != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_comma(FE::input::Key::_Comma, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_minus != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_minus(FE::input::Key::_Minus, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_period != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_period(FE::input::Key::_Period, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_slash != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_slash(FE::input::Key::_Slash, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_0 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_0(FE::input::Key::_0, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_1 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_1(FE::input::Key::_1, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_2 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_2(FE::input::Key::_2, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_3 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_3(FE::input::Key::_3, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_4 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_4(FE::input::Key::_4, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_5 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_5(FE::input::Key::_5, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_6 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_6(FE::input::Key::_6, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_7 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_7(FE::input::Key::_7, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_8 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_8(FE::input::Key::_8, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_9 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_9(FE::input::Key::_9, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_semicolon != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_semicolon(FE::input::Key::_Semicolon, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_equal != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_equal(FE::input::Key::_Equal, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_a != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_a(FE::input::Key::_A, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_b != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_b(FE::input::Key::_B, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_c != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_c(FE::input::Key::_C, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_d != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_d(FE::input::Key::_D, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_e != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_e(FE::input::Key::_E, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f(FE::input::Key::_F, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_g != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_g(FE::input::Key::_G, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_h != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_h(FE::input::Key::_H, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_i != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_i(FE::input::Key::_I, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_j != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_j(FE::input::Key::_J, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_k != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_k(FE::input::Key::_K, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_l != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_l(FE::input::Key::_L, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_m != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_m(FE::input::Key::_M, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_n != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_n(FE::input::Key::_N, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_o != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_o(FE::input::Key::_O, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_p != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_p(FE::input::Key::_P, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_q != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_q(FE::input::Key::_Q, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_r != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_r(FE::input::Key::_R, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_s != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_s(FE::input::Key::_S, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_t != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_t(FE::input::Key::_T, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_u != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_u(FE::input::Key::_U, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_v != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_v(FE::input::Key::_V, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_w != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_w(FE::input::Key::_W, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_x != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_x(FE::input::Key::_X, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_y != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_y(FE::input::Key::_Y, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_z != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_z(FE::input::Key::_Z, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_left_bracket != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_left_bracket(FE::input::Key::_LeftBracket, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_backslash != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_backslash(FE::input::Key::_Backslash, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_right_bracket != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_right_bracket(FE::input::Key::_RightBracket, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_grave_accent != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_grave_accent(FE::input::Key::_GraveAccent, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_world_1 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_world_1(FE::input::Key::_World1, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_world_2 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_world_2(FE::input::Key::_World2, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_escape != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_escape(FE::input::Key::_Escape, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_enter != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_enter(FE::input::Key::_Enter, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_tab != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_tab(FE::input::Key::_Tab, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_backspace != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_backspace(FE::input::Key::_Backspace, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_insert != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_insert(FE::input::Key::_Insert, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_delete != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_delete(FE::input::Key::_Delete, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_right != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_right(FE::input::Key::_Right, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_left != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_left(FE::input::Key::_Left, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_down != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_down(FE::input::Key::_Down, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_up != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_up(FE::input::Key::_Up, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_page_up != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_page_up(FE::input::Key::_PageUp, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_page_down != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_page_down(FE::input::Key::_PageDown, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_home != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_home(FE::input::Key::_Home, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_end != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_end(FE::input::Key::_End, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_caps_lock != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_caps_lock(FE::input::Key::_CapsLock, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_scroll_lock != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_scroll_lock(FE::input::Key::_ScrollLock, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_num_lock != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_num_lock(FE::input::Key::_NumLock, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_print_screen != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_print_screen(FE::input::Key::_PrintScreen, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pause != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pause(FE::input::Key::_Pause, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f1 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f1(FE::input::Key::_F1, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f2 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f2(FE::input::Key::_F2, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f3 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f3(FE::input::Key::_F3, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f4 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f4(FE::input::Key::_F4, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f5 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f5(FE::input::Key::_F5, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f6 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f6(FE::input::Key::_F6, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f7 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f7(FE::input::Key::_F7, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f8 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f8(FE::input::Key::_F8, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f9 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f9(FE::input::Key::_F9, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f10 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f10(FE::input::Key::_F10, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f11 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f11(FE::input::Key::_F11, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f12 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f12(FE::input::Key::_F12, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f13 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f13(FE::input::Key::_F13, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f14 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f14(FE::input::Key::_F14, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f15 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f15(FE::input::Key::_F15, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f16 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f16(FE::input::Key::_F16, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f17 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f17(FE::input::Key::_F17, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f18 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f18(FE::input::Key::_F18, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f19 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f19(FE::input::Key::_F19, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f20 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f20(FE::input::Key::_F20, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f21 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f21(FE::input::Key::_F21, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f22 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f22(FE::input::Key::_F22, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f23 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f23(FE::input::Key::_F23, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f24 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f24(FE::input::Key::_F24, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_f25 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_f25(FE::input::Key::_F25, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_0 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_0(FE::input::Key::_Pad0, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_1 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_1(FE::input::Key::_Pad1, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_2 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_2(FE::input::Key::_Pad2, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_3 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_3(FE::input::Key::_Pad3, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_4 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_4(FE::input::Key::_Pad4, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_5 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_5(FE::input::Key::_Pad5, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_6 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_6(FE::input::Key::_Pad6, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_7 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_7(FE::input::Key::_Pad7, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_8 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_8(FE::input::Key::_Pad8, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_9 != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_9(FE::input::Key::_Pad9, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_decimal != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_decimal(FE::input::Key::_PadDecimal, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_divide != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_divide(FE::input::Key::_PadDivide, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_multiply != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_multiply(FE::input::Key::_PadMultiply, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_subtract != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_subtract(FE::input::Key::_PadSubtract, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_add != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_add(FE::input::Key::_PadAdd, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_enter != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_enter(FE::input::Key::_PadEnter, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_pad_equal != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_pad_equal(FE::input::Key::_PadEqual, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_left_shift != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_left_shift(FE::input::Key::_LeftShift, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_left_control != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_left_control(FE::input::Key::_LeftControl, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_left_alt != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_left_alt(FE::input::Key::_LeftAlt, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_left_super != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_left_super(FE::input::Key::_LeftSuper, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_right_shift != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_right_shift(FE::input::Key::_RightShift, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_right_control != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_right_control(FE::input::Key::_RightControl, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_right_alt != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_right_alt(FE::input::Key::_RightAlt, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_right_super != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_right_super(FE::input::Key::_RightSuper, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}

	if (l_keyboard._key_menu != FE::input::KeyState::_Released)
	{
		_keyboard._keyboard_callbacks._key_menu(FE::input::Key::_Menu, FE::input::KeyState::_Released, l_keyboard._current_mode, 0);
	}


	// Only the button bits are cleared: the cursor position and the scroll offsets
	// are still valid readings, not something that can be held down.
	const FE::input::mouse_state l_mouse = _mouse._mouse_state;
	_mouse._mouse_state._button_left = FE::input::ButtonState::_Released;
	_mouse._mouse_state._button_right = FE::input::ButtonState::_Released;
	_mouse._mouse_state._button_middle = FE::input::ButtonState::_Released;
	_mouse._mouse_state._button_4th = FE::input::ButtonState::_Released;
	_mouse._mouse_state._button_5th = FE::input::ButtonState::_Released;
	_mouse._mouse_state._button_6th = FE::input::ButtonState::_Released;
	_mouse._mouse_state._button_7th = FE::input::ButtonState::_Released;
	_mouse._mouse_state._button_8th = FE::input::ButtonState::_Released;
	_mouse._mouse_state._current_mode = FE::input::KeyMode::_None;

	if (l_mouse._button_left != FE::input::ButtonState::_Released)
	{
		_mouse._mouse_callbacks._button_left(FE::input::Button::_Left, FE::input::ButtonState::_Released, l_mouse._current_mode);
	}

	if (l_mouse._button_right != FE::input::ButtonState::_Released)
	{
		_mouse._mouse_callbacks._button_right(FE::input::Button::_Right, FE::input::ButtonState::_Released, l_mouse._current_mode);
	}

	if (l_mouse._button_middle != FE::input::ButtonState::_Released)
	{
		_mouse._mouse_callbacks._button_middle(FE::input::Button::_Middle, FE::input::ButtonState::_Released, l_mouse._current_mode);
	}

	if (l_mouse._button_4th != FE::input::ButtonState::_Released)
	{
		_mouse._mouse_callbacks._button_4th(FE::input::Button::_4th, FE::input::ButtonState::_Released, l_mouse._current_mode);
	}

	if (l_mouse._button_5th != FE::input::ButtonState::_Released)
	{
		_mouse._mouse_callbacks._button_5th(FE::input::Button::_5th, FE::input::ButtonState::_Released, l_mouse._current_mode);
	}

	if (l_mouse._button_6th != FE::input::ButtonState::_Released)
	{
		_mouse._mouse_callbacks._button_6th(FE::input::Button::_6th, FE::input::ButtonState::_Released, l_mouse._current_mode);
	}

	if (l_mouse._button_7th != FE::input::ButtonState::_Released)
	{
		_mouse._mouse_callbacks._button_7th(FE::input::Button::_7th, FE::input::ButtonState::_Released, l_mouse._current_mode);
	}

	if (l_mouse._button_8th != FE::input::ButtonState::_Released)
	{
		_mouse._mouse_callbacks._button_8th(FE::input::Button::_8th, FE::input::ButtonState::_Released, l_mouse._current_mode);
	}
}


mouse_callbacks::mouse_callbacks() noexcept
    :   _cursor_position_callback(&__null_cursor_position_callback_action), 
        _scroll_callback(&__null_scroll_callback_action),
        _button_left(&__null_mouse_callback_action), _button_right(&__null_mouse_callback_action), _button_middle(&__null_mouse_callback_action),
	    _button_4th(&__null_mouse_callback_action), _button_5th(&__null_mouse_callback_action), _button_6th(&__null_mouse_callback_action), _button_7th(&__null_mouse_callback_action), _button_8th(&__null_mouse_callback_action)
{
}

FE::boolean keyboard_state::is_same_as(const keyboard_state& keys_p) const noexcept
{
	static_assert(sizeof(keyboard_state) > 32);
	__m256i l_mask = _mm256_xor_si256(	_mm256_loadu_si256(reinterpret_cast<const __m256i*>(&keys_p)), 
										_mm256_loadu_si256(reinterpret_cast<const __m256i*>(this))
	);
	
	return _mm256_movemask_epi8(l_mask) == 0;
}

_FE_FORCE_INLINE_ keyboard_state make_key_mask(std::initializer_list<FE::pair<Key, KeyState>> combination_p) noexcept
{
	keyboard_state l_mask = {};

	for (auto& l_combination : combination_p)
	{
		switch (l_combination._first)
		{
		case Key::_Space:
			l_mask._key_space = l_combination._second;
			break;

		case Key::_Apostrophe:
			l_mask._key_apostrophe = l_combination._second;
			break;

		case Key::_Comma:
			l_mask._key_comma = l_combination._second;
			break;

		case Key::_Minus:
			l_mask._key_minus = l_combination._second;
			break;

		case Key::_Period:
			l_mask._key_period = l_combination._second;
			break;

		case Key::_Slash:
			l_mask._key_slash = l_combination._second;
			break;

		case Key::_0:
			l_mask._key_0 = l_combination._second;
			break;

		case Key::_1:
			l_mask._key_1 = l_combination._second;
			break;

		case Key::_2:
			l_mask._key_2 = l_combination._second;
			break;

		case Key::_3:
			l_mask._key_3 = l_combination._second;
			break;

		case Key::_4:
			l_mask._key_4 = l_combination._second;
			break;

		case Key::_5:
			l_mask._key_5 = l_combination._second;
			break;

		case Key::_6:
			l_mask._key_6 = l_combination._second;
			break;

		case Key::_7:
			l_mask._key_7 = l_combination._second;
			break;

		case Key::_8:
			l_mask._key_8 = l_combination._second;
			break;

		case Key::_9:
			l_mask._key_9 = l_combination._second;
			break;

		case Key::_Semicolon:
			l_mask._key_semicolon = l_combination._second;
			break;

		case Key::_Equal:
			l_mask._key_equal = l_combination._second;
			break;

		case Key::_A:
			l_mask._key_a = l_combination._second;
			break;

		case Key::_B:
			l_mask._key_b = l_combination._second;
			break;

		case Key::_C:
			l_mask._key_c = l_combination._second;
			break;

		case Key::_D:
			l_mask._key_d = l_combination._second;
			break;

		case Key::_E:
			l_mask._key_e = l_combination._second;
			break;

		case Key::_F:
			l_mask._key_f = l_combination._second;
			break;

		case Key::_G:
			l_mask._key_g = l_combination._second;
			break;

		case Key::_H:
			l_mask._key_h = l_combination._second;
			break;

		case Key::_I:
			l_mask._key_i = l_combination._second;
			break;

		case Key::_J:
			l_mask._key_j = l_combination._second;
			break;

		case Key::_K:
			l_mask._key_k = l_combination._second;
			break;

		case Key::_L:
			l_mask._key_l = l_combination._second;
			break;

		case Key::_M:
			l_mask._key_m = l_combination._second;
			break;

		case Key::_N:
			l_mask._key_n = l_combination._second;
			break;

		case Key::_O:
			l_mask._key_o = l_combination._second;
			break;

		case Key::_P:
			l_mask._key_p = l_combination._second;
			break;

		case Key::_Q:
			l_mask._key_q = l_combination._second;
			break;

		case Key::_R:
			l_mask._key_r = l_combination._second;
			break;

		case Key::_S:
			l_mask._key_s = l_combination._second;
			break;

		case Key::_T:
			l_mask._key_t = l_combination._second;
			break;

		case Key::_U:
			l_mask._key_u = l_combination._second;
			break;

		case Key::_V:
			l_mask._key_v = l_combination._second;
			break;

		case Key::_W:
			l_mask._key_w = l_combination._second;
			break;

		case Key::_X:
			l_mask._key_x = l_combination._second;
			break;

		case Key::_Y:
			l_mask._key_y = l_combination._second;
			break;

		case Key::_Z:
			l_mask._key_z = l_combination._second;
			break;

		case Key::_LeftBracket:
			l_mask._key_left_bracket = l_combination._second;
			break;

		case Key::_Backslash:
			l_mask._key_backslash = l_combination._second;
			break;

		case Key::_RightBracket:
			l_mask._key_right_bracket = l_combination._second;
			break;

		case Key::_GraveAccent:
			l_mask._key_grave_accent = l_combination._second;
			break;

		case Key::_World1:
			l_mask._key_world_1 = l_combination._second;
			break;

		case Key::_World2:
			l_mask._key_world_2 = l_combination._second;
			break;

		case Key::_Escape:
			l_mask._key_escape = l_combination._second;
			break;

		case Key::_Enter:
			l_mask._key_enter = l_combination._second;
			break;

		case Key::_Tab:
			l_mask._key_tab = l_combination._second;
			break;

		case Key::_Backspace:
			l_mask._key_backspace = l_combination._second;
			break;

		case Key::_Insert:
			l_mask._key_insert = l_combination._second;
			break;

		case Key::_Delete:
			l_mask._key_delete = l_combination._second;
			break;

		case Key::_Right:
			l_mask._key_right = l_combination._second;
			break;

		case Key::_Left:
			l_mask._key_left = l_combination._second;
			break;

		case Key::_Down:
			l_mask._key_down = l_combination._second;
			break;

		case Key::_Up:
			l_mask._key_up = l_combination._second;
			break;

		case Key::_PageUp:
			l_mask._key_page_up = l_combination._second;
			break;

		case Key::_PageDown:
			l_mask._key_page_down = l_combination._second;
			break;

		case Key::_Home:
			l_mask._key_home = l_combination._second;
			break;

		case Key::_End:
			l_mask._key_end = l_combination._second;
			break;

		case Key::_CapsLock:
			l_mask._key_caps_lock = l_combination._second;
			break;

		case Key::_ScrollLock:
			l_mask._key_scroll_lock = l_combination._second;
			break;

		case Key::_NumLock:
			l_mask._key_num_lock = l_combination._second;
			break;

		case Key::_PrintScreen:
			l_mask._key_print_screen = l_combination._second;
			break;

		case Key::_Pause:
			l_mask._key_pause = l_combination._second;
			break;

		case Key::_F1:
			l_mask._key_f1 = l_combination._second;
			break;

		case Key::_F2:
			l_mask._key_f2 = l_combination._second;
			break;

		case Key::_F3:
			l_mask._key_f3 = l_combination._second;
			break;

		case Key::_F4:
			l_mask._key_f4 = l_combination._second;
			break;

		case Key::_F5:
			l_mask._key_f5 = l_combination._second;
			break;

		case Key::_F6:
			l_mask._key_f6 = l_combination._second;
			break;

		case Key::_F7:
			l_mask._key_f7 = l_combination._second;
			break;

		case Key::_F8:
			l_mask._key_f8 = l_combination._second;
			break;

		case Key::_F9:
			l_mask._key_f9 = l_combination._second;
			break;

		case Key::_F10:
			l_mask._key_f10 = l_combination._second;
			break;

		case Key::_F11:
			l_mask._key_f11 = l_combination._second;
			break;

		case Key::_F12:
			l_mask._key_f12 = l_combination._second;
			break;

		case Key::_F13:
			l_mask._key_f13 = l_combination._second;
			break;

		case Key::_F14:
			l_mask._key_f14 = l_combination._second;
			break;

		case Key::_F15:
			l_mask._key_f15 = l_combination._second;
			break;

		case Key::_F16:
			l_mask._key_f16 = l_combination._second;
			break;

		case Key::_F17:
			l_mask._key_f17 = l_combination._second;
			break;

		case Key::_F18:
			l_mask._key_f18 = l_combination._second;
			break;

		case Key::_F19:
			l_mask._key_f19 = l_combination._second;
			break;

		case Key::_F20:
			l_mask._key_f20 = l_combination._second;
			break;

		case Key::_F21:
			l_mask._key_f21 = l_combination._second;
			break;

		case Key::_F22:
			l_mask._key_f22 = l_combination._second;
			break;

		case Key::_F23:
			l_mask._key_f23 = l_combination._second;
			break;

		case Key::_F24:
			l_mask._key_f24 = l_combination._second;
			break;

		case Key::_F25:
			l_mask._key_f25 = l_combination._second;
			break;

		case Key::_Pad0:
			l_mask._key_pad_0 = l_combination._second;
			break;

		case Key::_Pad1:
			l_mask._key_pad_1 = l_combination._second;
			break;

		case Key::_Pad2:
			l_mask._key_pad_2 = l_combination._second;
			break;

		case Key::_Pad3:
			l_mask._key_pad_3 = l_combination._second;
			break;

		case Key::_Pad4:
			l_mask._key_pad_4 = l_combination._second;
			break;

		case Key::_Pad5:
			l_mask._key_pad_5 = l_combination._second;
			break;

		case Key::_Pad6:
			l_mask._key_pad_6 = l_combination._second;
			break;

		case Key::_Pad7:
			l_mask._key_pad_7 = l_combination._second;
			break;

		case Key::_Pad8:
			l_mask._key_pad_8 = l_combination._second;
			break;

		case Key::_Pad9:
			l_mask._key_pad_9 = l_combination._second;
			break;

		case Key::_PadDecimal:
			l_mask._key_pad_decimal = l_combination._second;
			break;

		case Key::_PadDivide:
			l_mask._key_pad_divide = l_combination._second;
			break;

		case Key::_PadMultiply:
			l_mask._key_pad_multiply = l_combination._second;
			break;

		case Key::_PadSubtract:
			l_mask._key_pad_subtract = l_combination._second;
			break;

		case Key::_PadAdd:
			l_mask._key_pad_add = l_combination._second;
			break;

		case Key::_PadEnter:
			l_mask._key_pad_enter = l_combination._second;
			break;

		case Key::_PadEqual:
			l_mask._key_pad_equal = l_combination._second;
			break;

		case Key::_LeftShift:
			l_mask._key_left_shift = l_combination._second;
			break;

		case Key::_LeftControl:
			l_mask._key_left_control = l_combination._second;
			break;

		case Key::_LeftAlt:
			l_mask._key_left_alt = l_combination._second;
			break;

		case Key::_LeftSuper:
			l_mask._key_left_super = l_combination._second;
			break;

		case Key::_RightShift:
			l_mask._key_right_shift = l_combination._second;
			break;

		case Key::_RightControl:
			l_mask._key_right_control = l_combination._second;
			break;

		case Key::_RightAlt:
			l_mask._key_right_alt = l_combination._second;
			break;

		case Key::_RightSuper:
			l_mask._key_right_super = l_combination._second;
			break;

		case Key::_Menu:
			l_mask._key_menu = l_combination._second;
			break;

		default:
			break;
		}
	}

	return l_mask;
}




FE::boolean mouse_state::is_same_as(FE::uint16 buttons_p) const noexcept
{
	var::uint16 l_current = 0;
	std::memcpy(&l_current, this, sizeof(l_current));

	return (l_current xor buttons_p) == 0;
}

FE::uint16 make_button_mask(std::initializer_list<FE::pair<Button, ButtonState>> combination_p) noexcept
{
	// The button bit fields are the first members, so they occupy the leading two bytes.
	static_assert(offsetof(mouse_state, _cursor_coordinate_x) >= sizeof(var::uint16));

	mouse_state l_mask = {};

	for (auto& l_combination : combination_p)
	{
		switch (l_combination._first)
		{
		case Button::_Left:
			l_mask._button_left = l_combination._second;
			break;

		case Button::_Right:
			l_mask._button_right = l_combination._second;
			break;

		case Button::_Middle:
			l_mask._button_middle = l_combination._second;
			break;

		case Button::_4th:
			l_mask._button_4th = l_combination._second;
			break;

		case Button::_5th:
			l_mask._button_5th = l_combination._second;
			break;

		case Button::_6th:
			l_mask._button_6th = l_combination._second;
			break;

		case Button::_7th:
			l_mask._button_7th = l_combination._second;
			break;

		case Button::_8th:
			l_mask._button_8th = l_combination._second;
			break;

		default:
			break;
		}
	}

	var::uint16 l_result = 0;
	std::memcpy(&l_result, &l_mask, sizeof(l_result));
	return l_result;
}
END_NAMESPACE