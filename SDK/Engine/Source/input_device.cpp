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
#include <FE/input_device.hpp>


BEGIN_NAMESPACE(FE::input_device)


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


mouse_callbacks::mouse_callbacks() noexcept
    :   _cursor_position_callback(&__null_cursor_position_callback_action), 
        _scroll_callback(&__null_scroll_callback_action),
        _button_left(&__null_mouse_callback_action), _button_right(&__null_mouse_callback_action), _button_middle(&__null_mouse_callback_action),
	    _button_4th(&__null_mouse_callback_action), _button_5th(&__null_mouse_callback_action), _button_6th(&__null_mouse_callback_action), _button_7th(&__null_mouse_callback_action), _button_8th(&__null_mouse_callback_action)
{
}


END_NAMESPACE