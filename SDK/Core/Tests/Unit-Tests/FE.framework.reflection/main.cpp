#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <FE/function.hxx>
#include <FE/framework/reflection/reflection.h>
#include <FE/framework/framework.hpp>

// std
#include <cstring>


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

class framework_test : public FE::framework::application_base
{
	virtual bool set_up(int argc_p, char** argv_p) override
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
		return true;
	}

	virtual int run(_MAYBE_UNUSED_ int argc_p, _MAYBE_UNUSED_ char** argv_p) override
	{
		FE::int32 l_exit_code = RUN_ALL_TESTS();
		std::cerr << "\n\n";
		benchmark::RunSpecifiedBenchmarks();
		std::cerr << "\n\n";
		return l_exit_code;
	}

	virtual void clean_up() override
	{
    	benchmark::Shutdown();
	}
};

CONFIGURE_A_FROGMAN_ENGINE_PROJECT()

return new framework_test;

END_OF_THE_FROGMAN_ENGINE_PROJECT_CONFIGURATION()




BEGIN_NAMESPACE(test)

struct vector3
{
	REGISTER_FE_CLASS(test::vector3);

	REGISTER_FE_PROPERTY(_x);
	var::float32 _x = 0.0f;

	REGISTER_FE_PROPERTY(_y);
	var::float32 _y = 0.0f;

	REGISTER_FE_PROPERTY(_z);
	var::float32 _z = 0.0f;
};

struct RGBA
{
	var::float32 _red;
	var::float32 _green;
	var::float32 _blue;
	var::float32 _alpha;
};

class super
{
	REGISTER_FE_CLASS(test::super);
protected:
	REGISTER_FE_PROPERTY(m_position);
	vector3 m_position;

public:
	super() noexcept = default;
	~super() noexcept = default;
};

class object : public super
{ 
	FE_CLASS_HAS_A_BASE(test::super);
	REGISTER_FE_CLASS(test::object);

	REGISTER_FE_PROPERTY(m_is_it_dead); 
	_MAYBE_UNUSED_ var::boolean m_is_it_dead; // a trivial plain old data

	REGISTER_FE_PROPERTY(m_colors);
	_MAYBE_UNUSED_ test::RGBA m_colors[2]; // a trivial plain old data array

public:
	object() noexcept : m_is_it_dead(false), m_colors{} {}
	object(FE::boolean dead_on_start_p, test::RGBA rgba_p) noexcept : m_is_it_dead{dead_on_start_p}, m_colors{rgba_p} {}

	REGISTER_FE_METHOD(test::object, initialize, std::string (void));
	std::string initialize() { return "object initialized"; }

	REGISTER_FE_METHOD(test::object, do_as_what_i_say, std::string(FE::length_t));
	std::string do_as_what_i_say(FE::length_t guess_its_size_p) noexcept
	{
		std::string s = "unsafe operations";
		if (s.size() != guess_its_size_p)
		{
			s = "you've failed to guess.";
		}
		return s; 
	}

	const test::RGBA* get_color() const noexcept { return this->m_colors; }
	FE::boolean is_dead() const noexcept { return this->m_is_it_dead; }
};
END_NAMESPACE


TEST(reflection, method_and_property)
{
	test::object l_object(true, test::RGBA{255.0f, 191.0f, 52.0f, 1.0f});
	std::any l_result = FE::framework::reflection::function::invoke(l_object, "std::string test::object::initialize(void)");
	EXPECT_TRUE(l_result.has_value());
	{
		std::string l_actual_result = std::any_cast<std::string>(l_result);
		EXPECT_TRUE(l_actual_result == "object initialized");
	}
	

	FE::arguments<var::length_t> l_args;
	l_args._first = 16;
	
	l_result = ::FE::framework::reflection::function::invoke(l_object, l_object.method_reflection_instance_do_as_what_i_say.get_signature(), std::move(l_args));
	EXPECT_TRUE(l_result.has_value());
	{
		std::string l_actual_result = std::any_cast<std::string>(l_result);
		EXPECT_TRUE(l_actual_result == "you've failed to guess.");
	}


	FE::framework::reflection::property::serialize<test::object>("Frogman-Engine-Tests/FE-Tests/Unit-Tests/FE.framework.reflection/serialized", "test-object.froggy", l_object);

	test::object l_new_object;
	FE::framework::reflection::property::deserialize<test::object>("Frogman-Engine-Tests/FE-Tests/Unit-Tests/FE.framework.reflection/serialized", "test-object.froggy", l_new_object);

	
	EXPECT_EQ(std::memcmp(l_new_object.get_color(), l_object.get_color(), sizeof(test::RGBA) * 2), 0);

	EXPECT_EQ(l_new_object.is_dead(), l_object.is_dead());
}




bool JesusLovesYou() noexcept
{
	return true;
}

// https://learn.microsoft.com/en-us/cpp/build/reference/gf-eliminate-duplicate-strings?view=msvc-170
TEST(reflection, function)
{
	FE::string l_function_signature = "bool JesusLovesYou() noexcept";
	FE::framework::reflection::function::register_task<FE::c_style_task<bool(void)>>(l_function_signature, &JesusLovesYou);
	EXPECT_TRUE(FE::framework::reflection::function::check_presence(l_function_signature));

	auto l_result = FE::framework::reflection::function::invoke(l_function_signature);

	_MAYBE_UNUSED_ FE::task_base* l_fn = FE::framework::reflection::function::retrieve(l_function_signature);

	EXPECT_TRUE(std::any_cast<FE::boolean>(l_result));
}


void FE_function_invocation_overhead_benchmark(benchmark::State& state_p) noexcept
{
	FE::string l_function_signature = "bool JesusLovesYou() noexcept";
	for(auto _ : state_p)
	{
		FE::framework::reflection::function::invoke(l_function_signature);
	}
}
BENCHMARK(FE_function_invocation_overhead_benchmark);
