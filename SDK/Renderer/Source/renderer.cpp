#include <FE/renderer.hxx>
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
#include <FE/prerequisites.hxx>
#include <FE/clock.hxx>
#include <FE/random.hxx>

#include <FE/blacklist_evaluator.hxx>
#include <FE/engine.hpp>
#include <FE/image.hpp>
#include <FE/video_player.hpp>

#include <FE/processors.hxx>
#include <FE/window.hxx>

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


renderer::renderer(FE::smart_ptr<FE::processors, FE::RefType::_Observer> processors_p, FE::smart_ptr<FE::window, FE::RefType::_Observer> window_p) noexcept
	:	m_backend(window_p),
		m_renderer_clock(),
		m_delta_milliseconds(0.0),

		m_processors(processors_p),
		m_window(window_p),
		m_should_exit(false),

		m_shader_headers(),
		m_shaders(framework::framework_base::get_framework().get_large_memory_resource()),
		m_shader_root_directory(engine::get_engine().get_game_root_directory())
{
	m_shader_root_directory += FE_TEXT(\\Assets\\Shaders);

	std::ifstream l_froggy(engine::get_engine().get_froggy_path().c_str(), std::ios::binary);
	FE::ifstream_guard l_froggy_file_stream(l_froggy);
	FE_ASSERT(l_froggy.is_open() == true, "Failed to open froggy file at path: %s", engine::get_engine().get_froggy_path().c_str());

	boost::json::object l_froggy_json = boost::json::parse(l_froggy).get_object();
	{
		m_shader_headers.reserve(1024); // reserve some arbitrary amount of headers to avoid too many reallocations; this value can be changed later if needed.
		FE_ASSERT(l_froggy_json["ShaderHeaders"].is_array() == true);

		FE::directory_string l_path(framework::framework_base::get_framework().get_large_memory_resource());
		for (auto& element : l_froggy_json["ShaderHeaders"].get_array())
		{
			FE_ASSERT(element.is_string() == true);
			auto l_tmp = element.get_string();
			l_path += FE::directory_string(l_tmp.begin(), l_tmp.end());

			auto l_pos = l_path.rfind(FE_TEXT(\\));
			FE_ASSERT(l_pos != std::pmr::string::npos, "Failed to find last occurrence of '\\' in shader header path.");

			l_path.replace(l_path.begin(), l_path.begin() + l_pos, m_shader_root_directory);

			std::fstream l_file(l_path.c_str(), std::ios::binary | std::ios::in);
			FE::fstream_guard l_file_stream(l_file);

			l_file.seekg(0, std::ios::end);
			std::streamsize l_size = l_file.tellg();
			l_file.seekg(0, std::ios::beg);

			auto& l_shader_header = m_shader_headers[l_path];
			l_shader_header._header_buffer = std::pmr::string(framework::framework_base::get_framework().get_large_memory_resource());
			l_shader_header._header_buffer.resize(l_size);

			l_file.read(l_shader_header._header_buffer.data(), l_size);

			l_shader_header._included_hlslis = std::pmr::vector<FE::internal::renderer::hlsli*>(framework::framework_base::get_framework().get_large_memory_resource());
		}
	}

	{
		m_shaders.reserve(1024); // reserve some arbitrary amount of shaders to avoid too many reallocations; this value can be changed later if needed.
		FE_ASSERT(l_froggy_json["Shaders"].is_array() == true);
		for (auto& shader : l_froggy_json["Shaders"].get_array())
		{
			m_shaders.emplace_back();
			m_shaders.back()._defines = std::pmr::vector<FE::internal::renderer::shader_define>(framework::framework_base::get_framework().get_large_memory_resource());
			m_shaders.back()._permutation_blacklist = std::pmr::vector<std::pmr::string>(framework::framework_base::get_framework().get_large_memory_resource());
			m_shaders.back()._macro_combinations = std::pmr::vector<std::pmr::vector<FE::internal::renderer::shader::macro>>(framework::framework_base::get_framework().get_large_memory_resource());
			m_shaders.back()._main_function = std::pmr::string(framework::framework_base::get_framework().get_large_memory_resource());
			m_shaders.back()._source_path = std::pmr::wstring(framework::framework_base::get_framework().get_large_memory_resource());

			auto& l_shader = shader.get_object();
			FE_ASSERT(l_shader["Defines"].is_array() == true);
			for (auto& define : l_shader["Defines"].get_array())
			{
				for (auto& [identifier, value_range] : define.get_object())
				{
					m_shaders.back()._defines.emplace_back();
					m_shaders.back()._defines.back()._identifier = std::pmr::string(identifier, framework::framework_base::get_framework().get_large_memory_resource());
					FE_ASSERT(value_range.is_array() == true);
					FE_ASSERT(value_range.get_array().size() == 2);
					FE_ASSERT(value_range.get_array().at(0).is_int64() == true);
					FE_ASSERT(value_range.get_array().at(1).is_int64() == true);
					m_shaders.back()._defines.back()._value_range._first = value_range.get_array().at(0).get_int64();
					m_shaders.back()._defines.back()._value_range._second = value_range.get_array().at(1).get_int64();
					FE_ASSERT(m_shaders.back()._defines.back()._value_range._first <= m_shaders.back()._defines.back()._value_range._second);
					m_shaders.back()._defines.back()._current_value = m_shaders.back()._defines.back()._value_range._first; // set current value to the minimum value in the range by default
				}
			}

			for (auto& blacklist : l_shader["PermutationBlacklist"].get_array())
			{
				FE_ASSERT(blacklist.is_string() == true);
				m_shaders.back()._permutation_blacklist.push_back(std::pmr::string(blacklist.get_string().c_str(), framework::framework_base::get_framework().get_large_memory_resource()));
			}

			FE_ASSERT(l_shader["MainFunction"].is_string() == true);
			m_shaders.back()._main_function = l_shader["MainFunction"].get_string();

			FE_ASSERT(l_shader["Source"].is_string() == true);
			auto l_tmp = l_shader["Source"].get_string();
			m_shaders.back()._source_path = std::pmr::wstring(l_tmp.begin(), l_tmp.end(), framework::framework_base::get_framework().get_large_memory_resource());

			auto l_pos = m_shaders.back()._source_path.rfind(L"\\");
			FE_ASSERT(l_pos != std::pmr::string::npos, "Failed to find last occurrence of '\\' in shader header path.");

			m_shaders.back()._source_path.replace(m_shaders.back()._source_path.begin(),
													m_shaders.back()._source_path.begin() + l_pos,

													std::pmr::wstring(m_shader_root_directory.begin(), m_shader_root_directory.end())
			);


			STRING_SWITCH(l_shader["ShaderTarget"].get_string().c_str())
			{
			STRING_CASE(FE::internal::renderer::SM5_vertex_shader_target) :
				m_shaders.back()._shader_target = FE::internal::renderer::ShaderTarget::_SM5_VertexShader;
				break;

			STRING_CASE(FE::internal::renderer::SM5_pixel_shader_target) :
				m_shaders.back()._shader_target = FE::internal::renderer::ShaderTarget::_SM5_PixelShader;
				break;

			STRING_CASE(FE::internal::renderer::SM5_geometry_shader_target) :
				m_shaders.back()._shader_target = FE::internal::renderer::ShaderTarget::_SM5_GeometryShader;
				break;

				STRING_CASE(FE::internal::renderer::SM5_hull_shader_target) :
				m_shaders.back()._shader_target = FE::internal::renderer::ShaderTarget::_SM5_HullShader;
				break;

			STRING_CASE(FE::internal::renderer::SM5_domain_shader_target) :
				m_shaders.back()._shader_target = FE::internal::renderer::ShaderTarget::_SM5_DomainShader;
				break;

			STRING_CASE(FE::internal::renderer::SM5_compute_shader_target) :
				m_shaders.back()._shader_target = FE::internal::renderer::ShaderTarget::_SM5_ComputeShader;
				break;

			_FE_NODEFAULT_;
			}
		}
	}
}


void FE::renderer::__main(class FE::world&) noexcept
{
	/*
	auto& l_engine = FE::engine::get_engine();
	auto& l_renderer = l_engine.get_renderer(FE::engine::auth{});
	auto& l_shader_headers = l_engine.get_shader_headers(FE::engine::auth{});
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
			[&l_engine, &l_shaders, &l_token_lists, n]()
			{
				std::fstream l_file_stream(l_shaders[n]._source_path.c_str(), std::ios::in | std::ios::binary);
				FE::fstream_guard l_file_guard(l_file_stream);

				std::pmr::string l_buffer(l_engine.get_large_memory_resource());
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
				[&l_engine, &l_shaders, &l_permutations_compiled, n]()
				{
					l_shaders[n].compile(l_engine.get_program_options().is_recompile_shaders_enabled());
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

		l_renderer.register_shaders(l_shaders);

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}


	while (l_renderer.m_should_exit.load(std::memory_order_acquire) == false) 	
	{
		l_renderer.render_frame();
	}
	*/
}

END_NAMESPACE
