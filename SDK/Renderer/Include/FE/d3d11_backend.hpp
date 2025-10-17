#ifndef _FE_RENDERER_D3D11_BACKEND_HPP_
#define _FE_RENDERER_D3D11_BACKEND_HPP_
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
#include <FE/prerequisites.h>
#ifdef _FE_ON_WINDOWS_X86_64_
#pragma comment(lib, "d3d11.lib")         // Direct3D 11 core
#pragma comment(lib, "dxgi.lib")          // DXGI for swap chain and adapter enumeration
#pragma comment(lib, "d3dcompiler.lib")   // Shader compilation
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
namespace wrl = ::Microsoft::WRL;




CLASS_FORWARD_DECLARATION(FE, renderer);


BEGIN_NAMESPACE(FE::internal::renderer)


class d3d11_backend
{
    class FE::renderer* const m_frontend;
    wrl::ComPtr<ID3D11Device5> m_device;
    wrl::ComPtr<ID3D11DeviceContext4> m_context;
    wrl::ComPtr<IDXGISwapChain4> m_swapchain;
    wrl::ComPtr<ID3D11RenderTargetView> m_render_target_view;
    wrl::ComPtr<IDXGIFactory7> m_factory;
	wrl::ComPtr<IDXGIAdapter4> m_adapter;
	DXGI_ADAPTER_DESC3 m_adapter_desc;
	BOOL m_should_allow_tearing;
    
public:
    d3d11_backend(class FE::renderer* const frontend_p) noexcept;
	~d3d11_backend() noexcept;

    void render_frame() noexcept;

    ID3D11Device* get_device() const noexcept { return m_device.Get(); }
    ID3D11DeviceContext* get_context() const noexcept { return m_context.Get(); }
};


END_NAMESPACE
#endif
#endif