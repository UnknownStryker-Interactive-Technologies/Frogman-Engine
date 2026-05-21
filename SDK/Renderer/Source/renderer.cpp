#include <FE/renderer.hxx>
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

#include <FE/blacklist_evaluator.hxx>
#include <FE/clock.hxx>
#include <FE/engine.hpp>
#include <FE/image.hpp>
#include <FE/video_player.hpp>

#include <FE/framework/game_processor.hxx>

#include <atomic>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_dx11.h>

#include <taskflow.hpp>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // for loading icons & images




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
		m_should_exit(false),
		m_pending_resolution_change(),

		m_saved_window_x(0),
		m_saved_window_y(0),
		m_saved_window_width(0),
		m_saved_window_height(0)
{
	FE_EXIT_IF(glfwInit() == GLFW_FALSE, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: The GLFW Window initialization failed.");
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // We do not want to create an OpenGL context
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE); // the D3D has its own buffering system
	glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE); // Make window visible upon creation
	
	m_monitors = glfwGetMonitors(&m_monitor_count);
	FE_EXIT_IF(m_monitors == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: Could not detect monitors; the GLFW Monitors retrieval failed.");

	m_primary_monitor = m_monitors[0];
	FE_EXIT_IF(m_primary_monitor == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: Could not detect a monitor; the GLFW Primary Monitor retrieval failed.");

	m_video_mode = glfwGetVideoMode(m_primary_monitor);
	FE_EXIT_IF(m_video_mode == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_InitializationFailure, "Frogman Engine Renderer Initialization Failure: The GLFW Video Mode retrieval failed.");
	glfwWindowHint(GLFW_RED_BITS, m_video_mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, m_video_mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, m_video_mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, m_video_mode->refreshRate);
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE); // Disable automatic content scaling based on monitor DPI


	if (m_window_config._is_fullscreen == true)
	{
		glfwWindowHint(GLFW_FLOATING, GLFW_TRUE); // Make window always on top
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Disable window decorations (title bar, borders, etc.)
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Do not allow window resizing
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE); // Start window maximized

		m_window = glfwCreateWindow(m_video_mode->width, m_video_mode->height, m_window_config._title.c_str(), m_primary_monitor, nullptr);
		// glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwWindowHint(GLFW_FLOATING, GLFW_FALSE); // Make window always not on top
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); // Enable window decorations (title bar, borders, etc.)
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // Allow window resizing
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE); // Start window not maximized

		m_window = glfwCreateWindow(m_video_mode->width, m_video_mode->height, m_window_config._title.c_str(), nullptr, nullptr);
	}


	FE_EXIT_IF(m_window == nullptr, FE::ErrorCode::_FatalRendererError_5XX_GLFW_WindowCreationFailure, "Frogman Engine Renderer Initialization Failure: The GLFW Window creation failed.");
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GLFW_TRUE); // Enable sticky keys input mode; the value remains until retrieved.
	
	if (m_window_config._icon_images.empty() == false)
	{
		glfwSetWindowIcon(m_window, (int)m_window_config._icon_images.size(), m_window_config._icon_images.data());

		for (GLFWimage& image : m_window_config._icon_images)
		{
			if (image.pixels == nullptr)
			{
				continue;
			}

			stbi_image_free(image.pixels);
			image.pixels = nullptr;
		}
	}

	// glfwMakeContextCurrent(m_window);
	m_backend = std::make_unique<FE::internal::renderer::backend>(this);

	glfwSetWindowCloseCallback(m_window, &__on_window_close);
	glfwSetFramebufferSizeCallback(m_window, &__on_window_resize);
	
	glfwSetKeyCallback(m_window, &FE::engine::key_callback);
	glfwSetMouseButtonCallback(m_window, &FE::engine::mouse_button_callback);
	glfwSetCursorPosCallback(m_window, &FE::engine::cursor_position_callback);
	glfwSetScrollCallback(m_window, &FE::engine::scroll_callback);
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

void renderer::render_frame() noexcept
{
	resolution l_pending_resolution = m_pending_resolution_change.exchange({}, std::memory_order_acq_rel);
	if ((l_pending_resolution._width > 0) && (l_pending_resolution._height > 0))
	{
		m_backend->resize_swap_chain_buffers(l_pending_resolution._width, l_pending_resolution._height);
	}

	m_render_delta_milliseconds.start_clock();

	m_backend->begin_frame();

	m_backend->end_frame();

	m_render_delta_milliseconds.end_clock();
	m_delta_milliseconds = m_render_delta_milliseconds.get_delta_milliseconds();
}


void FE::renderer::__on_window_close(GLFWwindow* window_p) noexcept
{
	FE::engine::get_engine().get_renderer(FE::engine::auth{}).m_should_exit.store(true, std::memory_order_release);
	FE::engine::get_engine().terminate_all_processors();
	glfwSetWindowShouldClose(window_p, GLFW_TRUE);
}

void renderer::__on_window_resize(_FE_MAYBE_UNUSED_ GLFWwindow* const window_p, FE::int32 new_width_p, FE::int32 new_height_p) noexcept
{
	if ((new_width_p <= 0) || (new_height_p <= 0))
	{
		return; // Ignore minimize events
	}

	FE::engine::get_engine().get_renderer(FE::engine::auth{}).m_pending_resolution_change.store( { ._width = (FE::uint32)new_width_p, ._height = (FE::uint32)new_height_p },
																			std::memory_order_release);
}

void FE::renderer::__renderer_main(class FE::component_base* const) noexcept
{
	auto& l_engine = FE::engine::get_engine();
	auto& l_renderer = l_engine.get_renderer(FE::engine::auth{});
	auto& l_shaders = l_engine.get_shaders(FE::engine::auth{});

	if (l_renderer.m_window_config._is_fullscreen == true)
	{
		l_renderer.toggle_borderless_fullscreen();
	}

	tf::Executor l_executor;
	tf::Taskflow l_taskflow; // Evaluate Permutation Blacklist
	for (var::int32 n = 0; n < l_shaders.size(); ++n)
	{
		l_taskflow.emplace
		(
			[&l_shaders, n]()
			{
				FE::internal::__filter_shader_macro_combinations(l_shaders[n]);
			}
		);
	}
	{
		auto l_future = l_executor.run(l_taskflow);
		HWND l_hwnd = glfwGetWin32Window(l_renderer.m_window); 	// --- intro videos (MF owns the HWND's swap chain in this scope) -------
		FE::video_player l_intro(l_hwnd);

		const auto& l_random_list = l_engine.get_project_config()._window_config._random_play_video_intro_paths;
		const auto& l_sequential_list = l_engine.get_project_config()._window_config._sequential_play_video_intro_paths;

		if (l_random_list.empty() == false)
		{
			FE::random_integer<var::uint64> l_rng;
			FE::uint64 l_idx = l_rng.ranged_random_integer(0, l_random_list.size() - 1);
			l_intro.play(l_random_list[l_idx].c_str());
		}

		for (const auto& l_path : l_sequential_list)
		{
			l_intro.play(l_path.c_str());
		}

		l_future.wait();
		l_taskflow.clear();
	} // l_intro destructs → MF::Shutdown → HWND free for the D3D backend
	

	{
		var::uint64 l_total_permutations = 0;
		std::atomic_uint64_t l_permutations_compiled = 0;

		for (var::int32 n = 0; n < l_shaders.size(); ++n)
		{
			l_total_permutations += l_shaders[n]._macro_combinations.size();

			l_taskflow.emplace
			(
				[&l_shaders, &l_permutations_compiled, n]()
				{
					l_shaders[n].compile();
					l_permutations_compiled.fetch_add(	l_shaders[n]._permutations.size(),
														std::memory_order_acq_rel
														);
				}
			);
		}

		l_executor.run(l_taskflow);
		
		for (FE::image& image : l_engine.get_project_config(FE::engine::auth{})._window_config._shader_compile_splash_images)
		{
			image.load_to_renderer(l_renderer.m_backend->get_device());
		}
		
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOther(l_renderer.m_window, false);
		ImGui_ImplDX11_Init(l_renderer.m_backend->get_device(), l_renderer.m_backend->get_context());

		FE::clock l_shader_compile_splash_duration;
		auto l_splash_iterator = l_engine.get_project_config()._window_config._shader_compile_splash_images.cbegin();

		l_shader_compile_splash_duration.start_clock();
		while (l_total_permutations > l_permutations_compiled.load(std::memory_order_acquire))
		{
			l_renderer.m_backend->begin_frame();

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplGlfw_NewFrame();  
			ImGui::NewFrame();

			l_shader_compile_splash_duration.end_clock();

			FE::uint32 l_duration_seconds = (FE::uint32)(l_shader_compile_splash_duration.get_delta_milliseconds() / 1000.0);
			if (l_duration_seconds >= l_engine.get_project_config()._window_config._splash_duration_in_seconds)
			{
				++l_splash_iterator;
				if (l_splash_iterator == l_engine.get_project_config()._window_config._shader_compile_splash_images.cend())
				{
					l_splash_iterator = l_engine.get_project_config()._window_config._shader_compile_splash_images.cbegin();
				}
				l_shader_compile_splash_duration.start_clock();
			}

			const FE::image& l_image = *l_splash_iterator;
			ImGuiIO& l_io = ImGui::GetIO();

			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(l_io.DisplaySize);
			ImGui::Begin
			(
				"##shader_compile_splash", nullptr,
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoDecoration
			);

			ImGui::Image(l_image.shader_resource_view(), l_io.DisplaySize);

			FE::float32 l_progress = (FE::float32)l_permutations_compiled.load(std::memory_order_acquire) / (FE::float32)l_total_permutations;

			ImVec2 l_window_size = ImGui::GetWindowSize();
			constexpr FE::float32 l_corner_padding = 50.0f;
			ImVec2 l_UI_pos{ l_corner_padding, l_window_size.y - l_corner_padding };
			ImGui::SetCursorPos(l_UI_pos);

			constexpr FE::float32 l_bar_thickness = 25.0f;
			ImVec2 l_bar_size{ l_window_size.x - (l_corner_padding*2), l_bar_thickness };
			ImGui::ProgressBar(l_progress, l_bar_size);

			ImGui::End();
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			l_renderer.m_backend->end_frame();
		}

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}


	while (l_renderer.m_should_exit.load(std::memory_order_acquire) == false) 	
	{
		l_renderer.render_frame();
	}
}


void renderer::toggle_borderless_fullscreen() noexcept
{
	if (m_window_config._is_fullscreen == false)
	{
		// Save windowed-mode state
		glfwGetWindowPos(m_window, &m_saved_window_x, &m_saved_window_y);
		glfwGetWindowSize(m_window, &m_saved_window_width, &m_saved_window_height);


		// Position to cover the target monitor
		int l_monitor_x = 0;
		int l_monitor_y = 0;
		glfwGetMonitorPos(m_primary_monitor, &l_monitor_x, &l_monitor_y);
		

		glfwSetWindowAttrib(m_window, GLFW_FLOATING, GLFW_TRUE);
		glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_FALSE);
		glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, GLFW_FALSE);
		glfwSetWindowAttrib(m_window, GLFW_MAXIMIZED, GLFW_TRUE);
		
		FE::engine::get_engine().get_renderer(FE::engine::auth{}).m_pending_resolution_change.store({._width = (FE::uint32)m_video_mode->width, ._height = (FE::uint32)m_video_mode->height},
																				std::memory_order_release);
		
		glfwSetWindowMonitor(m_window, nullptr,
			l_monitor_x, l_monitor_y,
			m_video_mode->width, m_video_mode->height,
			m_video_mode->refreshRate);

		m_window_config._is_fullscreen = true;
		return;
	}

	glfwSetWindowAttrib(m_window, GLFW_FLOATING, GLFW_FALSE);
	glfwSetWindowAttrib(m_window, GLFW_DECORATED, GLFW_TRUE);
	glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, GLFW_TRUE);
	glfwSetWindowAttrib(m_window, GLFW_MAXIMIZED, GLFW_FALSE);

	FE::engine::get_engine().get_renderer(FE::engine::auth{}).m_pending_resolution_change.store(	{ ._width = (FE::uint32)m_saved_window_width, ._height = (FE::uint32)m_saved_window_height },
																			std::memory_order_release);

	glfwSetWindowMonitor(m_window, nullptr,
		m_saved_window_x, m_saved_window_y,
		m_saved_window_width, m_saved_window_height,
		m_video_mode->refreshRate);

	m_window_config._is_fullscreen = false;
}


END_NAMESPACE
