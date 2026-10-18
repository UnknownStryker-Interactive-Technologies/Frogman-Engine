#include <FE/memory.hpp>
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/framework/framework.hxx>




std::pmr::memory_resource* FE::get_large_thread_local_memory_resource() noexcept
{
	return FE::framework::framework_base::get_framework().get_large_memory_resource();
}

std::pmr::memory_resource* FE::get_thread_local_memory_resource() noexcept
{
	return FE::framework::framework_base::get_framework().get_memory_resource();
}

std::pmr::memory_resource* FE::get_concurrent_memory_resource() noexcept
{
	return FE::framework::framework_base::get_framework().get_concurrent_memory_resource();
}