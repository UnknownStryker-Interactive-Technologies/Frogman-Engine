#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/string.hxx>
#include <FE/string_view.hxx>
#include <FE/clock.hpp>
using namespace FE;

/*
Profile Guided Optimization: https://learn.microsoft.com/en-us/cpp/build/profile-guided-optimizations?view=msvc-170

TO DO: 
1. variable table
                                 
																														  |--- int32 variable hash map
																														  |--- float64 variable hash map
																														  |--- FE::string variable hash map
std::unordered_map<Key: type name string, Value: std::unordered_map<Key: varaible name string, Value: varaible address>> -|--- FE::array variable hash map
																														  |--- std::list variable hash map
																														  |--- std::deque variable hash map
2. memory layout reflection for serialization  																		      |--- etc...
                                                                                      *
	                                                                                  |
	Shallower <----- Memory Hierarchy Depth ----> Deeper                      Lower Memory Address
                                                      |----------------|              |
	                                                  | - FE::string - |              |
	|----------------------------------|    ----------|FE::smart_ptr   |              |
	|  Target Entry Non-Trivial Object |    |         |length, capacity|              |
	|- member variables -              |    |         |----------------|              |
	|  FE::string m_raw_name --------------|----|                                         |
	|  FE::vector<float64, 3> m_vector |-------|                                      |
	|----------------------------------|       |       |----------------|             |
	                                           |       | - FE::vector - |             |
											   |-------| x              |             |
													   | y              |             |
													   | z              |             |
													   |----------------|     Higher Memory Address
													                                  |
																					  *
																					  
Memory Layer Traversal Order: Entry.FE::string m_raw_name -> FE::string.FE::smart_ptr -> FE::smart_ptr.m_smart_ptr data 
																									|
												    |-----------------------------------------------|
												    |
												    |--> FE::string.length -> FE::string.capacity
																					   |
																					   |
									           |---------------------------------------|
											   |
											   |--> FE::vector.x -> FE::vector.y -> FE::vector.z
*/ 
int main(int argc_p, char** argv_p)
{

  	testing::InitGoogleTest(&argc_p, argv_p);

	if (argv_p == nullptr)
	{
		char l_arg0_default[] = "benchmark";
		char* l_args_default = l_arg0_default;
		argc_p = 1;
		argv_p = &l_args_default;
	}
	
    benchmark::Initialize(&argc_p, argv_p);
	FE_ABORT_IF(benchmark::ReportUnrecognizedArguments(argc_p, argv_p) == true, "Failed to meet the expectation: Unrecognized Benchmark Arguments Detected.");
    int32 l_exit_code = RUN_ALL_TESTS();
	std::cerr << "\n\n";
	benchmark::RunSpecifiedBenchmarks();
	std::cerr << "\n\n";
    benchmark::Shutdown();
    return l_exit_code;
}



TEST(basic_string, constructors)
{
	using namespace FE;

	{
		FE::string l_string(count_t{ 3 }, 'A');
		EXPECT_TRUE(l_string == "AAA");
		FE::string l_another_string(l_string, index_t{ 1 });
		EXPECT_TRUE(l_another_string == "AA");
	}

	{
		FE::string l_string(count_t{ 3 }, 'A');
		EXPECT_TRUE(l_string == "AAA");
		FE::string l_another_string(std::move(l_string), index_t{ 1 });
		EXPECT_TRUE(l_another_string == "AA");
	}


	{
		FE::string l_string = "FE::basic_string<CharT, Allocator, traits>";
		EXPECT_TRUE(l_string == "FE::basic_string<CharT, Allocator, traits>");

		FE::string l_another_string(l_string, index_t{ 4 }, count_t{ 12 });
		EXPECT_TRUE(l_another_string == "basic_string");
	}

	{
		FE::string l_string = "FE::basic_string<CharT, Allocator, traits>";
		EXPECT_TRUE(l_string == "FE::basic_string<CharT, Allocator, traits>");

		FE::string l_another_string(std::move(l_string), index_t{ 4 }, count_t{ 12 });
		EXPECT_TRUE(l_another_string == "basic_string");
	}
	

	{
		FE::string l_string = "basic_string";
		FE::string l_another_string = l_string;
		EXPECT_TRUE(l_another_string == "basic_string");
	}

	{
		FE::string l_string = "basic_string";
		FE::string l_another_string = std::move(l_string);
		EXPECT_TRUE(l_another_string == "basic_string");
	}

	{
		FE::string l_string = "basic_string";
		FE::string_view l_string_view = l_string;

		FE::string l_another_string = l_string_view;
		EXPECT_TRUE(l_another_string == "basic_string");
	}

	{
		FE::string l_string = "basic_string";
		FE::string_view l_string_view = l_string;

		FE::string l_another_string(l_string_view, FE::index_t{ 6 }, FE::count_t{ 6 });
		EXPECT_TRUE(l_another_string == "string");
	}

	{
		FE::string l_string = "basic_string";
		FE::string l_another_string;
		l_another_string = std::move(l_string);
	}
}




TEST(basic_string, element_access)
{
	using namespace FE;

	FE::string l_string = "Cheese Cake";
	EXPECT_EQ(l_string[7], 'C');

	EXPECT_EQ(l_string.front(), 'C');
	EXPECT_EQ(l_string.back(), 'e');

	EXPECT_TRUE(algorithm::string::compare(l_string.data(), "Cheese Cake"));
	EXPECT_TRUE(algorithm::string::compare(l_string.c_str(), "Cheese Cake"));

	FE::string_view l_string_view = l_string;

	FE::string l_another_string;
	l_another_string = l_string_view;
}




TEST(basic_string, iterators)
{
	{
		FE::string l_string = "Cheese Cake";
		FE::string::iterator l_iterator = l_string.begin();
		for (FE::string::value_type c : l_string)
		{
			EXPECT_EQ(c, *l_iterator);
			++l_iterator;
		}
	}

	{
		FE::string l_string = "Cheese Cake";
		FE::string::const_iterator l_iterator = l_string.cbegin();
		for (FE::string::value_type c : l_string)
		{
			EXPECT_EQ(c, *l_iterator);
			++l_iterator;
		}
	}


	{
		FE::string l_string = "Cheese Cake";
		FE::algorithm::string::invert(l_string.data());
		FE::string l_cheese_cake = "Cheese Cake";

		FE::string::reverse_iterator l_iterator = l_string.rbegin();
		for (FE::string::value_type c : l_cheese_cake)
		{
			EXPECT_EQ(c, *l_iterator);
			++l_iterator;
		}
	}

	{
		FE::string l_string = "Cheese Cake";
		FE::algorithm::string::invert(l_string.data());
		FE::string l_cheese_cake = "Cheese Cake";

		FE::string::const_reverse_iterator l_iterator = l_string.crbegin();
		for (FE::string::value_type c : l_cheese_cake)
		{
			EXPECT_EQ(c, *l_iterator);
			++l_iterator;
		}
	}

	{
		FE::ASCII* l_str = "Pizza";
		FE::const_iterator<FE::contiguous_iterator<FE::ASCII>> l_cbegin{ l_str };
		FE::const_iterator<FE::contiguous_iterator<FE::ASCII>> l_cend{ l_str + strlen(l_str) };

		FE::string l_string(l_cbegin, l_cend);


		EXPECT_TRUE(l_string == "Pizza");
	}
}




TEST(basic_string, capacity)
{
	{
		FE::string l_string;
		EXPECT_TRUE(l_string.is_empty());
	}

	{
		FE::string l_string = "Aglio e olio";
		EXPECT_FALSE(l_string.is_empty());
	}

	{
		FE::string l_string = "Aglio e olio";
		EXPECT_EQ(l_string.length(), std::strlen(l_string.data()));
	}

	{
		FE::string l_string;
		EXPECT_EQ(l_string.max_length(), FE::max_value<FE::string::length_type>);
	}

	{
		FE::string l_string;
		l_string.reserve(10);
		EXPECT_EQ(l_string.capacity(), 11);

		l_string.extend(20);
		EXPECT_EQ(l_string.capacity(), 31);

		l_string = "Aglio e olio";
		l_string.shrink_to_fit();
		//
		l_string.clear();
		//
	}

}




TEST(basic_string, operations)
{
	// FE::string.insert()
	{
		FE::string l_string = "etreme programming is an apple.";
		l_string.extend(64);
		l_string.insert(1, 1, 'x');
		EXPECT_TRUE("extreme programming is an apple." == l_string);

		l_string = "TDD is T Driven Development";
		l_string.insert(8, "ime-Inefficiency");
		EXPECT_TRUE("TDD is Time-Inefficiency Driven Development" == l_string);

		EXPECT_EQ(l_string.length(), strlen(l_string.c_str()));

		l_string = "Unit testing is  TDD";
		FE::string l_not = "not";
		l_string.insert(16, l_not);
		EXPECT_TRUE("Unit testing is not TDD" == l_string);

		l_string.insert(0, "Unit testing is not TDD", 0, 12);
		EXPECT_TRUE("Unit testingUnit testing is not TDD" == l_string);

		l_string.insert(12, { ' ', 'f', 'r', 'a', 'm', 'e', 'w', 'o', 'r', 'k', '.', ' '});

		EXPECT_TRUE("Unit testing framework. Unit testing is not TDD" == l_string);
	}

	// FE::string.insert_range()
	{
		FE::string l_string = "C++ 23";
		l_string.reserve(32);
		l_string.insert_range(l_string.length(), "C# is awesome.", FE::algorithm::string::range{ 2, 14 });
		EXPECT_TRUE("C++ 23 is awesome." == l_string);
	}

	// FE::string.erase()
	{
		FE::string l_string = "Golden Apple";
		l_string.erase(FE::index_t{ 6 }, FE::count_t{ 6 });

		EXPECT_TRUE(l_string == "Golden");
	}

	// FE::string.push_back()
	// FE::string.pop_back()
	{
		FE::string l_string = "ABC";
		EXPECT_EQ(l_string.pop_back(), 'C');
		EXPECT_EQ(l_string.pop_back(), 'B');

		l_string.push_back('A');
		l_string.push_back('A');

		EXPECT_TRUE(FE::algorithm::string::compare("AAA", l_string.data()));
	}

	// append
	{
		FE::string l_string = "COD Modern Warfare is my favorite ";
		l_string.reserve(256);
		l_string.append(3, 'A');
		EXPECT_TRUE(l_string == "COD Modern Warfare is my favorite AAA");


		FE::string l_second_string = " shooter game.";
		l_second_string.reserve(256);
		l_string += l_second_string;
		EXPECT_TRUE(l_string == "COD Modern Warfare is my favorite AAA shooter game.");




		l_second_string = "Another my favorite ";
		l_second_string.append(l_string, 34, 3);
		EXPECT_TRUE(l_second_string == "Another my favorite AAA");


		l_second_string.append(" title is ", 0, 6);
		EXPECT_TRUE(l_second_string == "Another my favorite AAA title");


		l_second_string += " is Tom Clancy's";
		EXPECT_TRUE(l_second_string == "Another my favorite AAA title is Tom Clancy's");


		var::ASCII l_cstring[] = " Ghost Recon: ";
		l_second_string.append(FE::const_iterator<FE::contiguous_iterator<char>>{l_cstring}, FE::const_iterator<FE::contiguous_iterator<char>>{l_cstring + 14});
		EXPECT_TRUE(l_second_string == "Another my favorite AAA title is Tom Clancy's Ghost Recon: ");


		l_second_string.append({ 'F', 'u', 't', 'u', 'r', 'e', ' ', 'S', 'o', 'l', 'd', 'i', 'e', 'r', '.' });
		EXPECT_TRUE(l_second_string == "Another my favorite AAA title is Tom Clancy's Ghost Recon: Future Soldier.");
	}

	// append_range
	{
		FE::string l_string = "Design patterns are";
		l_string.reserve(64);
		l_string.append_range("They are not silver bullets.", FE::algorithm::string::range{ 8, 28 });
		EXPECT_TRUE(l_string == "Design patterns are not silver bullets.");
	}

	// operator +=
	{
		FE::string l_string = "Design patterns are";
		l_string.reserve(64);
		l_string += " not silver bullets.";
		EXPECT_TRUE(l_string == "Design patterns are not silver bullets.");
	}

	// starts_with
	{
		FE::string l_string = "Cheese Crust Pizza";
		EXPECT_TRUE(l_string.starts_with("Cheese"));
	}

	// ends_with
	{
		FE::string l_string = "Cheese Crust Pizza";
		EXPECT_TRUE(l_string.ends_with("Pizza"));
	}

	// contains
	{
		FE::string l_string = "Cheese Crust Pizza";
		EXPECT_TRUE(l_string.contains("Crust"));
	}

	// replace [impl from char_traits]
	{
		FE::string l_string = "Java";
		l_string.reserve(32);
		FE::string l_other = "ellyfish";
		l_other.reserve(32);
		l_string.replace(1, 3, l_other);
		EXPECT_TRUE(l_string == "Jellyfish");


		FE::string l_javachip = "Javachip";
		l_javachip.reserve(32);
		l_string.replace(1, 4, l_javachip, 1, 3);
		EXPECT_TRUE(l_string == "Javafish");


		l_string.replace(4, 4, "fish & chips", 12);
		EXPECT_TRUE(l_string == "Javafish & chips");


		l_string.replace(1, 9, "arvis");
		EXPECT_TRUE(l_string == "Jarvis chips");


		l_string = "boh!";
		l_string.replace(1, 1, 'a', 2);
		EXPECT_TRUE(l_string == "baah!");


		l_string = "C#";
		l_string.replace(1, 1, { '+', '+' });
		EXPECT_TRUE(l_string == "C++");


		FE::string l_carbon = "Carbon";
		l_carbon.reserve(32);
		l_string.replace(1, 3, l_carbon.cbegin() + 1, l_carbon.cend());
		EXPECT_TRUE(l_string == "Carbon");
	}

	// replace_with_range
	{
		FE::string l_string = "Javascript";
		FE::string l_other = "Typescript";
		l_string.reserve(64);
		l_other.reserve(64);

		l_string.replace_with_range(0, 4, l_other, FE::algorithm::string::range{ 0, 4 });
		EXPECT_TRUE(l_string == "Typescript");


		l_string = "x86 assembly";
		l_string.replace_with_range(3, 1, "-64 bit", FE::algorithm::string::range{ 0, 4 });
		EXPECT_TRUE(l_string == "x86-64 assembly");

		l_other = "an avx-512 SIMD instruction";
		l_string.replace_with_range(8, 7, l_other, FE::algorithm::string::range{ 4, 10 });
		EXPECT_TRUE(l_string == "x86-64 avx-512");
	}

	// substr
	{
		FE::string l_string = "FE::string is a string class template.";
		FE::string l_another = l_string.substr(FE::index_t{ 23 }, FE::count_t{ 5 });
		EXPECT_TRUE(l_another == "class");
	}

	// copy
	{
		FE::string l_string = "Fazbear Pizza";
		FE::string::value_type l_buffer[20] = "\0";
		l_string.copy(l_buffer, FE::capacity_t{ 20 }, FE::count_t{ 5 }, FE::index_t{ 8 });
		EXPECT_TRUE(FE::algorithm::string::compare(l_buffer, "Pizza"));
	}

	// resize
	{
		FE::string l_string = "Pizza";
		l_string.reserve(32);
		l_string.extend(64);
		l_string.resize(16);

		EXPECT_EQ(l_string.capacity(), 17);
	}
}




TEST(basic_string, search)
{
	// find
	{
		FE::string l_string = "A vampire has a peice of garlic in his pocket.";
		std::optional<FE::algorithm::string::range> l_result = l_string.find("garlic");
		EXPECT_TRUE(FE::algorithm::string::compare_ranged(l_string.data(), FE::algorithm::string::range{ l_result->_begin, l_result->_end }, "garlic", FE::algorithm::string::range{ 0, strlen("garlic") }));
	}

	// rfind
	{
		FE::string l_string = "This is a peice of a garlic from the vampire.";
		std::optional<FE::algorithm::string::range> l_result = l_string.rfind("a");
		EXPECT_TRUE(FE::algorithm::string::compare_ranged(l_string.data(), FE::algorithm::string::range{ l_result->_begin - 1, l_result->_end + 5 }, "vampire", FE::algorithm::string::range{ 0, strlen("vampire") }));
	}

	// count_chars
	{
		FE::string l_string = "Garlic Mayonnaise";
		std::optional<FE::algorithm::string::count<FE::string::value_type>> l_result = l_string.count_chars('a');
		EXPECT_EQ(l_result->_match_count, 3);
	}
}