#include <FE/garbage_collector.hxx>
#include <FE/memory.hpp>
#include <FE/framework/ECS.hxx>




FE::internal::garbage_collector::garbage_collector(FE::smart_ptr<FE::world, FE::RefType::_Observer> context_p, FE::uint64 batch_count_p, FE::size fiber_stack_size_p) noexcept
	:	base_type(context_p),
		m_root_getter(get_ecs()),
		m_batch_count(batch_count_p),
		m_delta_ms(0.0),
		m_fiber_stack_allocator(fiber_stack_size_p),
		m_should_terminate(false),
		m_garbage_collector(),

		m_garbage_investigator(),
		m_mutex(),
		m_condition_variable(),
		m_metadata_stack(),
		
		m_death_note()
{
	m_death_note = FE::make_owner<death_note>(FE::memory::get_thread_local_memory_resource()); // page alignment is guaranteed.
	m_garbage_collector = boost::fibers::fiber(std::allocator_arg, m_fiber_stack_allocator, &garbage_collector::__gc_main, this);
	m_garbage_investigator = boost::thread(&garbage_collector::__run_investigator, this);
	m_metadata_stack.reserve(8192); // preallocate some space to avoid frequent reallocations.
	m_visited_components.reserve(8192); // preallocate some space to avoid frequent reallocations.
}

FE::internal::garbage_collector::~garbage_collector() noexcept
{
	m_garbage_collector.join();
	m_garbage_investigator.join();
}


void FE::internal::garbage_collector::__gc_main(garbage_collector* const gc_p) noexcept
{
	var::boolean l_has_consumed_all = true;
	while (gc_p->m_should_terminate.load(std::memory_order_acquire) == false)
	{
		if (l_has_consumed_all == true)
		{
			boost::unique_lock<boost::mutex> l_lock(gc_p->m_mutex);
			gc_p->m_condition_variable.notify_all();
			l_has_consumed_all = false;
		}

		if (gc_p->m_death_note != nullptr)
		{
			var::uint64 l_batch_count = 0;
	
			for (FE::component_base* const component : gc_p->m_death_note->_component_kill_list)
			{
				FE_ASSERT(component != nullptr, "Assertion failed: component to be destroyed must not be a null pointer.");
				FE_ASSERT(component->m_metadata != nullptr, "Assertion failed: component to be destroyed must have valid metadata.");
				FE_ASSERT(component->m_metadata->_gc_metadata != nullptr, "Assertion failed: component to be destroyed must have valid GC metadata.");
				FE_ASSERT(component->m_metadata->_gc_metadata->_garbage_class == FE::internal::GarbageClass::_CertGarbage, "Assertion failed: component to be destroyed must be classified as certain garbage.");
				FE_ASSERT(component->m_metadata->_group != nullptr, "Assertion failed: component group iterator must not be null.");
				component->m_metadata->_group->remove_component(component->m_metadata->_index); // the component pointer becomes a dangling pointer.

				++l_batch_count;
				if (l_batch_count >= gc_p->m_batch_count)
				{
					l_batch_count = 0;
					boost::this_fiber::yield();
				}
			}

			for (FE::archetype_base* const entity : gc_p->m_death_note->_entity_kill_list)
			{
				FE_ASSERT(entity != nullptr, "Assertion failed: entity to be destroyed must not be a null pointer.");
				FE_ASSERT(entity->m_group != nullptr, "Assertion failed: entity group iterator must not be null.");
				entity->m_group->remove_entity(entity->m_index); // the entity pointer becomes a dangling pointer.

				++l_batch_count;
				if (l_batch_count >= gc_p->m_batch_count)
				{
					l_batch_count = 0;
					boost::this_fiber::yield();
				}
			}

			// Check if the _ProbGarbages are reachable from any entity root. If reachable, then they are still in-use, thus mark them as _NotAGarbage.
			for (auto entities : gc_p->m_root_getter.get_gc_root()._entity_roots)
			{
				for (auto& entity : *entities)
				{
					for (FE::component_base* const component : gc_p->m_death_note->_cyclic_component_reference_list)
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
						if (l_batch_count >= gc_p->m_batch_count)
						{
							l_batch_count = 0;
							boost::this_fiber::yield();
						}
					}
				}
			}

			// Kill all the remaining _ProbGarbages.
			for (FE::component_base* const component : gc_p->m_death_note->_cyclic_component_reference_list)
			{
				FE_ASSERT(component != nullptr, "Assertion failed: component to be destroyed must not be a null pointer.");
				FE_ASSERT(component->m_metadata != nullptr, "Assertion failed: component to be destroyed must have valid metadata.");
				FE_ASSERT(component->m_metadata->_gc_metadata != nullptr, "Assertion failed: component to be destroyed must have valid GC metadata.");
				FE_ASSERT(component->m_metadata->_group != nullptr, "Assertion failed: component group iterator must not be null.");
				
				if (component->m_metadata->_gc_metadata->_garbage_class.load(std::memory_order_acquire) == GarbageClass::_ProbGarbage)
				{
					component->m_metadata->_group->remove_component(component->m_metadata->_index); // the component pointer becomes a dangling pointer.
					++l_batch_count;
				}

				if (l_batch_count >= gc_p->m_batch_count)
				{
					l_batch_count = 0;
					boost::this_fiber::yield();
				}
			}

			l_has_consumed_all = true;
		}

		boost::this_fiber::yield();
	}
}

void FE::internal::garbage_collector::__run_investigator(garbage_collector* const gc_p) noexcept // this routine runs periodically in a separate thread.
{
	while (gc_p->m_should_terminate.load(std::memory_order_acquire) == false)
	{
		// sleep until gets notified to run a reachability analysis.
		boost::unique_lock<boost::mutex> l_lock(gc_p->m_mutex);
		gc_p->m_condition_variable.wait(l_lock);

		FE::smart_ptr<death_note, FE::RefType::_Owner> l_death_note = std::move(gc_p->m_death_note);

		// Mark garbages with zero observer count as certain garbage.
		for (auto entities : gc_p->m_root_getter.get_gc_root()._entity_roots)
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


		for (auto components : gc_p->m_root_getter.get_gc_root()._component_roots) // Mark garbages with zero observer count as certain garbage.
		{
			for (auto& component : *components)
			{
				if (component.observer_count() == 0)
				{
					FE_ASSERT(component->m_metadata != nullptr, "Assertion failed: component to be classified must have valid metadata.");
					FE_ASSERT(component->m_metadata->_gc_metadata != nullptr, "Assertion failed: component to be classified must have valid GC metadata.");

					component->m_metadata->_gc_metadata->_garbage_class.store(GarbageClass::_CertGarbage, std::memory_order_release);
					l_death_note->_component_kill_list.emplace_back(component.operator->());
					if (l_death_note->_component_kill_list.size() == l_death_note->_component_kill_list.capacity())
					{
						goto BreakComponentRA; // Break Component Reachability Analysis
					}
				}
			}
		}
BreakComponentRA:

		
		for (auto components : gc_p->m_root_getter.get_gc_root()._component_roots)
		{
			for (auto& component : *components)
			{
				FE_ASSERT(component->m_metadata != nullptr, "Assertion failed: component to be classified must have valid metadata.");
				FE_ASSERT(component->m_metadata->_gc_metadata != nullptr, "Assertion failed: component to be classified must have valid GC metadata.");

				if (component->m_metadata->_gc_metadata->_garbage_class == GarbageClass::_CertGarbage)
				{
					continue; // already classified as certain garbage.
				}

				// push the first root node onto the stack.
				gc_p->m_metadata_stack.emplace_back(component->m_metadata->_gc_metadata, 
													component->m_metadata->_gc_metadata->_member_components.begin()); // the iterator is pushed onto the stack to preserve the iteration progress.
			
				// DFS the graph
				while (gc_p->m_metadata_stack.size() > 0)
				{
					while (gc_p->m_metadata_stack.back()._member_components_iterator != gc_p->m_metadata_stack.back()._metadata->_member_components.end())
					{
						auto& l_current_iterator = gc_p->m_metadata_stack.back()._member_components_iterator; // get reference to the current iterator.
						FE::component_base* const l_current_member_component = (*l_current_iterator); // get the member component pointer.

						if (gc_p->m_visited_components.find(l_current_member_component) == gc_p->m_visited_components.end())
						{	// mark as not visited if not found.
							gc_p->m_visited_components.emplace(l_current_member_component); // mark it as visited.
						}
						else
						{	// cyclic reference detected, suspect memory leaks. The object will be marked as _CertGarbage later if it is unreachable from any entity root.
							l_current_member_component->m_metadata->_gc_metadata->_garbage_class.store(GarbageClass::_ProbGarbage, std::memory_order_release);
							l_death_note->_cyclic_component_reference_list.emplace_back(l_current_member_component);
							if (l_death_note->_cyclic_component_reference_list.size() == l_death_note->_cyclic_component_reference_list.capacity())
							{
								goto BreakComponentCRA; // Break Entity Reachability Analysis
							}
							gc_p->m_metadata_stack.pop_back(); // pop the duplicate node.
							continue;
						}

						// push onto the stack for the next traversal.
						gc_p->m_metadata_stack.emplace_back(l_current_member_component->m_metadata->_gc_metadata,
															l_current_member_component->m_metadata->_gc_metadata->_member_components.begin());
						++l_current_iterator; // move to the next member component.
						continue;
					}

					gc_p->m_metadata_stack.pop_back();
				}
			}
		}
BreakComponentCRA:

		// Clear them all for the next run.
		gc_p->m_metadata_stack.clear();
		gc_p->m_visited_components.clear();
		gc_p->m_death_note = std::move(l_death_note); // atomically submit the death note to the garbage collector.
	}
}
