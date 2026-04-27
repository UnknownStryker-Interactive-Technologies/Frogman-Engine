#ifndef _FROGMAN_ENGINE_ASYNC_HPP_
#define _FROGMAN_ENGINE_ASYNC_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technology/Frogman-Engine-Apache-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>
#include <FE/framework/processors.hxx>
#include <FE/engine.hpp>




BEGIN_NAMESPACE(FE)


using task_handle = typename framework::task::handle;

class async // async API
{
public:
	_FE_FORCE_INLINE_ static void schedule_task(const framework::task& task_p) noexcept
	{
		FE::engine::get_engine().get_processors().schedule_task(task_p);
	}

	_FE_FORCE_INLINE_ static task_handle schedule_waitable_task(const framework::task& task_p) noexcept
	{
		FE::engine::get_engine().get_processors().schedule_waitable_task(task_p);
	}

	_FE_FORCE_INLINE_ static FE::int32 get_async_processor_count() noexcept
	{
		return FE::engine::get_engine().count_async_processors();
	}

public:
	async() = delete;
	~async() = delete;
};


END_NAMESPACE
#endif