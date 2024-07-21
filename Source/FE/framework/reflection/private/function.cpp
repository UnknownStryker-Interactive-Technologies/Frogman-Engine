// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/framework/reflection/function.hpp>




BEGIN_NAMESPACE(FE::framework::reflection)

typename function::function_pool_type     function::s_function_pool(function::initial_size_in_bytes);
typename function::underlying_container*  function::s_task_map = nullptr;
typename function::lock_type              function::s_lock;

END_NAMESPACE