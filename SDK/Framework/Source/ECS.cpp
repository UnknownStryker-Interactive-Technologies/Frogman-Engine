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
#include <FE/framework/ECS.hpp>

#include <FE/framework/framework.hpp> // FE::framework::framework_base::get_engine()
#include <FE/framework/task.hpp> // for get_current_thread_id();




BEGIN_NAMESPACE(FE::framework)


ECS::ECS() noexcept
{
}

ECS::~ECS() noexcept
{
}

ECS::ECS(FE::size max_concurrency_p) noexcept
    : m_archetype_arena(max_concurrency_p, FE::framework::framework_base::get_engine().get_memory_resource()),
	  m_component_arena(max_concurrency_p, FE::framework::framework_base::get_engine().get_memory_resource())
{
}

FE::scalable_pool<FE::PoolPageCapacity::_256MB>& ECS::get_archetype_arena() noexcept
{
	return this->m_archetype_arena[get_current_thread_id()];
}

FE::scalable_pool<FE::PoolPageCapacity::_256MB>& ECS::get_component_arena() noexcept
{
	return this->m_component_arena[get_current_thread_id()];
}

END_NAMESPACE