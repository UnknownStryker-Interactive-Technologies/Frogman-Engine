// Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <memory>
#include <utility>

#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\ConstMemberDefaultConstructorTest.hpp>




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
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::holder_const_c4*(::holder_const_c4*)> >("construct ::holder_const_c4", &::construct<::holder_const_c4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::holder_const_c4*)> >("destruct ::holder_const_c4", &::destruct<::holder_const_c4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pod_const_ok1*(::pod_const_ok1*)> >("construct ::pod_const_ok1", &::construct<::pod_const_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pod_const_ok1*)> >("destruct ::pod_const_ok1", &::destruct<::pod_const_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pod_const_s1*(::pod_const_s1*)> >("construct ::pod_const_s1", &::construct<::pod_const_s1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pod_const_s1*)> >("destruct ::pod_const_s1", &::destruct<::pod_const_s1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pod_const_s2*(::pod_const_s2*)> >("construct ::pod_const_s2", &::construct<::pod_const_s2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pod_const_s2*)> >("destruct ::pod_const_s2", &::destruct<::pod_const_s2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pod_const_s3*(::pod_const_s3*)> >("construct ::pod_const_s3", &::construct<::pod_const_s3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pod_const_s3*)> >("destruct ::pod_const_s3", &::destruct<::pod_const_s3>);
}
