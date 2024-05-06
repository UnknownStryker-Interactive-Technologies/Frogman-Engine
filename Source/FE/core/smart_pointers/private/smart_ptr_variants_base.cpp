// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/smart_pointers/private/smart_ptr_variants_base.hpp>




BEGIN_NAMESPACE(FE::internal::smart_ptr)

thread_local typename smart_ptr_variants_base::ref_table_type smart_ptr_variants_base::tl_s_ref_table;
thread_local typename smart_ptr_variants_base::ref_table_recycler_type smart_ptr_variants_base::tl_s_ref_table_recycler;

END_NAMESPACE