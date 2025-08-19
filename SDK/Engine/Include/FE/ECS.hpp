#ifndef _FE_ECS_HPP_
#define _FE_ECS_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.h>
#include <FE/pair.hxx>
#include <FE/pool/block_pool.hxx>
#include <FE/pool/scalable_pool.hxx>

#include <FE/framework/reflection.hpp>

#include <array>
#include <list>
#include <memory> // std::shared_ptr, and std::weak_ptr
#include <memory_resource> // std::pmr::polymorphic_allocator
#include <utility> // std::forward
#include <vector>

// robin_hood::unordered_map
#include <robin_hood.h>




BEGIN_NAMESPACE(FE)


END_NAMESPACE
#endif