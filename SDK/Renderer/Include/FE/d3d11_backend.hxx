#ifndef _FE_RENDERER_D3D11_BACKEND_HXX_
#define _FE_RENDERER_D3D11_BACKEND_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

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

#include <glm/glm.hpp>

#include <span>

#include <absl/container/flat_hash_map.h>

#ifdef _FE_ON_WINDOWS_X86_64_
#include <d3dcommon.h>
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
namespace wrl = ::Microsoft::WRL;
#endif




CLASS_FORWARD_DECLARATION(FE, renderer);




BEGIN_NAMESPACE(FE::internal::renderer)

class shader;

constexpr FE::ASCII* const SM5_vertex_shader_target = "vs_5_0";
constexpr FE::ASCII* const SM5_pixel_shader_target = "ps_5_0";
constexpr FE::ASCII* const SM5_geometry_shader_target = "gs_5_0";
constexpr FE::ASCII* const SM5_hull_shader_target = "hs_5_0";
constexpr FE::ASCII* const SM5_domain_shader_target = "ds_5_0";
constexpr FE::ASCII* const SM5_compute_shader_target = "cs_5_0";

constexpr FE::ASCII* const SM6_vertex_shader_target = "vs_6_0";
constexpr FE::ASCII* const SM6_pixel_shader_target = "ps_6_0";
constexpr FE::ASCII* const SM6_geometry_shader_target = "gs_6_0";
constexpr FE::ASCII* const SM6_hull_shader_target = "hs_6_0";
constexpr FE::ASCII* const SM6_domain_shader_target = "ds_6_0";
constexpr FE::ASCII* const SM6_compute_shader_target = "cs_6_0";

enum struct ShaderTarget
{
    _SM5_VertexShader,
    _SM5_PixelShader,
    _SM5_GeometryShader,
    _SM5_HullShader,
    _SM5_DomainShader,
    _SM5_ComputeShader
};


class d3d11_backend
{
public:
    using gpu_info = DXGI_ADAPTER_DESC3;

private:
    class FE::renderer* const m_frontend;
    wrl::ComPtr<ID3D11Device5> m_device;
    wrl::ComPtr<ID3D11DeviceContext4> m_context;
    wrl::ComPtr<IDXGISwapChain4> m_swapchain;
    wrl::ComPtr<ID3D11Texture2D1> m_back_buffer;
    wrl::ComPtr<ID3D11RenderTargetView> m_render_target_view;
    wrl::ComPtr<IDXGIFactory7> m_factory;
	wrl::ComPtr<IDXGIAdapter4> m_adapter;
    gpu_info m_adapter_desc;
	BOOL m_should_allow_tearing;
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
    wrl::ComPtr<ID3D11Debug> m_debug;
#endif
    DXGI_PRESENT_PARAMETERS m_present_params;
    FE::float32 m_clear_color[4];
	D3D11_VIEWPORT m_viewport;

    wrl::ComPtr<ID3D11Texture2D> m_depth_stencil_buffer;
    wrl::ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;
    wrl::ComPtr<ID3D11DepthStencilState> m_depth_stencil_state;

	absl::flat_hash_map<var::uint64, wrl::ComPtr<ID3D11VertexShader>> m_vertex_shader_cache;
	absl::flat_hash_map<var::uint64, wrl::ComPtr<ID3D11PixelShader>> m_pixel_shader_cache;
    absl::flat_hash_map<var::uint64, wrl::ComPtr<ID3D11GeometryShader>> m_geometry_shader_cache;
	absl::flat_hash_map<var::uint64, wrl::ComPtr<ID3D11HullShader>> m_hull_shader_cache;
	absl::flat_hash_map<var::uint64, wrl::ComPtr<ID3D11DomainShader>> m_domain_shader_cache;
	absl::flat_hash_map<var::uint64, wrl::ComPtr<ID3D11ComputeShader>> m_compute_shader_cache;

public:
    d3d11_backend(class FE::renderer* const frontend_p) noexcept;
	~d3d11_backend() noexcept;

	void resize_swap_chain_buffers(FE::int32 new_width_p, FE::int32 new_height_p) noexcept;

	void begin_frame() noexcept;
    void end_frame() noexcept;

    _FE_FORCE_INLINE_ ID3D11Device5* get_device() const noexcept { return m_device.Get(); }
    _FE_FORCE_INLINE_ ID3D11DeviceContext4* get_context() const noexcept { return m_context.Get(); }
    _FE_FORCE_INLINE_ const gpu_info& get_gpu_info() const noexcept { return m_adapter_desc; }

    void register_shaders(std::pmr::vector<class ::FE::internal::renderer::shader>& shaders_p) noexcept;
};

#ifdef _FE_ON_WINDOWS_X86_64_
using backend = d3d11_backend;
#endif

END_NAMESPACE
#endif