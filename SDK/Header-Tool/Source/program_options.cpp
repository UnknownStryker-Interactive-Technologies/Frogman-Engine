/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "program_options.hpp"

// FE.Core string algorithm
#include <FE/algorithm/string.hxx>




program_options::program_options(FE::int32 argc_p, FE::tchar** argv_p) noexcept
	: m_path_options({
		{ "-path-to-copyright-notice=", "\0" },
		{ "-path-to-code-style-guide=", "\0" },
		{ "-path-to-project=", "\0" }
		}),

	m_fno_options({
		{ "-fno-copyright-notice", false },
		{ "-fno-code-style-guide", false },
		{ "-fno-reflection-helper", false },
		{ "-fno-op", false },
		{ "-fno-write", false }
		})
{
	for (var::int32 i = 0; i < argc_p; ++i)
	{
		auto l_result = this->m_fno_options.find(argv_p[i]);
		if (l_result == this->m_fno_options.end())
		{
			__parse_path_options(argv_p[i]);
			continue;
		}
		l_result->second = true;
	}
}

void program_options::__parse_path_options(FE::tchar* non_fno_option_p) noexcept
{
	auto l_symbol = FE::algorithm::string::find_the_first<var::tchar>(non_fno_option_p, '=');
	if (l_symbol == std::nullopt)
	{
		return;
	}

	auto l_result = this->m_path_options.find_ks(non_fno_option_p, l_symbol->_end);
	if (this->m_path_options.end() == l_result)
	{
		return;
	}

	l_result.value().assign(non_fno_option_p + l_symbol->_end);
}