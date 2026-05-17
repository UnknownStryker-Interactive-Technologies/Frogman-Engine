#ifndef _FE_RENDERER_SHADER_HXX_
#define _FE_RENDERER_SHADER_HXX_
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
#include <FE/prerequisites.hxx>
#include <FE/pair.hxx>

#include <FE/d3d11_backend.hxx>

#include <memory_resource>
#include <string>
#include <vector>




BEGIN_NAMESPACE(FE::internal::renderer)


struct shader_define
{
	std::pmr::string _identifier;
	FE::pair<var::int64, var::int64> _value_range;
	var::int64 _current_value;
};

class shader final
{
public:
	using macro = D3D_SHADER_MACRO;
	using compiled_source = wrl::ComPtr<ID3DBlob>;

	std::pmr::vector<shader_define> _defines;
	std::pmr::vector<std::pmr::string> _permutation_blacklist;
	std::pmr::vector<std::pmr::vector<macro>> _macro_combinations;
	std::pmr::string _main_function;
	std::pmr::wstring _source_path;
	std::pmr::vector<compiled_source> _permutations;
	internal::renderer::ShaderTarget _shader_target;

	~shader() noexcept;

private:
	void __build_shader_blob_cache_path(std::pmr::wstring& out_path_p, std::pmr::vector<macro>& macro_combination_p) const noexcept;

public:
	void compile() noexcept;
	FE::uint32 get_shader_compile_options() const noexcept;

};

END_NAMESPACE
#endif