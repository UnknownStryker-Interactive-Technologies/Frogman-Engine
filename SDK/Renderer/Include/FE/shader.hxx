#ifndef _FE_RENDERER_SHADER_HXX_
#define _FE_RENDERER_SHADER_HXX_
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
#include <FE/prerequisites.hxx>
#include <FE/pair.hxx>

#include <FE/d3d11_backend.hxx>

#include <memory_resource>
#include <optional>
#include <string>
#include <vector>

#include <concurrent_unordered_map.h>




BEGIN_NAMESPACE(FE::internal::renderer)


enum struct HlslTokenizerError
{
	_EmptyFileBuffer,
	_UnexpectedBlockCommentTermination,
	_MissingNullTerminator,
	_MissingBlockCommentTerminator,
	_MissingQuote,
	_MalformedIncludeDirectiveFormat
};

enum struct HlslToken : FE::uint8
{
	_LineCommentBegin, _LineCommentBody, _LineCommentEnd,

	_BlockCommentBegin, _BlockCommentBody, _BlockCommentEnd,

	_IncludeDirective
};

enum struct HlslContext : FE::uint8
{
	_LineComment,
	_BlockComment
};

struct hlsl_token
{
	HlslToken _type;
	FE::directory_string _value;
};

struct hlsli
{
	std::pmr::string _header_buffer;
	std::pmr::vector<hlsli*> _included_hlslis;
};

std::pmr::list<FE::internal::renderer::hlsl_token> __tokenize_hlsl(const std::pmr::string& buffer_p);

void __tokenize_hlsl_comments(	std::pmr::list<hlsl_token>& out_tokens_p, std::pmr::vector<FE::internal::renderer::HlslContext>& in_out_context_stack_p,
								FE::ASCII*& code_iterator_p
);

void __skip_hlsl_string_literals(FE::ASCII*& code_iterator_p, FE::ASCII* const end_p);

void __tokenize_hlsl_include_directives(std::pmr::list<hlsl_token>& out_tokens_p,
										FE::ASCII*& code_iterator_p
);

void __build_and_traverse_include_dependency_graph(  concurrency::concurrent_unordered_map<FE::directory_string, std::pmr::list<FE::internal::renderer::hlsl_token>>& token_lists_p,
										absl::flat_hash_map<FE::directory_string, FE::internal::renderer::hlsli>& in_out_shader_headers_p,
										std::pmr::vector<::FE::internal::renderer::shader>& in_out_shaders_p
) noexcept;


struct shader_define
{
	std::pmr::string _identifier;
	FE::pair<var::int64, var::int64> _value_range;
	var::int64 _current_value;
};

struct shader_blob
{
	wrl::com_ptr<ID3DBlob> _blob;
	var::uint64 _identifier;
};

class shader final
{
public:
	using macro = D3D_SHADER_MACRO;

	std::pmr::vector<shader_define> _defines;
	std::pmr::vector<std::pmr::string> _permutation_blacklist;
	std::pmr::vector<std::pmr::vector<macro>> _macro_combinations;
	std::pmr::string _main_function;
	FE::directory_string _source_path;
	std::pmr::vector<shader_blob> _permutations;
	internal::renderer::ShaderTarget _shader_target;
	var::boolean _is_hlsli_amended = false;

	~shader() noexcept;

private:
	FE::uint64 __build_shader_blob_cache_path(FE::directory_string& out_path_p, std::pmr::vector<macro>& macro_combination_p) const noexcept;

public:
	void compile(FE::boolean should_recompile_p) noexcept;

	constexpr FE::uint32 get_shader_compile_options() const noexcept
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
};


END_NAMESPACE
#endif