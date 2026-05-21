#ifndef _FROGMAN_ENGINE_WINDOW_CONTROL_HPP_
#define _FROGMAN_ENGINE_WINDOW_CONTROL_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
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
#pragma warning(disable: 4005)
#include <GLFW/glfw3.h>
#include <FE/engine.hpp>




BEGIN_NAMESPACE(FE)


enum struct CursorMode : FE::int32
{
	_VisibleAndFree = GLFW_CURSOR_NORMAL,
	_Invisible = GLFW_CURSOR_HIDDEN,
	_Gaming = GLFW_CURSOR_DISABLED,
	_VisibleAndConfined = GLFW_CURSOR_CAPTURED
};

_FE_FORCE_INLINE_ static void set_window_cursor_mode(CursorMode mode_p) noexcept
{
	glfwSetInputMode(FE::engine::get_engine().get_renderer().get_window(), GLFW_CURSOR, static_cast<FE::int32>(mode_p));
}


END_NAMESPACE
#endif