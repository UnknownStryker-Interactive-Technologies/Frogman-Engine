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
#include <FE/framework/game_processor.hxx>
#include <FE/framework/world.hxx>

#include <GLFW/glfw3.h>




FE::framework::game_processor::game_processor(FE::world& world_p, FE::size fiber_stack_size_p) noexcept
	:	m_scheduler(),
		m_world(&world_p),
		m_game_delta_ms(0.0),

		m_should_terminate(false),
	
		m_gc_batch_count(),
		m_metadata_stack(),
		m_visited_components(),
		m_death_note(),
		m_gc_delta_ms(0.0),
		m_mutex(),
		m_condition_variable()
{
	m_scheduler.create_fiber(fiber_stack_size_p);
	m_scheduler.create_fiber(fiber_stack_size_p);

	m_death_note = FE::make_owner<death_note>( FE::framework::framework_base::get_framework().get_memory_resource() ); // page alignment is guaranteed.
	m_metadata_stack.reserve(8192); // this is an arbitrary number, but it should be sufficient for most use cases. We can always resize the vector if we need more space.
	m_visited_components.reserve(8192); // this is an arbitrary number, but it should be sufficient for most use cases. We can always resize the set if we need more space.

	FE::fiber_scheduler::tl_s_this_thread_fiber_scheduler = &m_scheduler; // Set the thread-local pointer to this fiber scheduler instance.
}


class __this_pointer : public FE::component_base
{
public:
	FE::framework::game_processor* _this = nullptr;
};

void FE::framework::game_processor::__game_main_loop(FE::component_base* const arg_p) noexcept
{
	__this_pointer* const l_this = FE::polymorphic_cast<__this_pointer* const>(arg_p);
	FE_ASSERT(l_this != nullptr, "Assertion Failure: the argument argument conversion has failed due to a type-mismatch.");

	FE::clock l_delta_clock;
	FE::world_tag l_world_tag = l_this->_this->m_world->get_world_tag();
	typename FE::framework::ECS::component_table& l_component_table = l_this->_this->m_world->m_ecs.m_component_table;

	static_assert(sizeof(SystemCallPhase) == sizeof(var::uint32));
	for (var::uint32 phase = (var::uint32)SystemCallPhase::_EngineInitialization; phase != (var::uint32)SystemCallPhase::_GameTick; ++phase)
	{
		for (auto& [component_typeid, sysvec] : FE::framework::framework_base::get_framework().get_method_reflection().get_systems(l_world_tag, (SystemCallPhase)phase))
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
	while (l_this->_this->m_should_terminate.load(std::memory_order_acquire) == false)
	{
		l_delta_clock.start_clock();

		glfwPollEvents();
		for (var::uint32 phase = (var::uint32)SystemCallPhase::_GameTick; phase != (var::uint32)SystemCallPhase::_PostRender; ++phase)
		{
			for (auto& [component_typeid, sysvec] : FE::framework::framework_base::get_framework().get_method_reflection().get_systems(l_world_tag, (SystemCallPhase)phase))
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
		l_this->_this->m_game_delta_ms = l_delta_clock.get_delta_milliseconds();
		// l_this->_this->m_scheduler.switch_fiber_context(); // Yield to the GC fiber
	}


	for (var::uint32 phase = (var::uint32)SystemCallPhase::_EntityCleanUp; phase != (var::uint32)SystemCallPhase::_EngineCleanUp; ++phase)
	{
		for (auto& [component_typeid, sysvec] : FE::framework::framework_base::get_framework().get_method_reflection().get_systems(l_world_tag, (SystemCallPhase)phase))
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


void FE::framework::game_processor::execute() noexcept
{
	__this_pointer l_this;
	l_this._this = this;

	// FE::framework::game_processor::__game_main_loop(&l_this); // We can call the main loop directly since it will yield to the GC loop periodically, thus we don't need to schedule it as a task on the scheduler. This also simplifies the synchronization between the main loop and the GC loop, as they can directly access the shared data without worrying about task scheduling and synchronization issues.

	FE::task l_main_loop;
	l_main_loop._component = &l_this;
	l_main_loop._system = &FE::framework::game_processor::__game_main_loop;
	l_main_loop._task_type = TaskPriority::_Critical;

	FE::task l_gc_loop;
	l_gc_loop._component = &l_this;
	l_gc_loop._system = &FE::framework::game_processor::__gc_main;
	l_gc_loop._task_type = TaskPriority::_High;

	m_scheduler.schedule_task(l_main_loop);
	m_scheduler.schedule_task(l_gc_loop);

	_FE_MAYBE_UNUSED_ const int l_result = m_scheduler.execute();
	FE_ASSERT(l_result == _FE_SUCCEEDED_);

	std::thread l_investigator(&game_processor::__run_investigator, &l_this);
	l_investigator.join();
}

void FE::framework::game_processor::terminate() noexcept
{
	m_should_terminate.store(true, std::memory_order_release);
}




void FE::framework::game_processor::__gc_main(FE::component_base* const gc_p) noexcept
{
	__this_pointer* const l_this = FE::polymorphic_cast<__this_pointer* const>(gc_p);
	FE_ASSERT(l_this != nullptr, "Assertion Failure: the argument argument conversion has failed due to a type-mismatch.");

	var::boolean l_has_consumed_all = true;
	while (l_this->_this->m_should_terminate.load(std::memory_order_acquire) == false)
	{
		if (l_has_consumed_all == true)
		{
			std::unique_lock<std::mutex> l_lock(l_this->_this->m_mutex);
			l_this->_this->m_condition_variable.notify_all();
			l_has_consumed_all = false;
		}

		if (l_this->_this->m_death_note != nullptr)
		{
			var::uint64 l_batch_count = 0;

			for (FE::component_base* const component : l_this->_this->m_death_note->_component_kill_list)
			{
				FE_ASSERT(component != nullptr, "Assertion failed: component to be destroyed must not be a null pointer.");
				FE_ASSERT(component->m_metadata != nullptr, "Assertion failed: component to be destroyed must have valid metadata.");
				FE_ASSERT(component->m_metadata->_gc_metadata != nullptr, "Assertion failed: component to be destroyed must have valid GC metadata.");
				FE_ASSERT(component->m_metadata->_gc_metadata->_garbage_class == FE::internal::GarbageClass::_CertGarbage, "Assertion failed: component to be destroyed must be classified as certain garbage.");
				FE_ASSERT(component->m_metadata->_group != nullptr, "Assertion failed: component group iterator must not be null.");
				component->m_metadata->_group->remove_component(component->m_metadata->_index); // the component pointer becomes a dangling pointer.

				++l_batch_count;
				if (l_batch_count >= l_this->_this->m_gc_batch_count)
				{
					l_batch_count = 0;
					l_this->_this->m_scheduler.switch_fiber_context(); // Yield to the main loop fiber to avoid blocking the game thread for too long.
				}
			}

			for (FE::archetype_base* const entity : l_this->_this->m_death_note->_entity_kill_list)
			{
				FE_ASSERT(entity != nullptr, "Assertion failed: entity to be destroyed must not be a null pointer.");
				FE_ASSERT(entity->m_group != nullptr, "Assertion failed: entity group iterator must not be null.");
				entity->m_group->remove_entity(entity->m_index); // the entity pointer becomes a dangling pointer.

				++l_batch_count;
				if (l_batch_count >= l_this->_this->m_gc_batch_count)
				{
					l_batch_count = 0;
					l_this->_this->m_scheduler.switch_fiber_context(); // Yield to the main loop fiber to avoid blocking the game thread for too long.
				}
			}

			// Check if the _ProbGarbages are reachable from any entity root. If reachable, then they are still in-use, thus mark them as _NotAGarbage.
			for (auto entities : l_this->_this->m_world->m_ecs.m_gc_root._entity_roots)
			{
				for (auto& entity : *entities)
				{
					for (FE::component_base* const component : l_this->_this->m_death_note->_cyclic_component_reference_list)
					{
						FE_ASSERT(component != nullptr, "Assertion failed: component to be destroyed must not be a null pointer.");
						FE_ASSERT(component->m_metadata != nullptr, "Assertion failed: component to be destroyed must have valid metadata.");
						FE_ASSERT(component->m_metadata->_gc_metadata != nullptr, "Assertion failed: component to be destroyed must have valid GC metadata.");
						FE_ASSERT(component->m_metadata->_gc_metadata->_garbage_class == FE::internal::GarbageClass::_ProbGarbage, "Assertion failed: component to be destroyed must be classified as probable garbage.");
						FE_ASSERT(component->m_metadata->_group != nullptr, "Assertion failed: component group iterator must not be null.");

						if (entity->m_component_view_table.find(component->m_metadata->_type_hash) != entity->m_component_view_table.end())
						{	// reachable from an entity root, thus not a garbage.
							component->m_metadata->_gc_metadata->_garbage_class.store(FE::internal::GarbageClass::_NotAGarbage, std::memory_order_release);
						}

						++l_batch_count;
						if (l_batch_count >= l_this->_this->m_gc_batch_count)
						{
							l_batch_count = 0;
							l_this->_this->m_scheduler.switch_fiber_context(); // Yield to the main loop fiber to avoid blocking the game thread for too long.
						}
					}
				}
			}

			// Kill all the remaining _ProbGarbages.
			for (FE::component_base* const component : l_this->_this->m_death_note->_cyclic_component_reference_list)
			{
				FE_ASSERT(component != nullptr, "Assertion failed: component to be destroyed must not be a null pointer.");
				FE_ASSERT(component->m_metadata != nullptr, "Assertion failed: component to be destroyed must have valid metadata.");
				FE_ASSERT(component->m_metadata->_gc_metadata != nullptr, "Assertion failed: component to be destroyed must have valid GC metadata.");
				FE_ASSERT(component->m_metadata->_group != nullptr, "Assertion failed: component group iterator must not be null.");

				if (component->m_metadata->_gc_metadata->_garbage_class.load(std::memory_order_acquire) == FE::internal::GarbageClass::_ProbGarbage)
				{
					component->m_metadata->_group->remove_component(component->m_metadata->_index); // the component pointer becomes a dangling pointer.
					++l_batch_count;
				}

				if (l_batch_count >= l_this->_this->m_gc_batch_count)
				{
					l_batch_count = 0;
					l_this->_this->m_scheduler.switch_fiber_context(); // Yield to the main loop fiber to avoid blocking the game thread for too long.
				}
			}

			l_has_consumed_all = true;
		}

		l_this->_this->m_scheduler.switch_fiber_context(); // Yield to the main loop fiber to avoid blocking the game thread for too long.
	}
}

void FE::framework::game_processor::__run_investigator(FE::component_base* const gc_p) noexcept // this routine runs periodically in a separate thread.
{
	__this_pointer* const l_this = FE::polymorphic_cast<__this_pointer* const>(gc_p);
	FE_ASSERT(l_this != nullptr, "Assertion Failure: the argument argument conversion has failed due to a type-mismatch.");

	while (l_this->_this->m_should_terminate.load(std::memory_order_acquire) == false)
	{
		// sleep until gets notified to run a reachability analysis.
		std::unique_lock<std::mutex> l_lock(l_this->_this->m_mutex);
		l_this->_this->m_condition_variable.wait(l_lock);

		FE::smart_ptr<death_note, FE::RefType::_Owner> l_death_note = std::move(l_this->_this->m_death_note);

		// Mark garbages with zero observer count as certain garbage.
		for (auto entities : l_this->_this->m_world->m_ecs.m_gc_root._entity_roots)
		{
			for (auto& entity : *entities)
			{
				if (entity.observer_count() == 0)
				{
					l_death_note->_entity_kill_list.emplace_back(entity.operator->());
					if (l_death_note->_entity_kill_list.size() == l_death_note->_entity_kill_list.capacity())
					{
						goto BreakEntityRA; // Break Entity Reachability Analysis
					}
				}
			}
		}
	BreakEntityRA:


		for (auto components : l_this->_this->m_world->m_ecs.m_gc_root._component_roots) // Mark garbage with zero observer count as certain garbage.
		{
			for (auto& component : *components)
			{
				if (component.observer_count() == 0)
				{
					FE_ASSERT(component->m_metadata != nullptr, "Assertion failed: component to be classified must have valid metadata.");
					FE_ASSERT(component->m_metadata->_gc_metadata != nullptr, "Assertion failed: component to be classified must have valid GC metadata.");

					component->m_metadata->_gc_metadata->_garbage_class.store(FE::internal::GarbageClass::_CertGarbage, std::memory_order_release);
					l_death_note->_component_kill_list.emplace_back(component.operator->());
					if (l_death_note->_component_kill_list.size() == l_death_note->_component_kill_list.capacity())
					{
						goto BreakComponentRA; // Break Component Reachability Analysis
					}
				}
			}
		}
	BreakComponentRA:


		for (auto components : l_this->_this->m_world->m_ecs.m_gc_root._component_roots)
		{
			for (auto& component : *components)
			{
				FE_ASSERT(component->m_metadata != nullptr, "Assertion failed: component to be classified must have valid metadata.");
				FE_ASSERT(component->m_metadata->_gc_metadata != nullptr, "Assertion failed: component to be classified must have valid GC metadata.");

				if (component->m_metadata->_gc_metadata->_garbage_class == FE::internal::GarbageClass::_CertGarbage)
				{
					continue; // already classified as certain garbage.
				}

				// push the first root node onto the stack.
				l_this->_this->m_metadata_stack.emplace_back(component->m_metadata->_gc_metadata,
					component->m_metadata->_gc_metadata->_member_components.begin()); // the iterator is pushed onto the stack to preserve the iteration progress.

				// DFS the graph
				while (l_this->_this->m_metadata_stack.size() > 0)
				{
					while (l_this->_this->m_metadata_stack.back()._member_components_iterator != l_this->_this->m_metadata_stack.back()._metadata->_member_components.end())
					{
						auto& l_current_iterator = l_this->_this->m_metadata_stack.back()._member_components_iterator; // get reference to the current iterator.
						FE::component_base* const l_current_member_component = (*l_current_iterator); // get the member component pointer.

						if (l_this->_this->m_visited_components.find(l_current_member_component) == l_this->_this->m_visited_components.end())
						{	// mark as not visited if not found.
							l_this->_this->m_visited_components.emplace(l_current_member_component); // mark it as visited.
						}
						else
						{	// cyclic reference detected, suspect memory leaks. The object will be marked as _CertGarbage later if it is unreachable from any entity root.
							l_current_member_component->m_metadata->_gc_metadata->_garbage_class.store(FE::internal::GarbageClass::_ProbGarbage, std::memory_order_release);
							l_death_note->_cyclic_component_reference_list.emplace_back(l_current_member_component);
							if (l_death_note->_cyclic_component_reference_list.size() == l_death_note->_cyclic_component_reference_list.capacity())
							{
								goto BreakComponentCRA; // Break Entity Reachability Analysis
							}
							l_this->_this->m_metadata_stack.pop_back(); // pop the duplicate node.
							continue;
						}

						// push onto the stack for the next traversal.
						l_this->_this->m_metadata_stack.emplace_back(l_current_member_component->m_metadata->_gc_metadata,
							l_current_member_component->m_metadata->_gc_metadata->_member_components.begin());
						++l_current_iterator; // move to the next member component.
						continue;
					}

					l_this->_this->m_metadata_stack.pop_back();
				}
			}
		}
	BreakComponentCRA:

		// Clear them all for the next run.
		l_this->_this->m_metadata_stack.clear();
		l_this->_this->m_visited_components.clear();
		l_this->_this->m_death_note = std::move(l_death_note); // atomically submit the death note to the garbage collector.
	}
}
