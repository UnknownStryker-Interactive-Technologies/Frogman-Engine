#ifndef _FE_FREE_PROCESSORS_HPP_
#define _FE_FREE_PROCESSORS_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
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

#include <thread> // std::thread

#include <boost/fiber/fiber.hpp> // boost::fibers::fiber

#include <concurrent_queue.h> // concurrency::concurrent_queue will be replaced with an in-house implementation (FE::concurrent_queue<T, Allocator, Lock>).




BEGIN_NAMESPACE(FE::framework)


constexpr static FE::uint32 fibers_per_thread = 2;


class processor
{
	std::thread m_processor;
	boost::fibers::fiber m_fibers[fibers_per_thread];

public:
	processor() noexcept;
	~processor() noexcept;

	void run(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;

private:
	void __launch_fibers(FE::int32 argc_p, FE::ASCII** argv_p) noexcept;
};


class processors
{
	FE::int32 m_argc;
	FE::ASCII** m_argv;
	FE::uint32 m_software_thread_count;
	FE::uint32 m_fiber_count;

	std::unique_ptr<processor[]> m_processors;
	std::thread m_renderer_thread;
	boost::fibers::fiber m_game_fibers[fibers_per_thread];
	//concurrency::concurrent_queue<> m_render_target_queue;
public:
	processors(FE::int32 argc_p, FE::ASCII** argv_p, FE::uint32 software_thread_count_p) noexcept;
	~processors() noexcept;

	void run() noexcept;
};


END_NAMESPACE
#endif