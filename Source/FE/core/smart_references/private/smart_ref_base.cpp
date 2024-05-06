// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/smart_references/private/smart_ref_base.hpp>




BEGIN_NAMESPACE(FE::internal::smart_ref)

thread_local typename smart_ref_base::ref_table_type smart_ref_base::tl_s_ref_table;
thread_local typename smart_ref_base::ref_table_recycler_type smart_ref_base::tl_s_ref_table_recycler;

END_NAMESPACE