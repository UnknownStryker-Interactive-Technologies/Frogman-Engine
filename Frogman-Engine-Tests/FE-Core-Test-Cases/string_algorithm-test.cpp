#include <gtest/gtest.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#define _IMPORT_TO_STRING_
#include <FE/core/algorithm/string.hxx>
using namespace FE;
using namespace algorithm;




TEST(FE_algorithm_string, string_to_integer)
{
	char l_buffer[10] = "2023";
	var::int32 l_result = ::FE::algorithm::string::string_to_integer<char, ::FE::var::int32>(l_buffer);
	EXPECT_EQ(2023, l_result);


	l_result = ::FE::algorithm::string::char_to_integer<char, int>('1');
	EXPECT_EQ(l_result, 1);


	l_result = ::FE::algorithm::string::string_to_integer<char, ::FE::var::int32>("2023");
	EXPECT_EQ(2023, l_result);
}




TEST(FE_algorithm_string, length)
{
	char l_buffer[] = "FE::algorithm::string::length is a templated function that provides more functionalities than the traditional C-style length.";

	::FE::int64 l_result = ::FE::algorithm::string::length<char>(l_buffer);

	EXPECT_EQ(125, l_result);
}




TEST(FE_algorithm_string, compare_ranged)
{
	std::string l_left_string = "* Apple Mango *";
	std::string l_right_string = "- Mango -";
	algorithm::string::range left_substring_range{8, 13};
	algorithm::string::range right_substring_range{2, 7};

	EXPECT_TRUE(algorithm::string::compare_ranged(l_left_string.c_str(), left_substring_range, l_right_string.data(), right_substring_range));
	EXPECT_TRUE(algorithm::string::compare_ranged(l_left_string.data(), left_substring_range, l_right_string.c_str(), right_substring_range));

	EXPECT_TRUE(algorithm::string::compare_ranged(l_left_string.data(), left_substring_range, l_right_string.data(), right_substring_range));
	EXPECT_TRUE(algorithm::string::compare_ranged(l_left_string.c_str(), left_substring_range, l_right_string.c_str(), right_substring_range));
}




TEST(FE_algorithm_string, compare)
{
	char l_first_string[20] = "American";
	char l_second_string[20] = "British";

	EXPECT_TRUE( !::FE::algorithm::string::compare(l_first_string, l_second_string) );
}

TEST(FE_algorithm_string, constexpr_string_comparison)
{
	EXPECT_TRUE( ::FE::algorithm::string::compare(L"Jesus Loves You", L"Jesus Loves You") );
}




TEST(FE_algorithm_string, copy)
{
	char l_string[20] = "\n";

	::FE::algorithm::string::copy(l_string, "Hi, World", ::FE::algorithm::string::length("Hi, World"));

	EXPECT_TRUE( ::FE::algorithm::string::compare(l_string, "Hi, World") );
}




TEST(FE_algorithm_string, capitalize)
{
	std::string l_string = "experience our british premium bottle of water!";
	::FE::algorithm::string::capitalize<std::string::value_type>(l_string.data());

	EXPECT_TRUE(::FE::algorithm::string::compare(l_string.c_str(), "EXPERIENCE OUR BRITISH PREMIUM BOTTLE OF WATER!"));


	var::character l_grade = 'a';
	EXPECT_EQ(::FE::algorithm::string::capitalize<var::character>(l_grade), 'A');
}




TEST(FE_algorithm_string, to_lowercase)
{
	std::string l_string = "EXPERIENCE OUR KOREAN PREMIUM KIMCHI!";
	::FE::algorithm::string::to_lowercase<std::string::value_type>(l_string.data());

	EXPECT_TRUE(::FE::algorithm::string::compare(l_string.c_str(), "experience our korean premium kimchi!"));


	var::character l_grade = 'F';
	EXPECT_EQ(::FE::algorithm::string::to_lowercase<var::character>(l_grade), 'f');
}




TEST(FE_algorithm_string, capitalize_every_first_letter_of_words)
{
	std::string l_string = "experience the speed of light!";

	::FE::algorithm::string::capitalize_every_first_letter_of_words<std::string::value_type>(l_string.data());

	EXPECT_TRUE(::FE::algorithm::string::compare(l_string.data(), "Experience The Speed Of Light!"));
}




TEST(FE_algorithm_string, insensitive_comparison)
{
	std::string l_capitalized_string = "MANGO";
	std::string l_lowercased_string = "mango";

	EXPECT_TRUE(::FE::algorithm::string::insensitive_comparison(l_capitalized_string.c_str(), l_lowercased_string.data()));
	EXPECT_TRUE(::FE::algorithm::string::insensitive_comparison(l_capitalized_string.data(), l_lowercased_string.c_str()));

	EXPECT_TRUE(::FE::algorithm::string::insensitive_comparison(l_capitalized_string.data(), l_lowercased_string.data()));
	EXPECT_TRUE(::FE::algorithm::string::insensitive_comparison(l_capitalized_string.c_str(), l_lowercased_string.c_str()));
}




TEST(FE_algorithm_string, insensitive_ranged_comparison)
{
	std::string l_left_string = "* Apple MANGO *";
	std::string l_right_string = "- Mango -";
	algorithm::string::range left_substring_range{8, 13};
	algorithm::string::range right_substring_range{2, 7};

	EXPECT_TRUE(algorithm::string::insensitive_ranged_comparison(l_left_string.c_str(), left_substring_range, l_right_string.data(), right_substring_range));
	EXPECT_TRUE(algorithm::string::insensitive_ranged_comparison(l_left_string.data(), left_substring_range, l_right_string.c_str(), right_substring_range));

	EXPECT_TRUE(algorithm::string::insensitive_ranged_comparison(l_left_string.data(), left_substring_range, l_right_string.data(), right_substring_range));
	EXPECT_TRUE(algorithm::string::insensitive_ranged_comparison(l_left_string.c_str(), left_substring_range, l_right_string.c_str(), right_substring_range));
}




TEST(FE_algorithm_string, invert)
{
	std::string l_string = "!hatU fo ytisrevinU ehT";

	::FE::algorithm::string::invert<std::string::value_type>(l_string.data());

	EXPECT_TRUE(::FE::algorithm::string::compare(l_string.c_str(), "The University of Utah!"));
}




TEST(FE_algorithm_string, constexpr_hash_code)
{
	const char* l_cstring = "A bottle of water.";

	SWITCH(l_cstring)
	{
	CASE("A bottle of water."):
		EXPECT_TRUE(true);
		break;

	CASE("A bucket of milk."):
		EXPECT_TRUE(false);
		break;

	CASE("A cup of coke."):
		EXPECT_TRUE(false);
		break;

	default:
		EXPECT_TRUE(false);
		break;
	}
}




TEST(FE_algorithm_string, count_chars)
{
	char l_string[] = "Carrots contain carotene, antioxidants, etcetera.";

	auto l_result = ::FE::algorithm::string::count_chars(l_string, 't');

	EXPECT_EQ(l_result._match_count, 7);
	EXPECT_EQ(l_result._target, 't');
}

TEST(FE_algorithm_string, constexpr_count_chars)
{
	const char* l_string = "Carrots contain carotene, antioxidants, etcetera.";

	auto l_result = ::FE::algorithm::string::count_chars(l_string, 't');

	EXPECT_EQ(l_result._match_count, 7);
	EXPECT_EQ(l_result._target, 't');
}




TEST(FE_algorithm_string, find_the_first)
{
	{
		char l_string[] = "Carrots contain carotene, antioxidants, etcetera.";

		auto l_result = ::FE::algorithm::string::find_the_first(l_string, 'c');

		EXPECT_EQ(l_string[l_result->_begin], 'c');
	}

	{
		char l_string[] = "the University of Utah";

		std::optional l_result = ::FE::algorithm::string::find_the_first<var::character>(l_string, "Orange County");
		EXPECT_FALSE(l_result.has_value());

		l_result = ::FE::algorithm::string::find_the_first<var::character>(l_string, "Utah");

		EXPECT_TRUE(FE::algorithm::string::compare<char>(l_string + l_result->_begin, "Utah"));
	}
}


//#define _IMPORT_RELEASE_BUILD_MODE_MSVC_COMPILER_BUG_
TEST(FE_algorithm_string, find_the_last)
{
	{
		const char* l_string = "Jesus is the son of GOD";
		std::optional l_result = ::FE::algorithm::string::find_the_last<var::character>(l_string, "son");
		char l_substring[7] = "\0";
		FE::algorithm::string::copy(l_substring, l_string + l_result->_begin, 3);
		EXPECT_TRUE(FE::algorithm::string::compare<char>(l_substring, "son"));

		l_string = "Third Person Spec Ops Stealth Action Shooter";

		//#ifndef _IMPORT_RELEASE_BUILD_MODE_MSVC_COMPILER_BUG_
		//	EXPECT_FALSE(::FE::algorithm::string::search_very_last_substring<var::character>(l_string, "Expects Failure").has_value());
		//#else
		//	l_result = ::FE::algorithm::string::search_very_last_substring<var::character>(l_string, "Expects Failure");
		//	EXPECT_FALSE(l_result.has_value());
		//#endif

		l_result = ::FE::algorithm::string::find_the_last<var::character>(l_string, "Action");
		EXPECT_TRUE(l_result.has_value());
		FE::algorithm::string::copy(l_substring, l_string + l_result->_begin, 6);

		EXPECT_TRUE(FE::algorithm::string::compare<char>(l_substring, "Action"));

	}


	{
		char l_string[] = "Carrots contain carotene, antioxidants, etcetera.";

		auto l_result = ::FE::algorithm::string::find_the_last(l_string, 'c');

		EXPECT_EQ(l_string[l_result->_begin], 'c');
	}
}




TEST(FE_algorithm_string, count_chars_within_range)
{
	char l_string[] = "Carrots contain carotene, antioxidants, etcetera.";

	auto l_result = string::count_chars_within_range(l_string, string::range{25, 49}, 't');

	EXPECT_EQ(l_result._match_count, 4);
	EXPECT_EQ(l_result._target, 't');
}

TEST(FE_algorithm_string, constexpr_count_all_corresponding_chars_within_range)
{
	char l_string[] = "Carrots contain carotene, antioxidants, etcetera.";

	auto l_result = string::count_chars_within_range(l_string, string::range{25, 49}, 't');

	EXPECT_EQ(l_result._match_count, 4);
	EXPECT_EQ(l_result._target, 't');
}




TEST(FE_algorithm_string, find_the_first_within_range)
{
	{
		char l_string[] = "Carrots contain carotene, antioxidants, etcetera.";

		auto l_result = ::FE::algorithm::string::find_the_first_within_range(l_string, FE::algorithm::string::range{ 7, 15 }, 'c');

		EXPECT_EQ(l_string[l_result->_begin], 'c');
	}


	{
		char l_string[] = "Love never fails. But where there are prophecies, they will cease; where there are tongues, they will be stilled; where there is knowledge, it will pass away.";

		std::optional l_result = ::FE::algorithm::string::find_the_first_within_range<var::character>(l_string, string::range{ 0, 17 }, "Love never fails.");
		EXPECT_TRUE(l_result.has_value());

		l_result = ::FE::algorithm::string::find_the_first_within_range<var::character>(l_string, string::range{ 0, 120 }, "will");

		EXPECT_TRUE(FE::algorithm::string::compare_ranged<char>(l_string, string::range{ l_result->_begin, l_result->_end }, "will", string::range{ 0, 4 }));
	}
}

TEST(FE_algorithm_string, find_the_last_within_range)
{
	{
		char l_string[] = "Carrots contain carotene, antioxidants, etcetera.";

		auto l_result = ::FE::algorithm::string::find_the_last_within_range(l_string, FE::algorithm::string::range{ 41, 49 }, 'c');

		EXPECT_EQ(l_string[l_result->_begin], 'c');
	}


	{
		const char* l_string = "Love is patient, love is kind. It does not envy, it does not boast, it is not proud. 5 It does not dishonor others, it is not self-seeking, it is not easily angered, it keeps no record of wrongs. 6 Love does not delight in evil but rejoices with the truth. 7 It always protects, always trusts, always hopes, always perseveres.\n";
		std::optional l_result = ::FE::algorithm::string::find_the_last_within_range<var::character>(l_string, string::range{ 0, 21 }, "love");
		char l_substring[7] = "\0";
		FE::algorithm::string::copy(l_substring, l_string + l_result->_begin, 4);
		EXPECT_TRUE(FE::algorithm::string::compare<char>(l_substring, "love"));

		l_string = "1 Corinthians 13:4-8\n";
		l_result = ::FE::algorithm::string::find_the_last_within_range<var::character>(l_string, string::range{ 0, 13 }, "9");
		EXPECT_FALSE(l_result.has_value());

		l_result = ::FE::algorithm::string::find_the_last_within_range<var::character>(l_string, string::range{ 2, 13 }, "i");
		EXPECT_TRUE(l_result.has_value());
		FE::algorithm::string::copy(l_substring, l_string + l_result->_begin, 1);

		EXPECT_TRUE(FE::algorithm::string::compare<char>(l_substring, "i"));
	}
}




TEST(FE_algorithm_string, concatenate)
{
	{
		char l_string[50] = "Boost Libraries are ";
		::FE::algorithm::string::concatenate(l_string, 50, "written in C++.", ::FE::algorithm::string::length("written in C++."));

		EXPECT_TRUE(::FE::algorithm::string::compare("Boost Libraries are written in C++.", l_string));
	}


	{
		char l_string_buffer[100] = "Today is";
		::FE::algorithm::string::concatenate
		(
			l_string_buffer,
			100,
			{
				" May", " Fourteenth", " 2023."
			}
		);

		EXPECT_TRUE(::FE::algorithm::string::compare(l_string_buffer, "Today is May Fourteenth 2023."));
	}


	{
		char l_string_buffer[10] = "M";
		::FE::algorithm::string::concatenate
		(
			l_string_buffer,
			10,
			{
				'e', 'l', 'o', 'n'
			}
		);

		EXPECT_TRUE(::FE::algorithm::string::compare(l_string_buffer, "Melon"));
	}

}