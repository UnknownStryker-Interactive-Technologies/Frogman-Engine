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
#include <FE/engine.hxx>

#include <FE/framework/ECS.hxx>
#include <FE/framework/processors.hxx>
#include <FE/framework/reflection.hxx>

#include <FE/pool/memory_resource.hxx>

#include <FE/app.hpp>




void nullsys(FE::component_base*) noexcept {}




FE::engine::engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept 
	:	FE::framework::framework_base(argc_p, argv_p), 
		m_runtime_path(framework_base::get_memory_resource()),
		m_froggy_path(framework_base::get_memory_resource()),
		m_froggy(framework_base::get_memory_resource()),

		m_engine_info(),
		m_project_info(),
		m_path_lut(),
		m_project_config(),
		m_window_config(),

		m_should_exit(),
		m_renderer(),
		m_game_instance()
{
	m_runtime_path.reserve(_MAX_PATH_LENGTH_);
	FE::get_directory_of_current_executable(m_runtime_path.data(), (FE::uint32)m_runtime_path.capacity());
	m_runtime_path = m_runtime_path.c_str();
	m_runtime_path.shrink_to_fit();

	std::pmr::string::size_type l_pos = m_runtime_path.rfind('\\');
	FE_ASSERT(l_pos != std::pmr::string::npos, "Failed to find last occurrence of '\\' in current executable path.");
	m_froggy_path = m_runtime_path;
	++l_pos;
	std::pmr::string l_project_name = m_runtime_path.substr(l_pos, m_runtime_path.size() - l_pos);
	m_froggy_path.erase(l_pos, l_project_name.length());
	l_pos = l_project_name.rfind(".exe");
	FE_ASSERT(l_pos != std::pmr::string::npos, "Failed to find last occurrence of '.exe' in current executable path.");
	l_project_name.erase(l_pos, std::strlen(".exe")); // 4 is length of ".exe"

	l_pos = m_froggy_path.rfind(l_project_name.c_str());
	FE_ASSERT(l_pos != std::pmr::string::npos, "Failed to find last occurrence of project name in current executable path.");

	m_froggy_path.erase(l_pos + l_project_name.length(), m_froggy_path.length() - (l_pos + l_project_name.length()));
	m_froggy_path += "\\";
	m_froggy_path += l_project_name;
	m_froggy_path += ".froggy";
}

FE::engine::~engine() noexcept
{
}

FE::int32 FE::engine::launch(FE::int32 argc_p, FE::ASCII** argv_p)
{
	(argc_p);
	(argv_p);

	__load_reflection_data();
	__read_froggy();
	m_ecs = FE::make_unique<framework::ECS>(framework_base::get_memory_resource(), m_project_config->_max_entities, m_project_config->_max_component_type_count_hint, m_project_config->_max_system_count_hint);
	m_processors = FE::make_unique<framework::processors>(framework_base::get_memory_resource(), *m_ecs, m_program_options.get_max_concurrency(), m_project_config->_fibers_per_thread, m_project_config->_gc_iterations_per_frame, m_project_config->_fiber_stack_size);
	__initialize_window_and_renderer();
	return 0;
}

FE::int32 FE::engine::run()
{
	m_processors->run(	__renderer_main, nullptr, 
						nullsys, nullptr,
						nullsys, nullptr, 
						nullsys, nullptr);
	return 0;
}

FE::int32 FE::engine::shutdown()
{
	return 0;
}

void FE::engine::__read_froggy() noexcept
{
	std::ifstream l_froggy(m_froggy_path.c_str(), std::ios::binary);
	FE::ifstream_guard l_froggy_file_stream(l_froggy);
	FE_ASSERT(l_froggy.is_open() == true, "Failed to open froggy file at path: %s", m_froggy_path.c_str());

	boost::json::object l_froggy_json = boost::json::parse(l_froggy).as_object();
	{
		m_engine_info = FE::make_owner<FE::engine_info>(framework_base::get_memory_resource());
		FE_ASSERT(l_froggy_json["EngineInfo"].is_object() == true);
		auto l_engine_info = l_froggy_json["EngineInfo"].as_object();

		FE_ASSERT(l_engine_info["Version"].is_string() == true);
		m_engine_info->_version = std::pmr::string(l_engine_info["Version"].get_string().c_str(), framework_base::get_memory_resource());

		FE_ASSERT(l_engine_info["InstallationPath"].is_string() == true);
		m_engine_info->_installation_path = std::pmr::string(l_engine_info["InstallationPath"].get_string().c_str(), framework_base::get_memory_resource());
	}

	{
		m_project_info = FE::make_owner<FE::project_info>(framework_base::get_memory_resource());
		FE_ASSERT(l_froggy_json["ProjectInfo"].is_object() == true);
		auto l_project_info = l_froggy_json["ProjectInfo"].as_object();

		FE_ASSERT(l_project_info["ProjectType"].is_string() == true);
		m_project_info->_type = std::pmr::string(l_project_info["ProjectType"].get_string().c_str(), framework_base::get_memory_resource());

		FE_ASSERT(l_project_info["ProjectName"].is_string() == true);
		m_project_info->_name = std::pmr::string(l_project_info["ProjectName"].get_string().c_str(), framework_base::get_memory_resource());

		FE_ASSERT(l_project_info["ProjectPath"].is_string() == true);
		m_project_info->_path = std::pmr::string(l_project_info["ProjectPath"].get_string().c_str(), framework_base::get_memory_resource());

		if (l_project_info["EntryWorldPath"].is_null() == false)
		{
			FE_ASSERT(l_project_info["EntryWorldPath"].is_string() == true);
			m_project_info->_entry_world_path = std::pmr::string(l_project_info["EntryWorldPath"].get_string().c_str(), framework_base::get_memory_resource());
		}
	}

	{
		m_path_lut = FE::make_owner<FE::path_lut>(framework_base::get_memory_resource());
		FE_ASSERT(l_froggy_json["GlobalResourceLookUpTable"].is_object() == true);
		auto l_path_lut = l_froggy_json["GlobalResourceLookUpTable"].as_object();

		for (auto element : l_path_lut["WorldPaths"].get_array())
		{
			FE_ASSERT(element.is_string() == true);
			m_path_lut->_world_paths.push_back(std::pmr::string(element.get_string().data(), framework_base::get_memory_resource()));
		}

		for (auto element : l_path_lut["ModulePaths"].get_array())
		{
			FE_ASSERT(element.is_string() == true);
			m_path_lut->_module_paths.push_back(std::pmr::string(element.get_string().data(), framework_base::get_memory_resource()));
		}
	}

	{
		m_project_config = FE::make_owner<FE::project_config>(framework_base::get_memory_resource());
		FE_ASSERT(l_froggy_json["ProjectConfig"].is_object() == true);
		auto l_project_config = l_froggy_json["ProjectConfig"].as_object();

		if (l_project_config["CompressionMethod"].is_null() == false)
		{
			FE_ASSERT(l_project_config["CompressionMethod"].is_string() == true);
			FE::task_base* l_ptr = framework_base::get_method_reflection().retrieve(l_project_config["CompressionMethod"].get_string().data());
			FE_ASSERT(l_ptr != nullptr, "Failed to retrieve compression method from reflection metadata.");
			m_project_config->_compression_method = l_ptr->try_get_as_system();
		}

		if (l_project_config["DecompressionMethod"].is_null() == false)
		{
			FE_ASSERT(l_project_config["DecompressionMethod"].is_string() == true);
			FE::task_base* l_ptr = framework_base::get_method_reflection().retrieve(l_project_config["DecompressionMethod"].get_string().data());
			FE_ASSERT(l_ptr != nullptr, "Failed to retrieve decompression method from reflection metadata.");
			m_project_config->_decompression_method = l_ptr->try_get_as_system();
		}

		if (l_project_config["EncryptionMethod"].is_null() == false)
		{
			FE_ASSERT(l_project_config["EncryptionMethod"].is_string() == true);
			FE::task_base* l_ptr = framework_base::get_method_reflection().retrieve(l_project_config["EncryptionMethod"].get_string().data());
			FE_ASSERT(l_ptr != nullptr, "Failed to retrieve encryption method from reflection metadata.");
			m_project_config->_encryption_method = l_ptr->try_get_as_system();
		}

		if (l_project_config["DecryptionMethod"].is_null() == false)
		{
			FE_ASSERT(l_project_config["DecryptionMethod"].is_string() == true);
			FE::task_base* l_ptr = framework_base::get_method_reflection().retrieve(l_project_config["DecryptionMethod"].get_string().data());
			FE_ASSERT(l_ptr != nullptr, "Failed to retrieve decryption method from reflection metadata.");
			m_project_config->_decryption_method = l_ptr->try_get_as_system();
		}

		*const_cast<var::uint64*>(&(m_project_config->_max_entities)) = l_project_config["MaxEntities"].get_int64();
		FE_ASSERT(m_project_config->_max_entities > 0);

		*const_cast<var::uint64*>(&(m_project_config->_max_component_type_count_hint)) = l_project_config["MaxComponentTypeCountHint"].get_int64();
		FE_ASSERT(m_project_config->_max_component_type_count_hint > 0);

		*const_cast<var::uint64*>(&(m_project_config->_max_system_count_hint)) = l_project_config["MaxSystemCountHint"].get_int64();
		FE_ASSERT(m_project_config->_max_system_count_hint > 0);

		*const_cast<var::uint32*>(&(m_project_config->_gc_iterations_per_frame)) = static_cast<FE::uint32>(l_project_config["GCIterationsPerFrame"].get_int64());
		FE_ASSERT(m_project_config->_gc_iterations_per_frame > 0);

		*const_cast<var::uint64*>(&(m_project_config->_fiber_stack_size)) = l_project_config["FiberStackSize"].get_int64();
		FE_ASSERT(m_project_config->_fiber_stack_size > 1 * FE::one_KiB);

		*const_cast<var::uint16*>(&(m_project_config->_fibers_per_thread)) = static_cast<FE::uint16>( l_project_config["FibersPerThread"].get_int64() );
		FE_ASSERT(m_project_config->_fibers_per_thread > 0);
	}

	{
		m_window_config = FE::make_owner<FE::window_config>(framework_base::get_memory_resource());
		FE_ASSERT(l_froggy_json["WindowConfig"].is_object() == true);
		auto l_window_config = l_froggy_json["WindowConfig"].as_object();

		if (l_window_config["Title"].is_null() == false)
		{
			FE_ASSERT(l_window_config["Title"].is_string() == true);
			m_window_config->_title = std::pmr::string(l_window_config["Title"].get_string(), framework_base::get_memory_resource());
		}

		// m_window_config->_icon_images;
		*const_cast<var::int32*>(&(m_window_config->_monitor_index)) = static_cast<FE::int32>(l_window_config["MonitorIndex"].get_int64());

		*const_cast<var::boolean*>(&(m_window_config->_should_enable_vsync)) = l_window_config["ShouldEnableVSync"].get_bool();

		*const_cast<var::boolean*>(&(m_window_config->_is_on_the_top)) = l_window_config["IsAtopEverything"].get_bool();

		*const_cast<var::boolean*>(&(m_window_config->_should_scale_content_to_monitor_dpi)) = l_window_config["ShouldScaleContentToMonitorDPI"].get_bool();

		*const_cast<var::boolean*>(&(m_window_config->_has_border)) = l_window_config["HasBorder"].get_bool();

		*const_cast<var::uint8*>(&(m_window_config->_swap_chain_buffer_count)) = static_cast<FE::uint8>(l_window_config["SwapChainBufferCount"].get_int64());
		FE_ASSERT(m_window_config->_swap_chain_buffer_count > 0);

		// *const_cast<var::boolean*>(&(m_window_config->_is_virtual_reality_mode)) = l_window_config["IsVirtualRealityMode"].get_bool().value();

		*const_cast<var::boolean*>(&(m_window_config->_should_enable_hdr)) = l_window_config["ShouldEnableHDR"].get_bool();

		*const_cast<var::boolean*>(&(m_window_config->_is_resizable)) = l_window_config["IsResizable"].get_bool();

		*const_cast<var::boolean*>(&(m_window_config->_is_maximized)) = l_window_config["IsMaximized"].get_bool();

		*const_cast<var::boolean*>(&(m_window_config->_is_fullscreen)) = l_window_config["IsFullScreen"].get_bool();
	}
}

void FE::engine::__initialize_window_and_renderer() noexcept
{
	m_renderer = FE::make_owner<FE::renderer>(framework_base::get_memory_resource(), *m_window_config);
	glfwSetWindowCloseCallback(m_renderer->get_window(), &FE::engine::__on_window_close);
	glfwSetKeyCallback(m_renderer->get_window(), &FE::engine::__key_callback);
	glfwSetMouseButtonCallback(m_renderer->get_window(), &FE::engine::__mouse_button_callback);
	glfwSetCursorPosCallback(m_renderer->get_window(), &FE::engine::__cursor_position_callback);
	glfwSetScrollCallback(m_renderer->get_window(), &FE::engine::__scroll_callback);
}

void FE::engine::__renderer_main(_FE_MAYBE_UNUSED_ FE::component_base* engine_reference_p) noexcept
{
	while (FE::engine::__get_engine().m_should_exit.load(std::memory_order_acquire) == false) // TO DO: retrieve render target data from the game thread stash.
	{
		FE::engine::__get_engine().m_renderer->render_frame();
	}
}

void FE::engine::__on_window_close(GLFWwindow* window_p) noexcept
{
	FE::engine::__get_engine().m_should_exit.store(true, std::memory_order_release);
	FE::engine::__get_engine().m_processors->shutdown();
	glfwSetWindowShouldClose(window_p, GLFW_TRUE);
}

void FE::engine::__key_callback(GLFWwindow* const window_p, FE::int32 key_p, FE::int32 scancode_p, FE::int32 action_p, FE::int32 mods_p) noexcept
{
	(window_p);
	(key_p);
	(scancode_p);
	(action_p);
	(mods_p);
}

void FE::engine::__mouse_button_callback(GLFWwindow* const window_p, FE::int32 button_p, FE::int32 action_p, FE::int32 mods_p) noexcept
{
	(window_p);
	(button_p);
	(action_p);
	(mods_p);
}

void FE::engine::__cursor_position_callback(GLFWwindow* const window_p, double x_p, double y_p) noexcept
{
#ifdef _DEBUG_
	std::cout << "x: " << x_p << "; " << "y: " << y_p << ".\n";
#endif
	(window_p);
	(x_p);
	(y_p);
}

void FE::engine::__scroll_callback(GLFWwindow* const window_p, double x_offset_p, double y_offset_p) noexcept
{
	(window_p);
	(x_offset_p);
	(y_offset_p);
}
