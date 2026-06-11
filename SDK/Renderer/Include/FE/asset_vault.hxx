#ifndef _FE_RENDERER_ASSET_VAULT_HXX_
#define _FE_RENDERER_ASSET_VAULT_HXX_
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

#include <FE/static_mesh.hxx>

#include <absl/container/flat_hash_map.h>

#include <ufbx.h>




BEGIN_NAMESPACE(FE::internal::renderer)

class fbx_deleter
{
public:
    void operator()(ufbx_scene* const scene_p) const noexcept
    {
        ufbx_free_scene(scene_p);
	}
};
using fbx_scene_ptr = std::unique_ptr<ufbx_scene, fbx_deleter>;

_FE_FORCE_INLINE_ fbx_scene_ptr make_fbx_scene_ptr(FE::ASCII* const path_p, const ufbx_load_opts* opts_p, ufbx_error* error_p) noexcept
{
    return fbx_scene_ptr(ufbx_load_file(path_p, opts_p, error_p));
}

struct fbx
{
	fbx_scene_ptr _scene;
    
	std::pmr::vector<static_mesh> _static_meshes;
    std::pmr::vector<static_mesh_instance> _static_mesh_instances;
};

class asset_vault
{
	using cached_fbx = absl::flat_hash_map<var::uint64, fbx,

		absl::lts_20260107::DefaultHashContainerHash<var::uint64>,
		absl::lts_20260107::DefaultHashContainerEq<var::uint64>,

		std::pmr::polymorphic_allocator< std::pair<FE::uint64, fbx> >
    >;
    cached_fbx m_fbx;


public:
    asset_vault(std::pmr::memory_resource* memory_resource = std::pmr::get_default_resource()) noexcept;
    ~asset_vault() noexcept;


    fbx& import_fbx(FE::directory_char_t* const path_p) noexcept;


    asset_vault(const asset_vault&) = delete;
    asset_vault& operator=(const asset_vault&) = delete;

    asset_vault(asset_vault&&) = delete;
    asset_vault& operator=(asset_vault&&) = delete;
};

END_NAMESPACE
#endif