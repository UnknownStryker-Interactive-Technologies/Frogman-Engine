#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/ECS.hpp>
#include <FE/archetype_base.hpp>
#include <FE/component_base.hpp>
#include <FE/system_base.hpp>
#include <FE/engine.hpp>




class health_component : public FE::component_base
{
public:
	var::int32 _health = 100;

public:
	health_component() noexcept = default;
	virtual ~health_component() noexcept override {};
};

class damage_system : public FE::system_base
{
public:
	damage_system() noexcept = default;
	virtual ~damage_system() noexcept override = default;

	virtual void operator()(FE::component_base* const component_p) override
	{
		health_component* const l_hp = dynamic_cast<health_component* const>(component_p);
		FE_ASSERT(l_hp != nullptr)
		
		l_hp->_health -= 10;
	}
};

class player 
{
	FE_CLASS(player);

public:
	player() noexcept = default;
	 ~player() noexcept {};


};

