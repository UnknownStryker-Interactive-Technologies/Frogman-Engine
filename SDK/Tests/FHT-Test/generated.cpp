// Copyright © from 2024 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <memory>
#include <utility>

#include <C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\Pong\PongComponents.hpp>




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
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::box_collider_2d*(::pong::box_collider_2d*)> >("construct ::pong::box_collider_2d", &::construct<::pong::box_collider_2d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::box_collider_2d*)> >("destruct ::pong::box_collider_2d", &::destruct<::pong::box_collider_2d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::transform_2d*(::pong::transform_2d*)> >("construct ::pong::transform_2d", &::construct<::pong::transform_2d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::transform_2d*)> >("destruct ::pong::transform_2d", &::destruct<::pong::transform_2d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::velocity_2d*(::pong::velocity_2d*)> >("construct ::pong::velocity_2d", &::construct<::pong::velocity_2d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::velocity_2d*)> >("destruct ::pong::velocity_2d", &::destruct<::pong::velocity_2d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::circle_collider_2d*(::pong::circle_collider_2d*)> >("construct ::pong::circle_collider_2d", &::construct<::pong::circle_collider_2d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::circle_collider_2d*)> >("destruct ::pong::circle_collider_2d", &::destruct<::pong::circle_collider_2d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::player_intent*(::pong::player_intent*)> >("construct ::pong::player_intent", &::construct<::pong::player_intent>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::player_intent*)> >("destruct ::pong::player_intent", &::destruct<::pong::player_intent>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::paddle*(::pong::paddle*)> >("construct ::pong::paddle", &::construct<::pong::paddle>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::paddle*)> >("destruct ::pong::paddle", &::destruct<::pong::paddle>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::ball*(::pong::ball*)> >("construct ::pong::ball", &::construct<::pong::ball>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::ball*)> >("destruct ::pong::ball", &::destruct<::pong::ball>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::cpu_brain*(::pong::cpu_brain*)> >("construct ::pong::cpu_brain", &::construct<::pong::cpu_brain>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::cpu_brain*)> >("destruct ::pong::cpu_brain", &::destruct<::pong::cpu_brain>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::bounce_event*(::pong::bounce_event*)> >("construct ::pong::bounce_event", &::construct<::pong::bounce_event>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::bounce_event*)> >("destruct ::pong::bounce_event", &::destruct<::pong::bounce_event>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::previous_transform_2d*(::pong::previous_transform_2d*)> >("construct ::pong::previous_transform_2d", &::construct<::pong::previous_transform_2d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::previous_transform_2d*)> >("destruct ::pong::previous_transform_2d", &::destruct<::pong::previous_transform_2d>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::sprite_quad*(::pong::sprite_quad*)> >("construct ::pong::sprite_quad", &::construct<::pong::sprite_quad>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::sprite_quad*)> >("destruct ::pong::sprite_quad", &::destruct<::pong::sprite_quad>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::net_strip*(::pong::net_strip*)> >("construct ::pong::net_strip", &::construct<::pong::net_strip>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::net_strip*)> >("destruct ::pong::net_strip", &::destruct<::pong::net_strip>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::match_score*(::pong::match_score*)> >("construct ::pong::match_score", &::construct<::pong::match_score>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::match_score*)> >("destruct ::pong::match_score", &::destruct<::pong::match_score>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::rally_clock*(::pong::rally_clock*)> >("construct ::pong::rally_clock", &::construct<::pong::rally_clock>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::rally_clock*)> >("destruct ::pong::rally_clock", &::destruct<::pong::rally_clock>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::pong::match_report*(::pong::match_report*)> >("construct ::pong::match_report", &::construct<::pong::match_report>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::pong::match_report*)> >("destruct ::pong::match_report", &::destruct<::pong::match_report>);
}
