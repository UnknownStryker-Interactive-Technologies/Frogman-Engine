// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/reflection/type_info.hpp>

std::pmr::monotonic_buffer_resource FE::framework::reflection::internal::type_info::name::s_resource;
typename FE::framework::reflection::type_info::table_type FE::framework::reflection::type_info::s_type_information;
typename FE::framework::reflection::type_info::lock_type FE::framework::reflection::type_info::m_lock;