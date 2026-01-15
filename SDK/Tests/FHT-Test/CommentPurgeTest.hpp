/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
//#include <FE/framework/ECS.hxx>
//#include <FE/framework/reflection.hxx>
//
//
//
//
//class terrorist : public FE::archetype_base
//{
//public:
//	terrorist() = default;
//	terrorist(FE::framework::ECS& ecs_p) 
//		: FE::archetype_base(ecs_p)
//	{}
//	~terrorist() = default;
//};
//
//class AK47 : public FE::archetype_base
//{
//public:
//	AK47() = default;
//	AK47(FE::framework::ECS& ecs_p) 
//		: FE::archetype_base(ecs_p)
//	{}
//	~AK47() = default;
//};
//
//class ak_ammo : public FE::archetype_base
//{
//public:
//	ak_ammo() {};
//	ak_ammo(FE::framework::ECS& ecs_p) 
//		: FE::archetype_base(ecs_p)
//	{}
//	~ak_ammo() = default;
//};
//
//class ak_magazine : public FE::component_base
//{
//	FE_PROPERTY(_rounds);
//	std::vector<ak_ammo> _rounds;
//public:
//	ak_magazine() : _rounds() {};
//	~ak_magazine() = default;
//};
//
//
//
//
//class player : public FE::archetype_base
//{
//public:
//	player() = default;
//	player(FE::framework::ECS& ecs_p) noexcept
//		: FE::archetype_base(ecs_p)
//	{ }
//	virtual ~player() noexcept override = default;
//};
//
//class health : public FE::component_base
//{
//public:
//	FE_PROPERTY(_health);
//	int _health;
//
//	health() noexcept = default;
//	health(int health) noexcept
//		: _health(health)
//	{
//	}
//
//	~health() noexcept = default;
//};
//
//class weapon : public FE::component_base
//{
//	class AK47* fake;
//public:
//	FE_PROPERTY(_damage);
//	var::float32 _damage;
//
//	weapon() noexcept = default;
//
//	weapon(var::float32 damage) noexcept
//		: _damage(damage)
//	{
//	}
//
//	~weapon() noexcept = default;
//};
//
//class speed : public FE::component_base
//{
//public:
//	FE_PROPERTY(_speed);
//	var::float32 _speed;
//
//	speed() noexcept = default;
//
//	speed(var::float32 speed) noexcept
//		: _speed(speed)
//	{
//	}
//
//	~speed() noexcept = default;
//};


//void take_damage(FE::component_base* const component_p) noexcept 
//FE_SYSTEM(FE::SystemCallPhase::_WorldBegin, health) 
//{
//	(component_p);
//}
//
//void take_damage2(FE::component_base* const component_p) FE_SYSTEM(FE::SystemCallPhase::_WorldBegin, health) noexcept
//{
//	(component_p);
//}
//
//void take_damage3(FE::component_base* const component_p) FE_SYSTEM(FE::SystemCallPhase::_WorldBegin, health) noexcept;
//
//void take_damage4(FE::component_base* const component_p) noexcept FE_SYSTEM(FE::SystemCallPhase::_WorldBegin, health);

//void faulty_system_registration(FE::component_base* const component_p) noexcept FE_SYSTEM(FE::SystemCallPhase::_WorldBegin, player)
//{
//	(component_p);
//}
//
//void faulty_system_registration2(FE::component_base* const component_p) noexcept FE_SYSTEM(health, FE::SystemCallPhase::_WorldBegin)
//{
//	(component_p);
//}