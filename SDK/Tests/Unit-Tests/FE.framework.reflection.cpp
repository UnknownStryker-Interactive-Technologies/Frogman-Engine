#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <FE/function.hxx>
#include <FE/framework/reflection.hpp>

#include <FE/framework/super_object_base.hpp>

// std
#include <string>


#include <glm/vec3.hpp>




struct plain_old_data
{
	FE_CLASS(plain_old_data);

	FE_PROPERTY(_a);
	var::int32 _a;

	FE_PROPERTY(_b);
	var::float32 _b;

	FE_PROPERTY(_c);
	var::uint16 _c;
};

struct pod_with_array
{
	var::int32 _a;
	var::float32 _b;
	var::uint16 _c;
	var::uint32 _d[10];
};

struct object_with_string
{
	FE_CLASS(object_with_string);

	FE_PROPERTY(_a);
	std::string _a;
};

struct object_with_vector
{
	FE_CLASS(object_with_vector);

	FE_PROPERTY(_a);
	std::vector<std::string> _a;
};

class object : public object_with_vector
{
	FE_CLASS_HAS_A_BASE(object_with_vector);
	FE_CLASS(object);
	
	FE_PROPERTY(m_text);
	std::string m_text;
public:
	object() {};
	object(const std::string& text_p) : m_text(text_p) {/* std::cerr << method_reflection_instance_get_text.get_signature();*/ }

	FE_METHOD(get_text, FE::ASCII* (void) const);
	FE::ASCII* get_text() const
	{
		return this->m_text.c_str();
	}
	
	FE_STATIC_METHOD(greet, FE::ASCII*(void));
	static FE::ASCII* greet()
	{
		return "Hello, reflection system!";
	}
};

TEST(reflection, POD_serialization)
{
	plain_old_data l_pod;
	l_pod._a = 1;
	l_pod._b = 2.0f;
	l_pod._c = 3;

	FE::framework::framework_base::get_engine().get_property_reflection().serialize(std::filesystem::current_path(), FE_TEXT(serialized_pod.bin), l_pod);

	plain_old_data l_new_pod;
	FE::framework::framework_base::get_engine().get_property_reflection().deserialize(std::filesystem::current_path(), FE_TEXT(serialized_pod.bin), l_new_pod);

	EXPECT_EQ(l_pod._a, l_new_pod._a);
	EXPECT_EQ(l_pod._b, l_new_pod._b);
	EXPECT_EQ(l_pod._c, l_new_pod._c);
}

TEST(reflection, object_with_string_serialization)
{
	object_with_string l_str;
	l_str._a = "Hello World";

	FE::framework::framework_base::get_engine().get_property_reflection().serialize(std::filesystem::current_path(), FE_TEXT(serialized_strings.bin), l_str);

	object_with_string l_new_str;
	FE::framework::framework_base::get_engine().get_property_reflection().deserialize(std::filesystem::current_path(), FE_TEXT(serialized_strings.bin), l_new_str);

	EXPECT_STREQ(l_str._a.data(), l_new_str._a.data());
}

TEST(reflection, object_with_vector_serialization)
{
	object_with_vector l_strs;
	l_strs._a.push_back("Hello World");
	l_strs._a.push_back("Bye World");

	FE::framework::framework_base::get_engine().get_property_reflection().serialize(std::filesystem::current_path(), FE_TEXT(serialized_strings.bin), l_strs);

	object_with_vector l_new_strs;
	FE::framework::framework_base::get_engine().get_property_reflection().deserialize(std::filesystem::current_path(), FE_TEXT(serialized_strings.bin), l_new_strs);

	EXPECT_STREQ(l_strs._a[0].data(), l_new_strs._a[0].data());
	EXPECT_STREQ(l_strs._a[1].data(), l_new_strs._a[1].data());
}

TEST(reflection, method_call)
{
	object l_object("Jesus Loves You!");
	//std::cout << l_object.get_text_method_meta.get_signature() << std::endl;
	auto l_function_pointer = FE::framework::framework_base::get_engine().get_method_reflection().retrieve("FE::ASCII* class object::get_text(void) const");
	FE::ASCII* l_msg;
	(*l_function_pointer)(&l_object, &l_msg, nullptr);
	EXPECT_STREQ(l_msg, "Jesus Loves You!");

	auto l_greeter = FE::framework::framework_base::get_engine().get_method_reflection().retrieve("FE::ASCII* class object::greet(void)");
	(*l_greeter)(&l_msg, nullptr);
	EXPECT_STREQ(l_msg, "Hello, reflection system!");
}