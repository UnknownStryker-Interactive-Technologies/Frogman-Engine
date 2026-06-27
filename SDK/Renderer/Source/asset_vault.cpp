#include <FE/asset_vault.hxx>

#include <FE/algorithm/string.hxx>

#include <FE/framework/framework.hxx>

#include <boost/hash2/xxhash.hpp>

#include <filesystem>
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
FE::internal::renderer::asset_vault::asset_vault(std::pmr::memory_resource* memory_resource) noexcept
    :   m_fbx(std::pmr::polymorphic_allocator<std::pair<FE::uint64, static_mesh>>(memory_resource))
{
}

FE::internal::renderer::asset_vault::~asset_vault() noexcept
{
}

FE::internal::renderer::fbx& FE::internal::renderer::asset_vault::import_fbx(FE::directory_char_t* const path_p) noexcept
{
	std::filesystem::path l_path(path_p);
	l_path = std::filesystem::absolute(l_path);

	boost::hash2::xxhash_64 l_hasher(0);
	l_hasher.update(l_path.c_str(), FE::algorithm::string::length(l_path.c_str()));
	var::uint64 l_key = l_hasher.result();

	if (m_fbx.contains(l_key))
	{
		return m_fbx[l_key];
	}


	FE::internal::renderer::fbx& l_fbx = m_fbx[l_key];
	ufbx_load_opts l_opts = {};
	l_opts.target_unit_meters = one_meter_f64;

	ufbx_error l_error = {};

	std::string l_temp = l_path.string(); /* forgive me */
	l_fbx._scene = make_fbx_scene_ptr(l_temp.c_str(), &l_opts, &l_error);
	l_fbx._static_meshes = std::pmr::vector<static_mesh>( FE::framework::framework_base::get_framework().get_memory_resource() );
	l_fbx._static_mesh_instances = std::pmr::vector<static_mesh_instance>(FE::framework::framework_base::get_framework().get_memory_resource());
	
	if (l_fbx._scene == nullptr) _FE_UNLIKELY_
	{
		var::ASCII l_error_msg[1024];
		ufbx_format_error(l_error_msg, sizeof(l_error_msg), &l_error);

		FE_EXIT_IF(true, FE::ErrorCode::_FatalRendererError_5XX_FBXImportFailure, l_error_msg);
	}


	for (_FE_MAYBE_UNUSED_ const ufbx_node* node : l_fbx._scene->nodes)
	{


	}
	return l_fbx;
}