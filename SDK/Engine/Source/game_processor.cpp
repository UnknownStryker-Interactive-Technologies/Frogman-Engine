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

#include <FE/async.hpp>
#include <FE/engine.hpp>
#include <FE/memory.hpp>

#include <boost/fiber/all.hpp>




FE::internal::game_processor::game_processor(FE::smart_ptr<FE::world, FE::RefType::_Observer> context_p, FE::size fiber_stack_size_p) noexcept
	:	base_type(context_p),
		m_component_table_getter(get_ecs()),
		m_fiber_stack_allocator(fiber_stack_size_p),
		m_should_terminate(false),

		m_game_fiber(),
		m_delta_ms(0.0)
{}


void FE::internal::game_processor::run() noexcept
{
	boost::fibers::use_scheduling_algorithm<boost::fibers::algo::round_robin>();
	m_game_fiber = boost::fibers::fiber(std::allocator_arg, m_fiber_stack_allocator, &game_processor::__game_main, this);

	if (m_game_fiber.joinable())
	{
		m_game_fiber.join();
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
	FE::world_tag l_world_tag = FE::engine::get_game_instance().get_current_world()->get_world_tag();
	typename FE::framework::ECS::component_table& l_component_table = host_p->m_component_table_getter.get_component_table();

	static_assert( sizeof(SystemCallPhase) == sizeof(var::uint32) );
	for (var::uint32 phase = (var::uint32)SystemCallPhase::_EngineInitialization; phase != (var::uint32)SystemCallPhase::_GameTick; ++phase)
	{
		for (auto& [component_typeid, sysvec] : FE::engine::get_engine().get_method_reflection().get_systems(l_world_tag, (SystemCallPhase)phase))
		{
			auto l_component_table_iterator = l_component_table.find(component_typeid);
			if (l_component_table_iterator == l_component_table.end())
			{
				continue;
			}

			for (FE::system sys : sysvec)
			{
				FE_ASSERT(sys != nullptr, "Assertion failure: system function pointer cannot be null.");
				for (auto components = l_component_table_iterator->second._second.rbegin(); components != l_component_table_iterator->second._second.rend(); ++components)
				{
					for (FE::component& component : *components)
					{
						sys(component.operator->()); // Call the system on the component.
					}
				}
			}
		}
	}


	// We won't be able to efficiently parallelize this process if the asnyc processors are busy.
	while (host_p->m_should_terminate.load(std::memory_order_acquire) == false)
	{
		l_delta_clock.start_clock();

		glfwPollEvents();
		for (var::uint32 phase = (var::uint32)SystemCallPhase::_GameTick; phase != (var::uint32)SystemCallPhase::_PostRender; ++phase)
		{
			for (auto& [component_typeid, sysvec] : FE::engine::get_engine().get_method_reflection().get_systems(l_world_tag, (SystemCallPhase)phase))
			{
				auto l_component_table_iterator = l_component_table.find(component_typeid);
				if (l_component_table_iterator == l_component_table.end())
				{
					continue;
				}

				for (FE::system sys : sysvec)
				{
					FE_ASSERT(sys != nullptr, "Assertion failure: system function pointer cannot be null.");
					for (auto components = l_component_table_iterator->second._second.rbegin(); components != l_component_table_iterator->second._second.rend(); ++components)
					{
						for (FE::component& component : *components)
						{
							sys(component.operator->()); // Call the system on the component.
						}
					}
				}
			}
		}

		l_delta_clock.end_clock();
		host_p->m_delta_ms = l_delta_clock.get_delta_milliseconds();
		boost::this_fiber::yield(); // yield to GC.
	}


	for (var::uint32 phase = (var::uint32)SystemCallPhase::_EntityCleanUp; phase != (var::uint32)SystemCallPhase::_EngineCleanUp; ++phase)
	{
		for (auto& [component_typeid, sysvec] : FE::engine::get_engine().get_method_reflection().get_systems(l_world_tag, (SystemCallPhase)phase))
		{
			auto l_component_table_iterator = l_component_table.find(component_typeid);
			if (l_component_table_iterator == l_component_table.end())
			{
				continue;
			}

			for (FE::system sys : sysvec)
			{
				FE_ASSERT(sys != nullptr, "Assertion failure: system function pointer cannot be null.");
				for (auto components = l_component_table_iterator->second._second.rbegin(); components != l_component_table_iterator->second._second.rend(); ++components)
				{
					for (FE::component& component : *components)
					{
						sys(component.operator->()); // Call the system on the component.
					}
				}
			}
		}
	}
}