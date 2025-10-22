#include <FE/renderer.hpp>
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




BEGIN_NAMESPACE(FE)


renderer::renderer(const window_config& window_config_p) noexcept
	:	m_window(),
		m_primary_monitor(),
		m_monitors(),
		m_monitor_count(1),
		m_video_mode(),
		m_window_config(window_config_p),
		m_backend(),
		m_render_delta_milliseconds(),
		m_detla_milliseconds(0.0)
{
	FE_EXIT_IF(glfwInit() == GLFW_FALSE, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: The GLFW Window initialization failed.");
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // We do not want to create an OpenGL context
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE); // the D3D has its own buffering system
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE); // Make window visible upon creation
	
	m_monitors = glfwGetMonitors(&m_monitor_count);
	FE_EXIT_IF(m_monitors == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: Could not detect monitors; the GLFW Monitors retrieval failed.");
	FE_ASSERT(window_config_p._monitor_index < m_monitor_count);
	m_primary_monitor = m_monitors[window_config_p._monitor_index];
	FE_EXIT_IF(m_primary_monitor == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: Could not detect a monitor; the GLFW Primary Monitor retrieval failed.");

	m_video_mode = glfwGetVideoMode(m_primary_monitor);
	FE_EXIT_IF(m_video_mode == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: The GLFW Video Mode retrieval failed.");
	glfwWindowHint(GLFW_RED_BITS, m_video_mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, m_video_mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, m_video_mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, m_video_mode->refreshRate);

	if (m_window_config._width == 0)
	{
		*const_cast<var::int32*>( &(m_window_config._width) ) = m_video_mode->width;
	}
	if (m_window_config._height == 0)
	{
		*const_cast<var::int32*>( &(m_window_config._height) ) = m_video_mode->height;
	}

	if (m_window_config._is_fullscreen == true)
	{
		*const_cast<var::boolean*>(&(m_window_config._is_on_the_top)) = true;
		*const_cast<var::boolean*>(&(m_window_config._should_scale_content_to_monitor_dpi)) = true;
		*const_cast<var::boolean*>(&(m_window_config._has_border)) = false;
		*const_cast<var::boolean*>(&(m_window_config._is_virtual_reality_mode)) = false;
		*const_cast<var::boolean*>(&(m_window_config._is_resizable)) = false;
		*const_cast<var::boolean*>(&(m_window_config._is_maximized)) = true;
	}

	glfwWindowHint(GLFW_FLOATING, m_window_config._is_on_the_top); // Make window always on top
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, m_window_config._should_scale_content_to_monitor_dpi); // Enable automatic content scaling based on monitor DPI
	glfwWindowHint(GLFW_DECORATED, m_window_config._has_border); // Enable window decorations (title bar, borders, etc.)

	glfwWindowHint(GLFW_RESIZABLE, m_window_config._is_resizable); // Allow window resizing
	glfwWindowHint(GLFW_MAXIMIZED, m_window_config._is_maximized); // Start window maximized
	
	if (m_window_config._is_fullscreen == true)
	{
		m_window = glfwCreateWindow(m_window_config._width, m_window_config._height, m_window_config._title.c_str(), m_primary_monitor, nullptr);
	}
	else
	{
		m_window = glfwCreateWindow(m_window_config._width, m_window_config._height, m_window_config._title.c_str(), nullptr, nullptr);
	}


	FE_EXIT_IF(m_window == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_WindowCreationFailure, "Frogman Engine Renderer Initialization Failure: The GLFW Window creation failed.");
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE); // Enable sticky keys input mode; the value remains until retrieved.
	
	if (m_window_config._icon_images == nullptr)
	{
		glfwSetWindowIcon(m_window, 0, nullptr);
	}
	else
	{
		glfwSetWindowIcon(m_window, m_window_config._icon_image_count, m_window_config._icon_images);
	}

	glfwMakeContextCurrent(m_window);
	m_backend = std::make_unique<FE::internal::renderer::backend>(this);
}

renderer::~renderer() noexcept
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}


END_NAMESPACE