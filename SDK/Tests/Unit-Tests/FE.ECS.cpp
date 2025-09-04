#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <FE/framework/ECS.hpp>
#include <FE/framework/archetype_base.hpp>

#include <FE/framework/reflection.hpp>
#include <FE/framework/framework.hpp>

#include "entt/single_include/entt/entt.hpp"
#include "FE.ECS.hpp"




static 	FE::ECS ecs;


TEST(ECS, instantiate_entity)
{
	FE::entity<player> e = ecs.instanciate_entity<player>("TestEntity");
	ecs.destruct_entity(e);
}

TEST(ECS, add_component)
{
	FE::entity<player> e = ecs.instanciate_entity<player>("TestEntity");

	FE::component_view<speed> l_speed = ecs.add_component<speed>(e, 1.0f);
	FE::component_view<health> l_health = ecs.add_component<health>(e, 100);
	FE::component_view<weapon> l_weapon = ecs.add_component<weapon>(e, 10.0f);

	ecs.destruct_entity(e);
}

TEST(ECS, remove_component)
{
	FE::entity<player> e = ecs.instanciate_entity<player>("TestEntity");

	FE::component_view<speed> l_speed = ecs.add_component<speed>(e, 1.0f);
	FE::component_view<health> l_health = ecs.add_component<health>(e, 100);
	FE::component_view<weapon> l_weapon = ecs.add_component<weapon>(e, 10.0f);

	ecs.remove_component<speed>(e);
	ecs.remove_component<health>(e);
	ecs.remove_component<weapon>(e);

	ecs.destruct_entity(e);
}


TEST(ECS, deserialize)
{
	FE::init file;
	file["Archetypes"]["names"].emplace_back("player");
	file["Components"]["names"].emplace_back("health");
	file["Components"]["names"].emplace_back("weapon");
	file["Components"]["names"].emplace_back("speed");
	file["Systems"]["names"].emplace_back("damage_system");

	FE::ECS ecs2(file);
}

TEST(ECS, reflection_combo)
{
	FE::task_base* constructor = FE::framework::framework_base::get_framework()
		.get_framework().get_method_reflection().retrieve("::terrorist");

	FE::entity<terrorist> handle;
	FE::arguments<FE::ASCII*> construction_arg;
	construction_arg._first = "terrorist CDO";
	(*constructor)(&ecs, &handle, &construction_arg);




	FE::task_base* add_component_health = FE::framework::framework_base::get_framework()
		.get_framework().get_method_reflection().retrieve("::health");

	FE::component_view<health> health_view;
	FE::arguments<FE::archetype_base*> health_arg;
	health_arg._first = handle.operator->();
	(*add_component_health)(&ecs, &health_view, &health_arg);




	FE::task_base* remove_component_ak_magazine = FE::framework::framework_base::get_framework()
		.get_framework().get_method_reflection().retrieve("~::health");

	FE::arguments<FE::archetype_base*> removal_arg;
	removal_arg._first = handle.operator->();
	(*remove_component_ak_magazine)(&ecs, nullptr, &removal_arg);




	FE::task_base* destructor = FE::framework::framework_base::get_framework()
		.get_framework().get_method_reflection().retrieve("~::terrorist");

	FE::arguments<const FE::entity<terrorist>&> destruction_arg;
	destruction_arg._first = handle;
	(*destructor)(&ecs, nullptr, &destruction_arg);
}


// write benchmark against EnTT
void ECS_instantiate_entity(benchmark::State& state_p) noexcept
{
	for (auto _ : state_p)
	{
		FE::entity<player> e = ecs.instanciate_entity<player>("TestEntity");
		ecs.destruct_entity(e);
	}
}
BENCHMARK(ECS_instantiate_entity);

void ECS_add_component(benchmark::State& state_p) noexcept
{
	FE::entity<player> e = ecs.instanciate_entity<player>("TestEntity");
	for (auto _ : state_p)
	{
		FE::component_view<speed> l_speed = ecs.add_component<speed>(e, 1.0f);
		FE::component_view<health> l_health = ecs.add_component<health>(e, 100);
		FE::component_view<weapon> l_weapon = ecs.add_component<weapon>(e, 10.0f);

		ecs.remove_component<speed>(e);
		ecs.remove_component<health>(e);
		ecs.remove_component<weapon>(e);
	}
	ecs.destruct_entity(e);
}
BENCHMARK(ECS_add_component)->Iterations(10000);


void EnTT_instantiate_entity(benchmark::State& state_p) noexcept
{
	entt::registry registry;
	for (auto _ : state_p)
	{
		auto entity = registry.create();
		registry.destroy(entity);
	}
}
BENCHMARK(EnTT_instantiate_entity);

void EnTT_add_component(benchmark::State& state_p) noexcept
{
	entt::registry registry;
	auto entity = registry.create();
	for (auto _ : state_p)
	{
		registry.emplace<speed>(entity, 1.0f);
		registry.emplace<health>(entity, 100);
		registry.emplace<weapon>(entity, 10.0f);
		registry.remove<speed>(entity);
		registry.remove<health>(entity);
		registry.remove<weapon>(entity);
	}
	registry.destroy(entity);
}
BENCHMARK(EnTT_add_component)->Iterations(10000);