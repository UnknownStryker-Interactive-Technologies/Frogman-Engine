// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/pawn_base.hpp>




BEGIN_NAMESPACE(FE::framework)

pawn_base::pawn_base() noexcept
{
}

void pawn_base::on_construction()
{
}

void pawn_base::on_destruction()
{
}

void pawn_base::on_spawn(_FE_MAYBE_UNUSED_ glm::dvec3 world_coordinate_p)
{
}

void pawn_base::on_despawn()
{
}

void pawn_base::on_activate()
{
}

void pawn_base::on_deactivate()
{
}

void pawn_base::tick(_FE_MAYBE_UNUSED_ FE::float64 delta_second_p)
{
}

END_NAMESPACE
