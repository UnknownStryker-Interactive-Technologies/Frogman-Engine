#ifndef _FE_HEADER_TOOL_PROGRAM_OPTIONS_HPP_
#define _FE_HEADER_TOOL_PROGRAM_OPTIONS_HPP_
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
#include <FE/prerequisites.h>

// robin hood hash map
#include <robin_hood.h>

// trie
#include <tsl/htrie_map.h>

// std::stirng
#include <string>

// std::string_view
#include <string_view>



class program_options
{
	tsl::htrie_map<var::tchar, std::basic_string<var::tchar>> m_path_options;
	robin_hood::unordered_map<std::basic_string_view<var::tchar>, var::boolean> m_fno_options;

public:
	program_options(FE::int32 argc_p, FE::tchar** argv_p) noexcept;
	~program_options() noexcept = default;

	void __parse_path_options(FE::tchar* non_fno_option_p) noexcept;

	FE::tchar* view_path_to_copyright_notice_option_title() const noexcept { return this->m_path_options.find("-path-to-copyright-notice=").key().c_str(); }
	FE::tchar* get_path_to_copyright_notice() const noexcept { return this->m_path_options.find("-path-to-copyright-notice=").value().c_str(); }

	FE::tchar* view_path_to_project_option_title() const noexcept { return this->m_path_options.find("-path-to-project=").key().c_str(); }
	FE::tchar* get_path_to_project() const noexcept { return this->m_path_options.find("-path-to-project=").value().c_str(); }

	FE::tchar* view_fno_copyright_notice_option_title() const noexcept { return this->m_fno_options.find("-fno-copyright-notice")->first.data(); }
	FE::boolean is_fno_copyright_notice_defined() const noexcept { return this->m_fno_options.find("-fno-copyright-notice")->second; }

	FE::tchar* view_fno_code_style_guide_option_title() const noexcept { return this->m_fno_options.find("-fno-code-style-guide")->first.data(); }
	FE::boolean is_fno_code_style_guide_defined() const noexcept { return this->m_fno_options.find("-fno-code-style-guide")->second; }

	FE::tchar* view_fno_reflection_helper_option_title() const noexcept { return this->m_fno_options.find("-fno-reflection-helper")->first.data(); }
	FE::boolean is_fno_reflection_helper_defined() const noexcept { return this->m_fno_options.find("-fno-reflection-helper")->second; }

	FE::tchar* view_fno_op_option_title() const noexcept { return this->m_fno_options.find("-fno-op")->first.data(); }
	FE::boolean is_fno_op_defined() const noexcept { return this->m_fno_options.find("-fno-op")->second; }

	FE::tchar* view_fno_write_option_title() const noexcept { return this->m_fno_options.find("-fno-write")->first.data(); }
	FE::boolean is_fno_write_defined() const noexcept { return this->m_fno_options.find("-fno-write")->second; }
};


#endif