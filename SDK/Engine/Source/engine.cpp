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

#include <FE/processors.hxx>
#include <FE/framework/reflection.hxx>

#include <FE/memory_resource.hxx>

#include <FE/app.hpp>

// #define STB_IMAGE_IMPLEMENTATION
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
		m_should_tick(false),
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
	m_should_tick.store(true, std::memory_order_release);
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
	// schedule renderer::__main()
	FE::task l_renderer_main = {};
	l_renderer_main._system = &FE::renderer::__main;
	l_renderer_main._task_type = TaskPriority::_Critical;
	l_renderer_main._world = nullptr;
	m_processors->execute();
	m_processors->schedule_task(l_renderer_main);

	__game_main();

	return 0;
}

FE::int32 FE::engine::shutdown()
{
	m_processors->terminate();
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

		*const_cast<var::uint32*>(&(m_project_config._gc_batch_count)) = static_cast<FE::uint32>(l_project_config["GCIterationsPerFrame"].asInt64());
		FE_ASSERT(m_project_config._gc_batch_count > 0);

		*const_cast<var::uint32*>(&(m_project_config._frames_per_reachability_analysis)) = static_cast<FE::uint32>(l_project_config["FramesPerReachabilityAnalysis"].asInt64());
		FE_ASSERT(m_project_config._frames_per_reachability_analysis > 0);

		*const_cast<var::uint64*>(&(m_project_config._fiber_stack_size)) = l_project_config["FiberStackSize"].asInt64();
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