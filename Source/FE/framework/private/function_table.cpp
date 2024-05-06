#include <FE/framework/reflection/function_table.hpp>
#define _FUNCTION_TABLE_DEFUALT_SIZE_ 1024
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.


BEGIN_NAMESPACE(FE::framework)

std::unique_ptr<function_table::underlying_container> function_table::s_task_map;

std::mutex function_table::s_mutex;

END_NAMESPACE