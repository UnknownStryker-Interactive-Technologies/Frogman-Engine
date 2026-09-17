// Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <memory>
#include <utility>

#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\ToFix.hpp>




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
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::S17a*(::S17a*)> >("construct ::S17a", &::construct<::S17a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::S17a*)> >("destruct ::S17a", &::destruct<::S17a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::S17b*(::S17b*)> >("construct ::S17b", &::construct<::S17b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::S17b*)> >("destruct ::S17b", &::destruct<::S17b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::S17c*(::S17c*)> >("construct ::S17c", &::construct<::S17c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::S17c*)> >("destruct ::S17c", &::destruct<::S17c>);
}
