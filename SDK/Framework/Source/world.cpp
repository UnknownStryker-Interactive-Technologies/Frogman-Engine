// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/world.hpp>




BEGIN_NAMESPACE(FE::framework)

world::world() noexcept
{
}

world::~world() noexcept
{
}

void world::on_construction()
{
}

void world::on_destruction()
{
}

void world::tick(FE::float64 delta_second_p)
{
	(void)delta_second_p;
}

void world::serialize()
{
}

void world::deserialize()
{
}

END_NAMESPACE
