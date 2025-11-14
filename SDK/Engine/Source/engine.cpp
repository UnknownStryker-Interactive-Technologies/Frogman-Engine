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

#include <FE/game_processor.hxx>
#include <FE/pool/memory_resource.hxx>

#include <FE/app.hpp>




FE::engine::engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept 
	:	FE::framework::framework_base(argc_p, argv_p), 
		m_runtime_path(framework_base::get_memory_resource()),
		m_froggy_path(framework_base::get_memory_resource()),
		m_froggy(framework_base::get_memory_resource()),

		m_engine_info(),
		m_project_config(),

		m_should_exit(),
		m_game_processor(),
		m_renderer_thread(),
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
	m_ecs = FE::make_unique<framework::ECS>(framework_base::get_memory_resource(), 
											m_project_config->_max_engine_entities,
											m_project_config->_max_engine_component_type_count_hint);

	m_processors = FE::make_unique<framework::processors>(	framework_base::get_memory_resource(), 
															__calculate_async_processor_count(),
															m_project_config->_fibers_per_thread,
															m_project_config->_fiber_stack_size);

	m_game_instance = FE::make_owner<FE::game>(framework_base::get_memory_resource(),
		*m_ecs);


	m_game_processor = FE::make_owner<FE::internal::game_processor>(framework_base::get_memory_resource(),
		m_game_instance->get_current_world(),
		m_project_config->_gc_iterations_per_frame,
		m_project_config->_fiber_stack_size);

	__initialize_window_and_renderer();
	return 0;
}

FE::int32 FE::engine::run()
{
	m_processors->run();
	m_game_processor->run();
	m_renderer_thread.fork(__renderer_main, nullptr, m_project_config->_fiber_stack_size);

	return 0;
}

FE::int32 FE::engine::shutdown()
{
	m_renderer_thread.join();
	m_game_processor->shutdown();
	m_processors->shutdown();
	return 0;
}

void FE::engine::__read_froggy() noexcept
{
	//std::ifstream l_froggy(m_froggy_path.c_str(), std::ios::binary);
	//FE::ifstream_guard l_froggy_file_stream(l_froggy);
	//FE_ASSERT(l_froggy.is_open() == true, "Failed to open froggy file at path: %s", m_froggy_path.c_str());

	//boost::json::object l_froggy_json = boost::json::parse(l_froggy).as_object();
	//{
	//	m_engine_info = FE::make_owner<FE::engine_info>(framework_base::get_memory_resource());
	//	FE_ASSERT(l_froggy_json["EngineInfo"].is_object() == true);
	//	auto l_engine_info = l_froggy_json["EngineInfo"].as_object();

	//	FE_ASSERT(l_engine_info["Version"].is_string() == true);
	//	m_engine_info->_version = std::pmr::string(l_engine_info["Version"].get_string().c_str(), framework_base::get_memory_resource());
	//}

	//{
	//	m_path_lut = FE::make_owner<FE::path_lut>(framework_base::get_memory_resource());
	//	FE_ASSERT(l_froggy_json["GlobalResourceLookUpTable"].is_object() == true);
	//	auto l_path_lut = l_froggy_json["GlobalResourceLookUpTable"].as_object();


	//	if (l_path_lut["EntryWorldPath"].is_null() == false)
	//	{
	//		FE_ASSERT(l_path_lut["EntryWorldPath"].is_string() == true);
	//		m_project_info->_entry_world_path = std::pmr::string(l_path_lut["EntryWorldPath"].get_string().c_str(), framework_base::get_memory_resource());
	//	}

	//	for (auto element : l_path_lut["WorldPaths"].get_array())
	//	{
	//		FE_ASSERT(element.is_string() == true);
	//		m_path_lut->_world_paths.push_back(std::pmr::string(element.get_string().data(), framework_base::get_memory_resource()));
	//	}

	//	for (auto element : l_path_lut["ModulePaths"].get_array())
	//	{
	//		FE_ASSERT(element.is_string() == true);
	//		m_path_lut->_module_paths.push_back(std::pmr::string(element.get_string().data(), framework_base::get_memory_resource()));
	//	}
	//}

	//{
	//	m_project_config = FE::make_owner<FE::project_config>(framework_base::get_memory_resource());
	//	FE_ASSERT(l_froggy_json["ProjectConfig"].is_object() == true);
	//	auto l_project_config = l_froggy_json["ProjectConfig"].as_object();

	//	if (l_project_config["CompressionMethod"].is_null() == false)
	//	{
	//		FE_ASSERT(l_project_config["CompressionMethod"].is_string() == true);
	//		FE::task_base* l_ptr = framework_base::get_method_reflection().retrieve(l_project_config["CompressionMethod"].get_string().data());
	//		FE_ASSERT(l_ptr != nullptr, "Failed to retrieve compression method from reflection metadata.");
	//		m_project_config->_compression_method = l_ptr->try_get_as_system();
	//	}

	//	if (l_project_config["DecompressionMethod"].is_null() == false)
	//	{
	//		FE_ASSERT(l_project_config["DecompressionMethod"].is_string() == true);
	//		FE::task_base* l_ptr = framework_base::get_method_reflection().retrieve(l_project_config["DecompressionMethod"].get_string().data());
	//		FE_ASSERT(l_ptr != nullptr, "Failed to retrieve decompression method from reflection metadata.");
	//		m_project_config->_decompression_method = l_ptr->try_get_as_system();
	//	}

	//	if (l_project_config["EncryptionMethod"].is_null() == false)
	//	{
	//		FE_ASSERT(l_project_config["EncryptionMethod"].is_string() == true);
	//		FE::task_base* l_ptr = framework_base::get_method_reflection().retrieve(l_project_config["EncryptionMethod"].get_string().data());
	//		FE_ASSERT(l_ptr != nullptr, "Failed to retrieve encryption method from reflection metadata.");
	//		m_project_config->_encryption_method = l_ptr->try_get_as_system();
	//	}

	//	if (l_project_config["DecryptionMethod"].is_null() == false)
	//	{
	//		FE_ASSERT(l_project_config["DecryptionMethod"].is_string() == true);
	//		FE::task_base* l_ptr = framework_base::get_method_reflection().retrieve(l_project_config["DecryptionMethod"].get_string().data());
	//		FE_ASSERT(l_ptr != nullptr, "Failed to retrieve decryption method from reflection metadata.");
	//		m_project_config->_decryption_method = l_ptr->try_get_as_system();
	//	}

	//	*const_cast<var::uint64*>(&(m_project_config->_max_entities)) = l_project_config["MaxEngineEntities"].get_int64();
	//	FE_ASSERT(m_project_config->_max_entities > 0);

	//	*const_cast<var::uint64*>(&(m_project_config->_max_component_type_count_hint)) = l_project_config["MaxEngineComponentTypeCountHint"].get_int64();
	//	FE_ASSERT(m_project_config->_max_component_type_count_hint > 0);

	//	*const_cast<var::uint64*>(&(m_project_config->_max_system_count_hint)) = l_project_config["MaxEngineSystemCountHint"].get_int64();
	//	FE_ASSERT(m_project_config->_max_system_count_hint > 0);

	//	*const_cast<var::uint32*>(&(m_project_config->_gc_iterations_per_frame)) = static_cast<FE::uint32>(l_project_config["GCIterationsPerFrame"].get_int64());
	//	FE_ASSERT(m_project_config->_gc_iterations_per_frame > 0);

	//	*const_cast<var::uint64*>(&(m_project_config->_fiber_stack_size)) = l_project_config["FiberStackSize"].get_int64();
	//	FE_ASSERT(m_project_config->_fiber_stack_size > 1 * FE::one_KiB);

	//	*const_cast<var::uint16*>(&(m_project_config->_fibers_per_thread)) = static_cast<FE::uint16>( l_project_config["FibersPerThread"].get_int64() );
	//	FE_ASSERT(m_project_config->_fibers_per_thread > 0);
	//}

	//{
	//	m_window_config = FE::make_owner<FE::window_config>(framework_base::get_memory_resource());
	//	FE_ASSERT(l_froggy_json["WindowConfig"].is_object() == true);
	//	auto l_window_config = l_froggy_json["WindowConfig"].as_object();

	//	if (l_window_config["Title"].is_null() == false)
	//	{
	//		FE_ASSERT(l_window_config["Title"].is_string() == true);
	//		m_window_config->_title = std::pmr::string(l_window_config["Title"].get_string(), framework_base::get_memory_resource());
	//	}

	//	// m_window_config->_icon_images;
	//	*const_cast<var::int32*>(&(m_window_config->_monitor_index)) = static_cast<FE::int32>(l_window_config["MonitorIndex"].get_int64());

	//	*const_cast<var::boolean*>(&(m_window_config->_should_enable_vsync)) = l_window_config["ShouldEnableVSync"].get_bool();

	//	*const_cast<var::boolean*>(&(m_window_config->_is_on_the_top)) = l_window_config["IsAtopEverything"].get_bool();

	//	*const_cast<var::boolean*>(&(m_window_config->_should_scale_content_to_monitor_dpi)) = l_window_config["ShouldScaleContentToMonitorDPI"].get_bool();

	//	*const_cast<var::boolean*>(&(m_window_config->_has_border)) = l_window_config["HasBorder"].get_bool();

	//	*const_cast<var::uint8*>(&(m_window_config->_swap_chain_buffer_count)) = static_cast<FE::uint8>(l_window_config["SwapChainBufferCount"].get_int64());
	//	FE_ASSERT(m_window_config->_swap_chain_buffer_count > 0);

	//	// *const_cast<var::boolean*>(&(m_window_config->_is_virtual_reality_mode)) = l_window_config["IsVirtualRealityMode"].get_bool().value();

	//	*const_cast<var::boolean*>(&(m_window_config->_should_enable_hdr)) = l_window_config["ShouldEnableHDR"].get_bool();

	//	*const_cast<var::boolean*>(&(m_window_config->_is_resizable)) = l_window_config["IsResizable"].get_bool();

	//	*const_cast<var::boolean*>(&(m_window_config->_is_maximized)) = l_window_config["IsMaximized"].get_bool();

	//	*const_cast<var::boolean*>(&(m_window_config->_is_fullscreen)) = l_window_config["IsFullScreen"].get_bool();
	//}
}

void FE::engine::__initialize_window_and_renderer() noexcept
{
	m_renderer = FE::make_owner<FE::renderer>(framework_base::get_memory_resource(), m_project_config->_window_config);
	glfwSetWindowCloseCallback(m_renderer->get_window(), &FE::engine::__on_window_close);
	glfwSetKeyCallback(m_renderer->get_window(), &FE::engine::__key_callback);
	glfwSetMouseButtonCallback(m_renderer->get_window(), &FE::engine::__mouse_button_callback);
	glfwSetCursorPosCallback(m_renderer->get_window(), &FE::engine::__cursor_position_callback);
	glfwSetScrollCallback(m_renderer->get_window(), &FE::engine::__scroll_callback);
}

void FE::engine::__renderer_main(FE::component_base* const) noexcept
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
	FE::input_device::keyboard& l_keyboard = get_game_instance().get_current_world()->get_game_mode().get_controller().get_keyboard();
	FE::input_device::KeyState l_current_key_state = static_cast<FE::input_device::KeyState>(action_p);
	l_keyboard._keyboard_state._current_mode = static_cast<FE::input_device::KeyMode>(mods_p);

	switch (static_cast<FE::input_device::Key>(key_p))
	{
	case FE::input_device::Key::_Space:
		l_keyboard._keyboard_state._key_space = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_space) l_keyboard._keyboard_callbacks._key_space(FE::input_device::Key::_Space, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Apostrophe:
		l_keyboard._keyboard_state._key_apostrophe = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_apostrophe) l_keyboard._keyboard_callbacks._key_apostrophe(FE::input_device::Key::_Apostrophe, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Comma:
		l_keyboard._keyboard_state._key_comma = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_comma) l_keyboard._keyboard_callbacks._key_comma(FE::input_device::Key::_Comma, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Minus:
		l_keyboard._keyboard_state._key_minus = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_minus) l_keyboard._keyboard_callbacks._key_minus(FE::input_device::Key::_Minus, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Period:
		l_keyboard._keyboard_state._key_period = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_period) l_keyboard._keyboard_callbacks._key_period(FE::input_device::Key::_Period, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Slash:
		l_keyboard._keyboard_state._key_slash = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_slash) l_keyboard._keyboard_callbacks._key_slash(FE::input_device::Key::_Slash, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_0:
		l_keyboard._keyboard_state._key_0 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_0) l_keyboard._keyboard_callbacks._key_0(FE::input_device::Key::_0, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_1:
		l_keyboard._keyboard_state._key_1 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_1) l_keyboard._keyboard_callbacks._key_1(FE::input_device::Key::_1, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_2:
		l_keyboard._keyboard_state._key_2 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_2) l_keyboard._keyboard_callbacks._key_2(FE::input_device::Key::_2, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_3:
		l_keyboard._keyboard_state._key_3 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_3) l_keyboard._keyboard_callbacks._key_3(FE::input_device::Key::_3, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_4:
		l_keyboard._keyboard_state._key_4 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_4) l_keyboard._keyboard_callbacks._key_4(FE::input_device::Key::_4, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_5:
		l_keyboard._keyboard_state._key_5 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_5) l_keyboard._keyboard_callbacks._key_5(FE::input_device::Key::_5, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_6:
		l_keyboard._keyboard_state._key_6 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_6) l_keyboard._keyboard_callbacks._key_6(FE::input_device::Key::_6, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_7:
		l_keyboard._keyboard_state._key_7 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_7) l_keyboard._keyboard_callbacks._key_7(FE::input_device::Key::_7, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_8:
		l_keyboard._keyboard_state._key_8 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_8) l_keyboard._keyboard_callbacks._key_8(FE::input_device::Key::_8, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_9:
		l_keyboard._keyboard_state._key_9 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_9) l_keyboard._keyboard_callbacks._key_9(FE::input_device::Key::_9, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_Semicolon:
		l_keyboard._keyboard_state._key_semicolon = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_semicolon) l_keyboard._keyboard_callbacks._key_semicolon(FE::input_device::Key::_Semicolon, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Equal:
		l_keyboard._keyboard_state._key_equal = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_equal) l_keyboard._keyboard_callbacks._key_equal(FE::input_device::Key::_Equal, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_A:
		l_keyboard._keyboard_state._key_a = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_a) l_keyboard._keyboard_callbacks._key_a(FE::input_device::Key::_A, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_B:
		l_keyboard._keyboard_state._key_b = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_b) l_keyboard._keyboard_callbacks._key_b(FE::input_device::Key::_B, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_C:
		l_keyboard._keyboard_state._key_c = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_c) l_keyboard._keyboard_callbacks._key_c(FE::input_device::Key::_C, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_D:
		l_keyboard._keyboard_state._key_d = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_d) l_keyboard._keyboard_callbacks._key_d(FE::input_device::Key::_D, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_E:
		l_keyboard._keyboard_state._key_e = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_e) l_keyboard._keyboard_callbacks._key_e(FE::input_device::Key::_E, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F:
		l_keyboard._keyboard_state._key_f = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f) l_keyboard._keyboard_callbacks._key_f(FE::input_device::Key::_F, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_G:
		l_keyboard._keyboard_state._key_g = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_g) l_keyboard._keyboard_callbacks._key_g(FE::input_device::Key::_G, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_H:
		l_keyboard._keyboard_state._key_h = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_h) l_keyboard._keyboard_callbacks._key_h(FE::input_device::Key::_H, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_I:
		l_keyboard._keyboard_state._key_i = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_i) l_keyboard._keyboard_callbacks._key_i(FE::input_device::Key::_I, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_J:
		l_keyboard._keyboard_state._key_j = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_j) l_keyboard._keyboard_callbacks._key_j(FE::input_device::Key::_J, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_K:
		l_keyboard._keyboard_state._key_k = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_k) l_keyboard._keyboard_callbacks._key_k(FE::input_device::Key::_K, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_L:
		l_keyboard._keyboard_state._key_l = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_l) l_keyboard._keyboard_callbacks._key_l(FE::input_device::Key::_L, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_M:
		l_keyboard._keyboard_state._key_m = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_m) l_keyboard._keyboard_callbacks._key_m(FE::input_device::Key::_M, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_N:
		l_keyboard._keyboard_state._key_n = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_n) l_keyboard._keyboard_callbacks._key_n(FE::input_device::Key::_N, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_O:
		l_keyboard._keyboard_state._key_o = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_o) l_keyboard._keyboard_callbacks._key_o(FE::input_device::Key::_O, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_P:
		l_keyboard._keyboard_state._key_p = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_p) l_keyboard._keyboard_callbacks._key_p(FE::input_device::Key::_P, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Q:
		l_keyboard._keyboard_state._key_q = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_q) l_keyboard._keyboard_callbacks._key_q(FE::input_device::Key::_Q, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_R:
		l_keyboard._keyboard_state._key_r = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_r) l_keyboard._keyboard_callbacks._key_r(FE::input_device::Key::_R, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_S:
		l_keyboard._keyboard_state._key_s = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_s) l_keyboard._keyboard_callbacks._key_s(FE::input_device::Key::_S, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_T:
		l_keyboard._keyboard_state._key_t = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_t) l_keyboard._keyboard_callbacks._key_t(FE::input_device::Key::_T, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_U:
		l_keyboard._keyboard_state._key_u = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_u) l_keyboard._keyboard_callbacks._key_u(FE::input_device::Key::_U, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_V:
		l_keyboard._keyboard_state._key_v = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_v) l_keyboard._keyboard_callbacks._key_v(FE::input_device::Key::_V, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_W:
		l_keyboard._keyboard_state._key_w = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_w) l_keyboard._keyboard_callbacks._key_w(FE::input_device::Key::_W, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_X:
		l_keyboard._keyboard_state._key_x = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_x) l_keyboard._keyboard_callbacks._key_x(FE::input_device::Key::_X, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Y:
		l_keyboard._keyboard_state._key_y = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_y) l_keyboard._keyboard_callbacks._key_y(FE::input_device::Key::_Y, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Z:
		l_keyboard._keyboard_state._key_z = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_z) l_keyboard._keyboard_callbacks._key_z(FE::input_device::Key::_Z, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_LeftBracket:
		l_keyboard._keyboard_state._key_left_bracket = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_left_bracket) l_keyboard._keyboard_callbacks._key_left_bracket(FE::input_device::Key::_LeftBracket, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Backslash:
		l_keyboard._keyboard_state._key_backslash = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_backslash) l_keyboard._keyboard_callbacks._key_backslash(FE::input_device::Key::_Backslash, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_RightBracket:
		l_keyboard._keyboard_state._key_right_bracket = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_right_bracket) l_keyboard._keyboard_callbacks._key_right_bracket(FE::input_device::Key::_RightBracket, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_GraveAccent:
		l_keyboard._keyboard_state._key_grave_accent = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_grave_accent) l_keyboard._keyboard_callbacks._key_grave_accent(FE::input_device::Key::_GraveAccent, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_World1:
		l_keyboard._keyboard_state._key_world_1 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_world_1) l_keyboard._keyboard_callbacks._key_world_1(FE::input_device::Key::_World1, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_World2:
		l_keyboard._keyboard_state._key_world_2 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_world_2) l_keyboard._keyboard_callbacks._key_world_2(FE::input_device::Key::_World2, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_Escape:
		l_keyboard._keyboard_state._key_escape = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_escape) l_keyboard._keyboard_callbacks._key_escape(FE::input_device::Key::_Escape, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Enter:
		l_keyboard._keyboard_state._key_enter = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_enter) l_keyboard._keyboard_callbacks._key_enter(FE::input_device::Key::_Enter, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Tab:
		l_keyboard._keyboard_state._key_tab = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_tab) l_keyboard._keyboard_callbacks._key_tab(FE::input_device::Key::_Tab, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Backspace:
		l_keyboard._keyboard_state._key_backspace = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_backspace) l_keyboard._keyboard_callbacks._key_backspace(FE::input_device::Key::_Backspace, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Insert:
		l_keyboard._keyboard_state._key_insert = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_insert) l_keyboard._keyboard_callbacks._key_insert(FE::input_device::Key::_Insert, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Delete:
		l_keyboard._keyboard_state._key_delete = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_delete) l_keyboard._keyboard_callbacks._key_delete(FE::input_device::Key::_Delete, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_Right:
		l_keyboard._keyboard_state._key_right = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_right) l_keyboard._keyboard_callbacks._key_right(FE::input_device::Key::_Right, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Left:
		l_keyboard._keyboard_state._key_left = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_left) l_keyboard._keyboard_callbacks._key_left(FE::input_device::Key::_Left, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Down:
		l_keyboard._keyboard_state._key_down = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_down) l_keyboard._keyboard_callbacks._key_down(FE::input_device::Key::_Down, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Up:
		l_keyboard._keyboard_state._key_up = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_up) l_keyboard._keyboard_callbacks._key_up(FE::input_device::Key::_Up, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_PageUp:
		l_keyboard._keyboard_state._key_page_up = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_page_up) l_keyboard._keyboard_callbacks._key_page_up(FE::input_device::Key::_PageUp, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_PageDown:
		l_keyboard._keyboard_state._key_page_down = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_page_down) l_keyboard._keyboard_callbacks._key_page_down(FE::input_device::Key::_PageDown, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_Home:
		l_keyboard._keyboard_state._key_home = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_home) l_keyboard._keyboard_callbacks._key_home(FE::input_device::Key::_Home, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_End:
		l_keyboard._keyboard_state._key_end = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_end) l_keyboard._keyboard_callbacks._key_end(FE::input_device::Key::_End, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_CapsLock:
		l_keyboard._keyboard_state._key_caps_lock = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_caps_lock) l_keyboard._keyboard_callbacks._key_caps_lock(FE::input_device::Key::_CapsLock, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_ScrollLock:
		l_keyboard._keyboard_state._key_scroll_lock = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_scroll_lock) l_keyboard._keyboard_callbacks._key_scroll_lock(FE::input_device::Key::_ScrollLock, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_NumLock:
		l_keyboard._keyboard_state._key_num_lock = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_num_lock) l_keyboard._keyboard_callbacks._key_num_lock(FE::input_device::Key::_NumLock, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_PrintScreen:
		l_keyboard._keyboard_state._key_print_screen = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_print_screen) l_keyboard._keyboard_callbacks._key_print_screen(FE::input_device::Key::_PrintScreen, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Pause:
		l_keyboard._keyboard_state._key_pause = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pause) l_keyboard._keyboard_callbacks._key_pause(FE::input_device::Key::_Pause, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_F1:
		l_keyboard._keyboard_state._key_f1 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f1) l_keyboard._keyboard_callbacks._key_f1(FE::input_device::Key::_F1, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F2:
		l_keyboard._keyboard_state._key_f2 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f2) l_keyboard._keyboard_callbacks._key_f2(FE::input_device::Key::_F2, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F3:
		l_keyboard._keyboard_state._key_f3 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f3) l_keyboard._keyboard_callbacks._key_f3(FE::input_device::Key::_F3, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F4:
		l_keyboard._keyboard_state._key_f4 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f4) l_keyboard._keyboard_callbacks._key_f4(FE::input_device::Key::_F4, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F5:
		l_keyboard._keyboard_state._key_f5 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f5) l_keyboard._keyboard_callbacks._key_f5(FE::input_device::Key::_F5, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F6:
		l_keyboard._keyboard_state._key_f6 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f6) l_keyboard._keyboard_callbacks._key_f6(FE::input_device::Key::_F6, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F7:
		l_keyboard._keyboard_state._key_f7 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f7) l_keyboard._keyboard_callbacks._key_f7(FE::input_device::Key::_F7, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F8:
		l_keyboard._keyboard_state._key_f8 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f8) l_keyboard._keyboard_callbacks._key_f8(FE::input_device::Key::_F8, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F9:
		l_keyboard._keyboard_state._key_f9 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f9) l_keyboard._keyboard_callbacks._key_f9(FE::input_device::Key::_F9, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F10:
		l_keyboard._keyboard_state._key_f10 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f10) l_keyboard._keyboard_callbacks._key_f10(FE::input_device::Key::_F10, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F11:
		l_keyboard._keyboard_state._key_f11 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f11) l_keyboard._keyboard_callbacks._key_f11(FE::input_device::Key::_F11, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F12:
		l_keyboard._keyboard_state._key_f12 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f12) l_keyboard._keyboard_callbacks._key_f12(FE::input_device::Key::_F12, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F13:
		l_keyboard._keyboard_state._key_f13 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f13) l_keyboard._keyboard_callbacks._key_f13(FE::input_device::Key::_F13, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F14:
		l_keyboard._keyboard_state._key_f14 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f14) l_keyboard._keyboard_callbacks._key_f14(FE::input_device::Key::_F14, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F15:
		l_keyboard._keyboard_state._key_f15 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f15) l_keyboard._keyboard_callbacks._key_f15(FE::input_device::Key::_F15, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F16:
		l_keyboard._keyboard_state._key_f16 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f16) l_keyboard._keyboard_callbacks._key_f16(FE::input_device::Key::_F16, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F17:
		l_keyboard._keyboard_state._key_f17 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f17) l_keyboard._keyboard_callbacks._key_f17(FE::input_device::Key::_F17, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F18:
		l_keyboard._keyboard_state._key_f18 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f18) l_keyboard._keyboard_callbacks._key_f18(FE::input_device::Key::_F18, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F19:
		l_keyboard._keyboard_state._key_f19 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f19) l_keyboard._keyboard_callbacks._key_f19(FE::input_device::Key::_F19, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F20:
		l_keyboard._keyboard_state._key_f20 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f20) l_keyboard._keyboard_callbacks._key_f20(FE::input_device::Key::_F20, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F21:
		l_keyboard._keyboard_state._key_f21 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f21) l_keyboard._keyboard_callbacks._key_f21(FE::input_device::Key::_F21, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F22:
		l_keyboard._keyboard_state._key_f22 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f22) l_keyboard._keyboard_callbacks._key_f22(FE::input_device::Key::_F22, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F23:
		l_keyboard._keyboard_state._key_f23 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f23) l_keyboard._keyboard_callbacks._key_f23(FE::input_device::Key::_F23, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F24:
		l_keyboard._keyboard_state._key_f24 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f24) l_keyboard._keyboard_callbacks._key_f24(FE::input_device::Key::_F24, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_F25:
		l_keyboard._keyboard_state._key_f25 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_f25) l_keyboard._keyboard_callbacks._key_f25(FE::input_device::Key::_F25, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_Pad0:
		l_keyboard._keyboard_state._key_pad_0 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_0) l_keyboard._keyboard_callbacks._key_pad_0(FE::input_device::Key::_Pad0, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Pad1:
		l_keyboard._keyboard_state._key_pad_1 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_1) l_keyboard._keyboard_callbacks._key_pad_1(FE::input_device::Key::_Pad1, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Pad2:
		l_keyboard._keyboard_state._key_pad_2 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_2) l_keyboard._keyboard_callbacks._key_pad_2(FE::input_device::Key::_Pad2, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Pad3:
		l_keyboard._keyboard_state._key_pad_3 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_3) l_keyboard._keyboard_callbacks._key_pad_3(FE::input_device::Key::_Pad3, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Pad4:
		l_keyboard._keyboard_state._key_pad_4 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_4) l_keyboard._keyboard_callbacks._key_pad_4(FE::input_device::Key::_Pad4, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Pad5:
		l_keyboard._keyboard_state._key_pad_5 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_5) l_keyboard._keyboard_callbacks._key_pad_5(FE::input_device::Key::_Pad5, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Pad6:
		l_keyboard._keyboard_state._key_pad_6 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_6) l_keyboard._keyboard_callbacks._key_pad_6(FE::input_device::Key::_Pad6, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Pad7:
		l_keyboard._keyboard_state._key_pad_7 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_7) l_keyboard._keyboard_callbacks._key_pad_7(FE::input_device::Key::_Pad7, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Pad8:
		l_keyboard._keyboard_state._key_pad_8 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_8) l_keyboard._keyboard_callbacks._key_pad_8(FE::input_device::Key::_Pad8, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_Pad9:
		l_keyboard._keyboard_state._key_pad_9 = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_9) l_keyboard._keyboard_callbacks._key_pad_9(FE::input_device::Key::_Pad9, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_PadDecimal:
		l_keyboard._keyboard_state._key_pad_decimal = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_decimal) l_keyboard._keyboard_callbacks._key_pad_decimal(FE::input_device::Key::_PadDecimal, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_PadDivide:
		l_keyboard._keyboard_state._key_pad_divide = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_divide) l_keyboard._keyboard_callbacks._key_pad_divide(FE::input_device::Key::_PadDivide, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_PadMultiply:
		l_keyboard._keyboard_state._key_pad_multiply = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_multiply) l_keyboard._keyboard_callbacks._key_pad_multiply(FE::input_device::Key::_PadMultiply, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_PadSubtract:
		l_keyboard._keyboard_state._key_pad_subtract = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_subtract) l_keyboard._keyboard_callbacks._key_pad_subtract(FE::input_device::Key::_PadSubtract, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_PadAdd:
		l_keyboard._keyboard_state._key_pad_add = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_add) l_keyboard._keyboard_callbacks._key_pad_add(FE::input_device::Key::_PadAdd, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_PadEnter:
		l_keyboard._keyboard_state._key_pad_enter = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_enter) l_keyboard._keyboard_callbacks._key_pad_enter(FE::input_device::Key::_PadEnter, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_PadEqual:
		l_keyboard._keyboard_state._key_pad_equal = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_pad_equal) l_keyboard._keyboard_callbacks._key_pad_equal(FE::input_device::Key::_PadEqual, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_LeftShift:
		l_keyboard._keyboard_state._key_left_shift = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_left_shift) l_keyboard._keyboard_callbacks._key_left_shift(FE::input_device::Key::_LeftShift, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_LeftControl:
		l_keyboard._keyboard_state._key_left_control = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_left_control) l_keyboard._keyboard_callbacks._key_left_control(FE::input_device::Key::_LeftControl, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_LeftAlt:
		l_keyboard._keyboard_state._key_left_alt = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_left_alt) l_keyboard._keyboard_callbacks._key_left_alt(FE::input_device::Key::_LeftAlt, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_LeftSuper:
		l_keyboard._keyboard_state._key_left_super = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_left_super) l_keyboard._keyboard_callbacks._key_left_super(FE::input_device::Key::_LeftSuper, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_RightShift:
		l_keyboard._keyboard_state._key_right_shift = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_right_shift) l_keyboard._keyboard_callbacks._key_right_shift(FE::input_device::Key::_RightShift, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_RightControl:
		l_keyboard._keyboard_state._key_right_control = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_right_control) l_keyboard._keyboard_callbacks._key_right_control(FE::input_device::Key::_RightControl, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_RightAlt:
		l_keyboard._keyboard_state._key_right_alt = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_right_alt) l_keyboard._keyboard_callbacks._key_right_alt(FE::input_device::Key::_RightAlt, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	case FE::input_device::Key::_RightSuper:
		l_keyboard._keyboard_state._key_right_super = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_right_super) l_keyboard._keyboard_callbacks._key_right_super(FE::input_device::Key::_RightSuper, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;


	case FE::input_device::Key::_Menu:
		l_keyboard._keyboard_state._key_menu = l_current_key_state;
		if (l_keyboard._keyboard_callbacks._key_menu) l_keyboard._keyboard_callbacks._key_menu(FE::input_device::Key::_Menu, l_current_key_state, l_keyboard._keyboard_state._current_mode, scancode_p);
		break;

	default:
		break;
	}

	(window_p);
}

void FE::engine::__mouse_button_callback(GLFWwindow* const window_p, FE::int32 button_p, FE::int32 action_p, FE::int32 mods_p) noexcept
{
	FE::input_device::mouse& l_mouse = get_game_instance().get_current_world()->get_game_mode().get_controller().get_mouse();

	FE::input_device::ButtonState l_current_button_state = static_cast<FE::input_device::ButtonState>(action_p);
	l_mouse._mouse_state._current_mode = static_cast<FE::input_device::KeyMode>(mods_p);

	switch (static_cast<FE::input_device::Button>(button_p))
	{
	case FE::input_device::Button::_Left:
		l_mouse._mouse_state._button_left = l_current_button_state;
		if (l_mouse._mouse_callbacks._button_left) l_mouse._mouse_callbacks._button_left(FE::input_device::Button::_Left, l_current_button_state, l_mouse._mouse_state._current_mode);
		break;

	case FE::input_device::Button::_Right:
		l_mouse._mouse_state._button_right = l_current_button_state;
		if (l_mouse._mouse_callbacks._button_right) l_mouse._mouse_callbacks._button_right(FE::input_device::Button::_Right, l_current_button_state, l_mouse._mouse_state._current_mode);
		break;

	case FE::input_device::Button::_Middle:
		l_mouse._mouse_state._button_middle = l_current_button_state;
		if (l_mouse._mouse_callbacks._button_middle) l_mouse._mouse_callbacks._button_middle(FE::input_device::Button::_Middle, l_current_button_state, l_mouse._mouse_state._current_mode);
		break;

	case FE::input_device::Button::_4th:
		l_mouse._mouse_state._button_4th = l_current_button_state;
		if (l_mouse._mouse_callbacks._button_4th) l_mouse._mouse_callbacks._button_4th(FE::input_device::Button::_4th, l_current_button_state, l_mouse._mouse_state._current_mode);
		break;

	case FE::input_device::Button::_5th:
		l_mouse._mouse_state._button_5th = l_current_button_state;
		if (l_mouse._mouse_callbacks._button_5th) l_mouse._mouse_callbacks._button_5th(FE::input_device::Button::_5th, l_current_button_state, l_mouse._mouse_state._current_mode);
		break;

	case FE::input_device::Button::_6th:
		l_mouse._mouse_state._button_6th = l_current_button_state;
		if (l_mouse._mouse_callbacks._button_6th) l_mouse._mouse_callbacks._button_6th(FE::input_device::Button::_6th, l_current_button_state, l_mouse._mouse_state._current_mode);
		break;

	case FE::input_device::Button::_7th:
		l_mouse._mouse_state._button_7th = l_current_button_state;
		if (l_mouse._mouse_callbacks._button_7th) l_mouse._mouse_callbacks._button_7th(FE::input_device::Button::_7th, l_current_button_state, l_mouse._mouse_state._current_mode);
		break;

	case FE::input_device::Button::_8th:
		l_mouse._mouse_state._button_8th = l_current_button_state;
		if (l_mouse._mouse_callbacks._button_8th) l_mouse._mouse_callbacks._button_8th(FE::input_device::Button::_8th, l_current_button_state, l_mouse._mouse_state._current_mode);
		break;

	default:
		break;
	}

	(window_p);
}

void FE::engine::__cursor_position_callback(GLFWwindow* const window_p, double x_p, double y_p) noexcept
{
	FE::input_device::mouse& l_mouse = get_game_instance().get_current_world()->get_game_mode().get_controller().get_mouse();
	l_mouse._mouse_state._cursor_coordinate_x = x_p;
	l_mouse._mouse_state._cursor_coordinate_y = y_p;
	l_mouse._mouse_callbacks._cursor_position_callback(x_p, y_p);
	(window_p);
}

void FE::engine::__scroll_callback(GLFWwindow* const window_p, double x_offset_p, double y_offset_p) noexcept
{
	FE::input_device::mouse& l_mouse = get_game_instance().get_current_world()->get_game_mode().get_controller().get_mouse();
	l_mouse._mouse_state._cursor_coordinate_x = x_offset_p;
	l_mouse._mouse_state._cursor_coordinate_y = y_offset_p;
	l_mouse._mouse_callbacks._scroll_callback(x_offset_p, y_offset_p);
	(window_p);
}
