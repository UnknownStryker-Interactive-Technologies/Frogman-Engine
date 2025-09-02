#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <FE/framework/ECS.hpp>
#include <FE/framework/archetype_base.hpp>

#include <FE/framework/reflection.hpp>
#include <FE/framework/framework.hpp>

#include "entt/single_include/entt/entt.hpp"




class player : public FE::archetype_base
{
public:
	player() noexcept = default;
	virtual ~player() noexcept override = default;
};

class health : public FE::component_base
{
public:
	int _health;

	health() noexcept = default;

	health(int health) noexcept
		: _health(health)
	{
	}

	virtual ~health() noexcept override = default;
};

class weapon : public FE::component_base
{
public:
	var::float32 _damage;

	weapon() noexcept = default;

	weapon(var::float32 damage) noexcept
		: _damage(damage)
	{
	}

	virtual ~weapon() noexcept override = default;
};

class speed : public FE::component_base
{
public:
	var::float32 _speed;

	speed() noexcept = default;

	speed(var::float32 speed) noexcept
		: _speed(speed)
	{
	}

	virtual ~speed() noexcept override = default;
};

class damage_system : public FE::system_base
{
public:
	damage_system() noexcept = default;

	virtual ~damage_system() noexcept override = default;

	virtual void operator()(FE::component_base* const component_p) override
	{
		health* l_health = FE::polymorphic_cast<health*>(component_p);
		assert(l_health != nullptr);

		l_health->_health -= 10;
	}
};

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
	FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<FE::ECS, FE::entity<player>(FE::ASCII* const) > >("player", &::FE::ECS::instanciate_entity<player>);
	FE::task_base* constructor = FE::framework::framework_base::get_framework()
		.get_framework().get_method_reflection().retrieve("player");

	FE::entity<player> handle;
	FE::arguments<FE::ASCII*> construction_arg;
	construction_arg._first = "player CDO";
	(*constructor)(&ecs, &handle, &construction_arg);


	FE::framework::framework_base::get_framework().get_method_reflection().register_task< ::FE::cpp_style_task<FE::ECS, void(const FE::entity<player>&) > >("~player", &::FE::ECS::destruct_entity<player>);
	FE::task_base* destructor = FE::framework::framework_base::get_framework()
		.get_framework().get_method_reflection().retrieve("~player");

	FE::arguments<const FE::entity<player>&> destruction_arg;
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