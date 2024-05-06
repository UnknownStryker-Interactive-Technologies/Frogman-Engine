// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <FE/core/prerequisites.h>
#include <FE/framework/framework.hpp>
#include <FE/framework/reflection/function_table.hpp>
#include <FE/core/block_pool.hxx>
#include <FE/miscellaneous/private/macro_restrictions.h>




//https://learn.microsoft.com/en-us/cpp/build/profile-guided-optimizations?view=msvc-170
// Profile Guided Optimization
class test final : public FE::framework::application
{
public:
	test(FE::size_t function_table_size_p)
	{
		FE::framework::function_table::reserve(function_table_size_p);
	}

private:
	virtual bool set_up(_MAYBE_UNUSED_ int argc_p, _MAYBE_UNUSED_ char** argv_p) override final
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
		FE_EXPECT(benchmark::ReportUnrecognizedArguments(argc_p, argv_p), false, "Expectation Failed: Unrecognized Benchmark Arguments Detected.");
		
		return _FE_SUCCESS_;
	}
	
	virtual int run(_MAYBE_UNUSED_ int argc_p, _MAYBE_UNUSED_ char** argv_p) override final
	{

		let int32 l_exit_code = RUN_ALL_TESTS();
		std::cerr << "\n\n";
		benchmark::RunSpecifiedBenchmarks();
		std::cerr << "\n\n";
		return l_exit_code;
	}

	virtual void clean_up() override final
	{
		benchmark::Shutdown();
	}

public:
	virtual ~test() override
	{

	}
};




FE::framework::application::initializer_t g_init = FE::framework::application::create_application
(
	[]()
	{
		constexpr FE::size_t l_function_table_size = 128;
		return _CREATE_AN_APP_ test(l_function_table_size);
	}
);
