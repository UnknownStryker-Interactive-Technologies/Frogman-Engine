#include <FE/d3d11_backend.hxx>
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
#include <FE/renderer.hxx>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <FE/type_traits.hxx>
#include <FE/d3d11_backend.hxx>




BEGIN_NAMESPACE(FE::internal::renderer)


wrl::ComPtr<ID3DBlob> __compile_shader_from_file(FE::ASCII* const file_path_p, FE::ASCII* const entry_point_p, const ShaderTarget target_p) noexcept
{
	FE_ASSERT(file_path_p != nullptr);
	FE_ASSERT(entry_point_p != nullptr);

	var::wchar l_wide_path[_ALLOWED_DIRECTORY_LENGTH_] = L"\0";
	_FE_MAYBE_UNUSED_ FE::int32 l_length = MultiByteToWideChar(CP_UTF8, NULL, file_path_p, (int)strlen(file_path_p) + 1, l_wide_path, _ALLOWED_DIRECTORY_LENGTH_);
	FE_ASSERT(l_length > 0);

	var::uint32 l_flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS;
#ifdef _DEBUG_
	l_flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL0;
#elif defined(_RELWITHDEBINFO_)
	l_flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#else
	l_flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

	FE::ASCII* l_target = nullptr;
	switch (target_p)
	{
	case ShaderTarget::_VertexShader:
		l_target = vertex_shader_target;
		break;

	case ShaderTarget::_PixelShader:
		l_target = pixel_shader_target;
		break;

	case ShaderTarget::_GeometryShader:
		l_target = geometry_shader_target;
		break;

	case ShaderTarget::_HullShader:
		l_target = hull_shader_target;
		break;

	case ShaderTarget::_DomainShader:
		l_target = domain_shader_target;
		break;

	case ShaderTarget::_ComputeShader:
		l_target = compute_shader_target;
		break;

		_FE_NODEFAULT_;
	}

	wrl::ComPtr<ID3DBlob> l_bytecode;
	wrl::ComPtr<ID3DBlob> l_errors;
	const HRESULT l_result = D3DCompileFromFile(l_wide_path,
												nullptr,
												D3D_COMPILE_STANDARD_FILE_INCLUDE,
												entry_point_p,
												l_target,
												l_flags,
												0, // Legacy flag, should be set to 0
												&l_bytecode,
												&l_errors
	);

	if (l_errors != nullptr)
	{
		OutputDebugStringA((FE::ASCII*)l_errors->GetBufferPointer());
	}

	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_ShaderCompilationFailure, "Shader compilation failed.");

	return l_bytecode;
}


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
		m_present_params(),
		m_clear_color{ 0.0f, 0.0f, 0.0f, 1.0f },
		m_viewport{},

		m_depth_stencil_buffer(),
		m_depth_stencil_view(),
		m_depth_stencil_state()
{
	UINT l_create_device_flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
	l_create_device_flags |= D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	l_create_device_flags |= D3D11_CREATE_DEVICE_VIDEO_SUPPORT;
#if defined(_RELEASE_) || defined(_MINSIZEREL_)
	l_create_device_flags |= D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT;
	l_create_device_flags |= D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY;
#elif defined(_RELWITHDEBINFO_)
	l_create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#elif defined(_DEBUG_)
	l_create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT l_result = 0;
#ifdef _DEBUG_
	l_result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_factory));
	FE_EXIT_IF( l_result != S_OK,
				FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure,
				"Failed to create DXGI factory; the error code is ${%d@0}.", &l_result);
#else
	l_result = CreateDXGIFactory2(0, IID_PPV_ARGS(&m_factory));
	FE_EXIT_IF( l_result != S_OK,
				FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure,
				"Failed to create DXGI factory; the error code is ${%d@0}.", &l_result);
#endif
									
	l_result = m_factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&m_adapter));
	FE_EXIT_IF( l_result != S_OK,
				FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
				"Failed to detect a GPU; the error code is ${%d@0}.", &l_result);

	l_result = m_adapter->GetDesc3(&m_adapter_desc);
	FE_EXIT_IF( l_result != S_OK,
				FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
				"Failed to retrieve the GPU description; the error code is ${%d@0}.", &l_result);


	wrl::ComPtr<ID3D11Device> l_device;
	wrl::ComPtr<ID3D11DeviceContext> l_context;
	constexpr D3D_FEATURE_LEVEL l_feature_level[] = { D3D_FEATURE_LEVEL_11_1 };
	_FE_MAYBE_UNUSED_ D3D_FEATURE_LEVEL l_actual_feature_level;
	l_result = D3D11CreateDevice(m_adapter.Get(),
		D3D_DRIVER_TYPE_UNKNOWN,
		nullptr,
		l_create_device_flags,
		l_feature_level,
		sizeof(l_feature_level) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&l_device,
		&l_actual_feature_level,
		&l_context);
	FE_EXIT_IF(FAILED(l_result),
		FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
		"Failed to create D3D11 device; the error code is ${%d@0}.", &l_result);

	l_result = l_device.As(&m_device);
	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, "D3D11.4 unsupported; the error code is ${%d@0}.", &l_result);
	
	l_result = l_context.As(&m_context);
	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, "D3D11.4 unsupported; the error code is ${%d@0}.", &l_result);
	
	
	m_factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &m_should_allow_tearing, sizeof(m_should_allow_tearing));

#ifdef _DEBUG_
	if ( SUCCEEDED( m_device.As(&m_debug) ) )
	{
		m_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}
#endif

	
	HWND l_window_handle = glfwGetWin32Window(m_frontend->m_window);
	DXGI_SWAP_CHAIN_DESC1 l_swapchain_desc = 
	{
		.Width = (UINT)m_frontend->m_video_mode->width,
		.Height = (UINT)m_frontend->m_video_mode->height,
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = m_frontend->m_window_config._swap_chain_buffer_count,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = (m_should_allow_tearing == TRUE) ? (UINT)DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u,
	};
	l_swapchain_desc.SampleDesc.Count = 1; // No multi-sampling
	l_swapchain_desc.SampleDesc.Quality = 0; // multi-sampling quality level
	l_swapchain_desc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	DXGI_SWAP_CHAIN_FULLSCREEN_DESC l_fullscreen_desc = 
	{
		.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		.Scaling = DXGI_MODE_SCALING_UNSPECIFIED,
		.Windowed = !(m_frontend->m_window_config._is_fullscreen)
	};
	l_fullscreen_desc.RefreshRate.Numerator = m_frontend->m_video_mode->refreshRate;
	l_fullscreen_desc.RefreshRate.Denominator = 1;


	wrl::ComPtr<IDXGISwapChain1> l_swap_chain;

	l_result = m_factory->CreateSwapChainForHwnd(m_device.Get(), l_window_handle, &l_swapchain_desc, &l_fullscreen_desc, nullptr, &l_swap_chain);
	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererSwapChainCreationFailure, "Fail to create a swap chain; the error code is ${%d@0}.", &l_result);

	m_factory->MakeWindowAssociation(l_window_handle, DXGI_MWA_NO_WINDOW_CHANGES);

	l_result = l_swap_chain.As(&m_swapchain);
	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererSwapChainCreationFailure, "D3D11.4 unsupported; the error code is ${%d@0}.", &l_result);

	l_result = m_swapchain->GetBuffer(0, IID_PPV_ARGS(&m_back_buffer));
	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererSwapChainCreationFailure, "Fail to create the back buffer; the error code is ${%d@0}.", &l_result);
	
	_FE_MAYBE_UNUSED_ D3D11_TEXTURE2D_DESC l_desc{};
	m_back_buffer->GetDesc(&l_desc);
	FE_ASSERT(l_desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM, "Unexpected back buffer format; the error code is ${%d@0}.", &l_result);

	l_result = m_device->CreateRenderTargetView(m_back_buffer.Get(), nullptr, &m_render_target_view);
	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererRenderTargetViewCreationFailure, "Fail to create a render target view; the error code is ${%d@0}.", &l_result);


	m_viewport =
	{
		.TopLeftX = 0.0f,
		.TopLeftY = 0.0f,
		.Width = static_cast<FLOAT>(m_frontend->m_video_mode->width),
		.Height = static_cast<FLOAT>(m_frontend->m_video_mode->height),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f
	};

	m_context->RSSetViewports(1, &m_viewport);


	D3D11_TEXTURE2D_DESC l_depth_desc =
	{
		.Width = static_cast<UINT>(m_frontend->m_video_mode->width),
		.Height = static_cast<UINT>(m_frontend->m_video_mode->height),
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
		.SampleDesc = {.Count = 1, .Quality = 0 }, // MSAA disabled
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_DEPTH_STENCIL,
	};

	l_result = m_device->CreateTexture2D(&l_depth_desc, nullptr, &m_depth_stencil_buffer);
	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure,
		"Failed to create depth stencil buffer; the error code is ${%d@0}.", &l_result);

	l_result = m_device->CreateDepthStencilView(m_depth_stencil_buffer.Get(), nullptr, &m_depth_stencil_view);
	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure,
		"Failed to create depth stencil view; the error code is ${%d@0}.", &l_result);


	D3D11_DEPTH_STENCIL_DESC l_ds_state_desc =
	{
		.DepthEnable = TRUE,
		.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		.DepthFunc = D3D11_COMPARISON_LESS,
		.StencilEnable = FALSE,
	};

	l_result = m_device->CreateDepthStencilState(&l_ds_state_desc, &m_depth_stencil_state);
	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure,
		"Failed to create depth stencil state; the error code is ${%d@0}.", &l_result);

	m_context->OMSetDepthStencilState(m_depth_stencil_state.Get(), 0);


	m_context->OMSetRenderTargets(1, m_render_target_view.GetAddressOf(), m_depth_stencil_view.Get());
}

d3d11_backend::~d3d11_backend() noexcept
{
	BOOL l_is_fullscreen = FALSE;
	m_swapchain->GetFullscreenState(&l_is_fullscreen, nullptr);
	if (l_is_fullscreen == TRUE)
	{
		m_swapchain->SetFullscreenState(FALSE, nullptr);
	}
}


void d3d11_backend::resize_swap_chain_buffers(FE::int32 new_width_p, FE::int32 new_height_p) noexcept
{
	m_context->OMSetRenderTargets(0, nullptr, nullptr);

	m_render_target_view.Reset();
	m_back_buffer.Reset();


	FE_EXIT_IF
	(
		FAILED
		(
			m_swapchain->ResizeBuffers
			(
				m_frontend->m_window_config._swap_chain_buffer_count,
				new_width_p,
				new_height_p,
				DXGI_FORMAT_B8G8R8A8_UNORM,
				((m_should_allow_tearing == TRUE) ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0) | DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
			) 
		),
		FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
		"Failed to resize the swap chain buffers."
	);

	// Recreate back buffer
	FE_EXIT_IF(FAILED(m_swapchain->GetBuffer(0, IID_PPV_ARGS(&m_back_buffer))), 
		FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
		"Failed to recreate the back buffer.");

	_FE_MAYBE_UNUSED_ D3D11_TEXTURE2D_DESC l_desc{};
	m_back_buffer->GetDesc(&l_desc);
	FE_ASSERT(l_desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM, "Unexpected back buffer format.");

	// Recreate render target view
	FE_EXIT_IF(FAILED(m_device->CreateRenderTargetView(m_back_buffer.Get(), nullptr, &m_render_target_view)),
		FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
		"Failed to recreate the render target view.");




	m_depth_stencil_view.Reset();
	m_depth_stencil_buffer.Reset();


	D3D11_TEXTURE2D_DESC l_depth_desc =
	{
		.Width = static_cast<UINT>(new_width_p),
		.Height = static_cast<UINT>(new_height_p),
		.MipLevels = 1,
		.ArraySize = 1,
		.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
		.SampleDesc = {.Count = 1, .Quality = 0 },
		.Usage = D3D11_USAGE_DEFAULT,
		.BindFlags = D3D11_BIND_DEPTH_STENCIL,
	};

	FE_EXIT_IF(FAILED(m_device->CreateTexture2D(&l_depth_desc, nullptr, &m_depth_stencil_buffer)),
		FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure,
		"Failed to recreate depth stencil buffer.");

	FE_EXIT_IF(FAILED(m_device->CreateDepthStencilView(m_depth_stencil_buffer.Get(), nullptr, &m_depth_stencil_view)),
		FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure,
		"Failed to recreate depth stencil view.");


	m_viewport =
	{
		.TopLeftX = 0.0f,
		.TopLeftY = 0.0f,
		.Width = static_cast<FLOAT>(new_width_p),
		.Height = static_cast<FLOAT>(new_height_p),
		.MinDepth = 0.0f,
		.MaxDepth = 1.0f
	};

	m_context->RSSetViewports(1, &m_viewport);


	// Bind new render target
	m_context->OMSetRenderTargets(1, m_render_target_view.GetAddressOf(), m_depth_stencil_view.Get());
}

void d3d11_backend::render_frame() noexcept
{
	m_context->ClearRenderTargetView(m_render_target_view.Get(), m_clear_color);
	m_context->ClearDepthStencilView(m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	const HRESULT l_result = m_swapchain->Present1(	m_frontend->m_window_config._should_enable_vsync,
													((m_frontend->m_window_config._should_enable_vsync  == false) && (m_should_allow_tearing == TRUE)) ? DXGI_PRESENT_ALLOW_TEARING : 0,
													&m_present_params);

	if(l_result == DXGI_STATUS_OCCLUDED) _FE_UNLIKELY_
	{
		std::this_thread::yield();
	}
}


END_NAMESPACE
#endif