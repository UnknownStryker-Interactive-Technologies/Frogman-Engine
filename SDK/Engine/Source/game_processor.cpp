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
#include <FE/game_processor.hxx>
#include <boost/fiber/all.hpp>




FE::internal::game_processor::game_processor(FE::smart_ptr<FE::world, FE::RefType::_Observer> context_p, var::uint64 gc_batch_count_p, FE::size fiber_stack_size_p) noexcept
	:	m_world_proxy_getter(context_p),
		m_fiber_stack_allocator(fiber_stack_size_p),
		m_should_terminate(false),

		m_game_fiber(),
		m_delta_ms(0.0),

		m_gc_fiber(),
		m_gc_delta_ms(0.0),
		m_gc_iter_per_frame(gc_batch_count_p)
{}

void FE::internal::game_processor::run() noexcept
{
	boost::fibers::use_scheduling_algorithm<boost::fibers::algo::round_robin>();
	m_game_fiber = boost::fibers::fiber(std::allocator_arg, m_fiber_stack_allocator, &game_processor::__game_main, this);
	m_gc_fiber = boost::fibers::fiber(std::allocator_arg, m_fiber_stack_allocator, &game_processor::__gc_main, this);

	if (m_game_fiber.joinable())
	{
		m_game_fiber.join();
	}

	if (m_gc_fiber.joinable())
	{
		m_gc_fiber.join();
	}
}

void FE::internal::game_processor::shutdown() noexcept
{
	m_should_terminate.store(true, std::memory_order_release);
}


void FE::internal::game_processor::__game_main(game_processor* const host_p) noexcept
{
	FE_ASSERT(host_p != nullptr, "Assertion failure: host_p cannot be null.");
	FE::clock l_delta_clock;

	while (host_p->m_should_terminate.load(std::memory_order_acquire) == false)
	{
		l_delta_clock.start_clock();
		glfwPollEvents();

		l_delta_clock.end_clock();
		host_p->m_delta_ms = l_delta_clock.get_delta_milliseconds();
		boost::this_fiber::yield();
	}
}

void FE::internal::game_processor::__gc_main(game_processor* const host_p) noexcept
{
	(host_p);
}

