#include <FE/renderer.hxx>
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
#include <FE/engine.hpp>

#include <FE/framework/game_processor.hxx>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // for loading icons

#include <FE/video_player.hpp>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>




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
		m_delta_milliseconds(0.0),
		m_renderer_thread(),
		m_should_exit(false)
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
		*const_cast<var::uint32*>( &(m_window_config._width) ) = m_video_mode->width;
	}
	if (m_window_config._height == 0)
	{
		*const_cast<var::uint32*>( &(m_window_config._height) ) = m_video_mode->height;
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
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	}
	else
	{
		m_window = glfwCreateWindow(m_window_config._width, m_window_config._height, m_window_config._title.c_str(), nullptr, nullptr);
	}


	FE_EXIT_IF(m_window == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_WindowCreationFailure, "Frogman Engine Renderer Initialization Failure: The GLFW Window creation failed.");
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE); // Enable sticky keys input mode; the value remains until retrieved.
	
	if (m_window_config._icon_image.empty() == false)
	{
		glfwSetWindowIcon(m_window, (int)m_window_config._icon_image.size(), m_window_config._icon_image.data());

		for (GLFWimage& image : m_window_config._icon_image)
		{
			stbi_image_free(image.pixels);
		}
	}

	glfwMakeContextCurrent(m_window);
	m_backend = std::make_unique<FE::internal::renderer::backend>(this);

	glfwSetWindowCloseCallback(m_window, &__on_window_close);
	glfwSetKeyCallback(m_window, &FE::engine::__key_callback);
	glfwSetMouseButtonCallback(m_window, &FE::engine::__mouse_button_callback);
	glfwSetCursorPosCallback(m_window, &FE::engine::__cursor_position_callback);
	glfwSetScrollCallback(m_window, &FE::engine::__scroll_callback);
}

renderer::~renderer() noexcept
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}


void renderer::execute() noexcept
{
	m_renderer_thread = std::thread(__renderer_main, nullptr);
}

void renderer::terminate() noexcept
{
	m_renderer_thread.join();
}


void FE::renderer::__on_window_close(GLFWwindow* window_p) noexcept
{
	FE::engine::get_engine().m_renderer->m_should_exit.store(true, std::memory_order_release);
	FE::engine::get_engine().m_game_processor->terminate();
	// FE::engine::get_engine().m_processors->terminate();	
	glfwSetWindowShouldClose(window_p, GLFW_TRUE);
}

void FE::renderer::__renderer_main(class FE::component_base* const) noexcept
{
	auto& l_engine = FE::engine::get_engine();
	auto& l_renderer = *l_engine.m_renderer;
	std::unique_ptr<FE::internal::renderer::backend> l_backend = std::move(l_renderer.m_backend);

	HWND l_hwnd = glfwGetWin32Window(l_renderer.m_window);

	// --- intro videos (MF owns the HWND's swap chain in this scope) -------
	{
		FE::video_player l_intro(l_hwnd);

		auto& l_random_list = l_engine.get_project_config()._window_config._random_play_video_intro_paths;
		auto& l_sequential_list = l_engine.get_project_config()._window_config._sequential_play_video_intro_paths;

		if (l_random_list.empty() == false)
		{
			FE::random_integer<var::uint64> l_rng;
			FE::uint64 l_idx = l_rng.ranged_random_integer(0, l_random_list.size() - 1);
			l_intro.play(l_random_list[l_idx].c_str());
		}

		for (auto& l_path : l_sequential_list)
		{
			l_intro.play(l_path.c_str());
		}
	} // l_intro destructs → MF::Shutdown → HWND free for the D3D backend
	
	// --- game render backend takes over the HWND --------------------------
	l_renderer.m_backend = std::move(l_backend);

	while (l_renderer.m_should_exit.load(std::memory_order_acquire) == false)
	{
		l_renderer.render_frame();
	}
}


END_NAMESPACE