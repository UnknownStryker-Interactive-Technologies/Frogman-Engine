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
#include <FE/engine.hpp>

#include <FE/algorithm/string.hxx>
#include <FE/app.hpp>
#include <FE/blacklist_evaluator.hxx>
#include <FE/framework/reflection.hxx>
#include <FE/memory_resource.hxx>
#include <FE/processors.hxx>
#include <FE/random.hxx>
#include <FE/video_player.hpp>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_dx11.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h> // for loading icons




FE::engine_program_options::engine_program_options(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
	:	base(argc_p, argv_p),
		m_enable_fullscreen( "-enable-fullscreen", false ),
		m_enable_vsync("-enable-vsync", false),
		m_recompile_shaders("-recompile-shaders", false)
{
	for (var::int32 i = 0; i < argc_p; ++i)
	{
		if (algorithm::string::find_the_first<var::ASCII>(argv_p[i], m_enable_fullscreen._first) != std::nullopt)
		{
			m_enable_fullscreen._second = true;
			continue;
		}

		if (algorithm::string::find_the_first<var::ASCII>(argv_p[i], m_enable_vsync._first) != std::nullopt)
		{
			m_enable_vsync._second = true;
			continue;
		}

		if (algorithm::string::find_the_first<var::ASCII>(argv_p[i], m_recompile_shaders._first) != std::nullopt)
		{
			m_recompile_shaders._second = true;
			continue;
		}
	}
}

FE::boolean FE::engine_program_options::is_fullscreen_enabled() const noexcept
{
	return m_enable_fullscreen._second;
}

FE::ASCII* FE::engine_program_options::view_enable_fullscreen_title() const noexcept
{
	return m_enable_fullscreen._first;
}

FE::boolean FE::engine_program_options::is_vsync_enabled() const noexcept
{
	return m_enable_vsync._second;
}

FE::ASCII* FE::engine_program_options::view_enable_vsync_title() const noexcept
{
	return m_enable_vsync._first;
}

FE::boolean FE::engine_program_options::is_recompile_shaders_enabled() const noexcept
{
	return m_recompile_shaders._second;
}

FE::ASCII* FE::engine_program_options::view_recompile_shaders_title() const noexcept
{
	return m_recompile_shaders._first;
}


FE::engine::engine(std::unique_ptr<engine_program_options> options_p) noexcept
	:	base(std::move(options_p)),
		m_runtime_path(framework_base::get_large_memory_resource()),
		m_game_root_directory(framework_base::get_large_memory_resource()),
		m_froggy_path(framework_base::get_large_memory_resource()),
		m_froggy(framework_base::get_large_memory_resource()),

		m_engine_info(),
		m_project_config(),

		m_processors(),
		m_renderer(),
		m_window(),
		m_game(),
		m_worlds(framework_base::get_large_memory_resource()),
		m_current_world(),
		m_should_tick(true),
		m_runtime_clock()
{
	m_runtime_path.reserve(_MAX_PATH_LENGTH_);
	FE::get_directory_of_current_executable(m_runtime_path.data(), (FE::uint32)m_runtime_path.capacity());
	m_runtime_path = m_runtime_path.c_str();
	m_runtime_path.shrink_to_fit();

	std::pmr::string::size_type l_pos = m_runtime_path.rfind('\\');
	FE_ASSERT(l_pos != std::pmr::string::npos, "Failed to find last occurrence of '\\' in current executable path.");
	m_froggy_path = m_runtime_path;
	++l_pos;
	FE::directory_string l_project_name = m_runtime_path.substr(l_pos, m_runtime_path.size() - l_pos);
	m_froggy_path.erase(l_pos, l_project_name.length());
	l_pos = l_project_name.rfind(FE_TEXT(.exe));
	FE_ASSERT(l_pos != std::pmr::string::npos, "Failed to find last occurrence of '.exe' in current executable path.");
	l_project_name.erase(l_pos, std::strlen(".exe")); // 4 is length of ".exe"

	l_pos = m_froggy_path.rfind(l_project_name.c_str());
	FE_ASSERT(l_pos != std::pmr::string::npos, "Failed to find last occurrence of project name in current executable path.");

	m_froggy_path.erase(l_pos + l_project_name.length(), m_froggy_path.length() - (l_pos + l_project_name.length()));
	m_game_root_directory = m_froggy_path;
	m_froggy_path += FE_TEXT(\\);
	m_froggy_path += l_project_name;
	m_froggy_path += FE_TEXT(.froggy);

	m_project_config._window_config._should_enable_vsync = get_program_options().is_vsync_enabled();
	m_project_config._window_config._is_fullscreen = get_program_options().is_fullscreen_enabled();
}

FE::engine::~engine() noexcept
{}


void FE::engine::terminate_all_processors() noexcept
{
	m_should_tick.store(false, std::memory_order_release);
	m_processors->terminate();
}


FE::int32 FE::engine::launch(FE::int32 argc_p, FE::ASCII** argv_p)
{
	(argc_p);
	(argv_p);

	__load_reflection_data();
	__read_froggy();

	m_processors = FE::make_owner<FE::processors>(framework_base::get_large_memory_resource(),
		count_processors(),
		m_project_config._fibers_per_thread,
		m_project_config._fiber_stack_size);

	m_window = FE::make_owner<FE::window>(framework_base::get_large_memory_resource(), m_project_config._window_config);

	m_renderer = FE::make_owner<FE::renderer>(framework_base::get_large_memory_resource(), m_processors, m_window);

	m_game = FE::make_owner<FE::game>(framework_base::get_large_memory_resource());
	return 0;
}

FE::int32 FE::engine::run()
{
	auto& l_shader_headers = m_renderer->get_shader_headers();
	auto& l_shaders = m_renderer->get_shaders();

	if (m_window->get_window_config()._is_fullscreen == true)
	{
		m_window->toggle_borderless_fullscreen();
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

	concurrency::concurrent_unordered_map<FE::directory_string, std::pmr::list<FE::internal::renderer::hlsl_token>> l_token_lists;
	for (auto it = l_shader_headers.begin(); it != l_shader_headers.end(); ++it)
	{
		l_taskflow.emplace
		(
			[it, &l_token_lists]()
			{
				try
				{
					auto l_list = FE::internal::renderer::__tokenize_hlsl(it->second._header_buffer);
					l_token_lists[it->first] = std::move(l_list);
				}
				catch (_FE_MAYBE_UNUSED_ const FE::internal::renderer::HlslTokenizerError& err)
				{
					FE_LOG(FE::log::Severity::_Warning, "Failed to tokenize the HLSL shader header file at ${%s@0}; skipping this file.\nError code: ${%d@1}", it->first.c_str(), &err);
					return;
				}
			}
		);
	}

	for (var::int32 n = 0; n < l_shaders.size(); ++n)
	{
		l_taskflow.emplace
		(
			[this, &l_shaders, &l_token_lists, n]()
			{
				std::fstream l_file_stream(l_shaders[n]._source_path.c_str(), std::ios::in | std::ios::binary);
				FE::fstream_guard l_file_guard(l_file_stream);

				std::pmr::string l_buffer(get_large_memory_resource());
				l_file_stream.seekg(0, std::ios::end);
				l_buffer.resize(l_file_stream.tellg());
				l_file_stream.seekg(0, std::ios::beg);

				l_file_stream.read(l_buffer.data(), l_buffer.size());

				try
				{
					auto l_list = FE::internal::renderer::__tokenize_hlsl(l_buffer);
					l_token_lists[l_shaders[n]._source_path] = std::move(l_list);
				}
				catch (_FE_MAYBE_UNUSED_ const FE::internal::renderer::HlslTokenizerError& err)
				{
					FE_LOG(FE::log::Severity::_Warning, "Failed to tokenize the HLSL shader header file at ${%s@0}; skipping this file.\nError code: ${%d@1}", l_shaders[n]._source_path.c_str(), &err);
					return;
				}
			}
		);
	}

	{
		auto l_future = l_executor.run(l_taskflow); // run all queued tasks

		HWND l_hwnd = glfwGetWin32Window(m_window->get_window()); 	// --- intro videos (MF owns the HWND's swap chain in this scope) -------
		FE::video_player l_intro(l_hwnd);

		const auto& l_random_list = get_project_config()._window_config._random_play_video_intro_paths;
		const auto& l_sequential_list = get_project_config()._window_config._sequential_play_video_intro_paths;

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
	}	// l_intro destructs → MF::Shutdown → HWND free for the D3D backend

	FE::internal::renderer::__build_and_traverse_include_dependency_graph(l_token_lists, l_shader_headers, l_shaders);

	{
		var::uint64 l_total_permutations = 0;
		std::atomic_uint64_t l_permutations_compiled = 0;

		for (var::int32 n = 0; n < l_shaders.size(); ++n)
		{
			l_total_permutations += l_shaders[n]._macro_combinations.size();

			l_taskflow.emplace
			(
				[this, &l_shaders, &l_permutations_compiled, n]()
				{
					l_shaders[n].compile(get_program_options().is_recompile_shaders_enabled());
					l_permutations_compiled.fetch_add(l_shaders[n]._permutations.size(),
						std::memory_order_acq_rel
					);
				}
			);
		}

		l_executor.run(l_taskflow);

		for (FE::image& image : m_project_config._window_config._shader_compile_splash_images)
		{
			image.load_to_renderer(m_renderer->get_backend_device());
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOther(m_window->get_window(), false);
		ImGui_ImplDX11_Init(m_renderer->get_backend_device(), m_renderer->get_backend_device_context());

		FE::clock l_shader_compile_splash_duration;
		auto l_splash_iterator = get_project_config()._window_config._shader_compile_splash_images.cbegin();

		l_shader_compile_splash_duration.start_clock();
		while (l_total_permutations > l_permutations_compiled.load(std::memory_order_acquire))
		{
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			l_shader_compile_splash_duration.end_clock();

			FE::uint32 l_duration_seconds = (FE::uint32)(l_shader_compile_splash_duration.get_delta_milliseconds() / 1000.0);
			if (l_duration_seconds >= get_project_config()._window_config._splash_duration_in_seconds)
			{
				++l_splash_iterator;
				if (l_splash_iterator == get_project_config()._window_config._shader_compile_splash_images.cend())
				{
					l_splash_iterator = get_project_config()._window_config._shader_compile_splash_images.cbegin();
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
			ImVec2 l_bar_size{ l_window_size.x - (l_corner_padding * 2), l_bar_thickness };
			ImGui::ProgressBar(l_progress, l_bar_size);

			ImGui::End();
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}

		m_renderer->register_shaders(l_shaders);

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}


	FE::task l_renderer_main = {};
	l_renderer_main._system = &FE::renderer::__main;
	l_renderer_main._task_type = TaskPriority::_Critical;
	l_renderer_main._world = nullptr;

	m_processors->activate();

	m_processors->schedule_task(l_renderer_main);


	__game_main();

	return 0;
}

FE::int32 FE::engine::shutdown()
{
	terminate_all_processors();
	return 0;
}


void FE::engine::__game_main() noexcept
{
	FE::world::create_world(0);
	FE::world::enter_world(0);

	auto l_current_world = m_current_world;

	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_EngineInitialization))
	{
		sys(*l_current_world);
	}
	
	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_GameInstanceInitialization))
	{
		sys(*l_current_world);
	}

	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldInitialization))
	{
		sys(*l_current_world);
	}

	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldDefaultEntityInitialization))
	{
		sys(*l_current_world);
	}




	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_GameInstanceBegin))
	{
		sys(*l_current_world);
	}

	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldBegin))
	{
		sys(*l_current_world);
	}




	FE::clock l_delta_clock = {};
	FE::clock l_physics_delta_clock = {};
	l_physics_delta_clock.start_clock();
	var::uint64 l_frame_counter = 0;
	while (m_should_tick.load(std::memory_order_acquire))
	{
		l_delta_clock.start_clock();
		glfwPollEvents();

		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_PreGameInstanceTick))
		{
			sys(*l_current_world);
		}
		
		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_GameInstanceTick))
		{
			sys(*l_current_world);
		}

		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_PostGameInstanceTick))
		{
			sys(*l_current_world);
		}




		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_PreWorldTick))
		{
			sys(*l_current_world);
		}

		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldTick))
		{
			sys(*l_current_world);
		}

		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_PostWorldTick))
		{
			sys(*l_current_world);
		}




		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_PreEntityTick))
		{
			sys(*l_current_world);
		}

		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_EntityTick))
		{
			sys(*l_current_world);
		}

		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_PostEntityTick))
		{
			sys(*l_current_world);
		}




		l_physics_delta_clock.end_clock();
		if (l_physics_delta_clock.get_delta_milliseconds() >= l_current_world->fixed_physics_delta_milliseconds())
		{
			l_physics_delta_clock.start_clock();
			for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_PrePhysics))
			{
				sys(*l_current_world);
			}

			for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_StartPhysics))
			{
				sys(*l_current_world);
			}

			for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_Physics))
			{
				sys(*l_current_world);
			}

			for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_EndPhysics))
			{
				sys(*l_current_world);
			}

			for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_PostPhysics))
			{
				sys(*l_current_world);
			}
		}




		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_PostUpdateWork))
		{
			sys(*l_current_world);
		}




		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_PreRenderQueueCommit))
		{
			sys(*l_current_world);
		}

		for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_PostRenderQueueCommit))
		{
			sys(*l_current_world);
		}




		if (l_current_world != m_current_world)
		{
			for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldEnd))
			{
				sys(*l_current_world);
			}

			for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldDefaultEntityDeinitialization))
			{
				sys(*l_current_world);
			}

			for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldDeinitialization))
			{
				sys(*l_current_world);
			}


			l_current_world = m_current_world;


			for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldInitialization))
			{
				sys(*l_current_world);
			}

			for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldDefaultEntityInitialization))
			{
				sys(*l_current_world);
			}

			for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldBegin))
			{
				sys(*l_current_world);
			}
		}
		l_delta_clock.end_clock();
		FE::float64 l_delta = l_delta_clock.get_delta_milliseconds();
		l_current_world->set_delta_time(FE::world::auth{}, l_delta);

		if (l_delta >= 1000.0)
		{
			l_frame_counter = 0;
		}
		++l_frame_counter;
	}




	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldEnd))
	{
		sys(*l_current_world);
	}

	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_GameInstanceEnd))
	{
		sys(*l_current_world);
	}




	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldDefaultEntityDeinitialization))
	{
		sys(*l_current_world);
	}

	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_WorldDeinitialization))
	{
		sys(*l_current_world);
	}

	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_GameInstanceDeinitialization))
	{
		sys(*l_current_world);
	}

	for (auto sys : m_method_reflection.get_systems(l_current_world->get_world_tag(), FE::SystemCallPhase::_EngineDeinitialization))
	{
		sys(*l_current_world);
	}
}


void FE::engine::__read_froggy() noexcept
{
	std::ifstream l_froggy(m_froggy_path.c_str(), std::ios::binary);
	FE::ifstream_guard l_froggy_file_stream(l_froggy);
	FE_ASSERT(l_froggy.is_open() == true, "Failed to open froggy file at path: %s", m_froggy_path.c_str());

	Json::Value l_froggy_json;
	l_froggy >> l_froggy_json;
	{
		FE_ASSERT(l_froggy_json["EngineInfo"].isObject() == true);
		auto& l_engine_info = l_froggy_json["EngineInfo"];

		FE_ASSERT(l_engine_info["Version"].isString() == true);
		m_engine_info._version = std::pmr::string(l_engine_info["Version"].asCString(), framework_base::get_large_memory_resource());
	}

	{
		FE_ASSERT(l_froggy_json["ProjectConfig"].isObject() == true);
		auto& l_project_config = l_froggy_json["ProjectConfig"];

		FE_ASSERT(l_project_config["GlobalResourceLookUpTable"].isObject() == true);
		auto& l_resource_lut = l_project_config["GlobalResourceLookUpTable"];

		for (auto& element : l_resource_lut["WorldPaths"])
		{
			FE_ASSERT(element.isString() == true);
			const Json::String l_tmp = element.asString();
			m_project_config._path_lookup_table._world_paths.push_back(FE::directory_string(l_tmp.begin(), l_tmp.end(), framework_base::get_large_memory_resource()));
		}

		*const_cast<var::uint32*>(&(m_project_config._fiber_stack_size)) = static_cast<FE::uint32>(l_project_config["FiberStackSize"].asInt64());
		FE_ASSERT(m_project_config._fiber_stack_size > 1 * FE::one_KiB);

		*const_cast<var::uint16*>(&(m_project_config._fibers_per_thread)) = static_cast<FE::uint16>(l_project_config["FibersPerThread"].asInt64());
		FE_ASSERT(m_project_config._fibers_per_thread > 0);

		FE_ASSERT(l_project_config["WindowConfig"].isObject() == true);
		auto& l_window_config = l_project_config["WindowConfig"];

		if (l_window_config["Title"].isNull() == false)
		{
			FE_ASSERT(l_window_config["Title"].isString() == true);
			m_project_config._window_config._title = std::pmr::string(l_window_config["Title"].asCString(), framework_base::get_large_memory_resource());
		}


		m_project_config._window_config._icon_paths = std::pmr::vector<std::pmr::string>(framework_base::get_large_memory_resource());
		for (auto& element : l_window_config["IconPaths"])
		{
			FE_ASSERT(element.isString() == true);
			m_project_config._window_config._icon_paths.push_back(std::pmr::string{ element.asCString(), framework_base::get_large_memory_resource() });
			
			std::pmr::string l_path(framework::framework_base::get_framework().get_large_memory_resource());
#ifdef _FE_ON_WINDOWS_X86_64_
			const int l_path_length = (int)FE::algorithm::string::length(m_game_root_directory.c_str());
			const int l_utf8_length = WideCharToMultiByte(CP_UTF8, 0, m_game_root_directory.c_str(), l_path_length, nullptr, 0, nullptr, nullptr);
			FE_ASSERT(l_utf8_length > 0, "Failed to convert the image path to UTF-8.");
			l_path.resize(l_utf8_length);
			WideCharToMultiByte(CP_UTF8, 0,
				m_game_root_directory.c_str(), l_path_length,
				l_path.data(), l_utf8_length,
				nullptr, nullptr
			);
#else
			l_path_str = m_game_root_directory;
#endif
			l_path += "\\";
			l_path += m_project_config._window_config._icon_paths.back();

			m_project_config._window_config._icon_images = std::pmr::vector<GLFWimage>(framework_base::get_large_memory_resource());
			m_project_config._window_config._icon_images.emplace_back();
			m_project_config._window_config._icon_images.back().pixels = stbi_load(l_path.c_str(), &(m_project_config._window_config._icon_images.back().width), &(m_project_config._window_config._icon_images.back().height), nullptr, 4/*RGBA*/);
		}


		m_project_config._window_config._random_play_video_intro_paths = std::pmr::vector<FE::directory_string>(framework_base::get_large_memory_resource());
		FE::directory_string l_path(framework_base::get_large_memory_resource());
		for (auto& element : l_window_config["RandomPlayIntroVideoPaths"])
		{
			FE_ASSERT(element.isString() == true);
			l_path = m_game_root_directory;
			l_path += FE_TEXT(\\);
			const Json::String l_tmp = element.asString();
			l_path += FE::directory_string(l_tmp.begin(), l_tmp.end());
			m_project_config._window_config._random_play_video_intro_paths.push_back(std::move(l_path));
		}


		m_project_config._window_config._sequential_play_video_intro_paths = std::pmr::vector<FE::directory_string>(framework_base::get_large_memory_resource());
		for (auto& element : l_window_config["SequentialPlayIntroVideoPaths"])
		{
			FE_ASSERT(element.isString() == true);

			l_path = m_game_root_directory;
			l_path += FE_TEXT(\\);
			const Json::String l_tmp = element.asString();
			l_path += FE::directory_string(l_tmp.begin(), l_tmp.end());
			m_project_config._window_config._sequential_play_video_intro_paths.push_back(std::move(l_path));
		}


		*const_cast<var::uint8*>(&(m_project_config._window_config._swap_chain_buffer_count)) = static_cast<FE::uint8>(l_window_config["SwapChainBufferCount"].asInt64());
		FE_ASSERT(m_project_config._window_config._swap_chain_buffer_count > 0);


		m_project_config._window_config._shader_compile_splash_images = std::pmr::vector<FE::image>(framework_base::get_large_memory_resource());
		for (auto& element : l_window_config["ShaderCompileSplashImagePaths"])
		{
			FE_ASSERT(element.isString() == true);

			l_path = m_game_root_directory;
			l_path += FE_TEXT(\\);
			const Json::String l_tmp = element.asString();
			l_path += FE::directory_string(l_tmp.begin(), l_tmp.end());

			m_project_config._window_config._shader_compile_splash_images.emplace_back();
			m_project_config._window_config._shader_compile_splash_images.back().read_image_from_disk(l_path.c_str());
		}

		FE_ASSERT(l_window_config["ShaderCompileSplashImageDurationInSeconds"].isInt64() == true);
		m_project_config._window_config._splash_duration_in_seconds = (var::uint32)l_window_config["ShaderCompileSplashImageDurationInSeconds"].asInt64();
	}
}