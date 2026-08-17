#ifndef _FE_RENDERER_INPUT_CONTEXTS_HPP_
#define _FE_RENDERER_INPUT_CONTEXTS_HPP_
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
#include <FE/pair.hxx>
#include <GLFW/glfw3.h>




BEGIN_NAMESPACE(FE::input)


enum struct KeyState : FE::int8
{
    _Released = GLFW_RELEASE,
    _Pressed = GLFW_PRESS,
	_Repeated = GLFW_REPEAT
};

enum struct KeyMode : FE::int8
{
	_None,
	_Shift = GLFW_MOD_SHIFT,
	_Control = GLFW_MOD_CONTROL,
	_Alt = GLFW_MOD_ALT,
	_Super = GLFW_MOD_SUPER,
	_CapsLock = GLFW_MOD_CAPS_LOCK,
	_NumLock = GLFW_MOD_NUM_LOCK
};

enum struct Key : FE::int16
{
	_Space = GLFW_KEY_SPACE, _Apostrophe = GLFW_KEY_APOSTROPHE, _Comma = GLFW_KEY_COMMA, 
	_Minus = GLFW_KEY_MINUS, _Period = GLFW_KEY_PERIOD, _Slash = GLFW_KEY_SLASH,
	
	_0 = GLFW_KEY_0, _1 = GLFW_KEY_1, _2 = GLFW_KEY_2, _3 = GLFW_KEY_3, _4 = GLFW_KEY_4, _5 = GLFW_KEY_5, 
	_6 = GLFW_KEY_6, _7 = GLFW_KEY_7, _8 = GLFW_KEY_8, _9 = GLFW_KEY_9, 
	
	_Semicolon = GLFW_KEY_SEMICOLON, _Equal = GLFW_KEY_EQUAL,

	_A = GLFW_KEY_A, _B = GLFW_KEY_B, _C = GLFW_KEY_C, _D = GLFW_KEY_D, _E = GLFW_KEY_E, _F = GLFW_KEY_F, _G = GLFW_KEY_G, 
	_H = GLFW_KEY_H, _I = GLFW_KEY_I, _J = GLFW_KEY_J, _K = GLFW_KEY_K, _L = GLFW_KEY_L, _M = GLFW_KEY_M, _N = GLFW_KEY_N, 
	_O = GLFW_KEY_O, _P = GLFW_KEY_P, _Q = GLFW_KEY_Q, _R = GLFW_KEY_R, _S = GLFW_KEY_S, _T = GLFW_KEY_T, _U = GLFW_KEY_U, 
	_V = GLFW_KEY_V, _W = GLFW_KEY_W, _X = GLFW_KEY_X, _Y = GLFW_KEY_Y, _Z = GLFW_KEY_Z, 

	_LeftBracket = GLFW_KEY_LEFT_BRACKET, _Backslash = GLFW_KEY_BACKSLASH, 
	_RightBracket = GLFW_KEY_RIGHT_BRACKET, _GraveAccent = GLFW_KEY_GRAVE_ACCENT,
	
	_World1 = GLFW_KEY_WORLD_1, _World2 = GLFW_KEY_WORLD_2,
	
	_Escape = GLFW_KEY_ESCAPE, _Enter = GLFW_KEY_ENTER, _Tab = GLFW_KEY_TAB, 
	_Backspace = GLFW_KEY_BACKSPACE, _Insert = GLFW_KEY_INSERT, _Delete = GLFW_KEY_DELETE,

	_Right = GLFW_KEY_RIGHT, _Left = GLFW_KEY_LEFT, _Down = GLFW_KEY_DOWN, _Up = GLFW_KEY_UP,

	_PageUp = GLFW_KEY_PAGE_UP, _PageDown = GLFW_KEY_PAGE_DOWN,

	_Home = GLFW_KEY_HOME, _End = GLFW_KEY_END,

	_CapsLock = GLFW_KEY_CAPS_LOCK, _ScrollLock = GLFW_KEY_SCROLL_LOCK, _NumLock = GLFW_KEY_NUM_LOCK,

	_PrintScreen = GLFW_KEY_PRINT_SCREEN, _Pause = GLFW_KEY_PAUSE,

	_F1 = GLFW_KEY_F1, _F2 = GLFW_KEY_F2, _F3 = GLFW_KEY_F3, _F4 = GLFW_KEY_F4, _F5 = GLFW_KEY_F5, 
	_F6 = GLFW_KEY_F6, _F7 = GLFW_KEY_F7, _F8 = GLFW_KEY_F8, _F9 = GLFW_KEY_F9, _F10 = GLFW_KEY_F10,
	_F11 = GLFW_KEY_F11, _F12 = GLFW_KEY_F12, _F13 = GLFW_KEY_F13, _F14 = GLFW_KEY_F14, _F15 = GLFW_KEY_F15, 
	_F16 = GLFW_KEY_F16, _F17 = GLFW_KEY_F17, _F18 = GLFW_KEY_F18, _F19 = GLFW_KEY_F19, _F20 = GLFW_KEY_F20, 
	_F21 = GLFW_KEY_F21, _F22 = GLFW_KEY_F22, _F23 = GLFW_KEY_F23, _F24 = GLFW_KEY_F24, _F25 = GLFW_KEY_F25,

	_Pad0 = GLFW_KEY_KP_0, _Pad1 = GLFW_KEY_KP_1, _Pad2 = GLFW_KEY_KP_2, _Pad3 = GLFW_KEY_KP_3, _Pad4 = GLFW_KEY_KP_4, 
	_Pad5 = GLFW_KEY_KP_5, _Pad6 = GLFW_KEY_KP_6, _Pad7 = GLFW_KEY_KP_7, _Pad8 = GLFW_KEY_KP_8, _Pad9 = GLFW_KEY_KP_9, 
	
	_PadDecimal = GLFW_KEY_KP_DECIMAL, _PadDivide = GLFW_KEY_KP_DIVIDE, _PadMultiply = GLFW_KEY_KP_MULTIPLY, 
	_PadSubtract = GLFW_KEY_KP_SUBTRACT, _PadAdd = GLFW_KEY_KP_ADD, _PadEnter = GLFW_KEY_KP_ENTER, 
	_PadEqual = GLFW_KEY_KP_EQUAL, 

	_LeftShift = GLFW_KEY_LEFT_SHIFT, _LeftControl = GLFW_KEY_LEFT_CONTROL, 
	_LeftAlt = GLFW_KEY_LEFT_ALT, _LeftSuper = GLFW_KEY_LEFT_SUPER,
	
	_RightShift = GLFW_KEY_RIGHT_SHIFT, _RightControl = GLFW_KEY_RIGHT_CONTROL, 
	_RightAlt = GLFW_KEY_RIGHT_ALT, _RightSuper = GLFW_KEY_RIGHT_SUPER,
	
	_Menu = GLFW_KEY_MENU
};


class keyboard_callbacks final
{
public:
    using action = void(*)(Key key_p, KeyState state_p, KeyMode mode_p, _FE_MAYBE_UNUSED_ FE::int32 scancode_p);

    action _key_space, _key_apostrophe, _key_comma, _key_minus, _key_period, _key_slash;
	action _key_0, _key_1, _key_2, _key_3, _key_4, _key_5, _key_6, _key_7, _key_8, _key_9;
	action _key_semicolon, _key_equal;

	action _key_a, _key_b, _key_c, _key_d, _key_e, _key_f, _key_g, _key_h, _key_i, _key_j, _key_k, _key_l, _key_m,
	_key_n, _key_o, _key_p, _key_q, _key_r, _key_s, _key_t, _key_u, _key_v, _key_w, _key_x, _key_y, _key_z;

	action _key_left_bracket, _key_backslash, _key_right_bracket, _key_grave_accent;
	action _key_world_1, _key_world_2;
	action _key_escape, _key_enter, _key_tab, _key_backspace, _key_insert, _key_delete;
	action _key_right, _key_left, _key_down, _key_up;
	action _key_page_up, _key_page_down;
	action _key_home, _key_end;
	action _key_caps_lock, _key_scroll_lock, _key_num_lock;
	action _key_print_screen, _key_pause;

	action _key_f1, _key_f2, _key_f3, _key_f4, _key_f5, _key_f6, _key_f7, _key_f8, _key_f9, _key_f10,
	_key_f11, _key_f12, _key_f13, _key_f14, _key_f15, _key_f16, _key_f17, _key_f18, _key_f19, _key_f20, 
	_key_f21, _key_f22, _key_f23, _key_f24, _key_f25;

	action _key_pad_0, _key_pad_1, _key_pad_2, _key_pad_3, _key_pad_4, _key_pad_5, _key_pad_6, _key_pad_7, _key_pad_8, _key_pad_9, 
	_key_pad_decimal, _key_pad_divide, _key_pad_multiply, _key_pad_subtract, _key_pad_add, _key_pad_enter, _key_pad_equal;

	action _key_left_shift, _key_left_control, _key_left_alt, _key_left_super,
		_key_right_shift, _key_right_control, _key_right_alt, _key_right_super;

	action _key_menu;

public:
	keyboard_callbacks() noexcept;
	~keyboard_callbacks() noexcept = default;
};


class keyboard_state final
{
	static_assert((FE::int8)KeyState::_Released == 0);
public:
	KeyState _key_space			: 2 = KeyState::_Released;
	KeyState _key_apostrophe	: 2 = KeyState::_Released;
	KeyState _key_comma			: 2 = KeyState::_Released;
	KeyState _key_minus			: 2 = KeyState::_Released;
	KeyState _key_period		: 2 = KeyState::_Released;
	KeyState _key_slash			: 2 = KeyState::_Released;
	KeyState _key_0				: 2 = KeyState::_Released;
	KeyState _key_1				: 2 = KeyState::_Released;
	KeyState _key_2				: 2 = KeyState::_Released;
	KeyState _key_3				: 2 = KeyState::_Released;
	KeyState _key_4				: 2 = KeyState::_Released;
	KeyState _key_5				: 2 = KeyState::_Released;
	KeyState _key_6				: 2 = KeyState::_Released;
	KeyState _key_7				: 2 = KeyState::_Released;
	KeyState _key_8				: 2 = KeyState::_Released;
	KeyState _key_9				: 2 = KeyState::_Released;
	KeyState _key_semicolon		: 2 = KeyState::_Released;
	KeyState _key_equal			: 2 = KeyState::_Released;
	KeyState _key_a				: 2 = KeyState::_Released;
	KeyState _key_b				: 2 = KeyState::_Released;
	KeyState _key_c				: 2 = KeyState::_Released;
	KeyState _key_d				: 2 = KeyState::_Released;
	KeyState _key_e				: 2 = KeyState::_Released;
	KeyState _key_f				: 2 = KeyState::_Released;
	KeyState _key_g				: 2 = KeyState::_Released;
	KeyState _key_h				: 2 = KeyState::_Released;
	KeyState _key_i				: 2 = KeyState::_Released;
	KeyState _key_j				: 2 = KeyState::_Released;
	KeyState _key_k				: 2 = KeyState::_Released;
	KeyState _key_l				: 2 = KeyState::_Released;
	KeyState _key_m				: 2 = KeyState::_Released;
	KeyState _key_n				: 2 = KeyState::_Released;
	KeyState _key_o				: 2 = KeyState::_Released;
	KeyState _key_p				: 2 = KeyState::_Released;
	KeyState _key_q				: 2 = KeyState::_Released;
	KeyState _key_r				: 2 = KeyState::_Released;
	KeyState _key_s				: 2 = KeyState::_Released;
	KeyState _key_t				: 2 = KeyState::_Released;
	KeyState _key_u				: 2 = KeyState::_Released;
	KeyState _key_v				: 2 = KeyState::_Released;
	KeyState _key_w				: 2 = KeyState::_Released;
	KeyState _key_x				: 2 = KeyState::_Released;
	KeyState _key_y				: 2 = KeyState::_Released;
	KeyState _key_z				: 2 = KeyState::_Released;
	KeyState _key_left_bracket	: 2 = KeyState::_Released;
	KeyState _key_backslash		: 2 = KeyState::_Released;
	KeyState _key_right_bracket : 2 = KeyState::_Released;
	KeyState _key_grave_accent	: 2 = KeyState::_Released;
	KeyState _key_world_1		: 2 = KeyState::_Released;
	KeyState _key_world_2		: 2 = KeyState::_Released;
	KeyState _key_escape		: 2 = KeyState::_Released;
	KeyState _key_enter			: 2 = KeyState::_Released;
	KeyState _key_tab			: 2 = KeyState::_Released;
	KeyState _key_backspace		: 2 = KeyState::_Released;
	KeyState _key_insert		: 2 = KeyState::_Released;
	KeyState _key_delete		: 2 = KeyState::_Released;
	KeyState _key_right			: 2 = KeyState::_Released;
	KeyState _key_left			: 2 = KeyState::_Released;
	KeyState _key_down			: 2 = KeyState::_Released;
	KeyState _key_up			: 2 = KeyState::_Released;
	KeyState _key_page_up		: 2 = KeyState::_Released;
	KeyState _key_page_down		: 2 = KeyState::_Released;
	KeyState _key_home			: 2 = KeyState::_Released;
	KeyState _key_end			: 2 = KeyState::_Released;
	KeyState _key_caps_lock		: 2 = KeyState::_Released;
	KeyState _key_scroll_lock	: 2 = KeyState::_Released;
	KeyState _key_num_lock		: 2 = KeyState::_Released;
	KeyState _key_print_screen	: 2 = KeyState::_Released;
	KeyState _key_pause			: 2 = KeyState::_Released;
	KeyState _key_f1			: 2 = KeyState::_Released;
	KeyState _key_f2			: 2 = KeyState::_Released;
	KeyState _key_f3			: 2 = KeyState::_Released;
	KeyState _key_f4			: 2 = KeyState::_Released;
	KeyState _key_f5			: 2 = KeyState::_Released;
	KeyState _key_f6			: 2 = KeyState::_Released;
	KeyState _key_f7			: 2 = KeyState::_Released;
	KeyState _key_f8			: 2 = KeyState::_Released;
	KeyState _key_f9			: 2 = KeyState::_Released;
	KeyState _key_f10			: 2 = KeyState::_Released;
	KeyState _key_f11			: 2 = KeyState::_Released;
	KeyState _key_f12			: 2 = KeyState::_Released;
	KeyState _key_f13			: 2 = KeyState::_Released;
	KeyState _key_f14			: 2 = KeyState::_Released;
	KeyState _key_f15			: 2 = KeyState::_Released;
	KeyState _key_f16			: 2 = KeyState::_Released;
	KeyState _key_f17			: 2 = KeyState::_Released;
	KeyState _key_f18			: 2 = KeyState::_Released;
	KeyState _key_f19			: 2 = KeyState::_Released;
	KeyState _key_f20			: 2 = KeyState::_Released;
	KeyState _key_f21			: 2 = KeyState::_Released;
	KeyState _key_f22			: 2 = KeyState::_Released;
	KeyState _key_f23			: 2 = KeyState::_Released;
	KeyState _key_f24			: 2 = KeyState::_Released;
	KeyState _key_f25			: 2 = KeyState::_Released;
	KeyState _key_pad_0			: 2 = KeyState::_Released;
	KeyState _key_pad_1			: 2 = KeyState::_Released;
	KeyState _key_pad_2			: 2 = KeyState::_Released;
	KeyState _key_pad_3			: 2 = KeyState::_Released;
	KeyState _key_pad_4			: 2 = KeyState::_Released;
	KeyState _key_pad_5			: 2 = KeyState::_Released;
	KeyState _key_pad_6			: 2 = KeyState::_Released;
	KeyState _key_pad_7			: 2 = KeyState::_Released;
	KeyState _key_pad_8			: 2 = KeyState::_Released;
	KeyState _key_pad_9			: 2 = KeyState::_Released;
	KeyState _key_pad_decimal	: 2 = KeyState::_Released;
	KeyState _key_pad_divide	: 2 = KeyState::_Released;
	KeyState _key_pad_multiply	: 2 = KeyState::_Released;
	KeyState _key_pad_subtract	: 2 = KeyState::_Released;
	KeyState _key_pad_add		: 2 = KeyState::_Released;
	KeyState _key_pad_enter		: 2 = KeyState::_Released;
	KeyState _key_pad_equal		: 2 = KeyState::_Released;
	KeyState _key_left_shift	: 2 = KeyState::_Released;
	KeyState _key_left_control	: 2 = KeyState::_Released;
	KeyState _key_left_alt		: 2 = KeyState::_Released;
	KeyState _key_left_super	: 2 = KeyState::_Released;
	KeyState _key_right_shift	: 2 = KeyState::_Released;
	KeyState _key_right_control : 2 = KeyState::_Released;
	KeyState _key_right_alt		: 2 = KeyState::_Released;
	KeyState _key_right_super	: 2 = KeyState::_Released;
	KeyState _key_menu			: 2 = KeyState::_Released;
	FE::int8 _padding[2] = {0}; // Padding to align the structure to 32 bytes
	KeyMode _current_mode = KeyMode::_None;
public:
	keyboard_state() noexcept = default;
	~keyboard_state() noexcept = default;

	FE::boolean is_same_as(const keyboard_state& keys_p) const noexcept;
};

keyboard_state make_key_mask(std::initializer_list<FE::pair<Key, KeyState>> combination_p) noexcept;

class keyboard final
{
public:
	keyboard_callbacks _keyboard_callbacks;
	keyboard_state _keyboard_state;

public:
	keyboard() noexcept = default;
	~keyboard() noexcept = default;
};




enum struct Button
{
	_Left = GLFW_MOUSE_BUTTON_LEFT,
	_Right = GLFW_MOUSE_BUTTON_RIGHT,
	_Middle = GLFW_MOUSE_BUTTON_MIDDLE,
	_4th = GLFW_MOUSE_BUTTON_4,
	_5th = GLFW_MOUSE_BUTTON_5,
	_6th = GLFW_MOUSE_BUTTON_6,
	_7th = GLFW_MOUSE_BUTTON_7,
	_8th = GLFW_MOUSE_BUTTON_8
};

using ButtonState = KeyState;


class mouse_callbacks final
{
public:
	using action = void(*)(Button button_p, ButtonState state_p, KeyMode mode_p);
	using cursor_action = void(*)(FE::float64 x_p, FE::float64 y_p);
	using scroll_action = void(*)(FE::float64 x_offset_p, FE::float64 y_offset_p);

	cursor_action _cursor_position_callback;
	scroll_action _scroll_callback;
	action _button_left, _button_right, _button_middle;
	action	_button_4th, _button_5th, _button_6th, _button_7th, _button_8th;

public:
	mouse_callbacks() noexcept;
	~mouse_callbacks() noexcept = default;
};


class mouse_state final
{
public: // 8 buttons, 16 bits.
	ButtonState _button_left	: 2 = ButtonState::_Released;
	ButtonState _button_right	: 2 = ButtonState::_Released;
	ButtonState _button_middle	: 2 = ButtonState::_Released;
	ButtonState _button_4th		: 2 = ButtonState::_Released;
	ButtonState _button_5th		: 2 = ButtonState::_Released;
	ButtonState _button_6th		: 2 = ButtonState::_Released;
	ButtonState _button_7th		: 2 = ButtonState::_Released;
	ButtonState _button_8th		: 2 = ButtonState::_Released;
	
	var::float64 _cursor_coordinate_x = 0.0;
	var::float64 _cursor_coordinate_y = 0.0;
	var::float64 _scroll_x_offset = 0.0;
	var::float64 _scroll_y_offset = 0.0;
	KeyMode _current_mode = KeyMode::_None;

public:
	mouse_state() noexcept = default;
	~mouse_state() noexcept = default;

	FE::boolean is_same_as(FE::uint16 buttons_p) const noexcept;
};

FE::uint16 make_button_mask(std::initializer_list<FE::pair<Button, ButtonState>> combination_p) noexcept;

class mouse final
{
public:
	mouse_callbacks _mouse_callbacks;
	mouse_state _mouse_state;

public:
	mouse() noexcept = default;
	~mouse() noexcept = default;
};




class context final
{
public:
	keyboard _keyboard;
	mouse _mouse;

	void flush_pressed_keys() noexcept;
};

using context_id = var::uint64;

void __null_keyboard_callback_action(_FE_MAYBE_UNUSED_ Key key_p, _FE_MAYBE_UNUSED_ KeyState state_p, _FE_MAYBE_UNUSED_ KeyMode mode_p, _FE_MAYBE_UNUSED_ FE::int32 scancode_p) noexcept;
void __null_mouse_callback_action(_FE_MAYBE_UNUSED_ Button button_p, _FE_MAYBE_UNUSED_ ButtonState state_p, _FE_MAYBE_UNUSED_ KeyMode mode_p) noexcept;
void __null_cursor_position_callback_action(_FE_MAYBE_UNUSED_ FE::float64 x_p, _FE_MAYBE_UNUSED_ FE::float64 y_p) noexcept;
void __null_scroll_callback_action(_FE_MAYBE_UNUSED_ FE::float64 x_offset_p, _FE_MAYBE_UNUSED_ FE::float64 y_offset_p) noexcept;


END_NAMESPACE
#endif