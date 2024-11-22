// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/framework.hpp>

// google headers
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>


/* Linux tip:
ls: lists all folders in the current working directory
ls filename: checks the presence of files with the specified name.
*/


class test_engine : public FE::framework::framework_base
{
public:
	test_engine(_FE_MAYBE_UNUSED_ int argc_p, _FE_MAYBE_UNUSED_ FE::tchar** argv_p) noexcept : FE::framework::framework_base(argc_p, argv_p) 
	{ 
	};
	~test_engine() noexcept = default;

	virtual int launch(_FE_MAYBE_UNUSED_ int argc_p, _FE_MAYBE_UNUSED_ FE::tchar** argv_p) override
	{
		//const char** l_argv = (const char**)argv_p;
		testing::InitGoogleTest(&argc_p, (var::tchar**)argv_p);
		if (argv_p == nullptr)
		{
			FE::tchar l_arg0_default[] = "benchmark";
			FE::tchar* l_args_default = l_arg0_default;
			argc_p = 1;
			argv_p = &l_args_default;
		}
		benchmark::Initialize(&argc_p, (var::tchar**)argv_p);

		FE_EXIT(benchmark::ReportUnrecognizedArguments(argc_p, (var::tchar**)argv_p) == true, -1, "Failed to meet the expectation: Unrecognized Benchmark Arguments Detected.");
		return 0;
	}

	virtual int run() override
	{
		FE::int32 l_exit_code = RUN_ALL_TESTS();
		std::cerr << "\n\n";
		benchmark::RunSpecifiedBenchmarks();
		std::cerr << "\n\n";
		return l_exit_code;
	}

	virtual int shutdown() override
	{
		benchmark::Shutdown();
		return 0;
	}
};
CUSTOM_ENGINE(test_engine);

//FROGMAN_ENGINE();