#ifndef _FE_FRAMEWORK_REFLECTION_HPP_
#define _FE_FRAMEWORK_REFLECTION_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.h>
#include <FE/algorithm/string.hxx>
#include <FE/do_once.hxx>
#include <FE/function.hxx>
#include <FE/type_traits.hxx>

#include <FE/framework/reflection/method.hpp>
#include <FE/framework/reflection/property.hpp>
#include <FE/framework/reflection/type_info.hpp>

// std::string
#include <string>

// std::construct_at
#include <memory>




BEGIN_NAMESPACE(FE::framework)

class framework_base;

class reflection_system
{
	friend class framework_base;

	reflection_system(FE::size reflection_map_capacity_p) noexcept
		: _method_reflection(reflection_map_capacity_p), _property_reflection(reflection_map_capacity_p) {}
	~reflection_system() noexcept = default;

public:
	reflection::method _method_reflection;
	reflection::property _property_reflection;

private:
	static reflection_system* s_instance;

	static void initialize(FE::size reflection_map_capacity_p) noexcept
	{
		if (s_instance == nullptr)
		{
			s_instance = new reflection_system(reflection_map_capacity_p);
		}
	}

	static void shutdown() noexcept
	{
		if (s_instance != nullptr)
		{
			delete s_instance;
			s_instance = nullptr;
		}
	}

public:
	static reflection_system& access() noexcept
	{
		FE_ASSERT(s_instance != nullptr, "Reflection system is not initialized.");
		return *s_instance;
	}
};

END_NAMESPACE

/*
Frogman Engine serialization & deserialization system:

  Compatible component types: Plain old data, trivial fixed-sized arrays, FE smart pointers, std::basic_string, std::array, and std::vector.

	* C/C++ object that might contain a chain of derivation, components, other supported types, and a component's chain of derivation.

	* Raw pointers, non-trivial fixed-sized arrays, and references are not supported.
*/


#ifdef FE_CLASS_HAS_A_BASE
#error FE_CLASS_HAS_A_BASE is a reserved Frogman Engine macro keyword.
#else
#define FE_CLASS_HAS_A_BASE(base_class) \
using base_type = base_class; 
#endif


#ifdef FE_CLASS
#error REGISTER_FE_CLASS is a reserved Frogman Engine macro keyword.
#else
#define FE_CLASS(class_name) \
using is_reflective = decltype(true); \
class class_meta_data \
{ \
public: \
	using type = class_name; \
	class_meta_data() noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, ::FE::framework::reflection_system::access()._method_reflection.register_task< ::FE::c_style_task<void(void*), typename ::FE::function<void(void*)>::arguments_type> >(#class_name, &::FE::framework::reflection::construct_object<class_name>) ); \
	} \
}; 
#endif


#ifdef FE_METHOD
	#error FE_METHOD is a reserved Frogman Engine macro keyword.
#else
/*Parameter order:
1st - method name
2nd - method attributes[ EX: void(void) ]
3rd and so on - arguments buffer type[ EX: FE::string, var::length_t ] 
*/
#define FE_METHOD(namespace, method_name, ...) \
class method_reflection_##method_name \
{ \
public: \
	_FE_FORCE_INLINE_ method_reflection_##method_name() noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, ::FE::framework::reflection_system::access()._method_reflection.register_task<::FE::cpp_style_task<class_meta_data::type, __VA_ARGS__, typename FE::method<class_meta_data::type, __VA_ARGS__>::arguments_type>>(get_signature(), &class_meta_data::type::method_name)); \
	} \
	static ::std::string get_signature() noexcept \
	{ \
		::std::string l_full_signature = #__VA_ARGS__; \
		auto l_result = ::FE::algorithm::string::find_the_last(l_full_signature.c_str(), '('); \
		if (l_full_signature[l_result->_begin - 1] != ' ') \
		{ \
			l_full_signature.insert(l_result->_begin, " "); \
		} \
		::std::string l_signature = #namespace; \
		l_signature += "::"; l_signature += #method_name; \
		l_result = ::FE::algorithm::string::find_the_last(l_full_signature.c_str(), '('); \
		l_full_signature.insert(l_result->_begin, l_signature.c_str()); \
		return l_full_signature; \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ method_reflection_##method_name method_reflection_instance_##method_name;
#endif


#ifdef FE_PROPERTY
	#error REGISTER_FE_PROPERTY is a reserved Frogman Engine macro keyword.
#else
#define FE_PROPERTY(property_name)  \
class property_reflection_##property_name \
{ \
public: \
	_FE_FORCE_INLINE_ property_reflection_##property_name(typename class_meta_data::type* const this_p) noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, ::FE::framework::reflection_system::access()._property_reflection.register_property<typename class_meta_data::type, decltype(property_name)>(*this_p, this_p->property_name, #property_name)); \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ property_reflection_##property_name property_reflection_instance_##property_name = this;
#endif


#endif