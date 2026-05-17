/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/framework/type_info.hxx>




thread_local std::shared_ptr<std::pmr::monotonic_buffer_resource> FE::framework::reflection::type_info::tl_s_resource = std::make_shared<std::pmr::monotonic_buffer_resource>();
thread_local typename FE::framework::reflection::type_info::table_type FE::framework::reflection::type_info::tl_s_type_information(FE::framework::reflection::type_info::tl_s_resource.get());
std::atomic_uint64_t FE::framework::reflection::type_info::s_type_id_counter = 0;