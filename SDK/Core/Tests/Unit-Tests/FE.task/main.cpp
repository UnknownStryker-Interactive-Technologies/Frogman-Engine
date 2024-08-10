#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/function.hxx>
//#include <FE/experimental/task_queue.hxx>
using namespace FE;

#ifdef _LINUX_X86_64_
#include <sched.h>
#endif

// std
#include <cmath>
#include <string>
#include <vector>

/*
ls: lists all folders in the current working directory
ls filename: checks the presence of files with the specified name.
*/

class tls
{
public:
    tls()
    {
        std::cerr << "tls constructed\n";
    }

    ~tls()
    {
        std::cerr << "tls destructed\n";
    }
};

thread_local static tls tl_s_tls;

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




namespace test_sample
{
	var::float64 abs(float64& value_ref_p)
	{
		return ::abs(value_ref_p);
	}

	var::float64 abs(float64* value_ptr_p)
	{
		return ::abs(*value_ptr_p);
	}

	struct s
	{
		int32 fn(int32 p)
		{
			return p;
		}

		int32 fn(int32& ref_p)
		{
			return ref_p;
		}

		int32 fn(int32* ptr_p)
		{
			return *ptr_p;
		}
	};
}


void void_fn(int32 param_p) noexcept
{
	EXPECT_EQ(param_p, 0);
}

int32 int_fn(int32 param_p) noexcept
{
	EXPECT_EQ(param_p, 0);
	return param_p;
}


TEST(function, call_by_value)
{
	FE::function<long double(long double)> l_abs = abs;
	float64 l_input = -7.0;
	long double l_result = l_abs(l_input);
	EXPECT_EQ(l_result, 7.0f);
}

TEST(function, call_by_lvalue_reference)
{
	FE::function<var::float64(float64&)> l_abs = test_sample::abs;
	var::float64 l_input = -5.0;
	float64 l_result = l_abs(l_input);
	EXPECT_EQ(l_result, 5.0f);
}

TEST(function, call_by_address)
{
	FE::function<var::float64(float64*)> l_abs = test_sample::abs;
	var::float64 l_input = -5.0;
	float64 l_result = l_abs(&l_input);
	EXPECT_EQ(l_result, 5.0f);
}




TEST(method, call_by_value)
{
	FE::method<test_sample::s, int32(int32)> l_fn = &test_sample::s::fn;
	var::int32 l_input = 5;
	test_sample::s l_s;
	int32 l_result = l_fn(l_s, std::move(l_input));
	EXPECT_EQ(l_result, l_input);
}

TEST(method, call_by_lvalue_reference)
{
	FE::method<test_sample::s, int32(int32&)> l_fn = &test_sample::s::fn;
	int32 l_input = 5;
	test_sample::s l_s;
	int32 l_result = l_fn(l_s, l_input);
	EXPECT_EQ(l_result, l_input);
}

TEST(method, call_by_address)
{
	FE::method<test_sample::s, int32(int32*)> l_fn = &test_sample::s::fn;
	var::int32 l_input = 5;
	test_sample::s l_s;
	int32 l_result = l_fn(l_s, &l_input);
	EXPECT_EQ(l_result, l_input);
}




TEST(c_style_task, call_by_value)
{
	FE::c_style_task<long double(long double)> l_abs;
	l_abs._function = abs;

	FE::arguments<long double> l_arg0;
	l_arg0._first = -9.0f;

	std::any l_result = l_abs(&l_arg0);

	EXPECT_EQ( std::any_cast<long double>(l_result) , 9.0);
}

TEST(c_style_task, call_by_lvalue_reference)
{
	FE::c_style_task<var::float64(float64&)> l_abs;
	l_abs._function = test_sample::abs;

	FE::arguments<var::float64> l_arg0;
	l_arg0._first = -9.0f;
	std::any l_result = l_abs(&l_arg0);

	EXPECT_EQ( std::any_cast<var::float64>(l_result) , 9.0);
}

TEST(c_style_task, call_by_address)
{
	FE::c_style_task<var::float64(float64*)> l_abs;
	l_abs._function = test_sample::abs;

	FE::arguments<float64*> l_p;
	l_p._first = new float64(-9.0f);
	std::any l_result = l_abs(&l_p);
	delete l_p._first;

	EXPECT_EQ( std::any_cast<var::float64>(l_result) , 9.0);
}




TEST(cpp_style_task, call_by_value)
{
	FE::cpp_style_task<std::string, std::string&(const char*)> l_assign;
	std::string l_some_str;

	l_assign._method = &std::string::assign;
	l_assign.set_instance(&l_some_str);

	FE::arguments<const char*> l_arg0;
	l_arg0._first = "std::string";

	std::any l_result = l_assign(&l_arg0);

	EXPECT_TRUE(std::any_cast<std::string>(l_result) == "std::string");
}

TEST(cpp_style_task, call_by_lvalue_reference)
{
	FE::cpp_style_task<std::string, std::string&(const std::string&)> l_copy_assignment;
	std::string l_some_str;
	l_copy_assignment.set_instance(&l_some_str);
	l_copy_assignment._method = &std::string::operator=;

	FE::arguments<std::string> l_arg0; 
	l_arg0._first = "C++ is not Java.";
	std::any l_result = l_copy_assignment(&l_arg0);

	EXPECT_TRUE(std::any_cast<std::string>(l_result) == "C++ is not Java.");
}

TEST(cpp_style_task, call_by_address)
{
	FE::cpp_style_task<test_sample::s, int32(int32*)> l_fn;
	test_sample::s l_test_sample_instance;
	l_fn.set_instance(&l_test_sample_instance);
	l_fn._method = &test_sample::s::fn;

	FE::arguments<int32*> l_p;
	l_p._first = new int32(10);
	std::any l_result = l_fn(&l_p);
	delete l_p._first;

	EXPECT_EQ(std::any_cast<int32>(l_result), 10);
}


template<class Container>
void iterate(const Container& container_p) noexcept
{
	for(auto& value : container_p)
	{
		std::cout << value << ' ';
	}
	std::cout << '\n';
}
/*
TEST(task_queue, _)
{
	FE::arguments<const std::vector<int>> l_vector_iteration_arg(std::vector<int>{0, 1, 2, 3, 4, 5});
	FE::c_style_task<void(const std::vector<int>&)> l_vector_iteration(&iterate<std::vector<int>>);

	FE::arguments<const std::string> l_string_iteration_arg("std::basic_string");
	FE::c_style_task<void(const std::string&)> l_string_iteration(&iterate<std::string>);

	FE::experimental::task_queue<8> l_tasks;
	l_tasks.push(l_vector_iteration, l_vector_iteration_arg);
	l_tasks.push(l_string_iteration, l_string_iteration_arg);
	
	l_tasks.execute_front();
	l_tasks.pop();
	
	l_tasks.execute_front();
	l_tasks.pop();
}*/


namespace performance_benchmark
{
	void fn() noexcept
	{
/*
Severity	Code	Description	Project	File	Line	Suppression State	Details
Error		variable 'l_uint' set but not used [-Werror,-Wunused-but-set-variable]	FE.core.thread_test	C:\Users\leeho\OneDrive\臾몄꽌\GitHub\Frogman-Engine-Lab\Frogman-Engine-Tests\FE-Tests\Unit-Tests\FE.core.thread\main.cpp	291
*/
		_MAYBE_UNUSED_ static var::uint64 l_uint = 0;

		++l_uint;
	}

	void FE_function(benchmark::State& state_p) noexcept
	{
		FE::function<void(void)> l_fn = fn;

		for (_MAYBE_UNUSED_ auto _ : state_p)
		{
			l_fn();
		}
	}
	BENCHMARK(FE_function);


	void FE_polymorphic_generic_task(benchmark::State& state_p) noexcept
	{
		FE::c_style_task<void(void)> l_task = fn;

		for (_MAYBE_UNUSED_ auto _ : state_p)
		{
			l_task(nullptr);
		}
	}
	BENCHMARK(FE_polymorphic_generic_task);


	void std_function(benchmark::State& state_p) noexcept
	{
		std::function l_fn = fn;

		for (_MAYBE_UNUSED_ auto _ : state_p)
		{
			l_fn();
		}
	}
	BENCHMARK(std_function);
}