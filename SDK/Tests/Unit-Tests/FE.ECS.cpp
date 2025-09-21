#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <FE/framework/ECS.hpp>
#include <FE/framework/archetype_base.hpp>

#include <FE/framework/reflection.hpp>
#include <FE/framework/framework.hpp>

#include "FE.ECS.hpp"




static std::pmr::unsynchronized_pool_resource pool;
static 	FE::ECS ecs(&pool);

void take_damage(FE::component_base* const component_p) noexcept
{
	health* l_health = FE::polymorphic_cast<health*>(component_p);
	assert(l_health != nullptr);
	l_health->_health -= 10;
}




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

	// These steps are what will happen inside the game engine framework; the engine will load system pointers, and iterate over their target components.
	FE::system take_damage = ecs.find_system("::take_damage");
	take_damage(l_health.operator->());
	EXPECT_EQ(l_health->_health, 90);

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

TEST(ECS, attach_and_detach_components)
{
	FE::entity<player> e = ecs.instanciate_entity<player>("TestEntity");

	FE::component_view<speed> l_speed = ecs.add_component<speed>(e, 1.0f);
	FE::component_view<health> l_health = ecs.add_component<health>(e, 100);
	FE::component_view<weapon> l_weapon = ecs.add_component<weapon>(e, 10.0f);

	FE::entity<player> e2 = ecs.instanciate_entity<player>("TestEntity2");

	ecs.attatch_component(e2, ecs.detach_component<speed>(e));
	ecs.attatch_component(e2, ecs.detach_component<health>(e));
	ecs.attatch_component(e2, ecs.detach_component<weapon>(e));

	ecs.attatch_component(e, ecs.detach_component<speed>(e2));
	ecs.attatch_component(e, ecs.detach_component<health>(e2));
	ecs.attatch_component(e, ecs.detach_component<weapon>(e2));

	ecs.destruct_entity(e);
	ecs.destruct_entity(e2);
}


TEST(ECS, deserialize)
{
	FE::init file;
	file["Archetypes"]["names"].emplace_back("player");
	file["Components"]["names"].emplace_back("health");
	file["Components"]["names"].emplace_back("weapon");
	file["Components"]["names"].emplace_back("speed");
	file["Systems"]["names"].emplace_back("damage_system");

	FE::ECS ecs2(file, &pool);
}

TEST(ECS, entity_serialization)
{
	FE::entity<player> e = ecs.instanciate_entity<player>("TestEntity");
	FE::component_view<speed> l_speed = ecs.add_component<speed>(e, 1.0f);
	FE::component_view<health> l_health = ecs.add_component<health>(e, 100);
	FE::component_view<weapon> l_weapon = ecs.add_component<weapon>(e, 10.0f);

	FE::serialized_entity buffer = ecs.serialize_entity(e);

	FE::entity<player> ne = ecs.instanciate_entity<player>("New Entity");
	FE::component_view<speed> l_speed2 = ecs.add_component<speed>(ne);
	FE::component_view<health> l_health2 = ecs.add_component<health>(ne);
	FE::component_view<weapon> l_weapon2 = ecs.add_component<weapon>(ne);
	ecs.deserialize_entity(buffer, ne);

	EXPECT_EQ(l_speed->_speed, l_speed2->_speed);
	EXPECT_EQ(l_health->_health, l_health2->_health);
	EXPECT_EQ(l_weapon->_damage, l_weapon2->_damage);

	ecs.destruct_entity(e);
	ecs.destruct_entity(ne);
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
	FE::arguments<FE::entity<FE::archetype_base>> health_arg;
	health_arg._first = handle;
	(*add_component_health)(&ecs, &health_view, &health_arg);




	FE::task_base* remove_component_ak_magazine = FE::framework::framework_base::get_framework()
		.get_framework().get_method_reflection().retrieve("~::health");

	FE::arguments<FE::entity<FE::archetype_base>> removal_arg;
	removal_arg._first = handle;
	(*remove_component_ak_magazine)(&ecs, nullptr, &removal_arg);




	FE::task_base* destructor = FE::framework::framework_base::get_framework()
		.get_framework().get_method_reflection().retrieve("~::terrorist");

	FE::arguments<FE::entity<FE::archetype_base>> destruction_arg;
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
BENCHMARK(ECS_instantiate_entity)->Iterations(100);

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
BENCHMARK(ECS_add_component)->Iterations(100);