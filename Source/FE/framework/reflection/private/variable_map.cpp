// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/framework/reflection/variable_map.hpp>




BEGIN_NAMESPACE(FE::framework::reflection)

typename variable_map::underlying_container*    variable_map::s_variable_map = nullptr;
typename variable_map::lock_type                variable_map::s_lock;

END_NAMESPACE