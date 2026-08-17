#include <FE/d3d11_backend.hxx>
#if defined(_FE_RENDERER_D3D11_BACKEND_HXX_) && defined(_FE_ON_WINDOWS_X86_64_)
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
// FE.Core
#include <FE/type_traits.hxx>

#include <FE/framework/framework.hxx>

// FE.Renderer
#include <FE/window.hxx>
#include <FE/shader.hxx>
#include <Shaders/constant_buffer_layouts.h>

// STL
#include <algorithm>

// GLFW
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>






BEGIN_NAMESPACE(FE::internal::renderer)


d3d11_backend::d3d11_backend(FE::smart_ptr<FE::window, FE::RefType::_Observer> window_p) noexcept
    :   m_window(window_p),
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
        m_debug(),
#endif
        m_factory(),
        m_should_allow_tearing(),
        m_adapter(),
        m_adapter_desc{},
        m_device(),
        m_context(),
        m_viewport{},
        m_swapchain(),
        m_frame_buffer(),
        m_render_target_view(),
        m_rasterizer_state(),

        m_b0_updated_per_frame(),
        m_b1_updated_per_pass(),
        m_b2_updated_per_material(),
        m_b3_updated_per_object(),

        m_vertex_shader_cache(FE::framework::framework_base::get_framework().get_large_memory_resource()),
        m_pixel_shader_cache(FE::framework::framework_base::get_framework().get_large_memory_resource()),
        m_geometry_shader_cache(FE::framework::framework_base::get_framework().get_large_memory_resource()),
        m_hull_shader_cache(FE::framework::framework_base::get_framework().get_large_memory_resource()),
        m_domain_shader_cache(FE::framework::framework_base::get_framework().get_large_memory_resource()),
        m_compute_shader_cache(FE::framework::framework_base::get_framework().get_large_memory_resource()),

        m_input_layout_cache(FE::framework::framework_base::get_framework().get_large_memory_resource())
{
    FE_ASSERT(m_window.is_valid() == true, "The window pointer is null.");
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
    const UINT l_dxgi_factory_flags = DXGI_CREATE_FACTORY_DEBUG;
#else
    const UINT l_dxgi_factory_flags = 0;
#endif
    HRESULT l_result = CreateDXGIFactory2(l_dxgi_factory_flags, IID_PPV_ARGS(&m_factory));
    FE_EXIT_IF(	FAILED(l_result),
                FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDxgiFactoryCreationFailure,
                "Failed to create DXGI factory; the error code is ${%d@0}. If the error code is DXGI_ERROR_SDK_COMPONENT_MISSING (0x887A002DL) and if it occurs on Debug or RelWithDebInfo build, please install the Windows Graphics Tools optional feature.", &l_result
    );


    if (FAILED(m_factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &m_should_allow_tearing, sizeof(m_should_allow_tearing))))
    {
        m_should_allow_tearing = FALSE;
    }
        

    for (UINT i = 0; ; ++i)
    {
        l_result = m_factory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&m_adapter));

        FE_EXIT_IF( FAILED(l_result),
                    FE::ErrorCode::_FatalRendererError_5XX_RendererBackendGpuDetectionFailure,
                    "Failed to detect a GPU; the error code is ${%d@0}. Please ensure hardware GPU driver is available.", &l_result
        );

        l_result = m_adapter->GetDesc1(&m_adapter_desc);
        constexpr UINT l_vendor_id_microsoft = 0x1414;
        if (FAILED(l_result) || 
            (m_adapter_desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) || 
            (m_adapter_desc.VendorId == l_vendor_id_microsoft)
            )
        {
            continue;
        }

        break;
    }


    wrl::com_ptr<ID3D11Device> l_device;
    wrl::com_ptr<ID3D11DeviceContext> l_context;
    UINT l_create_device_flags = /*D3D11_CREATE_DEVICE_SINGLETHREADED |*/ D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_RELEASE_) || defined(_MINSIZEREL_)
    l_create_device_flags |= D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY;
#elif defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
    l_create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
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
        &l_context
    );
    FE_EXIT_IF( FAILED(l_result),
                FE::ErrorCode::_FatalRendererError_5XX_RendererBackendDeviceCreationFailure, 
                "Failed to create D3D11 device; the error code is ${%d@0}.", &l_result
    );
    l_result = l_device.As(&m_device);
    FE_EXIT_IF( FAILED(l_result), 
                FE::ErrorCode::_FatalRendererError_5XX_RendererBackendID3D11Device5Unavailable, 
                "D3D11.4 maybe unsupported; ID3D11Device5 is unavailable. The error code is ${%d@0}.", &l_result
    );
    l_result = l_context.As(&m_context);
    FE_EXIT_IF( FAILED(l_result), 
                FE::ErrorCode::_FatalRendererError_5XX_RendererBackendID3D11DeviceContext4Unavailable, 
                "D3D11.4 maybe unsupported; ID3D11DeviceContext4 is unavailable. The error code is ${%d@0}.", &l_result
    );
#if defined(_DEBUG_) || defined(_RELWITHDEBINFO_)
    l_result = m_device.As(&m_debug);
    FE_EXIT_IF( FAILED(l_result),
		        FE::ErrorCode::_FatalRendererError_5XX_RendererBackendID3D11DebugUnavailable,
		        "D3D11.4 maybe unsupported; ID3D11Debug is unavailable. The error code is ${%d@0}.", &l_result
	);
#endif

    
    HWND l_window_handle = glfwGetWin32Window(m_window->get_window());
	FE_ASSERT(l_window_handle != NULL, "Failed to get the native window handle from GLFW window.");
    DXGI_SWAP_CHAIN_DESC1 l_swapchain_desc = 
    {
        .Format = DXGI_FORMAT_B8G8R8A8_UNORM,
        .SampleDesc = {.Count = 1, .Quality = 0 }, // No multi-sampling
        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = std::clamp(m_window->get_window_config()._swap_chain_buffer_count, (var::uint8)2, (var::uint8)16),
        .Scaling = DXGI_SCALING_NONE,
        .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
        .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
        .Flags = (m_should_allow_tearing == TRUE) ? (UINT)DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u
    };
    glfwGetFramebufferSize(m_window->get_window(), (int*)&l_swapchain_desc.Width, (int*)&l_swapchain_desc.Height);
	FE_ASSERT(l_swapchain_desc.Width > 0 && l_swapchain_desc.Height > 0, "Invalid framebuffer size retrieved from GLFW window.");
    
    
    m_viewport =
    {
        .TopLeftX = 0.0f,
        .TopLeftY = 0.0f,
        .Width = static_cast<FLOAT>(l_swapchain_desc.Width),
        .Height = static_cast<FLOAT>(l_swapchain_desc.Height),
        .MinDepth = 0.0f,
        .MaxDepth = 1.0f
    };
    m_context->RSSetViewports(1, &m_viewport);


    wrl::com_ptr<IDXGISwapChain1> l_swap_chain;
    l_result = m_factory->CreateSwapChainForHwnd(m_device.Get(), l_window_handle, &l_swapchain_desc, nullptr, nullptr, &l_swap_chain);
    FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendSwapChainCreationFailure, "Fail to create a swap chain; the error code is ${%d@0}.", &l_result);
    
    l_result = l_swap_chain.As(&m_swapchain);
    FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendSwapChainCreationFailure, "D3D11.4 unsupported; the error code is ${%d@0}.", &l_result);


    l_result = m_factory->MakeWindowAssociation(l_window_handle, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER); // Let GLFW and Frogman Engine handle fullscreen toggle instead of DXGI
	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendMakeWindowAssociationFailure, "IDXGIFactory::MakeWindowAssociation() failure; the error code is ${%d@0}.", &l_result);

    // m_swapchain->GetBuffer(0, IID_PPV_ARGS(&m_frame_buffer)) is conceptually: m_frame_buffer = m_swapchain[0]
    l_result = m_swapchain->GetBuffer(0, IID_PPV_ARGS(&m_frame_buffer)); // 0 is valid for DXGI_SWAP_EFFECT_FLIP_DISCARD
    FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendFrameBufferRetrievalFailure, "Fail to retrieve the frame buffer; the error code is ${%d@0}.", &l_result);
    

    l_result = m_device->CreateRenderTargetView(m_frame_buffer.Get(), nullptr, &m_render_target_view);
    FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendRenderTargetViewCreationFailure, "Fail to create a render target view; the error code is ${%d@0}.", &l_result);


    D3D11_RASTERIZER_DESC2 l_rasterizer_desc = 
    {
        .FillMode = D3D11_FILL_SOLID,
        .CullMode = D3D11_CULL_BACK,
        .DepthClipEnable = TRUE,
    };
    l_result = m_device->CreateRasterizerState2(&l_rasterizer_desc, &m_rasterizer_state);
	FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_RendererBackendRasterizerStateCreationFailure, "Failed to create rasterizer state; the error code is ${%d@0}.", &l_result);
    m_context->RSSetState(m_rasterizer_state.Get());


    m_context->OMSetRenderTargets(1, m_render_target_view.GetAddressOf(), nullptr);


    m_b0_updated_per_frame._desc =
    { 
        .ByteWidth = sizeof(updated_per_frame),
        .Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
    };
    m_device->CreateBuffer(&m_b0_updated_per_frame._desc, nullptr, &m_b0_updated_per_frame._buffer);

	m_b1_updated_per_pass._desc =
    {
        .ByteWidth = sizeof(updated_per_pass),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
    };
    m_device->CreateBuffer(&m_b1_updated_per_pass._desc, nullptr, &m_b1_updated_per_pass._buffer);

    m_b2_updated_per_material._desc =
    {
        .ByteWidth = sizeof(updated_per_material),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
    };
    m_device->CreateBuffer(&m_b2_updated_per_material._desc, nullptr, &m_b2_updated_per_material._buffer);

	m_b3_updated_per_object._desc =
	{
		.ByteWidth = sizeof(updated_per_object),
		.Usage = D3D11_USAGE_DYNAMIC,
		.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
		.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
	};
	m_device->CreateBuffer(&m_b3_updated_per_object._desc, nullptr, &m_b3_updated_per_object._buffer);

    m_b3_updated_per_object._desc =
    {
        .ByteWidth = sizeof(updated_per_object),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
    };
	m_device->CreateBuffer(&m_b3_updated_per_object._desc, nullptr, &m_b3_updated_per_object._buffer);
}

d3d11_backend::~d3d11_backend() noexcept
{
}


void d3d11_backend::resize_swap_chain_buffers(FE::int32 new_width_p, FE::int32 new_height_p) noexcept
{
    FE_ASSERT(new_width_p > 0 && new_height_p > 0, "Invalid framebuffer size is given; the width is ${%d@0} and the height is ${%d@1}.", &new_width_p, &new_height_p);

    m_context->OMSetRenderTargets(0, nullptr, nullptr);
    m_render_target_view.Reset();
    m_frame_buffer.Reset();
    m_context->Flush();


    HRESULT l_result = m_swapchain->ResizeBuffers
    (
        0, // 0 preserves the existing buffer count that was clamped at the creation time
        static_cast<UINT>(new_width_p),
        static_cast<UINT>(new_height_p),
        DXGI_FORMAT_B8G8R8A8_UNORM,
        (m_should_allow_tearing == TRUE) ? (UINT)DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0u // Must be identical to the flags given at the creation time
    );
    FE_EXIT_IF( FAILED(l_result),
                FE::ErrorCode::_FatalRendererError_5XX_RendererBackendFrameBufferResizeFailure,
                "Failed to resize the swap chain buffers; the error code is ${%d@0}.", &l_result
    );

    l_result = m_swapchain->GetBuffer(0, IID_PPV_ARGS(&m_frame_buffer));
    FE_EXIT_IF( FAILED(l_result), 
                FE::ErrorCode::_FatalRendererError_5XX_RendererBackendFrameBufferRetrievalFailure,
                "Failed to recreate the back buffer; the error code is ${%d@0}.", &l_result
    );

	l_result = m_device->CreateRenderTargetView(m_frame_buffer.Get(), nullptr, &m_render_target_view);
    FE_EXIT_IF( FAILED(l_result),
                FE::ErrorCode::_FatalRendererError_5XX_RendererBackendRenderTargetViewCreationFailure, 
                "Failed to recreate the render target view; the error code is ${%d@0}.", &l_result
    );


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
    m_context->OMSetRenderTargets(1, m_render_target_view.GetAddressOf(), nullptr);
}


void d3d11_backend::begin_frame() noexcept
{
    m_context->OMSetRenderTargets(1, m_render_target_view.GetAddressOf(), nullptr);

    constexpr FLOAT l_clear_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
    m_context->ClearRenderTargetView(m_render_target_view.Get(), l_clear_color);
}

void d3d11_backend::end_frame() noexcept
{
    const HRESULT l_result = m_swapchain->Present(m_window->get_window_config()._should_enable_vsync,
                                                    ((m_window->get_window_config()._should_enable_vsync == false) && (m_should_allow_tearing == TRUE)) ? DXGI_PRESENT_ALLOW_TEARING : 0
    );

    if (l_result == DXGI_STATUS_OCCLUDED) _FE_UNLIKELY_
    {
        _mm_pause();
    }
}


void d3d11_backend::register_shaders(std::pmr::vector<::FE::internal::renderer::shader>& shaders_p) noexcept
{
    for (auto& shader : shaders_p)
    {
        switch (shader._shader_target)
        {
        case ShaderTarget::_SM5_VertexShader:
            for (auto& blob : shader._permutations)
            {
                wrl::com_ptr<ID3D11VertexShader> l_vertex_shader;
                const HRESULT l_result = m_device->CreateVertexShader(blob._blob->GetBufferPointer(), blob._blob->GetBufferSize(), nullptr, &l_vertex_shader);
                FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_VertexShaderCreationFailure, "Failed to create a vertex shader; the error code is ${%d@0}.", &l_result);
                
                m_vertex_shader_cache[blob._identifier] = l_vertex_shader;
            }
            break;


        case ShaderTarget::_SM5_PixelShader:
            for (auto& blob : shader._permutations)
            {
                wrl::com_ptr<ID3D11PixelShader> l_pixel_shader;
                const HRESULT l_result = m_device->CreatePixelShader(blob._blob->GetBufferPointer(), blob._blob->GetBufferSize(), nullptr, &l_pixel_shader);
                FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_PixelShaderCreationFailure, "Failed to create a pixel shader; the error code is ${%d@0}.", &l_result);
                
                m_pixel_shader_cache[blob._identifier] = l_pixel_shader;
            }
            break;


        case ShaderTarget::_SM5_GeometryShader:
            for (auto& blob : shader._permutations)
            {
                wrl::com_ptr<ID3D11GeometryShader> l_geometry_shader;
                const HRESULT l_result = m_device->CreateGeometryShader(blob._blob->GetBufferPointer(), blob._blob->GetBufferSize(), nullptr, &l_geometry_shader);
                FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_GeometryShaderCreationFailure, "Failed to create a geometry shader; the error code is ${%d@0}.", &l_result);
                
                m_geometry_shader_cache[blob._identifier] = l_geometry_shader;
            }
            break;


        case ShaderTarget::_SM5_HullShader:
            for (auto& blob : shader._permutations)
            {
                wrl::com_ptr<ID3D11HullShader> l_hull_shader;
                const HRESULT l_result = m_device->CreateHullShader(blob._blob->GetBufferPointer(), blob._blob->GetBufferSize(), nullptr, &l_hull_shader);
                FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_HullShaderCreationFailure, "Failed to create a hull shader; the error code is ${%d@0}.", &l_result);
                
                m_hull_shader_cache[blob._identifier] = l_hull_shader;
            }
            break;


        case ShaderTarget::_SM5_DomainShader:
            for (auto& blob : shader._permutations)
            {
                wrl::com_ptr<ID3D11DomainShader> l_domain_shader;
                const HRESULT l_result = m_device->CreateDomainShader(blob._blob->GetBufferPointer(), blob._blob->GetBufferSize(), nullptr, &l_domain_shader);
                FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_DomainShaderCreationFailure, "Failed to create a domain shader; the error code is ${%d@0}.", &l_result);
                
                m_domain_shader_cache[blob._identifier] = l_domain_shader;
            }
            break;


        case ShaderTarget::_SM5_ComputeShader:
            for (auto& blob : shader._permutations)
            {
                wrl::com_ptr<ID3D11ComputeShader> l_compute_shader;
                const HRESULT l_result = m_device->CreateComputeShader(blob._blob->GetBufferPointer(), blob._blob->GetBufferSize(), nullptr, &l_compute_shader);
                FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_ComputeShaderCreationFailure, "Failed to create a compute shader; the error code is ${%d@0}.", &l_result);
                
                m_compute_shader_cache[blob._identifier] = l_compute_shader;
            }
            break;
        }
    }
}

//void d3d11_backend::create_hlsl_struct_memory_layout(std::pmr::vector<class::FE::internal::renderer::shader>& shaders_p) noexcept
//{
//    std::pmr::vector<D3D11_INPUT_ELEMENT_DESC> l_descs(FE::framework::framework_base::get_framework().get_large_memory_resource());
//
//    for (auto& shader : shaders_p)
//    {
//        for (auto& blob : shader._permutations)
//        {
//            wrl::com_ptr<ID3D11ShaderReflection> l_reflection = nullptr;
//            HRESULT l_result = D3DReflect(blob._blob->GetBufferPointer(),
//                                    blob._blob->GetBufferSize(),
//                                    IID_ID3D11ShaderReflection,
//                                    &l_reflection
//            );
//
//            FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_ShaderReflectionFailure, "Failed to reflect shader bytecode; the error code is ${%d@0}.", &l_result);
//        
//            D3D11_SHADER_DESC l_shader_desc = {};
//            l_reflection->GetDesc(&l_shader_desc);
//
//            l_descs.reserve(l_shader_desc.InputParameters);
//
//            // 
//
//            wrl::com_ptr<ID3D11InputLayout> l_input_layout;
//            l_result = m_device->CreateInputLayout(	l_descs.data(),
//                                                    (UINT)l_descs.size(),
//                                                    blob._blob->GetBufferPointer(),
//                                                    blob._blob->GetBufferSize(),
//                                                    &l_input_layout
//            );
//
//            m_input_layout_cache[blob._identifier] = l_input_layout;
//
//            l_descs.clear();
//        }
//    }
//}

END_NAMESPACE
#endif