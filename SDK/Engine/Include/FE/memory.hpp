#ifndef _FROGMAN_ENGINE_MEMORY_HPP_
#define _FROGMAN_ENGINE_MEMORY_HPP_
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
#include <FE/prerequisites.hxx>
#include <FE/memory.hxx>
#include <FE/framework/framework.hxx>




BEGIN_NAMESPACE(FE)


std::pmr::memory_resource* get_large_thread_local_memory_resource() noexcept;
std::pmr::memory_resource* get_thread_local_memory_resource() noexcept;


END_NAMESPACE
#endif