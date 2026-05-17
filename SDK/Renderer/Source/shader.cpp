/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/shader.hxx>

#include <FE/engine.hpp>

#include <FE/algorithm/string.hxx>
#include <FE/algorithm/utility.hxx>
#include <FE/fstream_guard.hxx>

#include <fstream>

#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/xxhash.hpp>




FE::internal::renderer::shader::~shader() noexcept
{
	for (std::pmr::vector<macro>& permutation_macros : _macro_combinations)
	{
		for (macro& macro : permutation_macros)
		{
			if (macro.Definition == nullptr)
			{
				continue;
			}

			std::pmr::polymorphic_allocator<char> l_deallocator = permutation_macros.get_allocator();
			l_deallocator.deallocate((char*)macro.Definition, std::strlen(macro.Definition) + 1);
			macro.Definition = nullptr;
		}
	}
}


void FE::internal::renderer::shader::compile() noexcept
{
	_permutations.reserve(_macro_combinations.size());
	std::pmr::wstring l_blob_path(FE::engine::get_engine().get_large_memory_resource());

	FE::ASCII* l_shader_target = nullptr;
	switch (_shader_target)
	{
	case internal::renderer::ShaderTarget::_SM5_VertexShader:
		l_shader_target = FE::internal::renderer::SM5_vertex_shader_target;
		break;

	case internal::renderer::ShaderTarget::_SM5_PixelShader:
		l_shader_target = FE::internal::renderer::SM5_pixel_shader_target;
		break;

	case internal::renderer::ShaderTarget::_SM5_GeometryShader:
		l_shader_target = FE::internal::renderer::SM5_geometry_shader_target;
		break;

	case internal::renderer::ShaderTarget::_SM5_HullShader:
		l_shader_target = FE::internal::renderer::SM5_hull_shader_target;
		break;

	case internal::renderer::ShaderTarget::_SM5_DomainShader:
		l_shader_target = FE::internal::renderer::SM5_domain_shader_target;
		break;

	case internal::renderer::ShaderTarget::_SM5_ComputeShader:
		l_shader_target = FE::internal::renderer::SM5_compute_shader_target;
		break;


	case internal::renderer::ShaderTarget::_SM6_VertexShader:
		l_shader_target = FE::internal::renderer::SM6_vertex_shader_target;
		break;

	case internal::renderer::ShaderTarget::_SM6_PixelShader:
		l_shader_target = FE::internal::renderer::SM6_pixel_shader_target;
		break;

	case internal::renderer::ShaderTarget::_SM6_GeometryShader:
		l_shader_target = FE::internal::renderer::SM6_geometry_shader_target;
		break;

	case internal::renderer::ShaderTarget::_SM6_HullShader:
		l_shader_target = FE::internal::renderer::SM6_hull_shader_target;
		break;

	case internal::renderer::ShaderTarget::_SM6_DomainShader:
		l_shader_target = FE::internal::renderer::SM6_domain_shader_target;
		break;

	case internal::renderer::ShaderTarget::_SM6_ComputeShader:
		l_shader_target = FE::internal::renderer::SM6_compute_shader_target;
		break;
	}

	for (auto& macro_combination : _macro_combinations)
	{
		__build_shader_blob_cache_path(l_blob_path, macro_combination);

		std::fstream l_blob_file(l_blob_path.c_str(), std::ios::binary | std::ios::in); // read as a binary sequence
		FE::fstream_guard l_blob_file_guard(l_blob_file);
		if (l_blob_file.is_open() == true) // does blob exist?
		{
			_permutations.emplace_back();
			D3DReadFileToBlob(l_blob_path.c_str(), &_permutations.back());
			continue;
		}

		_permutations.emplace_back();
		wrl::ComPtr<ID3DBlob> l_errors;
		const HRESULT l_result = D3DCompileFromFile(_source_path.c_str(),
			macro_combination.data(),
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			_main_function.c_str(),
			l_shader_target,
			get_shader_compile_options(),
			FE::null, // Legacy flag, should be set to 0
			&_permutations.back(),
			&l_errors
		);

		if (l_errors != nullptr) _FE_UNLIKELY_
		{
			OutputDebugStringA((FE::ASCII*)l_errors->GetBufferPointer());
		}
		FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_ShaderCompilationFailure, "Shader compilation failed.");

		// stash blobs to disk
		D3DWriteBlobToFile(_permutations.back().Get(), l_blob_path.c_str(), TRUE);
	}
}

FE::uint32 FE::internal::renderer::shader::get_shader_compile_options() const noexcept
{
	var::uint32 l_flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS; // /Wx /W4
#ifdef _DEBUG_
	l_flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL0;
#elif defined(_RELWITHDEBINFO_)
	l_flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#else
	l_flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
	return l_flags;
}

void FE::internal::renderer::shader::__build_shader_blob_cache_path(std::pmr::wstring& out_path_p, std::pmr::vector<macro>& macro_combination_p) const noexcept
{
	boost::hash2::xxhash_64 l_hasher(0);
	const FE::renderer::gpu_info& l_gpu_info = FE::engine::get_engine().get_renderer().get_gpu_info();


	// GPU identification
	boost::hash2::hash_append(l_hasher, {}, std::wstring_view(l_gpu_info.Description, wcslen(l_gpu_info.Description))); // e.g. NVIDIA GeForce RTX 5070Ti
	boost::hash2::hash_append(l_hasher, {}, l_gpu_info.VendorId); // e.g. NVIDIA
	boost::hash2::hash_append(l_hasher, {}, l_gpu_info.DeviceId); // e.g. GB203
	boost::hash2::hash_append(l_hasher, {}, l_gpu_info.SubSysId); // e.g. ASUS
	boost::hash2::hash_append(l_hasher, {}, l_gpu_info.Revision); // GPU chip revision
	boost::hash2::hash_append(l_hasher, {}, l_gpu_info.DedicatedVideoMemory); // VRAM; it can vary between models. E.g. RTX 5060Ti 8GiB vs RTX 5060Ti 16GiB
	
	(macro_combination_p);
	// Shader Compiler identification
	{
		HMODULE l_module_handle = GetModuleHandleA(D3DCOMPILER_DLL_A);
		if (l_module_handle == 0) _FE_UNLIKELY_
		{
			l_module_handle = LoadLibraryA(D3DCOMPILER_DLL_A);
			FE_ASSERT(l_module_handle != 0);
		}

		thread_local static var::wchar tl_s_wide_path[_ALLOWED_DIRECTORY_LENGTH_] = L"\0";
		{
			_FE_MAYBE_UNUSED_ DWORD l_errcode = GetModuleFileNameW(l_module_handle, tl_s_wide_path, _ALLOWED_DIRECTORY_LENGTH_);
			FE_ASSERT(l_errcode != 0);
		}

		DWORD l_version_info_length = GetFileVersionInfoSizeW(tl_s_wide_path, nullptr);
		if (l_version_info_length != 0)
		{
			std::pmr::vector<var::byte> l_version_info_buffer(FE::engine::get_engine().get_large_memory_resource());
			l_version_info_buffer.resize(l_version_info_length);

			_FE_MAYBE_UNUSED_ BOOL l_errcode = GetFileVersionInfoW(tl_s_wide_path,
				0, // ignored
				l_version_info_length,
				l_version_info_buffer.data()
			);

			FE_ASSERT(l_errcode != 0);

			VS_FIXEDFILEINFO* l_file_info = nullptr;
			_FE_MAYBE_UNUSED_ UINT l_file_info_size = 0;
			VerQueryValueA(l_version_info_buffer.data(), "\\", (void**)&l_file_info, &l_file_info_size);

			boost::hash2::hash_append(l_hasher, {}, l_file_info->dwFileVersionMS);
			boost::hash2::hash_append(l_hasher, {}, l_file_info->dwFileVersionLS);
		}
	}


	// Shader source identification
	{
		std::fstream l_source_file(_source_path.c_str(), std::ios::binary | std::ios::in);
		FE_ASSERT(l_source_file.is_open() == true);
		FE::fstream_guard l_source_file_guard(l_source_file);

		std::pmr::string l_source_code(FE::engine::get_engine().get_large_memory_resource());
		l_source_file.seekg(0, std::ios::end);
		l_source_code.resize(l_source_file.tellg());
		l_source_file.seekg(0, std::ios::beg);

		l_source_file.read(l_source_code.data(), l_source_code.length());

		boost::hash2::hash_append(l_hasher, {}, l_source_code);
	}


	// Macro combination identification
	{
		for (const macro& macro : macro_combination_p)
		{
			if (macro.Name == nullptr) _FE_UNLIKELY_
			{
				break;
			}
			boost::hash2::hash_append(l_hasher, {}, std::string_view(macro.Name, strlen(macro.Name)));
			boost::hash2::hash_append(l_hasher, {}, std::string_view(macro.Definition, strlen(macro.Definition)));
		}
	}


	// Shader compile options identification
	boost::hash2::hash_append(l_hasher, {}, get_shader_compile_options());


	var::uint64 l_blob_name = l_hasher.result();
	std::pmr::wstring l_blob_name_string(FE::engine::get_engine().get_large_memory_resource());
	l_blob_name_string.resize(FE::algorithm::utility::count_uint_digit_length(l_blob_name));
	FE::algorithm::utility::uint_to_string(l_blob_name_string.data(), l_blob_name_string.length(), l_blob_name);

	out_path_p = _source_path;
	std::pmr::wstring::size_type l_pos = out_path_p.rfind(L"\\");
	FE_ASSERT(l_pos != std::pmr::wstring::npos);
	std::pmr::wstring::size_type l_to_remove_length = out_path_p.length() - l_pos;
	++l_pos;
	out_path_p.erase(l_pos, l_to_remove_length);
	out_path_p += l_blob_name_string;
	out_path_p += L".fesb"; // .Frogman Engine Shader Blob
}