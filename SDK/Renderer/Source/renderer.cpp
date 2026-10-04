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

#include <FE/engine.hpp>

#include <FE/processors.hxx>
#include <FE/window.hxx>

#include <atomic>

#include <taskflow.hpp>




BEGIN_NAMESPACE(FE)


renderer::renderer(FE::smart_ptr<FE::processors, FE::RefType::_Observer> processors_p, FE::smart_ptr<FE::window, FE::RefType::_Observer> window_p) noexcept
	:	m_backend(window_p),
		m_renderer_clock(),
		m_delta_milliseconds(0.0),

		m_processors(processors_p),
		m_window(window_p),

		m_shader_headers(),
		m_shaders(framework::framework_base::get_framework().get_large_memory_resource()),
		m_shader_root_directory(engine::get_engine().get_game_root_directory())
{
	m_shader_root_directory += FE_TEXT(\\Assets\\Shaders);

	std::ifstream l_froggy(engine::get_engine().get_froggy_path().c_str(), std::ios::binary);
	FE::ifstream_guard l_froggy_file_stream(l_froggy);
	FE_ASSERT(l_froggy.is_open() == true, "Failed to open froggy file at path: %s", engine::get_engine().get_froggy_path().c_str());

	Json::Value l_froggy_json;
	l_froggy >> l_froggy_json;
	{
		m_shader_headers.reserve(1024); // reserve some arbitrary amount of headers to avoid too many reallocations; this value can be changed later if needed.
		FE_ASSERT(l_froggy_json["ShaderHeaders"].isArray() == true);

		FE::directory_string l_path(framework::framework_base::get_framework().get_large_memory_resource());
		for (auto& element : l_froggy_json["ShaderHeaders"])
		{
			FE_ASSERT(element.isString() == true);
			const Json::String l_tmp = element.asString();
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
		FE_ASSERT(l_froggy_json["Shaders"].isArray() == true);
		for (auto& shader : l_froggy_json["Shaders"])
		{
			m_shaders.emplace_back();
			m_shaders.back()._defines = std::pmr::vector<FE::internal::renderer::shader_define>(framework::framework_base::get_framework().get_large_memory_resource());
			m_shaders.back()._permutation_blacklist = std::pmr::vector<std::pmr::string>(framework::framework_base::get_framework().get_large_memory_resource());
			m_shaders.back()._macro_combinations = std::pmr::vector<std::pmr::vector<FE::internal::renderer::shader::macro>>(framework::framework_base::get_framework().get_large_memory_resource());
			m_shaders.back()._main_function = std::pmr::string(framework::framework_base::get_framework().get_large_memory_resource());
			m_shaders.back()._source_path = std::pmr::wstring(framework::framework_base::get_framework().get_large_memory_resource());

			auto& l_shader = shader;
			FE_ASSERT(l_shader["Defines"].isArray() == true);
			for (auto& define : l_shader["Defines"])
			{
				for (auto define_it = define.begin(); define_it != define.end(); ++define_it)
				{
					const Json::String identifier = define_it.name();
					const Json::Value& value_range = *define_it;
					m_shaders.back()._defines.emplace_back();
					m_shaders.back()._defines.back()._identifier = std::pmr::string(identifier.c_str(), framework::framework_base::get_framework().get_large_memory_resource());
					FE_ASSERT(value_range.isArray() == true);
					FE_ASSERT(value_range.size() == 2);
					FE_ASSERT(value_range[0u].isInt64() == true);
					FE_ASSERT(value_range[1u].isInt64() == true);
					m_shaders.back()._defines.back()._value_range._first = value_range[0u].asInt64();
					m_shaders.back()._defines.back()._value_range._second = value_range[1u].asInt64();
					FE_ASSERT(m_shaders.back()._defines.back()._value_range._first <= m_shaders.back()._defines.back()._value_range._second);
					m_shaders.back()._defines.back()._current_value = m_shaders.back()._defines.back()._value_range._first; // set current value to the minimum value in the range by default
				}
			}

			for (auto& blacklist : l_shader["PermutationBlacklist"])
			{
				FE_ASSERT(blacklist.isString() == true);
				m_shaders.back()._permutation_blacklist.push_back(std::pmr::string(blacklist.asCString(), framework::framework_base::get_framework().get_large_memory_resource()));
			}

			FE_ASSERT(l_shader["MainFunction"].isString() == true);
			m_shaders.back()._main_function = l_shader["MainFunction"].asCString();

			FE_ASSERT(l_shader["Source"].isString() == true);
			const Json::String l_tmp = l_shader["Source"].asString();
			m_shaders.back()._source_path = std::pmr::wstring(l_tmp.begin(), l_tmp.end(), framework::framework_base::get_framework().get_large_memory_resource());

			auto l_pos = m_shaders.back()._source_path.rfind(L"\\");
			FE_ASSERT(l_pos != std::pmr::string::npos, "Failed to find last occurrence of '\\' in shader header path.");

			m_shaders.back()._source_path.replace(m_shaders.back()._source_path.begin(),
													m_shaders.back()._source_path.begin() + l_pos,

													std::pmr::wstring(m_shader_root_directory.begin(), m_shader_root_directory.end())
			);


			STRING_SWITCH(l_shader["ShaderTarget"].asCString())
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

FE::renderer::~renderer() noexcept
{

}


void FE::renderer::__main(class FE::world&) noexcept
{
	auto& l_engine = FE::engine::get_engine();

	while (l_engine.should_tick())
	{

	}
}


END_NAMESPACE
