#ifndef _FE_RENDERER_HPP_
#define _FE_RENDERER_HPP_
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
#include <FE/prerequisites.h>
#pragma warning(disable: 4005)
#include <GLFW/glfw3.h> // Do not redefine the macro defines! It prevents code compilation if /Wx is enabled.

#ifdef _FE_ON_WINDOWS_X86_64_
    #include <FE/d3d11_backend.hpp>
#else
	#include <FE/vulkan_backend.hpp>
#endif




BEGIN_NAMESPACE(FE)


namespace internal::renderer
{
#ifdef _FE_ON_WINDOWS_X86_64_
	using backend = d3d11_backend;
#else
	using backend = vulkan_backend;
#endif
}


struct window_config
{
    const std::pmr::string _title = "Frogman Game";
	GLFWimage* _icon_images = nullptr;
	FE::int32 _icon_image_count = 0;
	FE::int32 _monitor_index = 0;
    FE::boolean _should_enable_vsync = false;
    FE::boolean _is_on_the_top = true;
	FE::boolean _is_cursor_centered = true;
	FE::boolean _should_scale_content_to_monitor_dpi = true;
	FE::boolean _has_border = true;
	FE::uint8 _swap_chain_buffer_count = 3;
	FE::boolean _is_virtual_reality_mode = false;

    FE::int32 _width = 0;
    FE::int32 _height = 0;
    FE::boolean _is_resizable = true;
    FE::boolean _is_maximized = true;
    FE::boolean _is_fullscreen = false;
};


class renderer
{
	friend FE::internal::renderer::backend;
    GLFWwindow* m_window;
    GLFWmonitor* m_primary_monitor;
	GLFWmonitor** m_monitors;
	var::int32 m_monitor_count;
	const GLFWvidmode* m_video_mode;
	window_config m_window_config;
	FE::internal::renderer::backend m_backend; // renderer backend

public:
    renderer(const window_config& window_config_p) noexcept;
	~renderer() noexcept;

	void render_frame() noexcept;
};


END_NAMESPACE
#endif