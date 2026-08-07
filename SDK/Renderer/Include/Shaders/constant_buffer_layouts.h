#ifndef _CONSTANT_BUFFER_LAYOUTS_H_
#define _CONSTANT_BUFFER_LAYOUTS_H_
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
#ifdef __cplusplus
#include <glm/glm.hpp>
typedef __declspec(align(16)) glm::mat4 float4x4;
typedef __declspec(align(16)) glm::vec4 float4;
#endif




#ifdef __cplusplus
// b0
struct alignas(16) updated_per_frame 
{
    float4x4 _view_projection;
    float4 _screen_params;
    float4 _time_params;
};

// b1
struct alignas(16) updated_per_pass
{
    float4 _texel_size;
	float4 _blur_direction;
};

// b2
struct alignas(16) updated_per_material
{
    float4 _base_color;
    float4 _uv_transform;
    alignas(16) float _alpha_cutoff;
};

// b3
struct alignas(16) updated_per_object
{
	float4x4 _world_matrix;
	float4x4 _world_inverse_transpose;
};
#else
cbuffer updated_per_frame : register(b0)
{
    float4x4 _view_projection;
    float4 _screen_params;
    float4 _time_params;
};

cbuffer updated_per_pass : register(b1)
{
    float4 _texel_size;
    float4 _blur_direction;
};

cbuffer updated_per_material : register(b2)
{
    float4 _base_color;
    float4 _uv_transform;
    float _alpha_cutoff;
};

cbuffer updated_per_object : register(b3)
{
    float4x4 _world_matrix;
    float4x4 _world_inverse_transpose;
};
#endif




#endif