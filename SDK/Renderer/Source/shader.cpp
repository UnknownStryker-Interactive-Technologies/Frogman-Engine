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


void FE::internal::renderer::shader::compile(FE::boolean should_recompile_p) noexcept
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

	_FE_NODEFAULT_;
	}

	for (auto& macro_combination : _macro_combinations)
	{
		FE::uint64 l_blob_name = __build_shader_blob_cache_path(l_blob_path, macro_combination);

		std::fstream l_blob_file(l_blob_path.c_str(), std::ios::binary | std::ios::in); // read as a binary sequence
		FE::fstream_guard l_blob_file_guard(l_blob_file);
		if (l_blob_file.is_open() == true && should_recompile_p == false && _is_hlsli_amended == false) // does blob exist AND should not recompile AND is hlsli not amended?
		{
			_permutations.emplace_back();
			HRESULT l_result = D3DReadFileToBlob(l_blob_path.c_str(), &_permutations.back()._blob);
			FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_ShaderBlobCacheLoadFailure, "Shader blob cache load failed.");
			_permutations.back()._identifier = l_blob_name;
			continue;
		}

		_permutations.emplace_back();
		wrl::com_ptr<ID3DBlob> l_errors;
		HRESULT l_result = D3DCompileFromFile(_source_path.c_str(),
			macro_combination.data(),
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			_main_function.c_str(),
			l_shader_target,
			get_shader_compile_options(),
			FE::null, // Legacy flag, should be set to 0
			&_permutations.back()._blob,
			&l_errors
		);
		_permutations.back()._identifier = l_blob_name;

		if (l_errors != nullptr) _FE_UNLIKELY_
		{
			OutputDebugStringA((FE::ASCII*)l_errors->GetBufferPointer());
		}
		FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_ShaderCompilationFailure, "Shader compilation failed.");

		// stash blobs to disk
		l_result = D3DWriteBlobToFile(_permutations.back()._blob.Get(), l_blob_path.c_str(), TRUE);
		FE_EXIT_IF(FAILED(l_result), FE::ErrorCode::_FatalRendererError_5XX_ShaderBlobStashFailure, "Shader blob stash failed.");
	}
}

FE::uint64 FE::internal::renderer::shader::__build_shader_blob_cache_path(std::pmr::wstring& out_path_p, std::pmr::vector<macro>& macro_combination_p) const noexcept
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


	FE::uint64 l_blob_name = l_hasher.result();
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

	return l_blob_name;
}




std::pmr::list<FE::internal::renderer::hlsl_token> FE::internal::renderer::__tokenize_hlsl(const std::pmr::string& buffer_p)
{
	if (buffer_p.empty() == true)
	{
		throw HlslTokenizerError::_EmptyFileBuffer;
	}
		
	std::pmr::list<FE::internal::renderer::hlsl_token>l_tokens(FE::engine::get_engine().get_memory_resource());
	std::pmr::vector< FE::internal::renderer::HlslContext> l_context_stack(FE::engine::get_engine().get_memory_resource());

	FE::ASCII* l_iter = buffer_p.data();
	FE::ASCII* const l_end = buffer_p.data() + buffer_p.length();
	while (l_iter < l_end)
	{
		if (*l_iter <= ' ')
		{
			++l_iter;
			continue;
		}
		FE::ASCII* l_past = l_iter;
		__tokenize_hlsl_comments(l_tokens, l_context_stack, l_iter);
		__tokenize_hlsl_include_directives(l_tokens, l_iter);
		__skip_hlsl_string_and_character_literals(l_iter, l_end);

		if (l_past == l_iter) // hasn't advanced.
		{
			++l_iter; // skip irrelevant hlsl code for building include dependency graph.
		}
	}

	l_tokens.emplace_back(HlslToken::_EOF, FE_TEXT("\0"));
	return l_tokens;
}

void FE::internal::renderer::__tokenize_hlsl_comments(	std::pmr::list<FE::internal::renderer::hlsl_token>& out_tokens_p, std::pmr::vector<FE::internal::renderer::HlslContext>& in_out_context_stack_p, 
														FE::ASCII*& code_iterator_p)
{
	FE::ASCII l_input_buffer[3] = { code_iterator_p[0], code_iterator_p[1], '\0' };
	STRING_SWITCH(l_input_buffer)
	{
	STRING_CASE("//"):
		in_out_context_stack_p.emplace_back(HlslContext::_LineComment);
		out_tokens_p.emplace_back(HlslToken::_LineCommentBegin);
		code_iterator_p += sizeof(l_input_buffer) - 1;
		return;


	STRING_CASE("/*"):
		in_out_context_stack_p.emplace_back(HlslContext::_BlockComment);
		out_tokens_p.emplace_back(HlslToken::_BlockCommentBegin);
		code_iterator_p += sizeof(l_input_buffer) - 1;
		return;

	STRING_CASE("*/"):
		if (in_out_context_stack_p.back() != HlslContext::_BlockComment)
		{
			throw HlslTokenizerError::_UnexpectedBlockCommentTermination;
		}
		in_out_context_stack_p.pop_back();
		out_tokens_p.emplace_back(HlslToken::_BlockCommentEnd);
		code_iterator_p += sizeof(l_input_buffer) - 1;
		return;


	default:
		break;
	}


	if (*code_iterator_p == '\n' && in_out_context_stack_p.back() == HlslContext::_LineComment)
	{
		in_out_context_stack_p.pop_back();
		out_tokens_p.emplace_back(HlslToken::_LineCommentEnd);
		++code_iterator_p;
		return;
	}


	switch (in_out_context_stack_p.back())
	{
	case HlslContext::_LineComment:
		out_tokens_p.emplace_back(HlslToken::_LineCommentBody);
		{
			auto l_newline = FE::algorithm::string::find_the_first(code_iterator_p, '\n');
			if (l_newline == std::nullopt) _FE_UNLIKELY_
			{
				throw HlslTokenizerError::_MissingNullTerminator;
			}
			code_iterator_p += l_newline->_end;
		}
		return;

	case HlslContext::_BlockComment:
		out_tokens_p.emplace_back(HlslToken::_BlockCommentBody);
		{
			auto l_block_comment_opener = FE::algorithm::string::find_the_first(code_iterator_p, "/*");
			auto l_block_comment_termination = FE::algorithm::string::find_the_first(code_iterator_p, "*/");

			if (l_block_comment_termination == std::nullopt ||
				l_block_comment_opener->_begin < l_block_comment_termination->_begin) _FE_UNLIKELY_
			{
				throw HlslTokenizerError::_MissingBlockCommentTerminator;
			}
			code_iterator_p += l_block_comment_termination->_begin;
		}
		return;

	default:
		return;
	}
}

void FE::internal::renderer::__skip_hlsl_string_and_character_literals(FE::ASCII*& code_iterator_p, FE::ASCII* const end_p)
{
	if (*code_iterator_p != '\"') // if *code_iterator_p is not ", then return.
	{
		return;
	}
	++code_iterator_p; // skip the opening "

	var::ASCII l_input_buffer[3] = {};
	constexpr FE::ASCII* const l_extender = "\\\"";
	const FE::algorithm::string::range l_cmp_rng = { 0, FE::algorithm::string::length(l_input_buffer) };
	
	while (code_iterator_p < end_p)
	{
		l_input_buffer[0] = code_iterator_p[0];
		l_input_buffer[1] = code_iterator_p[1];
		l_input_buffer[2] = '\0';

		if (FE::algorithm::string::compare_ranged(l_extender, l_cmp_rng, l_input_buffer, l_cmp_rng) == true) // found \"
		{
			code_iterator_p += l_cmp_rng._end;
			continue;
		}

		if (*code_iterator_p == '\"')
		{
			++code_iterator_p;
			return;
		}

		++code_iterator_p;
	}

	throw HlslTokenizerError::_MissingQuote;
}

void FE::internal::renderer::__tokenize_hlsl_include_directives(std::pmr::list<::FE::internal::renderer::hlsl_token>& out_tokens_p, FE::ASCII*& code_iterator_p)
{
	if (*code_iterator_p != '#') // if *code_iterator_p is not #, then return.
	{
		return;
	}
	++code_iterator_p; // skip the #


	while (*code_iterator_p <= ' ')
	{
		++code_iterator_p;
	}


	FE::ASCII l_input_buffer[8] = 
	{
		code_iterator_p[0], // i
		code_iterator_p[1], // n
		code_iterator_p[2], // c
		code_iterator_p[3], // l
		code_iterator_p[4], // u
		code_iterator_p[5], // d
		code_iterator_p[6], // e
		'\0'
	};
	const FE::algorithm::string::range l_cmp_rng = { 0, FE::algorithm::string::length(l_input_buffer) };
	constexpr FE::ASCII l_include_directive[] = "include";

	if (FE::algorithm::string::compare_ranged(l_input_buffer, l_cmp_rng, l_include_directive, l_cmp_rng) == true)
	{
		code_iterator_p += l_cmp_rng._end;
		while (*code_iterator_p <= ' ')
		{
			++code_iterator_p;
		}

		std::optional<FE::algorithm::string::range>	l_include_path_rng = {};
		switch (*code_iterator_p)
		{
		case '<':
			++code_iterator_p; // skip the opening <
			l_include_path_rng = FE::algorithm::string::find_the_first(code_iterator_p, '>');
			if (l_include_path_rng == std::nullopt) _FE_UNLIKELY_
			{
				throw HlslTokenizerError::_MalformedIncludeDirectiveFormat;
			}
			out_tokens_p.emplace_back(HlslToken::_IncludeDirective, FE::directory_string(code_iterator_p, code_iterator_p + l_include_path_rng->_begin, FE::framework::framework_base::get_framework().get_memory_resource()));
			break;

		case '\"':
			++code_iterator_p; // skip the opening "
			l_include_path_rng = FE::algorithm::string::find_the_first(code_iterator_p, '\"');
			if (l_include_path_rng == std::nullopt) _FE_UNLIKELY_
			{
				throw HlslTokenizerError::_MalformedIncludeDirectiveFormat;
			}
			out_tokens_p.emplace_back(HlslToken::_IncludeDirective, FE::directory_string(code_iterator_p, code_iterator_p + l_include_path_rng->_begin,FE::framework::framework_base::get_framework().get_memory_resource()));
			break;

		default:
			throw HlslTokenizerError::_MalformedIncludeDirectiveFormat;
		}
	}
}

void FE::internal::renderer::__build_include_dependency_graph(	const concurrency::concurrent_unordered_map<FE::directory_string, std::pmr::list<FE::internal::renderer::hlsl_token>>& token_lists_p,
										absl::flat_hash_map<FE::directory_string, FE::internal::renderer::hlsli>& in_out_shader_headers_p,
										std::pmr::vector<::FE::internal::renderer::shader>& in_out_shaders_p) noexcept
{
	FE::directory_string l_shader_path(FE::engine::get_engine().get_large_memory_resource());
	for (const auto& [key, token_list] : token_lists_p)
	{
		for (const FE::internal::renderer::hlsl_token& token : token_list)
		{
			if (token._type == FE::internal::renderer::HlslToken::_IncludeDirective)
			{
				l_shader_path = FE::engine::get_engine().get_shader_root_directory();
				l_shader_path += FE_TEXT("\\");
				l_shader_path += token._value;

				auto l_header_it = in_out_shader_headers_p.find(l_shader_path);
				FE_EXIT_IF(l_header_it == in_out_shader_headers_p.end(), ErrorCode::_FatalRendererError_5XX_ShaderSubDirectoryCreationRestricted, "Shader subdirectory creation restricted; target HLSL file path: ${%s@0}.", l_shader_path.c_str());
			
				//in_out_shader_headers_p[l_shader_path]._included_hlslis;
				// DFS
			}
		}
	}
	(in_out_shaders_p);
}

