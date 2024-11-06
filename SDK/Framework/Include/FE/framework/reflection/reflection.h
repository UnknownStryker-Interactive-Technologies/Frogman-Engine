#ifndef _FE_FRAMEWORK_REFLECTION_HPP_
#define _FE_FRAMEWORK_REFLECTION_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/do_once.hxx>
#include <FE/function.hxx>
#include <FE/type_traits.hxx>

#include <FE/framework/string.hxx>
#include <FE/framework/reflection/function.hpp>
#include <FE/framework/reflection/property.hpp>
#include <FE/framework/reflection/type_info.hpp>


/*
Frogman Engine serialization & deserialization system supports:

  Compatible component types: Plain old data, fixed-sized arrays, FE smart pointers, std::basic_string, std::array, and std::vector.

	* C/C++ object that might contain the chain of derivation, components, other supported types, and components' chain of derivation.
	
	* RAW pointers and references are not supported.
*/


#ifdef FE_CLASS_HAS_A_BASE
	#error FE_CLASS_HAS_A_BASE is a reserved Frogman Engine macro keyword.
#else
#define FE_CLASS_HAS_A_BASE(base_class) \
public: \
using base_type = base_class; \
private:
#endif


#ifdef REGISTER_FE_CLASS
	#error REGISTER_FE_CLASS is a reserved Frogman Engine macro keyword.
#else
#define REGISTER_FE_CLASS(class_name) \
public: \
using is_reflective = decltype(true); \
class class_meta_data \
{ \
public: \
	using type = class_name; \
}; \
private:
#endif


#ifdef REGISTER_FE_METHOD
	#error REGISTER_FE_METHOD is a reserved Frogman Engine macro keyword.
#else
/*Parameter order:
1st - method name
2nd - method attributes[ EX: void(void) ]
3rd and so on - arguments buffer type[ EX: FE::string, var::length_t ] 
*/
#define REGISTER_FE_METHOD(namespace, method_name, ...) \
class method_reflection_##method_name \
{ \
public: \
	_FE_FORCE_INLINE_ method_reflection_##method_name() noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, ::FE::framework::reflection::function::register_task<::FE::cpp_style_task<class_meta_data::type, __VA_ARGS__, typename FE::method<class_meta_data::type, __VA_ARGS__>::arguments_type>>(get_signature(), &class_meta_data::type::method_name)); \
	} \
	static ::FE::string get_signature() noexcept \
	{ \
		::FE::string l_full_signature = #__VA_ARGS__; \
		auto l_result = l_full_signature.rfind("("); \
		if(l_full_signature[l_result->_begin - 1] != ' ') \
		{ \
		    l_full_signature.extend(1); \
		    l_full_signature.insert(l_result->_begin, " "); \
		} \
		::FE::string l_signature = #namespace; \
		l_signature.extend(::FE::algorithm::string::length(#method_name) + 2); \
		l_signature += "::"; \
		l_signature += #method_name; \
		l_full_signature.extend(l_signature.length()); \
		l_result = l_full_signature.rfind("("); \
		l_full_signature.insert(l_result->_begin, l_signature.c_str()); \
		if(l_full_signature.back() != ')') \
		{ \
		    l_result = l_full_signature.find(")"); \
			l_full_signature.erase(l_result->_end, l_full_signature.length() - l_result->_end); \
		} \
		return l_full_signature; \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ method_reflection_##method_name method_reflection_instance_##method_name;
#endif


#ifdef REGISTER_FE_PROPERTY
	#error REGISTER_FE_PROPERTY is a reserved Frogman Engine macro keyword.
#else
#define REGISTER_FE_PROPERTY(property_name)  \
class property_reflection_##property_name \
{ \
public: \
	_FE_FORCE_INLINE_ property_reflection_##property_name(typename class_meta_data::type* const this_p) noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, ::FE::framework::reflection::property::register_property<typename class_meta_data::type, decltype(property_name)>(*this_p, this_p->property_name, #property_name)); \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ property_reflection_##property_name property_reflection_instance_##property_name = this;
#endif


#endif