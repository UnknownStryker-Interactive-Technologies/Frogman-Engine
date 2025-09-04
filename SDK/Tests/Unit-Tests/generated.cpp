// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.h> 
#include <FE/framework/framework.hpp> 
#include <C:/Users/leeho/OneDrive/문서/GitHub/Frogman-Engine/SDK/Tests/Unit-Tests/FE.ECS.hpp>

void load_reflection_data()
{
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::terrorist>(::FE::ASCII* const)> >("::terrorist", &::FE::ECS::instanciate_entity<::terrorist>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(const ::FE::entity<::terrorist>&)> >("~::terrorist", &::FE::ECS::destruct_entity<::terrorist>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::AK47>(::FE::ASCII* const)> >("::AK47", &::FE::ECS::instanciate_entity<::AK47>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(const ::FE::entity<::AK47>&)> >("~::AK47", &::FE::ECS::destruct_entity<::AK47>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::ak_ammo>(::FE::ASCII* const)> >("::ak_ammo", &::FE::ECS::instanciate_entity<::ak_ammo>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(const ::FE::entity<::ak_ammo>&)> >("~::ak_ammo", &::FE::ECS::destruct_entity<::ak_ammo>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::player>(::FE::ASCII* const)> >("::player", &::FE::ECS::instanciate_entity<::player>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(const ::FE::entity<::player>&)> >("~::player", &::FE::ECS::destruct_entity<::player>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::ak_magazine>(::FE::archetype_base* const)> >("::ak_magazine", &::FE::ECS::add_component<::ak_magazine>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::archetype_base* const)> >("~::ak_magazine", &::FE::ECS::remove_component<::ak_magazine>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::health>(::FE::archetype_base* const)> >("::health", &::FE::ECS::add_component<::health>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::archetype_base* const)> >("~::health", &::FE::ECS::remove_component<::health>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::weapon>(::FE::archetype_base* const)> >("::weapon", &::FE::ECS::add_component<::weapon>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::archetype_base* const)> >("~::weapon", &::FE::ECS::remove_component<::weapon>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::speed>(::FE::archetype_base* const)> >("::speed", &::FE::ECS::add_component<::speed>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::archetype_base* const)> >("~::speed", &::FE::ECS::remove_component<::speed>);
}
