#ifndef _FE_HEADER_TOOL_STRING_PURGE_TEST_HPP_
#define _FE_HEADER_TOOL_STRING_PURGE_TEST_HPP_
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

#include <string>
#include <string_view>
#include <vector>


namespace FHT::test
{
	constexpr std::string_view test_cases_basic_strings[] = {
		"const char* str = \"hello world\";",
		R"IHateDelim(const char* str = ;)IHateDelim",
		R"( R"IHateDelim()IHateDelim\" )",
		R"( R"IHateDelim(const char* empty = ;)IHateDelim\" )",
		R"IHateDelim( R"IHateDelim()IHateDelim\" )IHateDelim",
		R"(std::string msg = ;)"
	};

	constexpr std::wstring_view test_cases_character_literals[] = {
		L"char c = 'a';",
		LR"IHateDelim(char c = ;)IHateDelim",
		LR"(char newline = '\n';)",
		LR"(char newline = ;)",
		LR"(char quote = '\'';)",
		LR"(char quote = ;)"
	};

	constexpr std::u8string_view test_cases_escaped_sequences[] = {
		u8"const char* path = \"C:\\Users\\file.txt\";",
		u8R"IHateDelim(const char* path = ;)IHateDelim",
		u8R"(const char* json = "{\"key\": \"value\"}";)",
		u8R"(const char* json = ;)",
		u8R"(const char* tabs = "line1\tline2\n";)",
		u8R"(const char* tabs = ;)"
	};

	constexpr std::u16string_view test_cases_raw_strings[] = {
		u"const char* raw = R\"(Hello \"world\")\";",
		uR"IHateDelim(const char* raw = ;)IHateDelim",
		uR"(const char* multiline = R"(Line1Line2Line3)\";)",
		uR"(const char* multiline = ;)"
	};

	constexpr std::u32string_view test_cases_mixed_code[] = {
		U"int x = 10; const char* str = \"value\"; return x;",
		UR"IHateDelim(int x = 10; const char* str = ; return x;)IHateDelim",
		UR"(if (name == "test") { char c = 'x'; })",
		UR"(if (name == ) { char c = ; })",
		UR"(printf("Result: %d\n", value);)",
		UR"(printf(, value);)"
	};

	constexpr std::string_view test_cases_edge_cases[] = {
		R"(const char* quote = "He said \"hello\"";)",
		R"IHateDelim(const char* quote = ;)IHateDelim",
		R"(char backslash = '\\';)",
		R"(char backslash = ;)",
		R"("unclosed string)",
		R"()",
		R"('unclosed char)",
		R"()"
	};

	constexpr std::string_view test_cases_concatenated[] = {
		R"(const char* str = "Hello " "World";)",
		R"IHateDelim(const char* str = ;)IHateDelim",
		R"(auto msg = "Line1\n"
                   "Line2\n";)",
		R"(auto msg = ;)"
	};

	constexpr std::string_view test_cases_preprocessor[] = {
		R"(#define MSG "Error message")",
		R"IHateDelim(#define MSG )IHateDelim",
		R"(#include "header.hpp")",
		R"(#include )",
		R"(#error "Compilation failed")",
		R"(#error )"
	};
}

#endif
