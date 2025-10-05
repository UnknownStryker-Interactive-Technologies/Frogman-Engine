// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.h> 
#include <FE/framework/framework.hpp> 
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Framework/Include/FE/framework/world.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Framework/Include/FE/framework/thread_id.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Framework/Include/FE/framework/system_base.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Framework/Include/FE/framework/game.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Framework/Include/FE/framework/archetype_base.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Framework/Include/FE/framework/component_base.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Framework/Include/FE/framework/framework.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Framework/Include/FE/framework/type_info.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Framework/Include/FE/framework/ECS.hpp>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Framework/Include/FE/framework/smart_ptr.hxx>
#include <C:/Users/leeho/Documents/GitHub/Frogman-Engine/SDK/CMake/../Framework/Include/FE/framework/reflection.hpp>



void load_reflection_data()
{
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::FE::archetype_base>(::FE::ASCII* const, ::FE::initializer&)> >("::FE::world", &::FE::ECS::instanciate_entity_from_initializer<::FE::world>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::entity<::FE::archetype_base>)> >("~::FE::world", &::FE::ECS::destruct_entity);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, ::FE::entity<::FE::archetype_base>(::FE::ASCII* const, ::FE::initializer&)> >("::FE::game", &::FE::ECS::instanciate_entity_from_initializer<::FE::game>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<::FE::ECS, void(::FE::entity<::FE::archetype_base>)> >("~::FE::game", &::FE::ECS::destruct_entity);

    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::archetype_base*(::FE::archetype_base*)> >("construct ::FE::archetype_base", &::std::construct_at<::FE::archetype_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::archetype_base*)> >("destruct ::FE::archetype_base", &::std::destroy_at<::FE::archetype_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::components*(::FE::components*)> >("construct ::FE::components", &::std::construct_at<::FE::components>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::components*)> >("destruct ::FE::components", &::std::destroy_at<::FE::components>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::component_base*(::FE::component_base*)> >("construct ::FE::component_base", &::std::construct_at<::FE::component_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::component_base*)> >("destruct ::FE::component_base", &::std::destroy_at<::FE::component_base>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::internal::ECS::component_identifier*(::FE::internal::ECS::component_identifier*)> >("construct ::FE::internal::ECS::component_identifier", &::std::construct_at<::FE::internal::ECS::component_identifier>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::internal::ECS::component_identifier*)> >("destruct ::FE::internal::ECS::component_identifier", &::std::destroy_at<::FE::internal::ECS::component_identifier>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::framework::reflection::type_info*(::FE::framework::reflection::type_info*)> >("construct ::FE::framework::reflection::type_info", &::std::construct_at<::FE::framework::reflection::type_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::framework::reflection::type_info*)> >("destruct ::FE::framework::reflection::type_info", &::std::destroy_at<::FE::framework::reflection::type_info>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::framework::reflection::internal::type_info::metadata*(::FE::framework::reflection::internal::type_info::metadata*)> >("construct ::FE::framework::reflection::internal::type_info::metadata", &::std::construct_at<::FE::framework::reflection::internal::type_info::metadata>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::framework::reflection::internal::type_info::metadata*)> >("destruct ::FE::framework::reflection::internal::type_info::metadata", &::std::destroy_at<::FE::framework::reflection::internal::type_info::metadata>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::framework::reflection::internal::type_info::c_style_deleter*(::FE::framework::reflection::internal::type_info::c_style_deleter*)> >("construct ::FE::framework::reflection::internal::type_info::c_style_deleter", &::std::construct_at<::FE::framework::reflection::internal::type_info::c_style_deleter>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::framework::reflection::internal::type_info::c_style_deleter*)> >("destruct ::FE::framework::reflection::internal::type_info::c_style_deleter", &::std::destroy_at<::FE::framework::reflection::internal::type_info::c_style_deleter>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::internal::smart_ptr::metadata*(::FE::internal::smart_ptr::metadata*)> >("construct ::FE::internal::smart_ptr::metadata", &::std::construct_at<::FE::internal::smart_ptr::metadata>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::internal::smart_ptr::metadata*)> >("destruct ::FE::internal::smart_ptr::metadata", &::std::destroy_at<::FE::internal::smart_ptr::metadata>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::framework::reflection::instance_metadata*(::FE::framework::reflection::instance_metadata*)> >("construct ::FE::framework::reflection::instance_metadata", &::std::construct_at<::FE::framework::reflection::instance_metadata>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::framework::reflection::instance_metadata*)> >("destruct ::FE::framework::reflection::instance_metadata", &::std::destroy_at<::FE::framework::reflection::instance_metadata>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::framework::reflection::enum_metadata*(::FE::framework::reflection::enum_metadata*)> >("construct ::FE::framework::reflection::enum_metadata", &::std::construct_at<::FE::framework::reflection::enum_metadata>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::framework::reflection::enum_metadata*)> >("destruct ::FE::framework::reflection::enum_metadata", &::std::destroy_at<::FE::framework::reflection::enum_metadata>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::framework::reflection::enum_registry*(::FE::framework::reflection::enum_registry*)> >("construct ::FE::framework::reflection::enum_registry", &::std::construct_at<::FE::framework::reflection::enum_registry>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::framework::reflection::enum_registry*)> >("destruct ::FE::framework::reflection::enum_registry", &::std::destroy_at<::FE::framework::reflection::enum_registry>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::framework::reflection::property_metadata*(::FE::framework::reflection::property_metadata*)> >("construct ::FE::framework::reflection::property_metadata", &::std::construct_at<::FE::framework::reflection::property_metadata>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::framework::reflection::property_metadata*)> >("destruct ::FE::framework::reflection::property_metadata", &::std::destroy_at<::FE::framework::reflection::property_metadata>);
}
