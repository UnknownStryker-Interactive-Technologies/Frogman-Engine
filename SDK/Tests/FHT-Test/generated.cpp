// Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <memory>
#include <utility>

#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\AliasTemplatePurgeTest.hpp>




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
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_WorldTick, &::alias_system_a6);
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_WorldTick, &::alias_system_a7);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_ok1*(::alias_after_ok1*)> >("construct ::alias_after_ok1", &::construct<::alias_after_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_ok1*)> >("destruct ::alias_after_ok1", &::destruct<::alias_after_ok1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_sentinel_a1*(::alias_sentinel_a1*)> >("construct ::alias_sentinel_a1", &::construct<::alias_sentinel_a1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_sentinel_a1*)> >("destruct ::alias_sentinel_a1", &::destruct<::alias_sentinel_a1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_ok2*(::alias_after_ok2*)> >("construct ::alias_after_ok2", &::construct<::alias_after_ok2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_ok2*)> >("destruct ::alias_after_ok2", &::destruct<::alias_after_ok2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_ok3*(::alias_after_ok3*)> >("construct ::alias_after_ok3", &::construct<::alias_after_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_ok3*)> >("destruct ::alias_after_ok3", &::destruct<::alias_after_ok3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_a1*(::alias_after_a1*)> >("construct ::alias_after_a1", &::construct<::alias_after_a1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_a1*)> >("destruct ::alias_after_a1", &::destruct<::alias_after_a1>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_ok4*(::alias_after_ok4*)> >("construct ::alias_after_ok4", &::construct<::alias_after_ok4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_ok4*)> >("destruct ::alias_after_ok4", &::destruct<::alias_after_ok4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_ok5*(::alias_after_ok5*)> >("construct ::alias_after_ok5", &::construct<::alias_after_ok5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_ok5*)> >("destruct ::alias_after_ok5", &::destruct<::alias_after_ok5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_sentinel_a3*(::alias_sentinel_a3*)> >("construct ::alias_sentinel_a3", &::construct<::alias_sentinel_a3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_sentinel_a3*)> >("destruct ::alias_sentinel_a3", &::destruct<::alias_sentinel_a3>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_sentinel_a4*(::alias_sentinel_a4*)> >("construct ::alias_sentinel_a4", &::construct<::alias_sentinel_a4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_sentinel_a4*)> >("destruct ::alias_sentinel_a4", &::destruct<::alias_sentinel_a4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_sentinel_a6*(::alias_sentinel_a6*)> >("construct ::alias_sentinel_a6", &::construct<::alias_sentinel_a6>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_sentinel_a6*)> >("destruct ::alias_sentinel_a6", &::destruct<::alias_sentinel_a6>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_ns_a4::alias_inner_a4*(::alias_ns_a4::alias_inner_a4*)> >("construct ::alias_ns_a4::alias_inner_a4", &::construct<::alias_ns_a4::alias_inner_a4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_ns_a4::alias_inner_a4*)> >("destruct ::alias_ns_a4::alias_inner_a4", &::destruct<::alias_ns_a4::alias_inner_a4>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_ns_a5::alias_after_a5*(::alias_ns_a5::alias_after_a5*)> >("construct ::alias_ns_a5::alias_after_a5", &::construct<::alias_ns_a5::alias_after_a5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_ns_a5::alias_after_a5*)> >("destruct ::alias_ns_a5::alias_after_a5", &::destruct<::alias_ns_a5::alias_after_a5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_ns_a5::alias_sentinel_a5*(::alias_ns_a5::alias_sentinel_a5*)> >("construct ::alias_ns_a5::alias_sentinel_a5", &::construct<::alias_ns_a5::alias_sentinel_a5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_ns_a5::alias_sentinel_a5*)> >("destruct ::alias_ns_a5::alias_sentinel_a5", &::destruct<::alias_ns_a5::alias_sentinel_a5>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_after_a2*(::alias_after_a2*)> >("construct ::alias_after_a2", &::construct<::alias_after_a2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_after_a2*)> >("destruct ::alias_after_a2", &::destruct<::alias_after_a2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::alias_sentinel_a2*(::alias_sentinel_a2*)> >("construct ::alias_sentinel_a2", &::construct<::alias_sentinel_a2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::alias_sentinel_a2*)> >("destruct ::alias_sentinel_a2", &::destruct<::alias_sentinel_a2>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::alias_after_kind_a3 >("::alias_after_kind_a3",
    {
        { ::alias_after_kind_a3::_A, "_A" }
    });
}
