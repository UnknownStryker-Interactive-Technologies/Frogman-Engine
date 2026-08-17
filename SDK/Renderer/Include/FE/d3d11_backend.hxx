#if !defined(_FE_RENDERER_D3D11_BACKEND_HXX_) && defined(_FE_ON_WINDOWS_X86_64_)
#define _FE_RENDERER_D3D11_BACKEND_HXX_
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
#include <FE/fqueue.hxx>

#include <FE/framework/smart_ptr.hxx>

#include <FE/constant_buffer.hxx>

#include <absl/container/flat_hash_map.h>
#include <glm/glm.hpp>
#include <span>

#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

namespace wrl
{
    template <typename T>
    using com_ptr = Microsoft::WRL::ComPtr<T>;
}

CLASS_FORWARD_DECLARATION(FE, window);




BEGIN_NAMESPACE(FE::internal::renderer)

class shader;

class d3d11_backend
{
public:
    using gpu_info = DXGI_ADAPTER_DESC1;
	using device = ID3D11Device5;
	using device_context = ID3D11DeviceContext4;

private:
    FE::smart_ptr<FE::window, FE::RefType::_Observer> m_window;
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
    wrl::com_ptr<ID3D11Debug> m_debug;
#endif
    wrl::com_ptr<IDXGIFactory7> m_factory;
    BOOL m_should_allow_tearing;
    wrl::com_ptr<IDXGIAdapter4> m_adapter;
	gpu_info m_adapter_desc;
    wrl::com_ptr<device> m_device;
    wrl::com_ptr<device_context> m_context;
    D3D11_VIEWPORT m_viewport;
    wrl::com_ptr<IDXGISwapChain4> m_swapchain;
    wrl::com_ptr<ID3D11Texture2D> m_frame_buffer;
    wrl::com_ptr<ID3D11RenderTargetView> m_render_target_view;
	wrl::com_ptr<ID3D11RasterizerState2> m_rasterizer_state;


    constant_buffer m_b0_updated_per_frame;
    constant_buffer m_b1_updated_per_pass;
    constant_buffer m_b2_updated_per_material;
    constant_buffer m_b3_updated_per_object;


	template <typename T>
    using cache_map = absl::flat_hash_map<  var::uint64, wrl::com_ptr<T>,
        absl::lts_20260107::DefaultHashContainerHash<var::uint64>,
        absl::lts_20260107::DefaultHashContainerEq<var::uint64>,
		std::pmr::polymorphic_allocator<std::pair<FE::uint64, wrl::com_ptr<T>>>
    >;
	
    cache_map<ID3D11VertexShader> m_vertex_shader_cache;
    cache_map<ID3D11PixelShader> m_pixel_shader_cache;
    cache_map<ID3D11GeometryShader> m_geometry_shader_cache;
    cache_map<ID3D11HullShader> m_hull_shader_cache;
    cache_map<ID3D11DomainShader> m_domain_shader_cache;
    cache_map<ID3D11ComputeShader> m_compute_shader_cache;

    cache_map<ID3D11InputLayout> m_input_layout_cache;

public:
    d3d11_backend(FE::smart_ptr<FE::window, FE::RefType::_Observer> window_p) noexcept;
	~d3d11_backend() noexcept;


    _FE_FORCE_INLINE_ device* get_device() const noexcept { return m_device.Get(); }
    _FE_FORCE_INLINE_ device_context* get_context() const noexcept { return m_context.Get(); }
    _FE_FORCE_INLINE_ const gpu_info& get_gpu_info() const noexcept { return m_adapter_desc; }


	void resize_swap_chain_buffers(FE::int32 new_width_p, FE::int32 new_height_p) noexcept;


	void begin_frame() noexcept;
    void end_frame() noexcept;


    void register_shaders(std::pmr::vector<class ::FE::internal::renderer::shader>& shaders_p) noexcept;
	//void create_hlsl_struct_memory_layout(std::pmr::vector<class ::FE::internal::renderer::shader>& shaders_p) noexcept;
};
using backend = d3d11_backend;


END_NAMESPACE
#endif