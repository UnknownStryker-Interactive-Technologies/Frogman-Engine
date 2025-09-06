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
#include <FE/framework/ECS.hpp>
#include <FE/framework/reflection.hpp>




class terrorist : public FE::archetype_base
{
public:
	terrorist() = default;
	~terrorist() = default;
};

class AK47 : public FE::archetype_base
{
public:
	AK47() = default;
	~AK47() = default;
};

class ak_ammo : public FE::archetype_base
{
public:
	ak_ammo() = default;
	~ak_ammo() = default;
};

class ak_magazine : public FE::component_base
{
	FE_PROPERTY(_rounds);
	std::vector<ak_ammo> _rounds;
public:
	ak_magazine() = default;
	~ak_magazine() = default;
};




class player : public FE::archetype_base
{
public:
	player() noexcept = default;
	virtual ~player() noexcept override = default;
};

class health : public FE::component_base
{
public:
	FE_PROPERTY(_health);
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
	FE_PROPERTY(_damage);
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
	FE_PROPERTY(_speed);
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




struct config
{
	FE_STRUCT();
};

class config2
{
};