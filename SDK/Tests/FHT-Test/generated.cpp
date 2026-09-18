// Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <memory>
#include <utility>

#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\LiteralTokenizationTest.hpp>




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
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_member_c10*(::lit_member_c10*)> >("construct ::lit_member_c10", &::construct<::lit_member_c10>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_member_c10*)> >("destruct ::lit_member_c10", &::destruct<::lit_member_c10>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l1d*(::lit_sentinel_l1d*)> >("construct ::lit_sentinel_l1d", &::construct<::lit_sentinel_l1d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l1d*)> >("destruct ::lit_sentinel_l1d", &::destruct<::lit_sentinel_l1d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_member_c11*(::lit_member_c11*)> >("construct ::lit_member_c11", &::construct<::lit_member_c11>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_member_c11*)> >("destruct ::lit_member_c11", &::destruct<::lit_member_c11>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l4a*(::lit_sentinel_l4a*)> >("construct ::lit_sentinel_l4a", &::construct<::lit_sentinel_l4a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l4a*)> >("destruct ::lit_sentinel_l4a", &::destruct<::lit_sentinel_l4a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_c*(::lit_sentinel_c*)> >("construct ::lit_sentinel_c", &::construct<::lit_sentinel_c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_c*)> >("destruct ::lit_sentinel_c", &::destruct<::lit_sentinel_c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l1a*(::lit_sentinel_l1a*)> >("construct ::lit_sentinel_l1a", &::construct<::lit_sentinel_l1a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l1a*)> >("destruct ::lit_sentinel_l1a", &::destruct<::lit_sentinel_l1a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l1b*(::lit_sentinel_l1b*)> >("construct ::lit_sentinel_l1b", &::construct<::lit_sentinel_l1b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l1b*)> >("destruct ::lit_sentinel_l1b", &::destruct<::lit_sentinel_l1b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_hidden_l2*(::lit_hidden_l2*)> >("construct ::lit_hidden_l2", &::construct<::lit_hidden_l2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_hidden_l2*)> >("destruct ::lit_hidden_l2", &::destruct<::lit_hidden_l2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l3a*(::lit_sentinel_l3a*)> >("construct ::lit_sentinel_l3a", &::construct<::lit_sentinel_l3a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l3a*)> >("destruct ::lit_sentinel_l3a", &::destruct<::lit_sentinel_l3a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l1c*(::lit_sentinel_l1c*)> >("construct ::lit_sentinel_l1c", &::construct<::lit_sentinel_l1c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l1c*)> >("destruct ::lit_sentinel_l1c", &::destruct<::lit_sentinel_l1c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l2*(::lit_sentinel_l2*)> >("construct ::lit_sentinel_l2", &::construct<::lit_sentinel_l2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l2*)> >("destruct ::lit_sentinel_l2", &::destruct<::lit_sentinel_l2>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l3b*(::lit_sentinel_l3b*)> >("construct ::lit_sentinel_l3b", &::construct<::lit_sentinel_l3b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l3b*)> >("destruct ::lit_sentinel_l3b", &::destruct<::lit_sentinel_l3b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l3c*(::lit_sentinel_l3c*)> >("construct ::lit_sentinel_l3c", &::construct<::lit_sentinel_l3c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l3c*)> >("destruct ::lit_sentinel_l3c", &::destruct<::lit_sentinel_l3c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l4b*(::lit_sentinel_l4b*)> >("construct ::lit_sentinel_l4b", &::construct<::lit_sentinel_l4b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l4b*)> >("destruct ::lit_sentinel_l4b", &::destruct<::lit_sentinel_l4b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l4c*(::lit_sentinel_l4c*)> >("construct ::lit_sentinel_l4c", &::construct<::lit_sentinel_l4c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l4c*)> >("destruct ::lit_sentinel_l4c", &::destruct<::lit_sentinel_l4c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l4d*(::lit_sentinel_l4d*)> >("construct ::lit_sentinel_l4d", &::construct<::lit_sentinel_l4d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l4d*)> >("destruct ::lit_sentinel_l4d", &::destruct<::lit_sentinel_l4d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_hidden_l5a*(::lit_hidden_l5a*)> >("construct ::lit_hidden_l5a", &::construct<::lit_hidden_l5a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_hidden_l5a*)> >("destruct ::lit_hidden_l5a", &::destruct<::lit_hidden_l5a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l6b*(::lit_sentinel_l6b*)> >("construct ::lit_sentinel_l6b", &::construct<::lit_sentinel_l6b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l6b*)> >("destruct ::lit_sentinel_l6b", &::destruct<::lit_sentinel_l6b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l5a*(::lit_sentinel_l5a*)> >("construct ::lit_sentinel_l5a", &::construct<::lit_sentinel_l5a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l5a*)> >("destruct ::lit_sentinel_l5a", &::destruct<::lit_sentinel_l5a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_hidden_l5b*(::lit_hidden_l5b*)> >("construct ::lit_hidden_l5b", &::construct<::lit_hidden_l5b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_hidden_l5b*)> >("destruct ::lit_hidden_l5b", &::destruct<::lit_hidden_l5b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l6a*(::lit_sentinel_l6a*)> >("construct ::lit_sentinel_l6a", &::construct<::lit_sentinel_l6a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l6a*)> >("destruct ::lit_sentinel_l6a", &::destruct<::lit_sentinel_l6a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l5b*(::lit_sentinel_l5b*)> >("construct ::lit_sentinel_l5b", &::construct<::lit_sentinel_l5b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l5b*)> >("destruct ::lit_sentinel_l5b", &::destruct<::lit_sentinel_l5b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_sentinel_l6c*(::lit_sentinel_l6c*)> >("construct ::lit_sentinel_l6c", &::construct<::lit_sentinel_l6c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_sentinel_l6c*)> >("destruct ::lit_sentinel_l6c", &::destruct<::lit_sentinel_l6c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_member_l7a*(::lit_member_l7a*)> >("construct ::lit_member_l7a", &::construct<::lit_member_l7a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_member_l7a*)> >("destruct ::lit_member_l7a", &::destruct<::lit_member_l7a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_member_l7b*(::lit_member_l7b*)> >("construct ::lit_member_l7b", &::construct<::lit_member_l7b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_member_l7b*)> >("destruct ::lit_member_l7b", &::destruct<::lit_member_l7b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::lit_ns_l7::lit_member_l7c*(::lit_ns_l7::lit_member_l7c*)> >("construct ::lit_ns_l7::lit_member_l7c", &::construct<::lit_ns_l7::lit_member_l7c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::lit_ns_l7::lit_member_l7c*)> >("destruct ::lit_ns_l7::lit_member_l7c", &::destruct<::lit_ns_l7::lit_member_l7c>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::kindZ1 >("::kindZ1",
    {
        { ::kindZ1::_A, "_A" },
        { ::kindZ1::_B, "_B" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::lit_hidden_kind_l2 >("::lit_hidden_kind_l2",
    {
        { ::lit_hidden_kind_l2::_A, "_A" }
    });
}
