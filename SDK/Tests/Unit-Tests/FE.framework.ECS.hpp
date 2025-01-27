#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.

#include <FE/framework/ECS.hpp>
#include <FE/framework/archetype_base.hpp>
#include <FE/framework/component_base.hpp>
#include <FE/framework/system_base.hpp>




class health_component : public FE::framework::component_base
{
public:
	var::int32 _health = 100;

public:
	health_component() noexcept = default;
	virtual ~health_component() noexcept override {};
};

class damage_system : public FE::framework::system_base
{
public:
	damage_system() noexcept = default;
	virtual ~damage_system() noexcept override = default;

	virtual void operator()(FE::framework::component_base* const component_p) override
	{
		health_component* const l_hp = dynamic_cast<health_component* const>(component_p);
		FE_ASSERT(l_hp != nullptr)
		
		l_hp->_health -= 10;
	}
};

class player : public FE::framework::archetype_base
{
	FE_CLASS(player);

	FE::framework::component_ptr<health_component> m_health;

public:
	player() noexcept = default;
	virtual ~player() noexcept override {};

	virtual void on_construction() override
	{
		this->m_health = this->attach_component_to<health_component>(this);
		this->m_health.lock()->subscribe_system<damage_system>();
	}

	virtual void on_destruction() override
	{
		this->m_health.lock()->unsubscribe_system<damage_system>();
		this->detach_component_of<health_component>(this);
	}
};

