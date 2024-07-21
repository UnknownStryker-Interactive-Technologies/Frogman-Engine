#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/array.hxx>
#include <FE/core/queue.hxx>
#include <FE/core/stack.hxx>

#include <FE/core/fstring.hxx>

// std
#include <stack>
#include <string>
#include <queue>


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
<<<<<<< HEAD
	|  FE::string m_raw_name --------------|----|                                         |
=======
	|  FE::string m_name --------------|----|                                         |
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
	|  FE::vector<float64, 3> m_vector |-------|                                      |
	|----------------------------------|       |       |----------------|             |
	                                           |       | - FE::vector - |             |
											   |-------| x              |             |
													   | y              |             |
													   | z              |             |
													   |----------------|     Higher Memory Address
													                                  |
																					  *
																					  
<<<<<<< HEAD
Memory Layer Traversal Order: Entry.FE::string m_raw_name -> FE::string.FE::smart_ptr -> FE::smart_ptr.m_smart_ptr data 
=======
Memory Layer Traversal Order: Entry.FE::string m_name -> FE::string.FE::smart_ptr -> FE::smart_ptr.m_smart_ptr data 
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
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
	using namespace FE;

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







TEST(queue, trivially_constructible_and_destructible)
{
	FE::fqueue<int, 10> l_another_queue = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	
	FE::fqueue<int, 10> l_queue(l_another_queue.cbegin(), l_another_queue.cend());
	FE::fqueue<int, 10>::const_iterator l_const_queue_iterator_begin = l_queue.cbegin();
	FE::fqueue<int, 10>::const_iterator l_const_queue_iterator_end = l_queue.cend();

	FE::fqueue<int, 10>::const_reverse_iterator l_const_queue_iterator_rbegin = l_queue.crbegin();
	FE::fqueue<int, 10>::const_reverse_iterator l_const_queue_iterator_rend = l_queue.crend();

	{
		FE::fqueue<int, 10> l_copied_queue = l_queue;
		FE::fqueue<int, 10>::const_iterator l_const_copied_queue_iterator_begin = l_copied_queue.cbegin();
		FE::fqueue<int, 10>::const_iterator l_const_copied_queue_iterator_end = l_copied_queue.cend();

		int l_expected_value = 0;
		while(l_const_queue_iterator_begin != l_const_queue_iterator_end)
		{
			EXPECT_EQ(*l_const_queue_iterator_begin, l_expected_value++);
			EXPECT_EQ(*l_const_queue_iterator_begin, *l_const_copied_queue_iterator_begin);
			++l_const_queue_iterator_begin;
			++l_const_copied_queue_iterator_begin;
		}


		l_const_queue_iterator_begin = l_queue.cbegin();
		l_const_copied_queue_iterator_begin = l_copied_queue.cbegin();


		FE::fqueue<int, 10> l_moved_queue = std::move(l_copied_queue);
		FE::fqueue<int, 10>::const_iterator l_const_moved_queue_iterator_begin = l_moved_queue.cbegin();
		FE::fqueue<int, 10>::const_iterator l_const_moved_queue_iterator_end = l_moved_queue.cend();

		while (l_const_moved_queue_iterator_begin != l_const_moved_queue_iterator_end)
		{
			EXPECT_EQ(*l_const_moved_queue_iterator_begin, *l_const_queue_iterator_begin);
			++l_const_moved_queue_iterator_begin;
			++l_const_queue_iterator_begin;

			EXPECT_EQ(*l_const_copied_queue_iterator_begin, 0);
		}

		l_copied_queue = { 5, 4, 3, 2, 1 };
		l_copied_queue.swap(l_queue);
	}
}

TEST(queue, constructible_and_destructible)
{
	FE::fqueue<std::string, 5> l_queue = { "hi", ",", " ", "world", "."};
	FE::fqueue<std::string, 5>::const_iterator l_const_queue_iterator_begin = l_queue.cbegin();
	FE::fqueue<std::string, 5>::const_iterator l_const_queue_iterator_end = l_queue.cend();

	FE::fqueue<std::string, 5>::const_reverse_iterator l_const_queue_iterator_rbegin = l_queue.crbegin();
	FE::fqueue<std::string, 5>::const_reverse_iterator l_const_queue_iterator_rend = l_queue.crend();

	{
		FE::fqueue<std::string, 5> l_copied_queue = l_queue;
		FE::fqueue<std::string, 5>::const_iterator l_const_copied_queue_iterator_begin = l_copied_queue.cbegin();
		FE::fqueue<std::string, 5>::const_iterator l_const_copied_queue_iterator_end = l_copied_queue.cend();

		while (l_const_queue_iterator_begin != l_const_queue_iterator_end)
		{
			EXPECT_EQ(*l_const_queue_iterator_begin, *l_const_copied_queue_iterator_begin);
			++l_const_queue_iterator_begin;
			++l_const_copied_queue_iterator_begin;
		}


		l_const_queue_iterator_begin = l_queue.cbegin();
		l_const_copied_queue_iterator_begin = l_copied_queue.cbegin();


		FE::fqueue<std::string, 5> l_moved_queue = std::move(l_copied_queue);
		FE::fqueue<std::string, 5>::const_iterator l_const_moved_queue_iterator_begin = l_moved_queue.cbegin();
		FE::fqueue<std::string, 5>::const_iterator l_const_moved_queue_iterator_end = l_moved_queue.cend();

		while (l_const_moved_queue_iterator_begin != l_const_moved_queue_iterator_end)
		{
			EXPECT_EQ(*l_const_moved_queue_iterator_begin, *l_const_queue_iterator_begin);
			++l_const_moved_queue_iterator_begin;
			++l_const_queue_iterator_begin;

			EXPECT_EQ(*l_const_copied_queue_iterator_begin, "\0");
		}

		l_copied_queue = { "hello", ",", " ", "C++", "."};
		l_copied_queue.swap(l_queue);
	}
}


TEST(queue, push_pop)
{
	using namespace FE;
	{
<<<<<<< HEAD
		FE::fqueue<var::uint64, 5> l_integral_queue;
=======
		FE::fqueue<var::int32, 5> l_integral_queue;
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		for (var::uint64 i = 0; i < l_integral_queue.capacity(); ++i)
		{
			l_integral_queue.push(i);
		}

		var::int32 l_expected_value = 0;
		for (var::uint64 i = 0; i < l_integral_queue.capacity() >> 1; ++i)
		{
			EXPECT_EQ(l_expected_value, l_integral_queue.pop());
			++l_expected_value;
		}

		for (var::uint64 i = 0; i < l_integral_queue.capacity() >> 1; ++i)
		{
			l_integral_queue.push(i);
		}

		l_integral_queue.pop_all();

<<<<<<< HEAD
		FE::fqueue<var::uint64, 5>::const_reverse_iterator l_const_iterator_begin = l_integral_queue.crbegin();
=======
		FE::fqueue<var::int32, 5>::const_reverse_iterator l_const_iterator_begin = l_integral_queue.crbegin();
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
	}


	std::initializer_list<std::string> l_initializer_list = { "hello", ",", " ", "C++", "." };
	FE::fqueue<std::string, 5> l_cstring_queue = { "hello", ",", " ", "C++", "." };
	FE::fqueue<FE::fstring<10>, 5> l_fstring_queue;

	FE::fqueue<std::string, 5>::const_iterator L_cstring_const_iterator_begin = l_cstring_queue.cbegin();
	FE::fqueue<FE::fstring<10>, 5>::const_iterator L_fstring_const_iterator_begin = l_fstring_queue.cbegin();

	EXPECT_EQ(l_cstring_queue.capacity(), l_fstring_queue.capacity());

	for (var::uint64 i = 0; i < l_cstring_queue.capacity(); ++i)
	{
		l_fstring_queue.push(l_cstring_queue.pop().c_str());

		EXPECT_EQ(strcmp(L_fstring_const_iterator_begin[i].c_str(), l_initializer_list.begin()[i].c_str()), 0);
	}
}

TEST(queue, multiple_assignments_)
{
	FE::fqueue<std::string, 5> l_queue = { "hi", ",", " ", "world", "." };
	l_queue = { "hello", ",", " ", "C++", "." };
	EXPECT_EQ(strcmp((l_queue.cbegin())->c_str(), "hello"), 0);
	EXPECT_EQ(strcmp((l_queue.cend() - 1)->c_str(), "."), 0);
	{
		std::initializer_list<const char*> l_initializer_list = { "hello", ",", " ", "C++", "." };
		auto l_initializer_list_cbegin = l_initializer_list.begin();
		EXPECT_EQ(strcmp((l_queue.cend() - 1)->c_str(), "."), 0);
		for (auto iterator = l_queue.cbegin(); iterator != l_queue.cend(); ++iterator)
		{
			EXPECT_EQ(strcmp(iterator->c_str(), *l_initializer_list_cbegin), 0);
			++l_initializer_list_cbegin;
		}
	}


	FE::fqueue<std::string, 5> l_another_queue = { "hi", ",", " ", "world", "." };
	l_queue = std::move(l_another_queue);
	{
		std::initializer_list<const char*> l_initializer_list = { "hi", ",", " ", "world", "." };
		auto l_initializer_list_cbegin = l_initializer_list.begin();
		for (auto iterator = l_queue.cbegin(); iterator != l_queue.cend(); ++iterator)
		{
			EXPECT_EQ(strcmp(iterator->c_str(), *l_initializer_list_cbegin), 0);
			++l_initializer_list_cbegin;
		}
	}
	l_queue = std::move(l_another_queue);
}

TEST(queue, container_comparison_)
{
	FE::fqueue<std::string, 5> l_languages1 = { "C++", "C#", "Python", "Java", "C"};
    EXPECT_EQ(strcmp(l_languages1.front().c_str(), "C++"), 0);
	FE::fqueue<std::string, 5> l_languages2 = { "C++", "C#", "Python", "Java", "C" };
    EXPECT_EQ(strcmp(l_languages2.back().c_str(), "C"), 0);
	EXPECT_TRUE(l_languages1 == l_languages2);

    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "C++"), 0);
    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "C#"), 0);
    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "Python"), 0);
    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "Java"), 0);
    EXPECT_FALSE(l_languages1 == l_languages2); // C++ is not equivalent to C

	l_languages1.push("Python");
	l_languages2.pop();
	EXPECT_TRUE(l_languages1 != l_languages2);
}


TEST(queue, pop_all)
{
	FE::fqueue<std::string, 3> l_languages;
	l_languages.push("Java");
	l_languages.push("Python");
	l_languages.push("C#");

	EXPECT_TRUE(l_languages.pop() == "Java");
	EXPECT_TRUE(l_languages.pop() == "Python");
	l_languages.push("C++");

	EXPECT_TRUE(l_languages.front() == "C#");
	EXPECT_TRUE(l_languages.back() == "C++");

	l_languages.pop_all();
	EXPECT_TRUE(l_languages.is_empty());
}




void FE_queue_push_and_pop(benchmark::State& state_p) noexcept
{
	FE::fqueue<const char*, 64> l_queue;

	for (auto _ : state_p)
	{
		l_queue.push("Pizza");
		l_queue.pop();
	}
}
BENCHMARK(FE_queue_push_and_pop);


void std_queue_push_and_pop(benchmark::State& state_p) noexcept
{
	std::queue<const char*> l_queue;

	for (auto _ : state_p)
	{
		l_queue.push("Pizza");
		l_queue.pop();
	}
}
BENCHMARK(std_queue_push_and_pop);













TEST(fstack, trivially_constructible_and_destructible)
{
	FE::fstack<int, 10> l_another_stack = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	FE::fstack<int, 10> l_stack(l_another_stack.cbegin(), l_another_stack.cend());
	FE::fstack<int, 10>::const_iterator l_const_stack_iterator_begin = l_stack.cbegin();
	FE::fstack<int, 10>::const_iterator l_const_stack_iterator_end = l_stack.cend();

	{
		FE::fstack<int, 10> l_copied_stack = l_stack;
		FE::fstack<int, 10>::const_iterator l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();
		FE::fstack<int, 10>::const_iterator l_const_copied_stack_iterator_end = l_copied_stack.cend();
		
		while(l_const_stack_iterator_begin != l_const_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_stack_iterator_begin, *l_const_copied_stack_iterator_begin);
			++l_const_stack_iterator_begin;
			++l_const_copied_stack_iterator_begin;
		}


		l_const_stack_iterator_begin = l_stack.cbegin();
		l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();


		FE::fstack<int, 10> l_moved_stack = std::move(l_copied_stack);
		FE::fstack<int, 10>::const_iterator l_const_moved_stack_iterator_begin = l_moved_stack.cbegin();
		FE::fstack<int, 10>::const_iterator l_const_moved_stack_iterator_end = l_moved_stack.cend();

		while (l_const_moved_stack_iterator_begin != l_const_moved_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_moved_stack_iterator_begin, *l_const_stack_iterator_begin);
			++l_const_moved_stack_iterator_begin;
			++l_const_stack_iterator_begin;

			EXPECT_EQ(*l_const_copied_stack_iterator_begin, 0);
		}

		l_copied_stack = { 5, 4, 3, 2, 1 };
		l_copied_stack.swap(l_stack);
	}
}

TEST(fstack, constructible_and_destructible)
{
	FE::fstack<std::string, 5> l_stack = { "hi", ",", " ", "world", "."};
	FE::fstack<std::string, 5>::const_iterator l_const_stack_iterator_begin = l_stack.cbegin();
	FE::fstack<std::string, 5>::const_iterator l_const_stack_iterator_end = l_stack.cend();

	FE::fstack<std::string, 5>::const_reverse_iterator l_const_stack_iterator_rbegin = l_stack.crbegin();
	FE::fstack<std::string, 5>::const_reverse_iterator l_const_stack_iterator_rend = l_stack.crend();

	{
		FE::fstack<std::string, 5> l_copied_stack = l_stack;
		FE::fstack<std::string, 5>::const_iterator l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();
		FE::fstack<std::string, 5>::const_iterator l_const_copied_stack_iterator_end = l_copied_stack.cend();

		while (l_const_stack_iterator_begin != l_const_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_stack_iterator_begin, *l_const_copied_stack_iterator_begin);
			++l_const_stack_iterator_begin;
			++l_const_copied_stack_iterator_begin;
		}


		l_const_stack_iterator_begin = l_stack.cbegin();
		l_const_copied_stack_iterator_begin = l_copied_stack.cbegin();


		FE::fstack<std::string, 5> l_moved_stack = std::move(l_copied_stack);
		FE::fstack<std::string, 5>::const_iterator l_const_moved_stack_iterator_begin = l_moved_stack.cbegin();
		FE::fstack<std::string, 5>::const_iterator l_const_moved_stack_iterator_end = l_moved_stack.cend();

		while (l_const_moved_stack_iterator_begin != l_const_moved_stack_iterator_end)
		{
			EXPECT_EQ(*l_const_moved_stack_iterator_begin, *l_const_stack_iterator_begin);
			++l_const_moved_stack_iterator_begin;
			++l_const_stack_iterator_begin;

			EXPECT_EQ(*l_const_copied_stack_iterator_begin, "\0");
		}

		l_copied_stack = { "hello", ",", " ", "C++", "."};
		l_copied_stack.swap(l_stack);
	}
}


TEST(fstack, push_pop)
{
	using namespace FE;
	{
<<<<<<< HEAD
		FE::fstack<var::uint32, 5> l_integral_stack;
		for (var::uint32 i = 0; i < l_integral_stack.capacity(); ++i)
=======
		FE::fstack<var::int32, 5> l_integral_stack;
		for (var::uint64 i = 0; i < l_integral_stack.capacity(); ++i)
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		{
			l_integral_stack.push(i);
		}

<<<<<<< HEAD
		var::uint64 l_expected_value = l_integral_stack.capacity();
		for (var::uint32 i = 0; i < l_integral_stack.capacity(); ++i)
=======
		var::int32 l_expected_value = l_integral_stack.capacity();
		for (var::uint64 i = 0; i < l_integral_stack.capacity(); ++i)
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		{
			--l_expected_value;
			EXPECT_EQ(l_expected_value, l_integral_stack.pop());
		}

<<<<<<< HEAD
		for (var::uint32 i = 0; i < l_integral_stack.capacity(); ++i)
=======
		for (var::uint64 i = 0; i < l_integral_stack.capacity(); ++i)
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		{
			l_integral_stack.push(i);
		}

		l_integral_stack.pop_all();

<<<<<<< HEAD
		FE::fstack<var::uint32, 5>::const_reverse_iterator l_const_iterator_begin = l_integral_stack.crbegin();
=======
		FE::fstack<var::int32, 5>::const_reverse_iterator l_const_iterator_begin = l_integral_stack.crbegin();
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948

		while (l_const_iterator_begin != l_integral_stack.crend())
		{
			EXPECT_EQ(*l_const_iterator_begin, 0);
			++l_const_iterator_begin;
		}
	}


	std::initializer_list<std::string> l_initializer_list = { "hello", ",", " ", "C++", "." };
	FE::fstack<std::string, 5> l_string_stack = { ".", "C++", " ", ",", "hello" };
	FE::fstack<FE::fstring<10>, 5> l_fstring_stack;

	FE::fstack<std::string, 5>::const_iterator L_cstring_const_iterator_begin = l_string_stack.cbegin();
	FE::fstack<FE::fstring<10>, 5>::const_iterator L_fstring_const_iterator_begin = l_fstring_stack.cbegin();

	EXPECT_EQ(l_string_stack.capacity(), l_fstring_stack.capacity());

	for (var::uint64 i = 0; i < l_string_stack.capacity(); ++i)
	{
		l_fstring_stack.push(l_string_stack.pop().c_str());
		
		EXPECT_EQ(strcmp(L_fstring_const_iterator_begin[i].c_str(), l_initializer_list.begin()[i].c_str()), 0);
	}
}

TEST(fstack, multiple_assignments_)
{
	FE::fstack<std::string, 5> l_stack = { "hi", ",", " ", "world", "." };
	l_stack = { "hello", ",", " ", "C++", "." };
	EXPECT_EQ(strcmp(l_stack.cbegin()->c_str(), "hello"), 0);
	EXPECT_EQ(strcmp((l_stack.cend() - 1)->c_str(), "."), 0);
	{
		std::initializer_list<const char*> l_initializer_list = { "hello", ",", " ", "C++", "." };
		auto l_initializer_list_cbegin = l_initializer_list.begin();
		EXPECT_EQ(strcmp((l_stack.cend() - 1)->c_str(), "."), 0);
		for (auto iterator = l_stack.cbegin(); iterator != l_stack.cend(); ++iterator)
		{
			EXPECT_EQ(strcmp(iterator->c_str(), *l_initializer_list_cbegin), 0);
			++l_initializer_list_cbegin;
		}
	}


	FE::fstack<std::string, 5> l_another_stack = { "hi", ",", " ", "world", "." };
	l_stack = std::move(l_another_stack);
	{
		std::initializer_list<const char*> l_initializer_list = { "hi", ",", " ", "world", "." };
		auto l_initializer_list_cbegin = l_initializer_list.begin();
		for (auto iterator = l_stack.cbegin(); iterator != l_stack.cend(); ++iterator)
		{
			EXPECT_EQ(strcmp(iterator->c_str(), *l_initializer_list_cbegin), 0);
			++l_initializer_list_cbegin;
		}
	}
	l_stack = std::move(l_another_stack);
}

TEST(fstack, container_comparison_)
{
	FE::fstack<std::string, 5> l_languages1 = { "C++", "C#", "Python", "Java", "C"};
    EXPECT_EQ(strcmp(l_languages1.top().c_str(), "C"), 0);
	FE::fstack<std::string, 5> l_languages2 = { "C++", "C#", "Python", "Java", "C" };
    EXPECT_EQ(strcmp(l_languages2.top().c_str(), "C"), 0);
	EXPECT_TRUE(l_languages1 == l_languages2);

    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "C"), 0);
    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "Java"), 0);
    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "Python"), 0);
    EXPECT_EQ(strcmp(l_languages1.pop().c_str(), "C#"), 0);
    EXPECT_EQ(strcmp(l_languages2.top().c_str(), "C"), 0);
    EXPECT_FALSE(l_languages1 == l_languages2); // C++ is not equivalent to C

	l_languages1.push("Python");
	l_languages2.pop();
	EXPECT_TRUE(l_languages1 != l_languages2); // Python is not equivalent to Java
}




void FE_stack_push_and_pop(benchmark::State& state_p) noexcept
{
	FE::fstack<const char*, 64> l_stack;

	for (auto _ : state_p)
	{
		l_stack.push("Pizza");
		l_stack.pop();
	}
}
BENCHMARK(FE_stack_push_and_pop);


void std_stack_push_and_pop(benchmark::State& state_p) noexcept
{
	std::stack<const char*, std::vector<const char*>> l_stack;

	for (auto _ : state_p)
	{
		l_stack.push("Pizza");
		l_stack.pop();
	}
}
BENCHMARK(std_stack_push_and_pop);


















TEST(farray, all)
{
	{
		FE::farray<const char*, 64> l_array;
		l_array.push_back("Pizza");
		
		EXPECT_EQ(strcmp(l_array.back(), "Pizza"), 0);
		l_array.pop_back();
	}
}


TEST(array, constructor_and_destructor)
{
	{
		FE::array<std::string> l_array;
	}

	{
		FE::array<std::string> l_array(2, "A bottle of water");
		EXPECT_TRUE(l_array[0] == "A bottle of water");
		EXPECT_TRUE(l_array[1] == "A bottle of water");
	}

	{
		std::initializer_list<std::string> l_initializer_list = {"A", "bottle", "of", "water"};
		FE::array<std::string> l_array(l_initializer_list.begin(), l_initializer_list.end());
		
		EXPECT_TRUE(l_array[0] == "A");
		EXPECT_TRUE(l_array[1] == "bottle");
		EXPECT_TRUE(l_array[2] == "of");
		EXPECT_TRUE(l_array[3] == "water");

	}

	{
		FE::array<std::string> l_array(2, "A bottle of water");
		FE::array<std::string> l_another_array = l_array;
		EXPECT_TRUE(l_another_array[0] == "A bottle of water");
		EXPECT_TRUE(l_another_array[1] == "A bottle of water");
	}

	{
		FE::array<std::string> l_array(2, "A bottle of water");
		FE::array<std::string> l_another_array = std::move(l_array);
		EXPECT_TRUE(l_another_array[0] == "A bottle of water");
		EXPECT_TRUE(l_another_array[1] == "A bottle of water");
	}

	{
		FE::array<std::string> l_array = { "A", "bottle", "of", "water" };
		EXPECT_TRUE(l_array[0] == "A");
		EXPECT_TRUE(l_array[1] == "bottle");
		EXPECT_TRUE(l_array[2] == "of");
		EXPECT_TRUE(l_array[3] == "water");
	}

	{
		FE::array<std::string> l_array(2, "A bottle of water");
		FE::array<std::string> l_another_array;
		l_another_array = l_array;
		EXPECT_TRUE(l_another_array[0] == "A bottle of water");
		EXPECT_TRUE(l_another_array[1] == "A bottle of water");
	}

	{
		FE::array<std::string> l_array(2, "A bottle of water");
		FE::array<std::string> l_another_array;
		l_another_array = std::move(l_array);
		EXPECT_TRUE(l_another_array[0] == "A bottle of water");
		EXPECT_TRUE(l_another_array[1] == "A bottle of water");
	}

	{
		FE::array<std::string> l_array;
		l_array = { "A", "bottle", "of", "water" };
		EXPECT_TRUE(l_array[0] == "A");
		EXPECT_TRUE(l_array[1] == "bottle");
		EXPECT_TRUE(l_array[2] == "of");
		EXPECT_TRUE(l_array[3] == "water");
	}
}




TEST(array, Element_access)
{
	{
		FE::array<std::string> l_array;
		l_array.reserve(2);
		l_array.assign(2, "A bottle of water");

		EXPECT_TRUE(l_array[0] == "A bottle of water");
		EXPECT_TRUE(l_array[1] == "A bottle of water");
	}

	{
		std::initializer_list<std::string> l_initializer_list = { "A", "bottle", "of", "water" };
		FE::array<std::string> l_array;
		l_array.reserve(8);
		l_array.assign(l_initializer_list.begin(), l_initializer_list.end());

		EXPECT_TRUE(l_array[0] == "A");
		EXPECT_TRUE(l_array[1] == "bottle");
		EXPECT_TRUE(l_array[2] == "of");
		EXPECT_TRUE(l_array[3] == "water");
	}

	{
		FE::array<std::string> l_array;
		l_array.reserve(8);
		l_array.assign({ "A", "bottle", "of", "water" });

		EXPECT_TRUE(l_array[0] == "A");
		EXPECT_TRUE(l_array[1] == "bottle");
		EXPECT_TRUE(l_array[2] == "of");
		EXPECT_TRUE(l_array[3] == "water");

	}
}




TEST(array, Iterators)
{
	{
		FE::array<std::string> l_array = { "A", "bottle", "of", "water" };
		std::initializer_list<std::string> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto value : l_array)
		{
			EXPECT_TRUE(value == *l_l_initializer_list_iterator);
			++l_l_initializer_list_iterator;
		}
	}


	{
		FE::array<std::string> l_array = { "A", "bottle", "of", "water" };
		std::initializer_list<std::string> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.cbegin(); it != l_array.cend(); ++it)
		{
			EXPECT_TRUE(*it == *l_l_initializer_list_iterator);
			++l_l_initializer_list_iterator;
		}
	}


	{
		FE::array<std::string> l_array = { "water", "of", "bottle", "A" };
		std::initializer_list<std::string> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.rbegin(); it != l_array.rend(); ++it)
		{
			EXPECT_TRUE(*it == *l_l_initializer_list_iterator);
			++l_l_initializer_list_iterator;
		}
	}


	{
		FE::array<std::string> l_array = { "water", "of", "bottle", "A" };
		std::initializer_list<std::string> l_initializer_list = { "A", "bottle", "of", "water" };
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.crbegin(); it != l_array.crend(); ++it)
		{
			EXPECT_TRUE(*it == *l_l_initializer_list_iterator);
			++l_l_initializer_list_iterator;
		}
	}
}




TEST(array, Capacity)
{
	{
		FE::array<std::string> l_array = { "water", "of", "bottle", "A" };
		EXPECT_FALSE(l_array.is_empty());

		EXPECT_EQ(l_array.size(), 4);

		l_array.reserve(10);
		EXPECT_EQ(l_array.capacity(), 10);

		l_array.extend(6);
		EXPECT_EQ(l_array.capacity(), 16);

		l_array.shrink_to_fit();
		EXPECT_EQ(l_array.capacity(), 4);
	}
}




TEST(array, Modifiers)
{
	{
		{
			FE::array<std::string> l_another_array = { "Christmas" };
			l_another_array.extend(2);
			l_another_array.emplace_back("Tree");
			EXPECT_TRUE(l_another_array[1] == "Tree");

			l_another_array.emplace_back("s");
			EXPECT_TRUE(l_another_array.back() == "s");
		}

	
	
	}
}






TEST(array, TrivialConstructorAndDestructor)
{
	using namespace FE;
	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array;
	}

	{
		FE::array<ASCII*> l_array(2, "A bottle of water");
=======
		FE::array<character*> l_array;
	}

	{
		FE::array<character*> l_array(2, "A bottle of water");
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		EXPECT_EQ(strcmp(l_array[0], "A bottle of water"), 0);
		EXPECT_EQ(strcmp(l_array[1], "A bottle of water"), 0);
	}

	{
<<<<<<< HEAD
		std::initializer_list<ASCII*> l_initializer_list = { "A", "bottle", "of", "water" };
		FE::array<ASCII*> l_array(l_initializer_list.begin(), l_initializer_list.end());
=======
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
		FE::array<character*> l_array(l_initializer_list.begin(), l_initializer_list.end());
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948

		EXPECT_EQ(strcmp(l_array[0], "A"), 0);
		EXPECT_EQ(strcmp(l_array[1], "bottle"), 0);
		EXPECT_EQ(strcmp(l_array[2], "of"), 0);
		EXPECT_EQ(strcmp(l_array[3], "water"), 0);

	}

	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array(2, "A bottle of water");
		FE::array<ASCII*> l_another_array = l_array;
=======
		FE::array<character*> l_array(2, "A bottle of water");
		FE::array<character*> l_another_array = l_array;
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		EXPECT_EQ(strcmp(l_another_array[0], "A bottle of water"), 0);
		EXPECT_EQ(strcmp(l_another_array[1], "A bottle of water"), 0);
	}

	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array(2, "A bottle of water");
		FE::array<ASCII*> l_another_array = std::move(l_array);
=======
		FE::array<character*> l_array(2, "A bottle of water");
		FE::array<character*> l_another_array = std::move(l_array);
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		EXPECT_EQ(strcmp(l_another_array[0], "A bottle of water"), 0);
		EXPECT_EQ(strcmp(l_another_array[1], "A bottle of water"), 0);
	}

	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array = { "A", "bottle", "of", "water" };
=======
		FE::array<character*> l_array = { "A", "bottle", "of", "water" };
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		EXPECT_EQ(strcmp(l_array[0], "A"), 0);
		EXPECT_EQ(strcmp(l_array[1], "bottle"), 0);
		EXPECT_EQ(strcmp(l_array[2], "of"), 0);
		EXPECT_EQ(strcmp(l_array[3], "water"), 0);
	}

	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array(2, "A bottle of water");
		FE::array<ASCII*> l_another_array;
=======
		FE::array<character*> l_array(2, "A bottle of water");
		FE::array<character*> l_another_array;
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		l_another_array = l_array;
		EXPECT_EQ(strcmp(l_another_array[0], "A bottle of water"), 0);
		EXPECT_EQ(strcmp(l_another_array[1], "A bottle of water"), 0);
	}

	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array(2, "A bottle of water");
		FE::array<ASCII*> l_another_array;
=======
		FE::array<character*> l_array(2, "A bottle of water");
		FE::array<character*> l_another_array;
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		l_another_array = std::move(l_array);
		EXPECT_EQ(strcmp(l_another_array[0], "A bottle of water"), 0);
		EXPECT_EQ(strcmp(l_another_array[1], "A bottle of water"), 0);
	}

	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array;
=======
		FE::array<character*> l_array;
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		l_array = { "A", "bottle", "of", "water" };
		EXPECT_EQ(strcmp(l_array[0], "A"), 0);
		EXPECT_EQ(strcmp(l_array[1], "bottle"), 0);
		EXPECT_EQ(strcmp(l_array[2], "of"), 0);
		EXPECT_EQ(strcmp(l_array[3], "water"), 0);
	}
}




TEST(array, TrivialElementAccess)
{
	using namespace FE;
	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array;
=======
		FE::array<character*> l_array;
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		l_array.reserve(2);
		l_array.assign(2, "A bottle of water");

		EXPECT_EQ(strcmp(l_array[0], "A bottle of water"), 0);
		EXPECT_EQ(strcmp(l_array[1], "A bottle of water"), 0);
	}

	{
<<<<<<< HEAD
		std::initializer_list<ASCII*> l_initializer_list = { "A", "bottle", "of", "water" };
		FE::array<ASCII*> l_array;
=======
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
		FE::array<character*> l_array;
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		l_array.reserve(8);
		l_array.assign(l_initializer_list.begin(), l_initializer_list.end());

		EXPECT_EQ(strcmp(l_array[0], "A"), 0);
		EXPECT_EQ(strcmp(l_array[1], "bottle"), 0);
		EXPECT_EQ(strcmp(l_array[2], "of"), 0);
		EXPECT_EQ(strcmp(l_array[3], "water"), 0);
	}

	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array;
=======
		FE::array<character*> l_array;
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		l_array.reserve(8);
		l_array.assign({ "A", "bottle", "of", "water" });

		EXPECT_EQ(strcmp(l_array[0], "A"), 0);
		EXPECT_EQ(strcmp(l_array[1], "bottle"), 0);
		EXPECT_EQ(strcmp(l_array[2], "of"), 0);
		EXPECT_EQ(strcmp(l_array[3], "water"), 0);

	}

}




TEST(array, TrivialIterators)
{
	using namespace FE;
	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array = { "A", "bottle", "of", "water" };
		std::initializer_list<ASCII*> l_initializer_list = { "A", "bottle", "of", "water" };
=======
		FE::array<character*> l_array = { "A", "bottle", "of", "water" };
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto value : l_array)
		{
			EXPECT_EQ(strcmp(value, *l_l_initializer_list_iterator), 0);
			++l_l_initializer_list_iterator;
		}
	}


	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array = { "A", "bottle", "of", "water" };
		std::initializer_list<ASCII*> l_initializer_list = { "A", "bottle", "of", "water" };
=======
		FE::array<character*> l_array = { "A", "bottle", "of", "water" };
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.cbegin(); it != l_array.cend(); ++it)
		{
			EXPECT_EQ(strcmp(*it, *l_l_initializer_list_iterator), 0);
			++l_l_initializer_list_iterator;
		}
	}


	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array = { "water", "of", "bottle", "A" };
		std::initializer_list<ASCII*> l_initializer_list = { "A", "bottle", "of", "water" };
=======
		FE::array<character*> l_array = { "water", "of", "bottle", "A" };
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.rbegin(); it != l_array.rend(); ++it)
		{
			EXPECT_EQ(strcmp(*it, *l_l_initializer_list_iterator), 0);
			++l_l_initializer_list_iterator;
		}
	}


	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array = { "water", "of", "bottle", "A" };
		std::initializer_list<ASCII*> l_initializer_list = { "A", "bottle", "of", "water" };
=======
		FE::array<character*> l_array = { "water", "of", "bottle", "A" };
		std::initializer_list<character*> l_initializer_list = { "A", "bottle", "of", "water" };
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		auto l_l_initializer_list_iterator = l_initializer_list.begin();

		for (auto it = l_array.crbegin(); it != l_array.crend(); ++it)
		{
			EXPECT_EQ(strcmp(*it, *l_l_initializer_list_iterator), 0);
			++l_l_initializer_list_iterator;
		}
	}
}




TEST(array, TrivialCapacity)
{
	using namespace FE;
	{
<<<<<<< HEAD
		FE::array<ASCII*> l_array = { "water", "of", "bottle", "A" };
=======
		FE::array<character*> l_array = { "water", "of", "bottle", "A" };
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
		EXPECT_FALSE(l_array.is_empty());

		EXPECT_EQ(l_array.size(), 4);

		l_array.reserve(10);
		EXPECT_EQ(l_array.capacity(), 10);

		l_array.extend(6);
		EXPECT_EQ(l_array.capacity(), 16);

		l_array.shrink_to_fit();
		EXPECT_EQ(l_array.capacity(), 4);
	}
}




TEST(array, TrivialModifiers)
{
	using namespace FE;
	{
		{
<<<<<<< HEAD
			FE::array<ASCII*> l_another_array = { "Christmas", "Tree" };
=======
			FE::array<character*> l_another_array = { "Christmas", "Tree" };
>>>>>>> 19ea598051b1a13a8ae6b12b0447f686f156f948
			l_another_array.extend(1);
			l_another_array.emplace_back("s");
			EXPECT_EQ(strcmp(l_another_array.back(), "s"), 0);
		}


	}
}