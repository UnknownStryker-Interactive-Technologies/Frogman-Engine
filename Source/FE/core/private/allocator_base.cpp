// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/private/allocator_base.hpp>
#include <FE/core/clock.hpp>


std::atomic_int64_t FE::allocator_base::s_total_memory_util = 0;

thread_local FE::var::int64 FE::allocator_base::tl_s_thread_local_memory_util = 0;