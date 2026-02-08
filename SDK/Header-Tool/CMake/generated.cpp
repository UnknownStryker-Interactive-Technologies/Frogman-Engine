// Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved. 
#include <FE/framework/reflection/private/load_reflection_data.hxx> 
#include <FE/framework.hxx> 
#include <C:\Users\leeho\Documents\GitHub\Frogman-Engine\SDK\Tests\FHT-Test\ReflextreeParseTest.hpp>



void load_reflection_data()
{


    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::final*(::final*)> >("construct ::final", &::std::construct_at<::final>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::final*)> >("destruct ::final", &::std::destroy_at<::final>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::GlobalTransform2D*(::GlobalTransform2D*)> >("construct ::GlobalTransform2D", &::std::construct_at<::GlobalTransform2D>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::GlobalTransform2D*)> >("destruct ::GlobalTransform2D", &::std::destroy_at<::GlobalTransform2D>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::GlobalVelocity*(::GlobalVelocity*)> >("construct ::GlobalVelocity", &::std::construct_at<::GlobalVelocity>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::GlobalVelocity*)> >("destruct ::GlobalVelocity", &::std::destroy_at<::GlobalVelocity>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::GlobalRigidBody*(::GlobalRigidBody*)> >("construct ::GlobalRigidBody", &::std::construct_at<::GlobalRigidBody>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::GlobalRigidBody*)> >("destruct ::GlobalRigidBody", &::std::destroy_at<::GlobalRigidBody>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Component*(::FE::Test::Component*)> >("construct ::FE::Test::Component", &::std::construct_at<::FE::Test::Component>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Component*)> >("destruct ::FE::Test::Component", &::std::destroy_at<::FE::Test::Component>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::SpriteComponent*(::FE::Test::SpriteComponent*)> >("construct ::FE::Test::SpriteComponent", &::std::construct_at<::FE::Test::SpriteComponent>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::SpriteComponent*)> >("destruct ::FE::Test::SpriteComponent", &::std::destroy_at<::FE::Test::SpriteComponent>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::PhysicsComponent*(::FE::Test::PhysicsComponent*)> >("construct ::FE::Test::PhysicsComponent", &::std::construct_at<::FE::Test::PhysicsComponent>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::PhysicsComponent*)> >("destruct ::FE::Test::PhysicsComponent", &::std::destroy_at<::FE::Test::PhysicsComponent>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::EntityBase*(::FE::Test::EntityBase*)> >("construct ::FE::Test::EntityBase", &::std::construct_at<::FE::Test::EntityBase>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::EntityBase*)> >("destruct ::FE::Test::EntityBase", &::std::destroy_at<::FE::Test::EntityBase>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::EnemyEntity*(::FE::Test::EnemyEntity*)> >("construct ::FE::Test::EnemyEntity", &::std::construct_at<::FE::Test::EnemyEntity>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::EnemyEntity*)> >("destruct ::FE::Test::EnemyEntity", &::std::destroy_at<::FE::Test::EnemyEntity>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::final*(::FE::Test::final*)> >("construct ::FE::Test::final", &::std::construct_at<::FE::Test::final>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::final*)> >("destruct ::FE::Test::final", &::std::destroy_at<::FE::Test::final>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::NonCopyableEntity*(::FE::Test::NonCopyableEntity*)> >("construct ::FE::Test::NonCopyableEntity", &::std::construct_at<::FE::Test::NonCopyableEntity>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::NonCopyableEntity*)> >("destruct ::FE::Test::NonCopyableEntity", &::std::destroy_at<::FE::Test::NonCopyableEntity>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::EntityFactory*(::FE::Test::EntityFactory*)> >("construct ::FE::Test::EntityFactory", &::std::construct_at<::FE::Test::EntityFactory>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::EntityFactory*)> >("destruct ::FE::Test::EntityFactory", &::std::destroy_at<::FE::Test::EntityFactory>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::IUpdatable*(::FE::Test::IUpdatable*)> >("construct ::FE::Test::IUpdatable", &::std::construct_at<::FE::Test::IUpdatable>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::IUpdatable*)> >("destruct ::FE::Test::IUpdatable", &::std::destroy_at<::FE::Test::IUpdatable>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::IRenderable*(::FE::Test::IRenderable*)> >("construct ::FE::Test::IRenderable", &::std::construct_at<::FE::Test::IRenderable>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::IRenderable*)> >("destruct ::FE::Test::IRenderable", &::std::destroy_at<::FE::Test::IRenderable>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::UpdatableRenderable*(::FE::Test::UpdatableRenderable*)> >("construct ::FE::Test::UpdatableRenderable", &::std::construct_at<::FE::Test::UpdatableRenderable>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::UpdatableRenderable*)> >("destruct ::FE::Test::UpdatableRenderable", &::std::destroy_at<::FE::Test::UpdatableRenderable>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::IGameSystem*(::FE::Test::IGameSystem*)> >("construct ::FE::Test::IGameSystem", &::std::construct_at<::FE::Test::IGameSystem>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::IGameSystem*)> >("destruct ::FE::Test::IGameSystem", &::std::destroy_at<::FE::Test::IGameSystem>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::RenderSystem*(::FE::Test::RenderSystem*)> >("construct ::FE::Test::RenderSystem", &::std::construct_at<::FE::Test::RenderSystem>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::RenderSystem*)> >("destruct ::FE::Test::RenderSystem", &::std::destroy_at<::FE::Test::RenderSystem>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Position*(::FE::Test::Position*)> >("construct ::FE::Test::Position", &::std::construct_at<::FE::Test::Position>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Position*)> >("destruct ::FE::Test::Position", &::std::destroy_at<::FE::Test::Position>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Bounds*(::FE::Test::Bounds*)> >("construct ::FE::Test::Bounds", &::std::construct_at<::FE::Test::Bounds>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Bounds*)> >("destruct ::FE::Test::Bounds", &::std::destroy_at<::FE::Test::Bounds>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Size*(::FE::Test::Size*)> >("construct ::FE::Test::Size", &::std::construct_at<::FE::Test::Size>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Size*)> >("destruct ::FE::Test::Size", &::std::destroy_at<::FE::Test::Size>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Transform*(::FE::Test::Transform*)> >("construct ::FE::Test::Transform", &::std::construct_at<::FE::Test::Transform>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Transform*)> >("destruct ::FE::Test::Transform", &::std::destroy_at<::FE::Test::Transform>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::AnimatedTransform*(::FE::Test::AnimatedTransform*)> >("construct ::FE::Test::AnimatedTransform", &::std::construct_at<::FE::Test::AnimatedTransform>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::AnimatedTransform*)> >("destruct ::FE::Test::AnimatedTransform", &::std::destroy_at<::FE::Test::AnimatedTransform>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::GameConfig*(::FE::Test::GameConfig*)> >("construct ::FE::Test::GameConfig", &::std::construct_at<::FE::Test::GameConfig>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::GameConfig*)> >("destruct ::FE::Test::GameConfig", &::std::destroy_at<::FE::Test::GameConfig>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::EntityFlags*(::FE::Test::EntityFlags*)> >("construct ::FE::Test::EntityFlags", &::std::construct_at<::FE::Test::EntityFlags>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::EntityFlags*)> >("destruct ::FE::Test::EntityFlags", &::std::destroy_at<::FE::Test::EntityFlags>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Nested::NestedInputManager*(::FE::Test::Nested::NestedInputManager*)> >("construct ::FE::Test::Nested::NestedInputManager", &::std::construct_at<::FE::Test::Nested::NestedInputManager>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Nested::NestedInputManager*)> >("destruct ::FE::Test::Nested::NestedInputManager", &::std::destroy_at<::FE::Test::Nested::NestedInputManager>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Nested::NestedInput*(::FE::Test::Nested::NestedInput*)> >("construct ::FE::Test::Nested::NestedInput", &::std::construct_at<::FE::Test::Nested::NestedInput>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Nested::NestedInput*)> >("destruct ::FE::Test::Nested::NestedInput", &::std::destroy_at<::FE::Test::Nested::NestedInput>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Nested::Deep::DeepActionHandler*(::FE::Test::Nested::Deep::DeepActionHandler*)> >("construct ::FE::Test::Nested::Deep::DeepActionHandler", &::std::construct_at<::FE::Test::Nested::Deep::DeepActionHandler>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Nested::Deep::DeepActionHandler*)> >("destruct ::FE::Test::Nested::Deep::DeepActionHandler", &::std::destroy_at<::FE::Test::Nested::Deep::DeepActionHandler>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::Nested::Deep::DeepAction*(::FE::Test::Nested::Deep::DeepAction*)> >("construct ::FE::Test::Nested::Deep::DeepAction", &::std::construct_at<::FE::Test::Nested::Deep::DeepAction>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::Nested::Deep::DeepAction*)> >("destruct ::FE::Test::Nested::Deep::DeepAction", &::std::destroy_at<::FE::Test::Nested::Deep::DeepAction>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::EdgeCases::OuterSystem*(::FE::Test::EdgeCases::OuterSystem*)> >("construct ::FE::Test::EdgeCases::OuterSystem", &::std::construct_at<::FE::Test::EdgeCases::OuterSystem>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::EdgeCases::OuterSystem*)> >("destruct ::FE::Test::EdgeCases::OuterSystem", &::std::destroy_at<::FE::Test::EdgeCases::OuterSystem>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::EdgeCases::PointerManager*(::FE::Test::EdgeCases::PointerManager*)> >("construct ::FE::Test::EdgeCases::PointerManager", &::std::construct_at<::FE::Test::EdgeCases::PointerManager>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::EdgeCases::PointerManager*)> >("destruct ::FE::Test::EdgeCases::PointerManager", &::std::destroy_at<::FE::Test::EdgeCases::PointerManager>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::EdgeCases::OuterData*(::FE::Test::EdgeCases::OuterData*)> >("construct ::FE::Test::EdgeCases::OuterData", &::std::construct_at<::FE::Test::EdgeCases::OuterData>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::EdgeCases::OuterData*)> >("destruct ::FE::Test::EdgeCases::OuterData", &::std::destroy_at<::FE::Test::EdgeCases::OuterData>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::EdgeCases::ArrayContainer*(::FE::Test::EdgeCases::ArrayContainer*)> >("construct ::FE::Test::EdgeCases::ArrayContainer", &::std::construct_at<::FE::Test::EdgeCases::ArrayContainer>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::EdgeCases::ArrayContainer*)> >("destruct ::FE::Test::EdgeCases::ArrayContainer", &::std::destroy_at<::FE::Test::EdgeCases::ArrayContainer>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<::FE::Test::EdgeCases::CallbackContainer*(::FE::Test::EdgeCases::CallbackContainer*)> >("construct ::FE::Test::EdgeCases::CallbackContainer", &::std::construct_at<::FE::Test::EdgeCases::CallbackContainer>);
    ::FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::c_style_task<void(::FE::Test::EdgeCases::CallbackContainer*)> >("destruct ::FE::Test::EdgeCases::CallbackContainer", &::std::destroy_at<::FE::Test::EdgeCases::CallbackContainer>);

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::GlobalGameState >("::GlobalGameState",
    {
        { ::GlobalGameState::MainMenu, "MainMenu" },
        { ::GlobalGameState::InGame, "InGame" },
        { ::GlobalGameState::Paused, "Paused" },
        { ::GlobalGameState::GameOver
, "GameOver
" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::GlobalGameState2 >("::GlobalGameState2",
    {
        { ::GlobalGameState2::MainMenu, "MainMenu" },
        { ::GlobalGameState2::InGame, "InGame" },
        { ::GlobalGameState2::Paused, "Paused" },
        { ::GlobalGameState2::GameOver
, "GameOver
" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::GlobalGameState3 >("::GlobalGameState3",
    {
        { ::GlobalGameState3::MainMenu, "MainMenu" },
        { ::GlobalGameState3::InGame, "InGame" },
        { ::GlobalGameState3::Paused, "Paused" },
        { ::GlobalGameState3::GameOver
, "GameOver
" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::GlobalGameState4 >("::GlobalGameState4",
    {
        { ::GlobalGameState4::MainMenu, "MainMenu" },
        { ::GlobalGameState4::InGame, "InGame" },
        { ::GlobalGameState4::Paused, "Paused" },
        { ::GlobalGameState4::GameOver
, "GameOver
" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::GlobalGameState5 >("::GlobalGameState5",
    {
        { ::GlobalGameState5::MainMenu, "MainMenu" },
        { ::GlobalGameState5::InGame, "InGame" },
        { ::GlobalGameState5::Paused, "Paused" },
        { ::GlobalGameState5::GameOver
, "GameOver
" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::GlobalGameState6 >("::GlobalGameState6",
    {
        { ::GlobalGameState6::MainMenu, "MainMenu" },
        { ::GlobalGameState6::InGame, "InGame" },
        { ::GlobalGameState6::Paused, "Paused" },
        { ::GlobalGameState6::GameOver
, "GameOver
" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::GlobalEntityType >("::GlobalEntityType",
    {
        { ::GlobalEntityType::Player , "Player " },
        { ::GlobalEntityType::Enemy , "Enemy " },
        { ::GlobalEntityType::Projectile , "Projectile " },
        { ::GlobalEntityType::Pickup , "Pickup " }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::GlobalEntityType2 >("::GlobalEntityType2",
    {
        { ::GlobalEntityType2::Player , "Player " },
        { ::GlobalEntityType2::Enemy , "Enemy " },
        { ::GlobalEntityType2::Projectile , "Projectile " },
        { ::GlobalEntityType2::Pickup , "Pickup " }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::GlobalEntityType3 >("::GlobalEntityType3",
    {
        { ::GlobalEntityType3::Player , "Player " },
        { ::GlobalEntityType3::Enemy , "Enemy " },
        { ::GlobalEntityType3::Projectile , "Projectile " },
        { ::GlobalEntityType3::Pickup , "Pickup " }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::GlobalEntityType4 >("::GlobalEntityType4",
    {
        { ::GlobalEntityType4::Player , "Player " },
        { ::GlobalEntityType4::Enemy , "Enemy " },
        { ::GlobalEntityType4::Projectile , "Projectile " },
        { ::GlobalEntityType4::Pickup , "Pickup " }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::InputState >("::FE::Test::InputState",
    {
        { ::FE::Test::InputState::Released, "Released" },
        { ::FE::Test::InputState::Pressed, "Pressed" },
        { ::FE::Test::InputState::Held, "Held" },
        { ::FE::Test::InputState::JustReleased
, "JustReleased
" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::RenderLayer >("::FE::Test::RenderLayer",
    {
        { ::FE::Test::RenderLayer::Background , "Background " },
        { ::FE::Test::RenderLayer::Gameplay , "Gameplay " },
        { ::FE::Test::RenderLayer::UI , "UI " },
        { ::FE::Test::RenderLayer::Debug , "Debug " },
        { ::FE::Test::RenderLayer::PostProcess , "PostProcess " }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::EntityID >("::FE::Test::EntityID",
    {
        { ::FE::Test::EntityID::Invalid , "Invalid " },
        { ::FE::Test::EntityID::First , "First " },
        { ::FE::Test::EntityID::MaxEntities , "MaxEntities " }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::DefaultWorldTag >("::FE::Test::DefaultWorldTag",
    {
        { ::FE::Test::DefaultWorldTag::MainWorld
, "MainWorld
" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::ReservedEnum >("::FE::Test::ReservedEnum",
    
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::Nested::NestedInputType >("::FE::Test::Nested::NestedInputType",
    {
        { ::FE::Test::Nested::NestedInputType::Keyboard, "Keyboard" },
        { ::FE::Test::Nested::NestedInputType::Mouse, "Mouse" },
        { ::FE::Test::Nested::NestedInputType::Gamepad, "Gamepad" },
        { ::FE::Test::Nested::NestedInputType::Touch
, "Touch
" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::Nested::Deep::DeepActionType >("::FE::Test::Nested::Deep::DeepActionType",
    {
        { ::FE::Test::Nested::Deep::DeepActionType::Move, "Move" },
        { ::FE::Test::Nested::Deep::DeepActionType::Attack
, "Attack
" }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::EdgeCases::LargeIDType >("::FE::Test::EdgeCases::LargeIDType",
    {
        { ::FE::Test::EdgeCases::LargeIDType::None , "None " },
        { ::FE::Test::EdgeCases::LargeIDType::Small , "Small " },
        { ::FE::Test::EdgeCases::LargeIDType::Medium , "Medium " },
        { ::FE::Test::EdgeCases::LargeIDType::Large , "Large " },
        { ::FE::Test::EdgeCases::LargeIDType::Maximum , "Maximum " }
    });

    ::FE::framework::framework_base::get_framework().get_enum_reflection().register_enum_struct< ::FE::Test::EdgeCases::CharacterEnum >("::FE::Test::EdgeCases::CharacterEnum",
    {
        { ::FE::Test::EdgeCases::CharacterEnum::TypeA , "TypeA " },
        { ::FE::Test::EdgeCases::CharacterEnum::TypeB , "TypeB " },
        { ::FE::Test::EdgeCases::CharacterEnum::TypeC , "TypeC " }
    });
}
