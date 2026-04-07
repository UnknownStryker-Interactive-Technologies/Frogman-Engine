#ifndef _FE_RENDERER_HXX_
#define _FE_RENDERER_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#pragma warning(disable: 4005)
#include <GLFW/glfw3.h> // Do not redefine the macro defines! It prevents code compilation if /Wx is enabled.

#ifdef _FE_ON_WINDOWS_X86_64_
    #include <FE/d3d11_backend.hxx>
#else
	#include <FE/vulkan_backend.hxx>
#endif

#include <FE/clock.hxx>
#include <FE/framework/processors.hxx>




BEGIN_NAMESPACE(FE)


namespace internal::renderer
{
#ifdef _FE_ON_WINDOWS_X86_64_
	using backend = d3d11_backend;
#else
	using backend = vulkan_backend;
#endif
}


struct window_config // fields are immutable after window creation; modifying these values will not affect any.
{
    std::pmr::string _title = "Frogman Game";
	std::pmr::string _icon_path = "\0";
	std::optional<GLFWimage> _icon_image;
	FE::int32 _monitor_index = 0;
    FE::boolean _should_enable_vsync = false;
    FE::boolean _is_on_the_top = false;
	FE::boolean _should_scale_content_to_monitor_dpi = true;
	FE::boolean _has_border = true;
	FE::uint8 _swap_chain_buffer_count = 3;
	_FE_MAYBE_UNUSED_ FE::boolean _is_virtual_reality_mode = false;
	FE::boolean _should_enable_hdr = false;

    FE::int32 _width = 0;
    FE::int32 _height = 0;
    FE::boolean _is_resizable = true;
    FE::boolean _is_maximized = true;
    var::boolean _is_fullscreen = false;
};


class renderer
{
	friend FE::internal::renderer::backend;
public:
#ifdef _FE_ON_WINDOWS_X86_64_
	using view_port = D3D11_VIEWPORT;
#else
	using view_port = VkViewport;
#endif
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

public:
    renderer(const window_config& window_config_p) noexcept;
	~renderer() noexcept;

	void execute() noexcept;
	void terminate() noexcept;

	_FE_FORCE_INLINE_ void render_frame() noexcept 
	{ 
		m_render_delta_milliseconds.start_clock();
		m_backend->render_frame(); 
		m_render_delta_milliseconds.end_clock();
		m_delta_milliseconds = m_render_delta_milliseconds.get_delta_milliseconds();
	}

	_FE_FORCE_INLINE_ FE::boolean should_close() const noexcept
	{
		FE_ASSERT(m_window != nullptr);
		return glfwWindowShouldClose(m_window) == GLFW_TRUE;
	}

	_FE_FORCE_INLINE_ GLFWwindow* get_window() const noexcept { return m_window; }

private:
	static void __on_window_close(GLFWwindow* window_p) noexcept;
	static void __renderer_main(class FE::component_base* const) noexcept;
};


END_NAMESPACE
#endif