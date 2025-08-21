#include <gtest/gtest.h>
#include <benchmark/benchmark.h>

#include <FE/framework/ECS.hpp>
#include<FE/framework/archetype_base.hpp>




class object : public FE::archetype_base
{
	int health = 0;
public:
	object() noexcept = default;
	object(int health) noexcept
		: health(health)
	{
	}
	~object() noexcept = default;
};