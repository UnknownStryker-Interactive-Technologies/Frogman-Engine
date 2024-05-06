#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/thread.hpp>
#include <FE/core/function.hxx>
#include <cmath>
using namespace FE;
#pragma warning(disable: 4244)



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

TEST(thread, fork_and_join)
{
	FE::thread l_thread;
	FE::c_style_task<void(int32), FE::FORWARD_DATA::_AS_RVALUE_REF, var::int32> l_void_fn;
	l_void_fn._function = void_fn;
	l_void_fn._arguments_buffer._first = 0;

	l_thread.fork(&l_void_fn);
	l_thread.join();




	FE::c_style_task<int32(int32), FE::FORWARD_DATA::_AS_RVALUE_REF, var::int32> l_int_fn;
	l_int_fn._function = int_fn;
	l_int_fn._arguments_buffer._first = 0;

	l_thread.fork(&l_int_fn);
	l_thread.join();

	EXPECT_EQ(l_int_fn._task_result, 0);
}




TEST(function, call_by_value)
{
	FE::function<long double(long double), FE::FORWARD_DATA::_AS_RVALUE_REF> l_abs = abs;
	float64 l_input = -7.0;
	float64 l_result = l_abs(l_input);
	EXPECT_EQ(l_result, 7.0f);
}

TEST(function, call_by_lvalue_reference)
{
	FE::function<var::float64(float64&), FE::FORWARD_DATA::_AS_LVALUE_REF> l_abs = test_sample::abs;
	var::float64 l_input = -5.0;
	float64 l_result = l_abs(l_input);
	EXPECT_EQ(l_result, 5.0f);
}

TEST(function, call_by_address)
{
	FE::function<var::float64(float64*), FE::FORWARD_DATA::_AS_RVALUE_REF> l_abs = test_sample::abs;
	var::float64 l_input = -5.0;
	float64 l_result = l_abs(&l_input);
	EXPECT_EQ(l_result, 5.0f);
}




TEST(method, call_by_value)
{
	FE::method<test_sample::s, int32(int32), FE::FORWARD_DATA::_AS_RVALUE_REF> l_fn = &test_sample::s::fn;
	var::int32 l_input = 5;
	test_sample::s l_s;
	int32 l_result = l_fn(l_s, std::move(l_input));
	EXPECT_EQ(l_result, l_input);
}

TEST(method, call_by_lvalue_reference)
{
	FE::method<test_sample::s, int32(int32&), FE::FORWARD_DATA::_AS_LVALUE_REF> l_fn = &test_sample::s::fn;
	int32 l_input = 5;
	test_sample::s l_s;
	int32 l_result = l_fn(l_s, l_input);
	EXPECT_EQ(l_result, l_input);
}

TEST(method, call_by_address)
{
	FE::method<test_sample::s, int32(int32*), FE::FORWARD_DATA::_AS_RVALUE_REF> l_fn = &test_sample::s::fn;
	var::int32 l_input = 5;
	test_sample::s l_s;
	int32 l_result = l_fn(l_s, &l_input);
	EXPECT_EQ(l_result, l_input);
}




TEST(c_style_task, call_by_value)
{
	FE::c_style_task<long double(long double), FE::FORWARD_DATA::_AS_RVALUE_REF, long double> l_abs;
	l_abs._function = abs;
	l_abs._arguments_buffer._first = -9.0f;

	l_abs();

	EXPECT_EQ(l_abs._task_result, 9.0);
}

TEST(c_style_task, call_by_lvalue_reference)
{
	FE::c_style_task<var::float64(float64&), FE::FORWARD_DATA::_AS_RVALUE_REF, var::float64> l_abs;
	l_abs._function = test_sample::abs;
	l_abs._arguments_buffer._first = -9.0f;

	l_abs();

	EXPECT_EQ(l_abs._task_result, 9.0);
}

TEST(c_style_task, call_by_address)
{
	FE::c_style_task<var::float64(float64*), FE::FORWARD_DATA::_AS_RVALUE_REF, var::float64*> l_abs;
	l_abs._function = test_sample::abs;
	l_abs._arguments_buffer._first = &FE::buffer<var::float64>::set_and_get(-9.0f);

	l_abs();

	EXPECT_EQ(l_abs._task_result, 9.0);
}




TEST(cpp_style_task, call_by_value)
{
	std::string l_some_str;

	FE::cpp_style_task<std::string, std::string&(const char*), FE::FORWARD_DATA::_AS_RVALUE_REF, const char*> l_reserve;
	l_reserve._arguments_buffer._first = "std::string";
	l_reserve._method = &std::string::assign;
	l_reserve._instance_ptr = &l_some_str;

	l_reserve();
	EXPECT_TRUE(l_reserve._task_result == "std::string");
}

TEST(cpp_style_task, call_by_lvalue_reference)
{
	std::string l_some_str;

	FE::cpp_style_task<std::string, std::string&(const std::string&), FE::FORWARD_DATA::_AS_LVALUE_REF, std::string> l_copy_assignment;
	l_copy_assignment._arguments_buffer._first = "C++ is not Java.";
	l_copy_assignment._method = &std::string::operator=;
	l_copy_assignment._instance_ptr = &l_some_str;

	l_copy_assignment();

	EXPECT_TRUE(l_copy_assignment._task_result == "C++ is not Java.");
}

TEST(cpp_style_task, call_by_address)
{
	test_sample::s l_test_sample_instance;
	FE::cpp_style_task<test_sample::s, int32(int32*), FE::FORWARD_DATA::_AS_RVALUE_REF, int32*> l_fn;

	int32 l_input = 10;
	l_fn._arguments_buffer._first = &l_input;
	l_fn._instance_ptr = &l_test_sample_instance;
	l_fn._method = &test_sample::s::fn;

	l_fn();

	EXPECT_EQ(l_fn._task_result, 10);
}




namespace performance_benchmark
{
	void fn() noexcept
	{
		static let var::uint32 l_uint32 = 0;
		benchmark::DoNotOptimize(l_uint32);
		++l_uint32;
	}

	void FE_function(benchmark::State& state_p) noexcept
	{
		FE::function<void(void)> l_fn = fn;
		benchmark::DoNotOptimize(l_fn);
		for (auto _ : state_p)
		{
			l_fn();
		}
	}
	BENCHMARK(FE_function);


	void FE_polymorphic_generic_task(benchmark::State& state_p) noexcept
	{
		FE::c_style_task<void(void)> l_task = fn;
		benchmark::DoNotOptimize(l_task);
		for (auto _ : state_p)
		{
			l_task();
		}
	}
	BENCHMARK(FE_polymorphic_generic_task);


	void std_function(benchmark::State& state_p) noexcept
	{
		std::function l_fn = fn;
		benchmark::DoNotOptimize(l_fn);
		for (auto _ : state_p)
		{
			l_fn();
		}
	}
	BENCHMARK(std_function);
}