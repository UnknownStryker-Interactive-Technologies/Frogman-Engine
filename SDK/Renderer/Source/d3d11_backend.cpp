#include <FE/d3d11_backend.hpp>
#ifdef _FE_ON_WINDOWS_X86_64_
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
#include <FE/renderer.hpp>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>




BEGIN_NAMESPACE(FE::internal::renderer)


d3d11_backend::d3d11_backend(class FE::renderer* const frontend_p) noexcept
	:	m_frontend(frontend_p),
		m_device(),
		m_context(),
		m_swapchain(),
		m_render_target_view(),
		m_factory(),
		m_adapter(),
		m_adapter_desc(),
		m_should_allow_tearing(FALSE)
{
	UINT l_create_device_flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
	l_create_device_flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	l_create_device_flags |= D3D11_CREATE_DEVICE_VIDEO_SUPPORT;
#ifndef _DEBUG_
	l_create_device_flags |= D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT;
	l_create_device_flags |= D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY;
#else
	l_create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
	l_create_device_flags |= D3D11_CREATE_DEVICE_DEBUGGABLE;
#endif


#ifdef _DEBUG_
	FE_EXIT_IF(	CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_factory)) != S_OK,
				FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure,
				"Failed to create DXGI factory.");
#else
	FE_EXIT_IF(	CreateDXGIFactory2(0, IID_PPV_ARGS(&m_factory)) != S_OK,
				FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure,
				"Failed to create DXGI factory.");
#endif
									

	FE_EXIT_IF(	m_factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&m_adapter)) != S_OK, 
				FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
				"Failed to detect a GPU.");


	FE_EXIT_IF(	m_adapter->GetDesc3(&m_adapter_desc) != S_OK, 
				FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
				"Failed to retrieve the GPU description");


	m_factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &m_should_allow_tearing, sizeof(m_should_allow_tearing));


	wrl::ComPtr<ID3D11Device> l_device;
	wrl::ComPtr<ID3D11DeviceContext> l_context;
	constexpr D3D_FEATURE_LEVEL l_feature_level = D3D_FEATURE_LEVEL_11_1;
	FE_EXIT_IF(FAILED(D3D11CreateDevice(m_adapter.Get(),
										D3D_DRIVER_TYPE_HARDWARE, 
										nullptr,
										l_create_device_flags,
										&l_feature_level,
										1,
										D3D11_SDK_VERSION,
										&l_device,
										nullptr,
										&l_context)),
		FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
		"Failed to create D3D11 device.");
	l_device.As(&m_device);
	l_context.As(&m_context);

	HWND l_window_handle = glfwGetWin32Window(m_frontend->m_window);
	DXGI_SWAP_CHAIN_DESC1 l_swapchain_desc = {};
	l_swapchain_desc.Width = m_frontend->m_window_config._width;
	l_swapchain_desc.Height = m_frontend->m_window_config._height;
	l_swapchain_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	l_swapchain_desc.Stereo = m_frontend->m_window_config._is_virtual_reality_mode;
	l_swapchain_desc.SampleDesc.Count = 1; // No multi-sampling
	l_swapchain_desc.SampleDesc.Quality = 0; // multi-samplin quality level
	l_swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	l_swapchain_desc.BufferCount = m_frontend->m_window_config._swap_chain_buffer_count; 
	l_swapchain_desc.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;
	l_swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	l_swapchain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	l_swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;


}

d3d11_backend::~d3d11_backend() noexcept
{
}


END_NAMESPACE
#endif