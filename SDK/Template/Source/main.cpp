#include <FE/engine.hpp>

#define GLM_FORCE_SIMD_AVX
#include <glm/glm.hpp> // https://github.com/g-truc/glm/blob/master/manual.md#section2_11

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>

#include <DirectXMath.h>
#include <d3dcompiler.h>

#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")




enum struct RendererErrorCode : FE::int32
{
	_FatalError_GLFW_InitFailure = 900,
	_FatalError_GLFW_WindowCreationFailure = 901,

	_FatalError_DXGI_FactoryCreationFailure = 1100,
	_FatalError_D3D11_DeviceCreationFailure = 1101,
	_FatalError_DXGI_SwapChainCreationFailure = 1102
};


// https://graphicsprogramming.github.io/learnd3d11/1-introduction/1-1-getting-started/1-1-2-hello-d3d11/
class D3D11 : public FE::game_engine
{
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_device_context;
	Microsoft::WRL::ComPtr<IDXGIFactory2> m_dxgi_factory;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> m_swap_chain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_render_target;

	FE::ASCII* m_title;
	GLFWmonitor* m_primary_monitor;
	const GLFWvidmode* m_video_mode;
	var::int32 m_width;
	var::int32 m_height;
	GLFWwindow* m_window;

public:
	D3D11(FE::int32 argc_p, FE::ASCII** agrv_p) noexcept
		: FE::game_engine(argc_p, agrv_p), m_device(), m_device_context(), m_dxgi_factory(), m_swap_chain(), m_render_target(), m_title("LearnD3D11 - Hello Window")
	{
		FE_EXIT(glfwInit() == GLFW_FALSE, RendererErrorCode::_FatalError_GLFW_InitFailure, "GLFW: Unable to initialize");
		this->m_primary_monitor = glfwGetPrimaryMonitor();
		this->m_video_mode = glfwGetVideoMode(this->m_primary_monitor);
		this->m_width = static_cast<int32_t>(this->m_video_mode->width * 0.9f);
		this->m_height = static_cast<int32_t>(this->m_video_mode->height * 0.9f);
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		this->m_window = glfwCreateWindow(this->m_width, this->m_height, this->m_title, nullptr, nullptr);
		FE_EXIT(this->m_window == nullptr, RendererErrorCode::_FatalError_GLFW_WindowCreationFailure, "GLFW: Unable to create window");

		FE::int32 l_window_left = (this->m_video_mode->width / 2) - (this->m_width / 2);
		FE::int32 l_window_top = (this->m_video_mode->height / 2) - (this->m_height / 2);
		glfwSetWindowPos(this->m_window, l_window_left, l_window_top);
	}

	~D3D11() noexcept
	{
		glfwDestroyWindow(this->m_window);
		glfwTerminate();
	}

	virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** agrv_p) override
	{
		(void)argc_p;
		(void)agrv_p;
		FE_EXIT(FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgi_factory))) == true, RendererErrorCode::_FatalError_DXGI_FactoryCreationFailure, "DXGI: Unable to create DXGIFactory");
		
		constexpr D3D_FEATURE_LEVEL l_device_feature_level = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1;
		FE_EXIT(FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &l_device_feature_level, 1, D3D11_SDK_VERSION, &m_device, nullptr, &m_device_context)), RendererErrorCode::_FatalError_D3D11_DeviceCreationFailure, "D3D11: Failed to create device and device Context");
		
		DXGI_SWAP_CHAIN_DESC1 l_swap_chain_descriptor = {};
		l_swap_chain_descriptor.Width = this->m_width;
		l_swap_chain_descriptor.Height = this->m_height;
		l_swap_chain_descriptor.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
		l_swap_chain_descriptor.SampleDesc.Count = 1;
		l_swap_chain_descriptor.SampleDesc.Quality = 0;
		l_swap_chain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		l_swap_chain_descriptor.BufferCount = 2;
		l_swap_chain_descriptor.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
		l_swap_chain_descriptor.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
		l_swap_chain_descriptor.Flags = {};

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC l_swap_chain_fullscreen_descriptor = {};
		l_swap_chain_fullscreen_descriptor.Windowed = true;

		FE_EXIT(FAILED(this->m_dxgi_factory->CreateSwapChainForHwnd(this->m_device.Get(), glfwGetWin32Window(this->m_window), &l_swap_chain_descriptor, &l_swap_chain_fullscreen_descriptor, nullptr, &m_swap_chain)), RendererErrorCode::_FatalError_DXGI_SwapChainCreationFailure, "DXGI: Failed to create swapchain");


		return 0;
	};

	virtual FE::int32 run() override
	{
		while (glfwWindowShouldClose(this->m_window) == GLFW_FALSE)
		{
			glfwPollEvents();
			// future update code
			// future render code
		}
		return 0;
	};

	virtual FE::int32 shutdown() override
	{
		return 0;
	};
};
CUSTOM_ENGINE(D3D11);