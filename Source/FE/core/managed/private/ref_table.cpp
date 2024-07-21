// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/managed/private/ref_table.hpp>




BEGIN_NAMESPACE(FE::internal::managed)

thread_local FE::block_pool<sizeof(ref_block<void>), ref_table::ref_table_page_capacity> ref_table::tl_s_ref_block_pool;

END_NAMESPACE