#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
//
//#include <FE/framework/ECS.hpp>
//#include<FE/framework/archetype_base.hpp>
//
//
//
//
//class object : public FE::archetype_base
//{
//	int health = 0;
//public:
//	object() noexcept = default;
//	object(int health) noexcept
//		: health(health)
//	{
//	}
//	virtual ~object() noexcept override = default;
//};
//
//class speed : public FE::component_base
//{
//public:
//	speed() noexcept = default;
//	speed(var::float32 speed) noexcept
//		: _speed(speed)
//	{
//	}
//	virtual ~speed() noexcept override = default;
//	var::float32 _speed;
//};
//
//
//TEST(ECS, instantiate_entity)
//{
//	FE::ECS ecs;
//
//	FE::entity<object> e = ecs.instanciate_entity<object>("TestEntity", 100);
//
//	ecs.add_component<speed>(e, "TestComponent", 0.0f);
//
//	ecs.destruct_entity(e);
//}

#include <robin_hood.h>

TEST(robin_hood, int_key)
{
	robin_hood::unordered_map<int, const char *> map;
	map.emplace(1, "one");
}