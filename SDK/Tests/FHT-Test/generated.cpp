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
    ::FE::framework::framework_base::get_framework().get_method_reflection().associate_system(FEWorldTag::Boot, FE::_WorldTick, &::sys7);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Sentinel4b*(::Sentinel4b*)> >("construct ::Sentinel4b", &::construct<::Sentinel4b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Sentinel4b*)> >("destruct ::Sentinel4b", &::destruct<::Sentinel4b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Sentinel4c*(::Sentinel4c*)> >("construct ::Sentinel4c", &::construct<::Sentinel4c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Sentinel4c*)> >("destruct ::Sentinel4c", &::destruct<::Sentinel4c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::X7*(::X7*)> >("construct ::X7", &::construct<::X7>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::X7*)> >("destruct ::X7", &::destruct<::X7>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::task_queue12a*(::task_queue12a*)> >("construct ::task_queue12a", &::construct<::task_queue12a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::task_queue12a*)> >("destruct ::task_queue12a", &::destruct<::task_queue12a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Dead14*(::Dead14*)> >("construct ::Dead14", &::construct<::Dead14>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Dead14*)> >("destruct ::Dead14", &::destruct<::Dead14>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::After7b*(::After7b*)> >("construct ::After7b", &::construct<::After7b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::After7b*)> >("destruct ::After7b", &::destruct<::After7b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::task_queue12b*(::task_queue12b*)> >("construct ::task_queue12b", &::construct<::task_queue12b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::task_queue12b*)> >("destruct ::task_queue12b", &::destruct<::task_queue12b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Sentinel8*(::Sentinel8*)> >("construct ::Sentinel8", &::construct<::Sentinel8>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Sentinel8*)> >("destruct ::Sentinel8", &::destruct<::Sentinel8>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Sentinel14*(::Sentinel14*)> >("construct ::Sentinel14", &::construct<::Sentinel14>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Sentinel14*)> >("destruct ::Sentinel14", &::destruct<::Sentinel14>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::N8::Q8*(::N8::Q8*)> >("construct ::N8::Q8", &::construct<::N8::Q8>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::N8::Q8*)> >("destruct ::N8::Q8", &::destruct<::N8::Q8>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::D15*(::D15*)> >("construct ::D15", &::construct<::D15>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::D15*)> >("destruct ::D15", &::destruct<::D15>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::D15b*(::D15b*)> >("construct ::D15b", &::construct<::D15b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::D15b*)> >("destruct ::D15b", &::destruct<::D15b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Sentinel15*(::Sentinel15*)> >("construct ::Sentinel15", &::construct<::Sentinel15>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Sentinel15*)> >("destruct ::Sentinel15", &::destruct<::Sentinel15>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::Sentinel18*(::Sentinel18*)> >("construct ::Sentinel18", &::construct<::Sentinel18>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::Sentinel18*)> >("destruct ::Sentinel18", &::destruct<::Sentinel18>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::N7::Before7b*(::N7::Before7b*)> >("construct ::N7::Before7b", &::construct<::N7::Before7b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::N7::Before7b*)> >("destruct ::N7::Before7b", &::destruct<::N7::Before7b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::N7::Reopened7b*(::N7::Reopened7b*)> >("construct ::N7::Reopened7b", &::construct<::N7::Reopened7b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::N7::Reopened7b*)> >("destruct ::N7::Reopened7b", &::destruct<::N7::Reopened7b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::T7*(::T7*)> >("construct ::T7", &::construct<::T7>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::T7*)> >("destruct ::T7", &::destruct<::T7>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::S17a*(::S17a*)> >("construct ::S17a", &::construct<::S17a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::S17a*)> >("destruct ::S17a", &::destruct<::S17a>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::N7::Inner7::S7*(::N7::Inner7::S7*)> >("construct ::N7::Inner7::S7", &::construct<::N7::Inner7::S7>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::N7::Inner7::S7*)> >("destruct ::N7::Inner7::S7", &::destruct<::N7::Inner7::S7>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::S17b*(::S17b*)> >("construct ::S17b", &::construct<::S17b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::S17b*)> >("destruct ::S17b", &::destruct<::S17b>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::S17c*(::S17c*)> >("construct ::S17c", &::construct<::S17c>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::S17c*)> >("destruct ::S17c", &::destruct<::S17c>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::E7 >("::E7",
    {
        { ::E7::V, "V" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::E18a >("::E18a",
    {
        { ::E18a::A, "A" },
        { ::E18a::B, "B" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::E18b >("::E18b",
    {
        { ::E18b::C, "C" },
        { ::E18b::D, "D" }
    });
}
