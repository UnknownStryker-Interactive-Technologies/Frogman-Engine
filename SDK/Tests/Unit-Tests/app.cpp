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
	test_engine(FE::int32 argc_p, FE::tchar** argv_p) noexcept : FE::framework::framework_base(argc_p, argv_p)
	{ 
	};
	~test_engine() noexcept = default;

	virtual FE::int32 launch(FE::int32 argc_p, FE::tchar** argv_p) override
	{
		//__load_all_class_reflection_data_from_dll();
		var::int32 l_argc = argc_p;
		testing::InitGoogleTest(&l_argc, (var::tchar**)argv_p);
		if (argv_p == nullptr)
		{
			FE::tchar l_arg0_default[] = "benchmark";
			FE::tchar* l_args_default = l_arg0_default;
			l_argc = 1;
			argv_p = &l_args_default;
		}
		benchmark::Initialize(&l_argc, (var::tchar**)argv_p);

		FE_EXIT(benchmark::ReportUnrecognizedArguments(l_argc, (var::tchar**)argv_p) == true, -1, "Failed to meet the expectation: Unrecognized Benchmark Arguments Detected.");
		return 0;
	}

	virtual FE::int32 run() override
	{
		FE::int32 l_exit_code = RUN_ALL_TESTS();
		std::cerr << "\n\n";
		benchmark::RunSpecifiedBenchmarks();
		std::cerr << "\n\n";
		return l_exit_code;
	}

	virtual FE::int32 shutdown() override
	{
		benchmark::Shutdown();
		return 0;
	}
};
CUSTOM_ENGINE(test_engine);

//FROGMAN_ENGINE();