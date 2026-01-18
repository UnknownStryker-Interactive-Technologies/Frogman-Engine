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
	struct string_purge_test_case
	{
		std::string_view _input;
		std::string_view _expected_output;
		std::string_view _description;
	};


	class string_purge_tester
	{
	public:
		string_purge_tester() noexcept = default;
		~string_purge_tester() noexcept = default;

		void add_test_case(std::string_view input_p, std::string_view expected_output_p, std::string_view description_p) noexcept;
		
		[[nodiscard]] bool run_all_tests() noexcept;
		[[nodiscard]] bool run_single_test(size_t test_index_p) noexcept;
		
		void print_test_results() const noexcept;
		[[nodiscard]] size_t get_passed_count() const noexcept { return _passed_count; }
		[[nodiscard]] size_t get_failed_count() const noexcept { return _failed_count; }
		[[nodiscard]] size_t get_total_count() const noexcept { return _test_cases.size(); }

	private:
		std::vector<string_purge_test_case> _test_cases;
		size_t _passed_count = 0;
		size_t _failed_count = 0;

		[[nodiscard]] bool verify_test(const string_purge_test_case& test_case_p) noexcept;
	};


	void initialize_string_purge_test_suite(string_purge_tester& tester_p) noexcept;

	[[nodiscard]] bool test_basic_string_literals() noexcept;
	[[nodiscard]] bool test_character_literals() noexcept;
	[[nodiscard]] bool test_escaped_characters() noexcept;
	[[nodiscard]] bool test_raw_string_literals() noexcept;
	[[nodiscard]] bool test_multiline_strings() noexcept;
	[[nodiscard]] bool test_nested_quotes() noexcept;
	[[nodiscard]] bool test_empty_strings() noexcept;
	[[nodiscard]] bool test_mixed_content() noexcept;
	[[nodiscard]] bool test_edge_cases() noexcept;
	[[nodiscard]] bool test_unicode_literals() noexcept;
	[[nodiscard]] bool test_concatenated_strings() noexcept;
	[[nodiscard]] bool test_preprocessor_strings() noexcept;

	[[nodiscard]] bool run_all_string_purge_tests() noexcept;


	constexpr std::string_view test_cases_basic_strings[] = {
		R"(const char* str = "hello world";)",
		R"(const char* str = ;)",
		R"(const char* empty = "";)",
		R"(const char* empty = ;)",
		R"(std::string msg = "Error: failed";)",
		R"(std::string msg = ;)"
	};

	constexpr std::string_view test_cases_character_literals[] = {
		R"(char c = 'a';)",
		R"(char c = ;)",
		R"(char newline = '\n';)",
		R"(char newline = ;)",
		R"(char quote = '\'';)",
		R"(char quote = ;)"
	};

	constexpr std::string_view test_cases_escaped_sequences[] = {
		R"(const char* path = "C:\\Users\\file.txt";)",
		R"(const char* path = ;)",
		R"(const char* json = "{\"key\": \"value\"}";)",
		R"(const char* json = ;)",
		R"(const char* tabs = "line1\tline2\n";)",
		R"(const char* tabs = ;)"
	};

	constexpr std::string_view test_cases_raw_strings[] = {
		R"delim(const char* raw = R"(Hello "world")";)delim",
		R"(const char* raw = ;)",
		R"delim(const char* multiline = R"(Line1
Line2
Line3)";)delim",
		R"(const char* multiline = ;)"
	};

	constexpr std::string_view test_cases_mixed_code[] = {
		R"(int x = 10; const char* str = "value"; return x;)",
		R"(int x = 10; const char* str = ; return x;)",
		R"(if (name == "test") { char c = 'x'; })",
		R"(if (name == ) { char c = ; })",
		R"(printf("Result: %d\n", value);)",
		R"(printf(, value);)"
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
