#ifndef _FE_RENDERER_HXX_
#define _FE_RENDERER_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

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

#include <FE/clock.hxx>
#include <FE/pair.hxx>

#include <FE/framework/processors.hxx>

#include <FE/image.hpp>
#include <FE/shader.hxx>

#pragma warning(disable: 4005)
#include <GLFW/glfw3.h> // Do not redefine the macro defines! It prevents code compilation if /Wx is enabled.

#ifdef _FE_ON_WINDOWS_X86_64_
    #include <FE/d3d11_backend.hxx>
#else
	#include <FE/opengl_es.hxx>
#endif




BEGIN_NAMESPACE(FE)


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


class renderer
{
	friend FE::internal::renderer::backend;

	struct resolution
	{
		var::uint32 _width;
		var::uint32 _height;
	};

public:
	using gpu_info = internal::renderer::backend::gpu_info;

private:
    GLFWwindow* m_window;
    GLFWmonitor* m_primary_monitor;
	GLFWmonitor** m_monitors;
	var::int32 m_monitor_count;
	const GLFWvidmode* m_video_mode;
	window_config m_window_config;
	std::unique_ptr<FE::internal::renderer::backend> m_backend; // renderer backend
	FE::clock m_render_delta_milliseconds;
	var::float64 m_delta_milliseconds;

	std::thread m_renderer_thread;
	std::atomic_bool m_should_exit;
	std::atomic<resolution> m_pending_resolution_change;

	var::int32 m_saved_window_x;
	var::int32 m_saved_window_y;
	var::int32 m_saved_window_width;
	var::int32 m_saved_window_height;

public:
    renderer(const window_config& window_config_p) noexcept;
	~renderer() noexcept;

	void execute() noexcept;
	void terminate() noexcept;

	void render_frame() noexcept;

	_FE_FORCE_INLINE_ FE::boolean should_close() const noexcept
	{
		FE_ASSERT(m_window != nullptr);
		return glfwWindowShouldClose(m_window) == GLFW_TRUE;
	}

	_FE_FORCE_INLINE_ GLFWwindow* get_window() const noexcept { return m_window; }

	void toggle_borderless_fullscreen() noexcept;

	_FE_FORCE_INLINE_ const gpu_info& get_gpu_info() const noexcept { return m_backend->get_gpu_info(); }

	_FE_FORCE_INLINE_ void register_shaders(std::pmr::vector<::FE::internal::renderer::shader>& shaders_p) noexcept
	{
		m_backend->register_shaders(shaders_p);
	}

private:
	static void __on_window_close(GLFWwindow* window_p) noexcept;
	static void __on_window_resize(GLFWwindow* const window_p, FE::int32 new_width_p, FE::int32 new_height_p) noexcept;
	static void __renderer_main(class FE::component_base* const) noexcept;
};


END_NAMESPACE
#endif