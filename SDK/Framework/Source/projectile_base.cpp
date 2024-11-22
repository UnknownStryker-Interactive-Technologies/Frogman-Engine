// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/projectile_base.hpp>




BEGIN_NAMESPACE(FE::framework)

projectile_base::projectile_base() noexcept
{
}

void projectile_base::on_construction()
{
}

void projectile_base::on_destruction()
{
}

void projectile_base::on_spawn(_FE_MAYBE_UNUSED_ glm::dvec3 world_coordinate_p)
{
}

void projectile_base::on_despawn()
{
}

void projectile_base::on_activate()
{
}

void projectile_base::on_deactivate()
{
}

void projectile_base::tick(_FE_MAYBE_UNUSED_ FE::float64 delta_second_p)
{
}

END_NAMESPACE