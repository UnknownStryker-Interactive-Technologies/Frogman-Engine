//#include <gtest/gtest.h>
//#include <benchmark/benchmark.h>
//
//#include <FE/function.hxx>
//#include <FE/framework/reflection/reflection.h>
//
//// std
//#include <cstring>
//
//#include <glm/vec3.hpp>
//
//
//
//
//BEGIN_NAMESPACE(test)
//
//
//class super
//{
//	REGISTER_FE_CLASS(test::super);
//protected:
//	REGISTER_FE_PROPERTY(m_position);
//	glm::dvec3 m_position;
//
//public:
//	super() noexcept = default;
//	~super() noexcept = default;
//};
//
//class object /*: public super*/
//{ 
//	//FE_CLASS_HAS_A_BASE(test::super);
//	REGISTER_FE_CLASS(test::object);
//
//	REGISTER_FE_PROPERTY(m_is_it_dead);
//	_FE_MAYBE_UNUSED_ var::boolean m_is_it_dead; // a trivial plain old data
//
//	REGISTER_FE_PROPERTY(m_name);
//	std::string m_name;
//
//public:
//	object() noexcept : m_is_it_dead(false) {}
//	object(FE::boolean set_p, const std::string& name_p) noexcept : m_is_it_dead(set_p), m_name(name_p) {}
//
//	REGISTER_FE_METHOD(test::object, initialize, std::string (void));
//	std::string initialize() { return "object initialized"; }
//
//	REGISTER_FE_METHOD(test::object, do_as_what_i_say, std::string(FE::length_t));
//	std::string do_as_what_i_say(FE::length_t guess_its_size_p) noexcept
//	{
//		std::string s = "unsafe operations";
//		if (s.size() != guess_its_size_p)
//		{
//			s = "you've failed to guess.";
//		}
//		return s; 
//	}
//
//	FE::boolean is_dead() const noexcept { return this->m_is_it_dead; }
//	std::string& get_name() noexcept { return this->m_name; }
//};
//
//END_NAMESPACE
//
//
//
//
//TEST(reflection, method)
//{
//	test::object l_object(true, "John doe");
//	std::any l_result = FE::framework::reflection::function::invoke(l_object, "std::string test::object::initialize(void)");
//	EXPECT_TRUE(l_result.has_value());
//	{
//		std::string l_actual_result = std::any_cast<std::string>(l_result);
//		EXPECT_TRUE(l_actual_result == "object initialized");
//	}
//	
//
//	FE::arguments<var::length_t> l_args;
//	l_args._first = 16;
//	
//	l_result = ::FE::framework::reflection::function::invoke(l_object, l_object.method_reflection_instance_do_as_what_i_say.get_signature(), std::move(l_args));
//	EXPECT_TRUE(l_result.has_value());
//	{
//		std::string l_actual_result = std::any_cast<std::string>(l_result);
//		EXPECT_TRUE(l_actual_result == "you've failed to guess.");
//	}
//}
//
//
//TEST(reflectiom, property)
//{
//	test::object l_object(true, "John doe");
//	FE::framework::reflection::property::serialize<test::object>(std::filesystem::current_path() / FE_TEXT(serialized), FE_TEXT(test - object.froggy), l_object);
//
//	test::object l_new_object;
//	FE::framework::reflection::property::deserialize<test::object>(std::filesystem::current_path() / FE_TEXT(serialized), FE_TEXT(test - object.froggy), l_new_object);
//
//	EXPECT_EQ(l_new_object.get_name(), l_object.get_name());
//	EXPECT_EQ(l_new_object.is_dead(), l_object.is_dead());
//}
//
//
//bool JesusLovesYou() noexcept
//{
//	return true;
//}
//
//// https://learn.microsoft.com/en-us/cpp/build/reference/gf-eliminate-duplicate-strings?view=msvc-170
//TEST(reflection, function)
//{
//	FE::string l_function_signature = "bool JesusLovesYou() noexcept";
//	FE::framework::reflection::function::register_task<FE::c_style_task<bool(void)>>(l_function_signature, &JesusLovesYou);
//	EXPECT_TRUE(FE::framework::reflection::function::check_presence(l_function_signature));
//
//	auto l_result = FE::framework::reflection::function::invoke(l_function_signature);
//
//	_FE_MAYBE_UNUSED_ FE::task_base* l_fn = FE::framework::reflection::function::retrieve(l_function_signature);
//
//	EXPECT_TRUE(std::any_cast<FE::boolean>(l_result));
//}
//
//
//void FE_function_invocation_overhead_benchmark(benchmark::State& state_p) noexcept
//{
//	FE::string l_function_signature = "bool JesusLovesYou() noexcept";
//	for(auto _ : state_p)
//	{
//		FE::framework::reflection::function::invoke(l_function_signature);
//	}
//}
//BENCHMARK(FE_function_invocation_overhead_benchmark);
