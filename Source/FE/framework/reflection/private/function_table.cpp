// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/framework/reflection/function_table.hpp>




BEGIN_NAMESPACE(FE::framework::reflection)

typename function_table::function_pool_type     function_table::s_function_pool(function_table::initial_size_in_bytes);
typename function_table::underlying_container*  function_table::s_task_map = nullptr;
typename function_table::lock_type              function_table::s_lock;

END_NAMESPACE