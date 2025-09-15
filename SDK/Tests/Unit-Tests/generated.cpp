// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.h> 
#include <FE/framework/framework.hpp> 
#include <C:/Users/leeho/OneDrive/문서/GitHub/Frogman-Engine/SDK/Tests/Unit-Tests/FE.ECS.hpp>


void serialize_component_ak_magazine(std::pmr::string& out_buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().serialize(out_buffer_p, *FE::polymorphic_cast<::ak_magazine* const>(component_p), version_p);
}

void deserialize_component_ak_magazine(const std::pmr::string& buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().deserialize(buffer_p, *FE::polymorphic_cast<::ak_magazine* const>(component_p), version_p);
}

void serialize_component_health(std::pmr::string& out_buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().serialize(out_buffer_p, *FE::polymorphic_cast<::health* const>(component_p), version_p);
}

void deserialize_component_health(const std::pmr::string& buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().deserialize(buffer_p, *FE::polymorphic_cast<::health* const>(component_p), version_p);
}

void serialize_component_weapon(std::pmr::string& out_buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().serialize(out_buffer_p, *FE::polymorphic_cast<::weapon* const>(component_p), version_p);
}

void deserialize_component_weapon(const std::pmr::string& buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().deserialize(buffer_p, *FE::polymorphic_cast<::weapon* const>(component_p), version_p);
}

void serialize_component_speed(std::pmr::string& out_buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().serialize(out_buffer_p, *FE::polymorphic_cast<::speed* const>(component_p), version_p);
}

void deserialize_component_speed(const std::pmr::string& buffer_p, ::FE::component_base* const component_p, ::FE::ASCII* const version_p) noexcept
{
    ::FE::framework::framework_base::get_framework().get_property_reflection().deserialize(buffer_p, *FE::polymorphic_cast<::speed* const>(component_p), version_p);
}


void load_reflection_data()
{
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::terrorist>(::FE::ASCII* const)> >("::terrorist", &::FE::ECS::instanciate_entity<::terrorist>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::entity<::FE::archetype_base>)> >("~::terrorist", &::FE::ECS::destruct_entity);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::AK47>(::FE::ASCII* const)> >("::AK47", &::FE::ECS::instanciate_entity<::AK47>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::entity<::FE::archetype_base>)> >("~::AK47", &::FE::ECS::destruct_entity);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::ak_ammo>(::FE::ASCII* const)> >("::ak_ammo", &::FE::ECS::instanciate_entity<::ak_ammo>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::entity<::FE::archetype_base>)> >("~::ak_ammo", &::FE::ECS::destruct_entity);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::player>(::FE::ASCII* const)> >("::player", &::FE::ECS::instanciate_entity<::player>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::entity<::FE::archetype_base>)> >("~::player", &::FE::ECS::destruct_entity);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::ak_magazine>(::FE::entity<::FE::archetype_base>)> >("::ak_magazine", &::FE::ECS::add_component<::ak_magazine>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::entity<::FE::archetype_base>)> >("~::ak_magazine", &::FE::ECS::remove_component<::ak_magazine>);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("serialize_component_ak_magazine", &serialize_component_ak_magazine);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(const ::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("deserialize_component_ak_magazine", &deserialize_component_ak_magazine);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::health>(::FE::entity<::FE::archetype_base>)> >("::health", &::FE::ECS::add_component<::health>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::entity<::FE::archetype_base>)> >("~::health", &::FE::ECS::remove_component<::health>);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("serialize_component_health", &serialize_component_health);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(const ::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("deserialize_component_health", &deserialize_component_health);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::weapon>(::FE::entity<::FE::archetype_base>)> >("::weapon", &::FE::ECS::add_component<::weapon>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::entity<::FE::archetype_base>)> >("~::weapon", &::FE::ECS::remove_component<::weapon>);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("serialize_component_weapon", &serialize_component_weapon);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(const ::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("deserialize_component_weapon", &deserialize_component_weapon);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::component_view<::speed>(::FE::entity<::FE::archetype_base>)> >("::speed", &::FE::ECS::add_component<::speed>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::entity<::FE::archetype_base>)> >("~::speed", &::FE::ECS::remove_component<::speed>);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("serialize_component_speed", &serialize_component_speed);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(const ::std::pmr::string&, ::FE::component_base* const, ::FE::ASCII* const)> >("deserialize_component_speed", &deserialize_component_speed);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::component_base* const)> >("::take_damage", &::take_damage);
}
