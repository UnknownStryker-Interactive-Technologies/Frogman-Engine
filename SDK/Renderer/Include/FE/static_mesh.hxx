#ifndef _FE_RENDERER_STATIC_MESH_HXX_
#define _FE_RENDERER_STATIC_MESH_HXX_
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
#include <glm/glm.hpp>
#include <wrl/client.h>

#include <ufbx.h>




BEGIN_NAMESPACE(FE::internal::renderer)

struct static_mesh_vertex
{
    glm::vec3 _position;
    glm::vec3 _normal;
    glm::vec2 _uv;
};

struct static_mesh
{
    Microsoft::WRL::ComPtr<ID3D11Buffer> _vertex_buffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> _index_buffer;
    var::uint64 _vertex_count = 0;
    var::uint64 _index_count = 0;

	var::uint32 _size_of_vertex = sizeof(static_mesh_vertex);
};

struct static_mesh_instance
{
    var::uint32 _mesh_index;

    glm::mat4 _local_transform;
    glm::mat4 _world_transform;
};

END_NAMESPACE
#endif