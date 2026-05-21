#ifndef _FE_header_tool_TOKEN_HPP_
#define _FE_header_tool_TOKEN_HPP_
/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

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
#include "vocabulary.hpp"




using directory_t = std::pmr::basic_string<var::wchar>;
using file_buffer_t = std::pmr::basic_string<var::UTF8>;




struct token
{
	Vocabulary _vocabulary;
	var::uint32 _token_number;
	file_buffer_t _code;
	FE::wchar* _header_file_path;

	_FE_NODISCARD_ FE::boolean operator==(const token& rhs_p) const noexcept
	{
		return (_vocabulary == rhs_p._vocabulary);
	}

	_FE_NODISCARD_ FE::boolean operator!=(const token& rhs_p) const noexcept
	{
		return (_vocabulary != rhs_p._vocabulary);
	}
};

#endif