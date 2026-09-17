// Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <memory>
#include <utility>

#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\MultilineCommentInBodyTest.hpp>




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
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_ok1*(::body_comment_ok1*)> >("construct ::body_comment_ok1", &::construct<::body_comment_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_ok1*)> >("destruct ::body_comment_ok1", &::destruct<::body_comment_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_b2*(::body_comment_b2*)> >("construct ::body_comment_b2", &::construct<::body_comment_b2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_b2*)> >("destruct ::body_comment_b2", &::destruct<::body_comment_b2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_sentinel_b4*(::body_comment_sentinel_b4*)> >("construct ::body_comment_sentinel_b4", &::construct<::body_comment_sentinel_b4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_sentinel_b4*)> >("destruct ::body_comment_sentinel_b4", &::destruct<::body_comment_sentinel_b4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_b5*(::body_comment_b5*)> >("construct ::body_comment_b5", &::construct<::body_comment_b5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_b5*)> >("destruct ::body_comment_b5", &::destruct<::body_comment_b5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_sentinel_b5*(::body_comment_sentinel_b5*)> >("construct ::body_comment_sentinel_b5", &::construct<::body_comment_sentinel_b5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_sentinel_b5*)> >("destruct ::body_comment_sentinel_b5", &::destruct<::body_comment_sentinel_b5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_ns_ok5::body_comment_ok5*(::body_comment_ns_ok5::body_comment_ok5*)> >("construct ::body_comment_ns_ok5::body_comment_ok5", &::construct<::body_comment_ns_ok5::body_comment_ok5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_ns_ok5::body_comment_ok5*)> >("destruct ::body_comment_ns_ok5::body_comment_ok5", &::destruct<::body_comment_ns_ok5::body_comment_ok5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_ok2*(::body_comment_ok2*)> >("construct ::body_comment_ok2", &::construct<::body_comment_ok2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_ok2*)> >("destruct ::body_comment_ok2", &::destruct<::body_comment_ok2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_ok3*(::body_comment_ok3*)> >("construct ::body_comment_ok3", &::construct<::body_comment_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_ok3*)> >("destruct ::body_comment_ok3", &::destruct<::body_comment_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_b1*(::body_comment_b1*)> >("construct ::body_comment_b1", &::construct<::body_comment_b1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_b1*)> >("destruct ::body_comment_b1", &::destruct<::body_comment_b1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::body_comment_b3*(::body_comment_b3*)> >("construct ::body_comment_b3", &::construct<::body_comment_b3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::body_comment_b3*)> >("destruct ::body_comment_b3", &::destruct<::body_comment_b3>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::body_comment_kind_ok4 >("::body_comment_kind_ok4",
    {
        { ::body_comment_kind_ok4::_A, "_A" },
        { ::body_comment_kind_ok4::_B, "_B" }
    });
}
