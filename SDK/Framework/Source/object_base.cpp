// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/object_base.hpp>




BEGIN_NAMESPACE(FE::framework)

object_base::object_base() noexcept : m_position(), m_is_active(false)
{
}

void object_base::on_construction()
{
}

void object_base::on_destruction()
{
}

void object_base::on_spawn(_FE_MAYBE_UNUSED_ glm::dvec3 world_coordinate_p)
{
}

void object_base::on_despawn()
{
}

void object_base::on_activate()
{
}

void object_base::on_deactivate()
{
}

void object_base::tick(_FE_MAYBE_UNUSED_ FE::float64 delta_second_p)
{
}

END_NAMESPACE