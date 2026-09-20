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
#include <FE/image.hpp>

#include <FE/algorithm/string.hxx>

#include <FE/framework/framework.hxx>

#include <stb_image.h>




FE::image::image() noexcept
    :	m_shader_resource_view(),
        m_texture(),
        m_width(0),
	    m_height(0),
	    m_pixels(nullptr)
{
}

FE::image::~image() noexcept
{
    if (m_pixels != nullptr)
    {
        stbi_image_free(m_pixels);
        m_pixels = nullptr;
    }
}

FE::image::image(const image& other_p) noexcept
    :   m_shader_resource_view(other_p.m_shader_resource_view),
	    m_texture(other_p.m_texture),
	    m_width(other_p.m_width),
	    m_height(other_p.m_height),
	    m_pixels()

{
}

FE::image& FE::image::operator=(const image& other_p) noexcept
{
	m_shader_resource_view = other_p.m_shader_resource_view;
	m_texture = other_p.m_texture;

	m_width = other_p.m_width;
	m_height = other_p.m_height;
	return *this;
}

FE::image::image(image&& other_p) noexcept
    : m_shader_resource_view(std::move(other_p.m_shader_resource_view)),
      m_texture(std::move(other_p.m_texture)),
      m_width(other_p.m_width),
      m_height(other_p.m_height),
      m_pixels(other_p.m_pixels)
{
    other_p.m_width = 0;
    other_p.m_height = 0;
	other_p.m_pixels = nullptr;
}

FE::image& FE::image::operator=(image&& other_p) noexcept
{
    // TODO: insert return statement here
    m_shader_resource_view = std::move(other_p.m_shader_resource_view);
    m_texture = std::move(other_p.m_texture);

    m_width = other_p.m_width;
	other_p.m_width = 0;

    m_height = other_p.m_height;
	other_p.m_height = 0;

    m_pixels = other_p.m_pixels;
	other_p.m_pixels = nullptr;
	return *this;
}

bool FE::image::read_image_from_disk(FE::directory_char_t* const path_p) noexcept
{
    if (path_p == nullptr) _FE_UNLIKELY_
    {
        FE_ASSERT(path_p != nullptr);
		return false;
    }

    if (m_pixels != nullptr)
    {
        stbi_image_free(m_pixels);
        m_pixels = nullptr;
	}

    std::pmr::string l_path_str(framework::framework_base::get_framework().get_large_memory_resource());
#ifdef _FE_ON_WINDOWS_X86_64_
    const int l_path_length = (int)FE::algorithm::string::length(path_p);
    const int l_utf8_length = WideCharToMultiByte(CP_UTF8, 0, path_p, l_path_length, nullptr, 0, nullptr, nullptr);
    FE_ASSERT(l_utf8_length > 0, "Failed to convert the image path to UTF-8.");
    l_path_str.resize(l_utf8_length);
    WideCharToMultiByte(CP_UTF8, 0,
        path_p, l_path_length,
        l_path_str.data(), l_utf8_length,
        nullptr, nullptr
    );
#else
    l_path_str = path_p;
#endif

    m_pixels = stbi_load(l_path_str.c_str(), &m_width, &m_height, nullptr, 4/*RGBA*/);
    if( m_pixels == nullptr) _FE_UNLIKELY_
    {
        FE_ASSERT(m_pixels != nullptr, "Failed to load image from disk.");
        return false;
	}
    return true;
}

bool FE::image::load_to_renderer(ID3D11Device5* const device_p) noexcept
{
    if (device_p == nullptr) _FE_UNLIKELY_
    {
        FE_ASSERT(device_p != nullptr);
        return false;
    }

    if (m_pixels == nullptr) _FE_UNLIKELY_
    {
        return false;
    }

    D3D11_TEXTURE2D_DESC l_desc = 
    {
        .Width = (UINT)m_width,
        .Height = (UINT)m_height,
        .MipLevels = 1,
        .ArraySize = 1,
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        .SampleDesc = { .Count = 1, .Quality = 0},
        .Usage = D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_SHADER_RESOURCE
    };

    D3D11_SUBRESOURCE_DATA l_init =
    {
        .pSysMem = m_pixels,
		.SysMemPitch = (UINT)(m_width * 4) // Image width * 4 bytes per pixel (RGBA)
    };

    _FE_MAYBE_UNUSED_ HRESULT l_result = device_p->CreateTexture2D(&l_desc, &l_init, &m_texture);
	FE_ASSERT(SUCCEEDED(l_result), "Failed to create texture for image.");

    l_result = device_p->CreateShaderResourceView(m_texture.Get(), nullptr, &m_shader_resource_view);
	FE_ASSERT(SUCCEEDED(l_result), "Failed to create shader resource view for image.");

    stbi_image_free(m_pixels);
    m_pixels = nullptr;
    return true;
}