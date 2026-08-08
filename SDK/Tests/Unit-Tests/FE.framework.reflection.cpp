#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <FE/function.hxx>
#include <FE/framework/reflection.hxx>

#include <FE/framework.hxx>

// std
#include <string>

#include <glm/vec3.hpp>




template<typename T>
class generic_base
{
	FE_CLASS();

private:
	FE_PROPERTY(m_value);
	T m_value;

public:
	generic_base() {};
	~generic_base() = default;
	generic_base(const T& value_p) : m_value(value_p) {}

	FE_METHOD(get_value, T(void) const);
	inline T get_value() const
	{
		return this->m_value;
	}
};

class child_of_generic_base : public generic_base<int>
{
	FE_CLASS_HAS_A_BASE(generic_base<int>);
	FE_CLASS();

private:
	FE_PROPERTY(m_child_value);
	var::int32 m_child_value;

public:
	child_of_generic_base() : base_type() {};
	~child_of_generic_base() = default;
	child_of_generic_base(const int& value_p, const int& child_value_p) : generic_base<int>(value_p), m_child_value(child_value_p) {}
	
	FE_METHOD(get_child_value, int(void) const);
	inline int get_child_value() const
	{
		return this->m_child_value;
	}
};

struct plain_old_data
{
	FE_STRUCT();

	FE_PROPERTY(_a);
	var::int32 _a;

	FE_PROPERTY(_b);
	var::float32 _b;

	FE_PROPERTY(_c);
	var::uint16 _c;
};

struct pod_with_array
{
	FE_STRUCT();

	FE_PROPERTY(_a);
	var::int32 _a;

	FE_PROPERTY(_b);
	var::float32 _b;

	FE_PROPERTY(_c);
	var::uint16 _c;

	FE_PROPERTY(_d);
	var::uint32 _d[10];
};
//static_assert(FE::is_trivial_v<pod_with_array> == true);

struct object_with_string
{
	FE_STRUCT();

	FE_PROPERTY(_a);
	std::string _a;
};

struct object_with_vector
{
	FE_STRUCT();

	FE_PROPERTY(_a);
	std::vector<std::string> _a;

	FE_PROPERTY(_b);
	std::vector<std::vector<std::vector<pod_with_array>>> _b;
};

class object : public object_with_vector
{
	FE_CLASS_HAS_A_BASE(object_with_vector);
	FE_CLASS();
	
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

	std::pmr::string l_serialized_pod;
	FE::framework::framework_base::get_framework().get_property_reflection().serialize(l_serialized_pod, l_pod, "v0.0.0");

	plain_old_data l_new_pod;
	FE::framework::framework_base::get_framework().get_property_reflection().deserialize(l_serialized_pod, l_new_pod, "v0.0.0");

	EXPECT_EQ(l_pod._a, l_new_pod._a);
	EXPECT_EQ(l_pod._b, l_new_pod._b);
	EXPECT_EQ(l_pod._c, l_new_pod._c);
}

TEST(reflection, object_with_string_serialization)
{
	object_with_string l_str;
	l_str._a = "Hello World";

	std::pmr::string l_serialized;
	FE::framework::framework_base::get_framework().get_property_reflection().serialize(l_serialized, l_str, "v0.0.0");

	object_with_string l_new_str;
	FE::framework::framework_base::get_framework().get_property_reflection().deserialize(l_serialized, l_new_str, "v0.0.0");

	EXPECT_STREQ(l_str._a.data(), l_new_str._a.data());
}

TEST(reflection, object_with_vector_serialization)
{
	object_with_vector l_strs;
	l_strs._a.push_back("Hello World");
	l_strs._a.push_back("Bye World");

	auto& l_ = l_strs._b.emplace_back().emplace_back().emplace_back();
	l_._a = 42;
	l_._b = 3.14f;
	l_._c = 7;
	memset(l_._d, 0, sizeof(l_._d));
	l_._d[0] = 1;


	std::pmr::string l_serialized;
	FE::framework::framework_base::get_framework().get_property_reflection().serialize(l_serialized, l_strs, "v0.0.0");

	object_with_vector l_new_strs;
	FE::framework::framework_base::get_framework().get_property_reflection().deserialize(l_serialized, l_new_strs, "v0.0.0");

	EXPECT_STREQ(l_strs._a[0].data(), l_new_strs._a[0].data());
	EXPECT_STREQ(l_strs._a[1].data(), l_new_strs._a[1].data());

	EXPECT_EQ(l_strs._b[0][0][0]._a, l_new_strs._b[0][0][0]._a);
	EXPECT_EQ(l_strs._b[0][0][0]._b, l_new_strs._b[0][0][0]._b);
	EXPECT_EQ(l_strs._b[0][0][0]._c, l_new_strs._b[0][0][0]._c);
	EXPECT_EQ(l_strs._b[0][0][0]._d[0], l_new_strs._b[0][0][0]._d[0]);
}

TEST(reflection, method_call)
{
	object l_object("Jesus Loves You!");
	//std::cout <<  << std::endl;
	auto l_function_pointer = FE::framework::framework_base::get_framework().get_method_reflection().retrieve(l_object.get_text_method_meta.get_method_name());
	FE::ASCII* l_msg;
	(*l_function_pointer)(&l_object, &l_msg, nullptr);
	EXPECT_STREQ(l_msg, "Jesus Loves You!");

	auto l_greeter = FE::framework::framework_base::get_framework().get_method_reflection().retrieve(l_object.greet_static_method_meta.get_method_name());
	(*l_greeter)(&l_msg, nullptr);
	EXPECT_STREQ(l_msg, "Hello, reflection system!");
}

TEST(reflection, property)
{
	object l_object;
	FE::framework::reflection::instance_metadata* l_obj_mem_layout = FE::framework::framework_base::get_framework().get_property_reflection().get_instance_metadata<object>();
	EXPECT_TRUE(l_obj_mem_layout != nullptr);

	std::string* const l_property = l_obj_mem_layout->get_property_of<std::string>(l_object, "m_text");
	EXPECT_TRUE(l_property != nullptr);
	l_property->assign("Jesus Loves You!");
	EXPECT_STREQ(l_property->c_str(), "Jesus Loves You!");
}


enum struct Color
{
	_Red,
	_Green,
	_Blue
};

TEST(reflection, enum_struct)
{
	FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct<Color>("Color",
		{
			{Color::_Red, "_Red"},
			{Color::_Green, "_Green"},
			{Color::_Blue, "_Blue"}
		}
	);

	FE::framework::reflection::enum_metadata* l_enum_struct_metadata = FE::framework::framework_base::get_framework().get_enum_reflection().retrieve_enum_struct_metadata("Color");
	EXPECT_TRUE(l_enum_struct_metadata != nullptr);
	EXPECT_STREQ(l_enum_struct_metadata->enum_to_string(Color::_Red), "_Red"); // Should return "Red"
	EXPECT_EQ(*l_enum_struct_metadata->string_to_enum<Color>("_Green"), Color::_Green); // Should return Color::Green
	EXPECT_STREQ(l_enum_struct_metadata->get_typename(), "Color");
}