#ifndef _FE_HEADER_TOOL_STRING_PURGE_TEST_HPP_
#define _FE_HEADER_TOOL_STRING_PURGE_TEST_HPP_
/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

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
		R"(const char* str = ;)",
		R"(const char* empty = "";)",
		R"(const char* empty = ;)",
		R"(std::string msg = "Error: failed";)",
		R"(std::string msg = ;)"
	};

	constexpr std::wstring_view test_cases_character_literals[] = {
		L"char c = 'a';",
		LR"(char c = ;)",
		LR"(char newline = '\n';)",
		LR"(char newline = ;)",
		LR"(char quote = '\'';)",
		LR"(char quote = ;)"
	};

	constexpr std::u8string_view test_cases_escaped_sequences[] = {
		u8"const char* path = \"C:\\Users\\file.txt\";",
		u8R"(const char* path = ;)",
		u8R"(const char* json = "{\"key\": \"value\"}";)",
		u8R"(const char* json = ;)",
		u8R"(const char* tabs = "line1\tline2\n";)",
		u8R"(const char* tabs = ;)"
	};

	constexpr std::u16string_view test_cases_raw_strings[] = {
		u"const char* raw = R\"(Hello \"world\")\";",
		uR"(const char* raw = ;)",
		uR"(const char* multiline = R"(Line1Line2Line3)";)",
		uR"(const char* multiline = ;)"
	};

	constexpr std::u32string_view test_cases_mixed_code[] = {
		U"int x = 10; const char* str = \"value\"; return x;",
		UR"(int x = 10; const char* str = ; return x;)",
		UR"(if (name == "test") { char c = 'x'; })",
		UR"(if (name == ) { char c = ; })",
		UR"(printf("Result: %d\n", value);)",
		UR"(printf(, value);)"
	};

	constexpr std::string_view test_cases_edge_cases[] = {
		R"(const char* quote = "He said \"hello\"";)",
		R"(const char* quote = ;)",
		R"(char backslash = '\\';)",
		R"(char backslash = ;)",
		R"("unclosed string)",
		R"()",
		R"('unclosed char)",
		R"()"
	};

	constexpr std::string_view test_cases_concatenated[] = {
		R"(const char* str = "Hello " "World";)",
		R"(const char* str = ;)",
		R"(auto msg = "Line1\n"
                   "Line2\n";)",
		R"(auto msg = ;)"
	};

	constexpr std::string_view test_cases_preprocessor[] = {
		R"(#define MSG "Error message")",
		R"(#define MSG )",
		R"(#include "header.hpp")",
		R"(#include )",
		R"(#error "Compilation failed")",
		R"(#error )"
	};
}

#endif
