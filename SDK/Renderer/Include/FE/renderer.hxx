#ifndef _FE_RENDERER_HXX_
#define _FE_RENDERER_HXX_
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

#include <FE/framework/smart_ptr.hxx>

#include <FE/shader.hxx>

#pragma warning(disable: 4005)
#include <GLFW/glfw3.h>
#include <FE/d3d11_backend.hxx>




BEGIN_NAMESPACE(FE)

class processors;
class window;
class world;

class renderer
{
	friend FE::internal::renderer::backend;
	struct resolution
	{
		var::uint32 _width;
		var::uint32 _height;
	};

public:
	using gpu_info = internal::renderer::backend::gpu_info;
	using shader_header_list = absl::flat_hash_map<FE::directory_string, ::FE::internal::renderer::hlsli>;
	using shader_list = std::pmr::vector<::FE::internal::renderer::shader>;

private:
	FE::internal::renderer::backend m_backend;
	FE::clock m_renderer_clock;
	var::float64 m_delta_milliseconds;

	FE::smart_ptr<class FE::processors, FE::RefType::_Observer> m_processors;
	FE::smart_ptr<class FE::window, FE::RefType::_Observer> m_window;
	std::atomic_bool m_should_exit;

	shader_header_list m_shader_headers;
	shader_list m_shaders;
	FE::directory_string m_shader_root_directory;

public:
    renderer(FE::smart_ptr<class FE::processors, FE::RefType::_Observer> processors_p,
			 FE::smart_ptr<class FE::window, FE::RefType::_Observer> window_p) noexcept;
	~renderer() noexcept;


	_FE_FORCE_INLINE_ const gpu_info& get_gpu_info() const noexcept { return m_backend.get_gpu_info(); }

	_FE_FORCE_INLINE_ shader_header_list& get_shader_headers() noexcept { return m_shader_headers; }
	_FE_FORCE_INLINE_ shader_list& get_shaders() noexcept { return m_shaders; }

	_FE_FORCE_INLINE_ const FE::directory_string& get_shader_root_directory() const noexcept { return m_shader_root_directory; }

	_FE_FORCE_INLINE_ void register_shaders(std::pmr::vector<::FE::internal::renderer::shader>& shaders_p) noexcept
	{
		m_backend.register_shaders(shaders_p);
	}


	static void __main(class FE::world&) noexcept;
};


END_NAMESPACE
#endif