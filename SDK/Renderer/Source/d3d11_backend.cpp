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
		m_back_buffer(),
		m_render_target_view(),
		m_factory(),
		m_adapter(),
		m_adapter_desc(),
		m_should_allow_tearing(FALSE),
#ifdef _DEBUG_
		m_debug(),
#endif
		m_present_params()
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


	wrl::ComPtr<ID3D11Device> l_device;
	wrl::ComPtr<ID3D11DeviceContext> l_context;
	constexpr D3D_FEATURE_LEVEL l_feature_level = D3D_FEATURE_LEVEL_11_1;
	_FE_MAYBE_UNUSED_ D3D_FEATURE_LEVEL l_actual_feature_level;
	FE_EXIT_IF(FAILED(D3D11CreateDevice(m_adapter.Get(),
										D3D_DRIVER_TYPE_UNKNOWN, 
										nullptr,
										l_create_device_flags,
										&l_feature_level,
										1,
										D3D11_SDK_VERSION,
										&l_device,
										&l_actual_feature_level,
										&l_context)),
		FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
		"Failed to create D3D11 device.");
	FE_ASSERT(l_actual_feature_level == D3D_FEATURE_LEVEL_11_1, "D3D_FEATURE_LEVEL_11_1 unsupported.");
	FE_EXIT_IF(FAILED(l_device.As(&m_device)), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, "D3D11.4 unsupported.");
	FE_EXIT_IF(FAILED(l_context.As(&m_context)), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, "D3D11.4 unsupported.");
	
	
	m_factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &m_should_allow_tearing, sizeof(m_should_allow_tearing));

#ifdef _DEBUG_
	if ( SUCCEEDED( m_device.As(&m_debug) ) )
	{
		m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
#endif

	HWND l_window_handle = glfwGetWin32Window(m_frontend->m_window);
	DXGI_SWAP_CHAIN_DESC1 l_swapchain_desc{};
	l_swapchain_desc.Width = m_frontend->m_window_config._width;
	l_swapchain_desc.Height = m_frontend->m_window_config._height;
	l_swapchain_desc.Format = (m_frontend->m_window_config._should_enable_hdr == true) ? DXGI_FORMAT_R10G10B10A2_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM;
	l_swapchain_desc.Stereo = m_frontend->m_window_config._is_virtual_reality_mode;
	l_swapchain_desc.SampleDesc.Count = 1; // No multi-sampling
	l_swapchain_desc.SampleDesc.Quality = 0; // multi-samplin quality level
	l_swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	l_swapchain_desc.BufferCount = m_frontend->m_window_config._swap_chain_buffer_count; 
	l_swapchain_desc.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;
	l_swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	l_swapchain_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	l_swapchain_desc.Flags = (m_should_allow_tearing == TRUE) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
	l_swapchain_desc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC l_fullscreen_desc{};
	l_fullscreen_desc.RefreshRate.Numerator = m_frontend->m_video_mode->refreshRate;
	l_fullscreen_desc.RefreshRate.Denominator = 1;
	l_fullscreen_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	l_fullscreen_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	l_fullscreen_desc.Windowed = !(m_frontend->m_window_config._is_fullscreen);

	wrl::ComPtr<IDXGISwapChain1> l_swap_chain;
	FE_EXIT_IF(FAILED(m_factory->CreateSwapChainForHwnd(m_device.Get(), l_window_handle, &l_swapchain_desc, &l_fullscreen_desc, nullptr, &l_swap_chain)), FE::ErrorCode::_FatalRendererError_5XX_RendererSwapChainCreationFailure, "Fail to create a swap chain.");
	FE_EXIT_IF(FAILED(l_swap_chain.As(&m_swapchain)), FE::ErrorCode::_FatalRendererError_5XX_RendererSwapChainCreationFailure, "D3D11.4 unsupported.");
	FE_EXIT_IF(FAILED(m_swapchain->GetBuffer(0, IID_PPV_ARGS(&m_back_buffer))), FE::ErrorCode::_FatalRendererError_5XX_RendererSwapChainCreationFailure, "Fail to create the back buffer.");
	
	_FE_MAYBE_UNUSED_ D3D11_TEXTURE2D_DESC l_desc{};
	m_back_buffer->GetDesc(&l_desc);
	FE_ASSERT(l_desc.Format == (m_frontend->m_window_config._should_enable_hdr == true) ? DXGI_FORMAT_R10G10B10A2_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM, "Unexpected back buffer format.");

	FE_EXIT_IF(FAILED(m_device->CreateRenderTargetView(m_back_buffer.Get(), nullptr, &m_render_target_view)), FE::ErrorCode::_FatalRendererError_5XX_RendererRenderTargetViewCreationFailure, "Fail to create a render target view.");
	m_context->OMSetRenderTargets(1, m_render_target_view.GetAddressOf(), nullptr);
}

d3d11_backend::~d3d11_backend() noexcept
{
}


void d3d11_backend::toggle_fullscreen_mode() noexcept
{
	wrl::ComPtr<IDXGIOutput> l_output;
	_FE_MAYBE_UNUSED_ HRESULT l_result = m_swapchain->GetContainingOutput(&l_output);
	FE_ASSERT(SUCCEEDED(l_result));

	m_swapchain->SetFullscreenState((BOOL)m_frontend->m_window_config._is_fullscreen, l_output.Get());
}

void d3d11_backend::resize_swap_chain_buffers(FE::int32 new_width_p, FE::int32 new_height_p) noexcept
{
	m_render_target_view.Reset();
	m_back_buffer.Reset();

	FE_EXIT_IF(FAILED(m_swapchain->ResizeBuffers(m_frontend->m_window_config._swap_chain_buffer_count,
								new_width_p,
								new_height_p,
								(m_frontend->m_window_config._should_enable_hdr == true) ? DXGI_FORMAT_R10G10B10A2_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM,
								(m_should_allow_tearing == TRUE) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0) | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH),
		FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
		"Failed to resize the swap chain buffers.");

	// Recreate back buffer
	FE_EXIT_IF(FAILED(m_swapchain->GetBuffer(0, IID_PPV_ARGS(&m_back_buffer))), 
		FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
		"Failed to recreate the back buffer.");

	_FE_MAYBE_UNUSED_ D3D11_TEXTURE2D_DESC l_desc{};
	m_back_buffer->GetDesc(&l_desc);
	FE_ASSERT(l_desc.Format == (m_frontend->m_window_config._should_enable_hdr == true) ? DXGI_FORMAT_R10G10B10A2_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM, "Unexpected back buffer format.");

	// Recreate render target view
	FE_EXIT_IF(FAILED(m_device->CreateRenderTargetView(m_back_buffer.Get(), nullptr, &m_render_target_view)),
		FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
		"Failed to recreate the render target view.");

	// Bind new render target
	m_context->OMSetRenderTargets(1, m_render_target_view.GetAddressOf(), nullptr);
}

void d3d11_backend::render_frame() noexcept
{
	FE::float32 l_clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_context->ClearRenderTargetView(m_render_target_view.Get(), l_clear_color);

	m_swapchain->Present1(	m_frontend->m_window_config._should_enable_vsync,
							((m_frontend->m_window_config._should_enable_vsync  == false) && (m_should_allow_tearing == TRUE)) ? DXGI_PRESENT_ALLOW_TEARING : 0,
							&m_present_params);
}


END_NAMESPACE
#endif