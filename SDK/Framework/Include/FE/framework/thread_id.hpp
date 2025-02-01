#ifndef _FE_FRAMEWORK_TASK_HPP_
#define _FE_FRAMEWORK_TASK_HPP_
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
#include <FE/clock.hpp>

// std::shared_ptr
#include <memory>

// std::pmr::synchronized_pool_resource
#include <memory_resource>

// std::mutex, std::lock_guard
#include <mutex>

// std::thread
#include <thread>

#include <taskflow.hpp>

// boost::function
#include <boost/function.hpp>

#include <FE/synchronized_access.hxx>




BEGIN_NAMESPACE(FE::framework)


FE::uint16 get_current_thread_id() noexcept;


END_NAMESPACE
#endif