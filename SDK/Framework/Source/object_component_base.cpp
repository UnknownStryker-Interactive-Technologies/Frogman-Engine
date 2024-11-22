#include <FE/framework/object_component_base.hpp>




BEGIN_NAMESPACE(FE::framework)

object_component_base::object_component_base() noexcept
{
}

void object_component_base::on_construction()
{
}

void object_component_base::on_destruction()
{
}

void object_component_base::tick(_FE_MAYBE_UNUSED_ FE::float64 delta_second_p)
{
}

END_NAMESPACE