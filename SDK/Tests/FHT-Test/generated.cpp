// Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <memory>
#include <utility>

#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\TemplatePurgeTest.hpp>




template <typename T, typename... Args>
T* construct(T* location_p, Args&&... args_p)
{
    new(location_p) T( ::std::forward<Args>(args_p)... );
    return location_p;
}


template <typename T>
void destruct(T* ptr_p)
{
    if (ptr_p == nullptr) return;
    ptr_p->~T();
}




void load_reflection_data()
{
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Specialized<int>*(::Specialized<int>*)> >("construct ::Specialized<int>", &::construct<::Specialized<int>>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Specialized<int>*)> >("destruct ::Specialized<int>", &::destruct<::Specialized<int>>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Specialized<double>*(::Specialized<double>*)> >("construct ::Specialized<double>", &::construct<::Specialized<double>>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Specialized<double>*)> >("destruct ::Specialized<double>", &::destruct<::Specialized<double>>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::MemberFunctionTemplates*(::MemberFunctionTemplates*)> >("construct ::MemberFunctionTemplates", &::construct<::MemberFunctionTemplates>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::MemberFunctionTemplates*)> >("destruct ::MemberFunctionTemplates", &::destruct<::MemberFunctionTemplates>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Factorial<0>*(::Factorial<0>*)> >("construct ::Factorial<0>", &::construct<::Factorial<0>>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Factorial<0>*)> >("destruct ::Factorial<0>", &::destruct<::Factorial<0>>);
}
