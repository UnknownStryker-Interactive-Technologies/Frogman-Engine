// Copyright © from 2023 to current, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.
//#include <FE/framework.hxx>
#include <FE/framework.hxx>

// google headers
#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include "register_concurrent_benchmarks.h"

/* Linux tip:
ls: lists all folders in the current working directory
ls filename: checks the presence of files with the specified name.
*/


class test_engine : public FE::framework::framework_base
{
	bool m_should_skip_gtests = false;

public:
	test_engine(std::unique_ptr<FE::framework::program_option> options_p) noexcept : FE::framework::framework_base(std::move(options_p))
	{
	};
	~test_engine() noexcept = default;

	virtual FE::int32 launch(FE::int32 argc_p, FE::ASCII** argv_p) override
	{
		register_concurrent_benchmarks();

		var::int32 l_argc = argc_p;
		testing::InitGoogleTest(&l_argc, (var::ASCII**)argv_p);
		if (argv_p == nullptr)
		{
			FE::ASCII l_arg0_default[] = "benchmark";
			FE::ASCII* l_args_default = l_arg0_default;
			l_argc = 1;
			argv_p = &l_args_default;
		}
		benchmark::Initialize(&l_argc, (var::ASCII**)argv_p);

		//FE_EXIT_IF(benchmark::ReportUnrecognizedArguments(l_argc, (var::ASCII**)argv_p) == true, -1, "Failed to meet the expectation: Unrecognized Benchmark Arguments Detected.");
		
		__load_reflection_data();

		for (var::int32 i = 0; i < l_argc; ++i)
		{
			if (strcmp(argv_p[i], "--skip-gtests") == 0)
			{
				m_should_skip_gtests = true;
				break;
			}
		}
		return 0;
	}

	virtual FE::int32 run() override
	{
		var::int32 l_exit_code = 0;
		if (m_should_skip_gtests == false)
		{
			l_exit_code = RUN_ALL_TESTS();
			std::cerr << "\n\n";
		}
		else
		{
			std::cerr << "Skipping Google Tests...\n\n";
		}
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
CUSTOM_ENGINE(test_engine, FE::framework::program_option);
