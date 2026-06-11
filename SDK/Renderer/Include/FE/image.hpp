#ifndef _FE_RENDERER_IMAGE_HPP_
#define _FE_RENDERER_IMAGE_HPP_
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

#include <d3d11.h>
#include <d3d11_4.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <wrl/client.h>




BEGIN_NAMESPACE(FE)


class image
{
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shader_resource_view;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture;
    var::int32 m_width;
    var::int32 m_height;
    var::byte* m_pixels;

public:
	image() noexcept;
    ~image() noexcept;

    image(const image& other_p) noexcept;
    image& operator=(const image& other_p) noexcept;

    image(image&& other_p) noexcept;
    image& operator=(image&& other_p) noexcept;

    bool read_image_from_disk(FE::directory_char_t* const path_p) noexcept;
    bool load_to_renderer(ID3D11Device5* const device_p) noexcept;
    
	_FE_FORCE_INLINE_ ImTextureID shader_resource_view() const noexcept { return (ImTextureID)m_shader_resource_view.Get(); }
    _FE_FORCE_INLINE_ var::int32 width() const noexcept { return m_width; }
    _FE_FORCE_INLINE_ var::int32 height() const noexcept { return m_height; }
};


END_NAMESPACE
#endif