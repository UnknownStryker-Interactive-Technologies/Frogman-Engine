#ifndef _FE_RENDERER_WINDOW_HXX_
#define _FE_RENDERER_WINDOW_HXX_
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

#include <FE/image.hpp>
#pragma warning(disable: 4005)
#include <GLFW/glfw3.h>




BEGIN_NAMESPACE(FE)

class renderer;

struct window_config // fields are immutable after window creation; modifying these values will not affect any.
{
	std::pmr::string _title = "Frogman Game";
	std::pmr::vector<std::pmr::string> _icon_paths;
	std::pmr::vector<GLFWimage> _icon_images;

	std::pmr::vector<FE::directory_string> _random_play_video_intro_paths;
	std::pmr::vector<FE::directory_string> _sequential_play_video_intro_paths;
	std::pmr::vector<FE::image> _shader_compile_splash_images;
	var::uint32 _splash_duration_in_seconds;

	FE::uint8 _swap_chain_buffer_count = 3;

	var::boolean _should_enable_vsync = false;
	var::boolean _is_fullscreen = false;
};


class window
{
	struct resolution
	{
		var::uint32 _width;
		var::uint32 _height;
	};

private:
	GLFWwindow* m_window;
	GLFWmonitor* m_primary_monitor;
	GLFWmonitor** m_monitors;
	var::int32 m_monitor_count;
	const GLFWvidmode* m_video_mode;
	window_config m_window_config;

	std::atomic<resolution> m_pending_resolution_change;

	var::int32 m_saved_window_x;
	var::int32 m_saved_window_y;
	var::int32 m_saved_window_width;
	var::int32 m_saved_window_height;

public:
	window(const window_config& window_config_p) noexcept;
	~window() noexcept;

	_FE_FORCE_INLINE_ FE::boolean should_close() const noexcept
	{
		FE_ASSERT(m_window != nullptr);
		return glfwWindowShouldClose(m_window) == GLFW_TRUE;
	}

	_FE_FORCE_INLINE_ const window_config& get_window_config() const noexcept { return m_window_config; }
	_FE_FORCE_INLINE_ GLFWwindow* get_window() const noexcept { return m_window; }
	void toggle_borderless_fullscreen() noexcept;

private:
	static void __on_window_close(GLFWwindow* window_p) noexcept;
	static void __on_window_resize(GLFWwindow* const window_p, FE::int32 new_width_p, FE::int32 new_height_p) noexcept;
};


END_NAMESPACE
#endif