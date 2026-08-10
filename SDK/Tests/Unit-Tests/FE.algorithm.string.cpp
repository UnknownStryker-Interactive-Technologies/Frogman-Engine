#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.
// std
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

// FE
#include <FE/algorithm/string.hxx>




// copy
TEST(FE_algorithm_string, copy)
{
	char l_buffer[20] = "Hello, World!";
	char l_destination[20] = "\0";

	FE::algorithm::string::copy(l_destination, l_buffer, ::FE::algorithm::string::length(l_buffer));

	EXPECT_STREQ(l_buffer, l_destination);
}

// count_chars
TEST(FE_algorithm_string, count_chars)
{
	char l_buffer[20] = "Hello, World!";
	auto l_result = FE::algorithm::string::count_chars(l_buffer, 'l');

	EXPECT_EQ(3, l_result._match_count);
	EXPECT_EQ('l', l_result._target);
}

void frogman_count_chars(benchmark::State& state_p) noexcept
{
	char l_buffer[20] = "Hello, World!";
	for (auto _ : state_p)
	{
		auto r = FE::algorithm::string::count_chars(l_buffer, 'l');
		(void)r;
	}
}
BENCHMARK(frogman_count_chars);

void std_count_chars(benchmark::State& state_p) noexcept
{
	char l_buffer[20] = "Hello, World!";
	for (auto _ : state_p)
	{
		auto r = std::count<char*, char>(l_buffer, l_buffer + 20, 'l');
		(void)r;
	}
}
BENCHMARK(std_count_chars);




// capitalize
TEST(FE_algorithm_string, capitalize)
{
	char l_buffer[20] = "hello, world!";
	FE::algorithm::string::capitalize(l_buffer);

	EXPECT_STREQ("HELLO, WORLD!", l_buffer);
}

void frogman_capitalize(benchmark::State& state_p) noexcept
{
	char l_buffer[20] = "hello, world!";
	for (auto _ : state_p)
	{
		FE::algorithm::string::capitalize(l_buffer);
	}
}
BENCHMARK(frogman_capitalize);

void std_capitalize(benchmark::State& state_p) noexcept
{
	char l_buffer[20] = "hello, world!";
	for (auto _ : state_p)
	{
		for (var::uint32 i = 0; i < 20; ++i)
		{
			l_buffer[i] = (char)std::toupper(l_buffer[i]);
		}
	}
}
BENCHMARK(std_capitalize);




// capitalize a character
TEST(FE_algorithm_string, capitalize_char)
{
	char l_buffer = 'a';
	char l_result = FE::algorithm::string::capitalize(l_buffer);

	EXPECT_EQ('A', l_result);
}

void frogman_capitalize_char(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		char l_buffer = 'a';
		char l_result = FE::algorithm::string::capitalize(l_buffer);
		(void)l_result;
	}
}
BENCHMARK(frogman_capitalize_char);

void std_capitalize_char(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		char l_buffer = 'a';
		l_buffer = (char)std::toupper(l_buffer);
	}
}
BENCHMARK(std_capitalize_char);




// to_lowercase
TEST(FE_algorithm_string, to_lowercase)
{
	char l_buffer[20] = "HELLO, WORLD!";
	FE::algorithm::string::to_lowercase(l_buffer);

	EXPECT_STREQ("hello, world!", l_buffer);
}

// to_lowercase a character
TEST(FE_algorithm_string, to_lowercase_char)
{
	char l_buffer = 'A';
	char l_result = FE::algorithm::string::to_lowercase(l_buffer);

	EXPECT_EQ('a', l_result);
}

// capitalize_the_first_letter_of_words
TEST(FE_algorithm_string, capitalize_the_first_letter_of_words)
{
	char l_buffer[20] = "hello, world!";
	FE::algorithm::string::capitalize_the_first_letter_of_words(l_buffer);

	EXPECT_STREQ("Hello, World!", l_buffer);
}

// length
TEST(FE_algorithm_string, length)
{
	EXPECT_EQ(9, FE::algorithm::string::length("Hi, World"));

	char l_buffer[2039] 
		= "ONE PAGE (HALF) Bit zero wakes first, rightmost, least, the register does not care for grace, only position, only place -- LSB, the quiet start of every count. Sixty-four lanes hum awake in silicon, waiting for a clock signal cast across the die, a comparison, an equal sign, epi8, epi16, epi32 -- the width decides which door in the fence they walk through, BW or F, no third way in between. The mask arrives not as a verdict spoken twice but born already true or false, no vector held a moment in between, predicate from birth, k-register keeper of a truth no wider byte could hold. Somewhere a loop asks a foolish question -- modulo, when less-than would have done, and division answers slow and resentful, twenty cycles then forty, while comparison waits one, impatient, pipelined and forgiving, always cheap. Trailing zeros count the distance to the first yes, countr from the right, countl from the left, symmetric strangers pointing two opposite ways across the same sixty-four rooms, and somewhere in the difference, a width minus one, minus a count, finds the highest bit that ever mattered. Somewhere a console halves its own wide lanes to keep a clock from ever slipping, somewhere a compiler drops a whitepaper's promise of two hundred fifty six and doubles down on native width instead. None of this remembers being asked. The silicon does not know it is a metaphor. It only counts, compares, and moves along. Ninety-five percent of every desktop already carries this, already has the width, the count, the fused compare and branch, so the last five stay a rounding error for a shooter built on Zen and modern Core. A constexpr function may be written around an intrinsic it can never run -- ill-formed, no diagnostic required, five careful words that mean the silence is not proof of safety, only proof that no one asked loudly enough to be told no. Two kilobytes. Half a page, still exact, half the silicon, half the borrowed weight, every byte accounted, nothing wasted, the last line closing exactly where it always must.";
	auto l_result = FE::algorithm::string::length(l_buffer);

	EXPECT_EQ(2038, l_result);
}

void frogman_string_length(benchmark::State& state_p) noexcept
{
	char l_buffer[2039] 
		= "ONE PAGE (HALF) Bit zero wakes first, rightmost, least, the register does not care for grace, only position, only place -- LSB, the quiet start of every count. Sixty-four lanes hum awake in silicon, waiting for a clock signal cast across the die, a comparison, an equal sign, epi8, epi16, epi32 -- the width decides which door in the fence they walk through, BW or F, no third way in between. The mask arrives not as a verdict spoken twice but born already true or false, no vector held a moment in between, predicate from birth, k-register keeper of a truth no wider byte could hold. Somewhere a loop asks a foolish question -- modulo, when less-than would have done, and division answers slow and resentful, twenty cycles then forty, while comparison waits one, impatient, pipelined and forgiving, always cheap. Trailing zeros count the distance to the first yes, countr from the right, countl from the left, symmetric strangers pointing two opposite ways across the same sixty-four rooms, and somewhere in the difference, a width minus one, minus a count, finds the highest bit that ever mattered. Somewhere a console halves its own wide lanes to keep a clock from ever slipping, somewhere a compiler drops a whitepaper's promise of two hundred fifty six and doubles down on native width instead. None of this remembers being asked. The silicon does not know it is a metaphor. It only counts, compares, and moves along. Ninety-five percent of every desktop already carries this, already has the width, the count, the fused compare and branch, so the last five stay a rounding error for a shooter built on Zen and modern Core. A constexpr function may be written around an intrinsic it can never run -- ill-formed, no diagnostic required, five careful words that mean the silence is not proof of safety, only proof that no one asked loudly enough to be told no. Two kilobytes. Half a page, still exact, half the silicon, half the borrowed weight, every byte accounted, nothing wasted, the last line closing exactly where it always must.";
	for (auto _ : state_p)
	{
		benchmark::DoNotOptimize(l_buffer);
		auto r = FE::algorithm::string::length(l_buffer);
		(void)r;
	}
}
BENCHMARK(frogman_string_length);

void std_string_length(benchmark::State& state_p) noexcept
{
	char l_buffer[2039] 
		= "ONE PAGE (HALF) Bit zero wakes first, rightmost, least, the register does not care for grace, only position, only place -- LSB, the quiet start of every count. Sixty-four lanes hum awake in silicon, waiting for a clock signal cast across the die, a comparison, an equal sign, epi8, epi16, epi32 -- the width decides which door in the fence they walk through, BW or F, no third way in between. The mask arrives not as a verdict spoken twice but born already true or false, no vector held a moment in between, predicate from birth, k-register keeper of a truth no wider byte could hold. Somewhere a loop asks a foolish question -- modulo, when less-than would have done, and division answers slow and resentful, twenty cycles then forty, while comparison waits one, impatient, pipelined and forgiving, always cheap. Trailing zeros count the distance to the first yes, countr from the right, countl from the left, symmetric strangers pointing two opposite ways across the same sixty-four rooms, and somewhere in the difference, a width minus one, minus a count, finds the highest bit that ever mattered. Somewhere a console halves its own wide lanes to keep a clock from ever slipping, somewhere a compiler drops a whitepaper's promise of two hundred fifty six and doubles down on native width instead. None of this remembers being asked. The silicon does not know it is a metaphor. It only counts, compares, and moves along. Ninety-five percent of every desktop already carries this, already has the width, the count, the fused compare and branch, so the last five stay a rounding error for a shooter built on Zen and modern Core. A constexpr function may be written around an intrinsic it can never run -- ill-formed, no diagnostic required, five careful words that mean the silence is not proof of safety, only proof that no one asked loudly enough to be told no. Two kilobytes. Half a page, still exact, half the silicon, half the borrowed weight, every byte accounted, nothing wasted, the last line closing exactly where it always must.";
	for (auto _ : state_p)
	{
		benchmark::DoNotOptimize(l_buffer);
		auto r = std::strlen(l_buffer);
		(void)r;
	}
}
BENCHMARK(std_string_length);




// count_chars_within_range
TEST(FE_algorithm_string, count_chars_within_range)
{
	char l_buffer[20] = "Hello, World!";
	auto l_result = FE::algorithm::string::count_chars_within_range(l_buffer, FE::algorithm::string::range{ 0, 5 }, 'l');

	EXPECT_EQ(2, l_result._match_count);
	EXPECT_EQ('l', l_result._target);
}

// compare
TEST(FE_algorithm_string, compare)
{
	char l_buffer1[20] = "Hello, World!";
	char l_buffer2[20] = "Hello, World!";

	auto l_result = FE::algorithm::string::compare(l_buffer1, l_buffer2);

	EXPECT_TRUE(l_result);
}

void frogman_string_compare(benchmark::State& state_p) noexcept
{
	char l_buffer1[20] = "Hello, World!";
	char l_buffer2[20] = "Hello, World!";
	for (auto _ : state_p)
	{
		auto r = FE::algorithm::string::compare(l_buffer1, l_buffer2);
		(void)r;
	}
}
BENCHMARK(frogman_string_compare);

void std_string_compare(benchmark::State& state_p) noexcept
{
	char l_buffer1[20] = "Hello, World!";
	char l_buffer2[20] = "Hello, World!";
	for (auto _ : state_p)
	{
		auto r = std::strcmp(l_buffer1, l_buffer2);
		(void)r;
	}
}	
BENCHMARK(std_string_compare);




// compare_ranged
TEST(FE_algorithm_string, compare_ranged)
{
	char l_buffer1[20] = "Hello, World!";
	char l_buffer2[20] = "Hello, World!";

	auto l_result = FE::algorithm::string::compare_ranged(l_buffer1, FE::algorithm::string::range{ 0, 5 }, l_buffer2, FE::algorithm::string::range{ 0, 5 });

	EXPECT_TRUE(l_result);
}

// insensitive_comparison
TEST(FE_algorithm_string, insensitive_comparison)
{
	char l_buffer1[20] = "Hello, World!";
	char l_buffer2[20] = "HELLO, WORLD!";

	auto l_result = FE::algorithm::string::insensitive_comparison(l_buffer1, l_buffer2);

	EXPECT_TRUE(l_result);
}

// insensitive_ranged_comparison
TEST(FE_algorithm_string, insensitive_ranged_comparison)
{
	char l_buffer1[20] = "Hello, World!";
	char l_buffer2[20] = "HELLO, WORLD!";

	FE::boolean l_result = FE::algorithm::string::insensitive_ranged_comparison(l_buffer1, FE::algorithm::string::range{ 0, 5 }, l_buffer2, FE::algorithm::string::range{ 0, 5 });

	EXPECT_TRUE(l_result);
}

// concatenate
TEST(FE_algorithm_string, concatenate)
{
	char l_buffer[20] = "Hello, ";
	FE::algorithm::string::concatenate(l_buffer, 20, "World!", 6);

	EXPECT_STREQ("Hello, World!", l_buffer);
}

void frogman_string_concatenate(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		char l_buffer[20] = "Hello, ";
		FE::algorithm::string::concatenate(l_buffer, 20, "World!", 6);
	}
}
BENCHMARK(frogman_string_concatenate);

void std_string_concatenate(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		char l_buffer[20] = "Hello, ";
		strcat_s(l_buffer, "World!");
	}
}
BENCHMARK(std_string_concatenate);




// concatenate characters
TEST(FE_algorithm_string, concatenate_chars)
{
	char l_buffer[20] = "Hello, ";
	FE::algorithm::string::concatenate(l_buffer, 20, { 'W', 'o', 'r', 'l', 'd', '!' });

	EXPECT_STREQ("Hello, World!", l_buffer);
}

// find_the_first
TEST(FE_algorithm_string, find_the_first)
{
	char l_buffer[20] = "Hello, World!";
	auto l_result = FE::algorithm::string::find_the_first(l_buffer, 'W');
	EXPECT_EQ(7, l_result->_begin);

	l_result = FE::algorithm::string::find_the_first(l_buffer, 'X');
	EXPECT_EQ(l_result, std::nullopt);
}

void frogman_find_the_first(benchmark::State& state_p) noexcept
{
	std::string l_buffer = "Hello, World!";
	const char* l_str = l_buffer.data();
	for (auto _ : state_p)
	{
		auto r = FE::algorithm::string::find_the_first(l_str, 'W');
		(void)r;
	}
}
BENCHMARK(frogman_find_the_first);

void std_find_the_first(benchmark::State& state_p) noexcept
{
	std::string l_buffer = "Hello, World!";
	const char* l_str = l_buffer.data();
	(void)l_str;
	for (auto _ : state_p)
	{
		auto r = l_buffer.find('W');
		(void)r;
	}
}
BENCHMARK(std_find_the_first);




// find_the_last
TEST(FE_algorithm_string, find_the_last)
{
	char l_buffer[20] = "Hello, World!";
	auto l_result = FE::algorithm::string::find_the_last(l_buffer, 'l');
	EXPECT_EQ(10, l_result->_begin);

	l_result = FE::algorithm::string::find_the_first(l_buffer, 'X');
	EXPECT_EQ(l_result, std::nullopt);
}

void frogman_find_the_last(benchmark::State& state_p) noexcept
{
	std::string l_buffer = "Hello, World!";
	const char* l_str = l_buffer.data();
	for (auto _ : state_p)
	{
		auto r = FE::algorithm::string::find_the_last(l_str, 'l');
		(void)r;
	}
}
BENCHMARK(frogman_find_the_last);

void std_find_the_last(benchmark::State& state_p) noexcept
{
	std::string l_buffer = "Hello, World!";
	const char* l_str = l_buffer.data();
	(void)l_str;
	for (auto _ : state_p)
	{
		auto r = l_buffer.rfind('l');
		(void)r;
	}
}
BENCHMARK(std_find_the_last);




// find_the_first_within_range
TEST(FE_algorithm_string, find_the_first_within_range)
{
	char l_buffer[20] = "Hello, World!";
	auto l_result = FE::algorithm::string::find_the_first_within_range(l_buffer, FE::algorithm::string::range{ 0, 5 }, 'W');
	EXPECT_EQ(std::nullopt, l_result);

	l_result = FE::algorithm::string::find_the_first_within_range(l_buffer, FE::algorithm::string::range{ 0, 5 }, 'e');
	EXPECT_EQ(l_result->_begin, 1);
}

// find_the_last_within_range
TEST(FE_algorithm_string, find_the_last_within_range)
{
	char l_buffer[20] = "Hello, World!";
	auto l_result = FE::algorithm::string::find_the_last_within_range(l_buffer, FE::algorithm::string::range{ 0, 5 }, 'l');
	EXPECT_EQ(3, l_result->_begin);

	l_result = FE::algorithm::string::find_the_last_within_range(l_buffer, FE::algorithm::string::range{ 0, 5 }, 'W');
	EXPECT_EQ(std::nullopt, l_result);
}

// find_the_first substring
TEST(FE_algorithm_string, find_the_first_substring)
{
	char l_buffer[20] = "Hello, World World!";
	auto l_result = FE::algorithm::string::find_the_first(l_buffer, "World");
	EXPECT_EQ(7, l_result->_begin);

	l_result = FE::algorithm::string::find_the_first(l_buffer, "Xorld");
	EXPECT_EQ(std::nullopt, l_result);

	l_result = FE::algorithm::string::find_the_first(l_buffer, "Hold");
	EXPECT_EQ(l_result, std::nullopt);
}

// find_the_last substring
TEST(FE_algorithm_string, find_the_last_substring)
{
	char l_buffer[20] = "Hello, World World!";
	auto l_result = FE::algorithm::string::find_the_last(l_buffer, "World");
	EXPECT_EQ(13, l_result->_begin);

	l_result = FE::algorithm::string::find_the_last(l_buffer, "Xorld");
	EXPECT_EQ(std::nullopt, l_result);

	l_result = FE::algorithm::string::find_the_last(l_buffer, "Hold");
	EXPECT_EQ(l_result, std::nullopt);
}

// find_the_first_within_range substring
TEST(FE_algorithm_string, find_the_first_within_range_substring)
{
	char l_buffer[30] = "Hello Hello, World World!";
	auto l_result = FE::algorithm::string::find_the_first_within_range(l_buffer, FE::algorithm::string::range{ 0, 15 }, "Hello");
	EXPECT_EQ(0, l_result->_begin);

	l_result = FE::algorithm::string::find_the_first_within_range(l_buffer, FE::algorithm::string::range{ 0, 15 }, "Hi");
	EXPECT_EQ(std::nullopt, l_result);

	l_result = FE::algorithm::string::find_the_first_within_range(l_buffer, FE::algorithm::string::range{ 0, 15 }, "loW");
	EXPECT_EQ(std::nullopt, l_result);
}

// find_the_last_within_range substring
TEST(FE_algorithm_string, find_the_last_within_range_substring)
{
	char l_buffer[25] = "Hi Hi, World World!";
	auto l_result = FE::algorithm::string::find_the_last_within_range(l_buffer, FE::algorithm::string::range{ 0, 10 }, "Hi");
	EXPECT_EQ(3, l_result->_begin);

	l_result = FE::algorithm::string::find_the_last_within_range(l_buffer, FE::algorithm::string::range{ 0, 15 }, "Bye");
	EXPECT_EQ(std::nullopt, l_result);

	l_result = FE::algorithm::string::find_the_last_within_range(l_buffer, FE::algorithm::string::range{ 0, 15 }, "Hold");
	EXPECT_EQ(std::nullopt, l_result);
}

// char_to_integer
TEST(FE_algorithm_string, char_to_integer)
{
	char l_buffer = '1';
	auto l_result = FE::algorithm::string::char_to_integer<char, int>(l_buffer);

	EXPECT_EQ(1, l_result);
}

// string_to_integer
TEST(FE_algorithm_string, string_to_integer)
{
	char l_buffer[10] = "2023";
	auto l_result = FE::algorithm::string::string_to_integer<char, int>(l_buffer);

	EXPECT_EQ(2023, l_result);
}

void frogman_string_to_integer(benchmark::State& state_p) noexcept
{
	char l_buffer[10] = "2023";
	for (auto _ : state_p)
	{
		auto r = FE::algorithm::string::string_to_integer<char, int>(l_buffer);
		(void)r;
	}
}
BENCHMARK(frogman_string_to_integer);

void std_string_to_integer(benchmark::State& state_p) noexcept
{
	char l_buffer[10] = "2023";
	for (auto _ : state_p)
	{
		auto r = std::atoi(l_buffer);
		(void)r;
	}
}
BENCHMARK(std_string_to_integer);




// invert
TEST(FE_algorithm_string, invert)
{
	char l_buffer[20] = "Hello, World!";
	FE::algorithm::string::invert(l_buffer);

	EXPECT_STREQ("!dlroW ,olleH", l_buffer);
}

void frogman_invert(benchmark::State& state_p) noexcept
{
	char l_buffer[20] = "Hello, World!";
	for (auto _ : state_p)
	{
		FE::algorithm::string::invert(l_buffer);
	}
}
BENCHMARK(frogman_invert);

void std_invert(benchmark::State& state_p) noexcept
{
	char l_buffer[20] = "Hello, World!";
	for (auto _ : state_p)
	{
		std::reverse(l_buffer, l_buffer + 13);
	}
}
BENCHMARK(std_invert);