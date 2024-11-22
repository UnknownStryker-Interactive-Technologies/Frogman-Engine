// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/game_state_base.hpp>




BEGIN_NAMESPACE(FE::framework)

game_state_base::game_state_base() noexcept
{
}

void game_state_base::on_construction()
{
}

void game_state_base::on_destruction()
{
}

void game_state_base::tick(_FE_MAYBE_UNUSED_ FE::float64 delta_second_p)
{
}

END_NAMESPACE