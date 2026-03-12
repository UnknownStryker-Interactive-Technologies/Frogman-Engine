#ifndef _FROGMAN_ENGINE_GARBAGE_COLLECTOR_
#define _FROGMAN_ENGINE_GARBAGE_COLLECTOR_
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

#include <boost/fiber/fiber.hpp>
#include <boost/thread/thread.hpp>
#include <FE/framework/processors.hxx>
#include <FE/world.hpp>

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>

#include <absl/container/node_hash_set.h>

#include <vector>




BEGIN_NAMESPACE(FE::internal)


struct death_note
{
	FE::farray<class FE::archetype_base*, 511> _entity_kill_list;
	FE::farray<class FE::component_base*, 511> _component_kill_list;
	FE::farray<class FE::component_base*, 511> _cyclic_component_reference_list;
};

struct gc_metadata_stack
{
	FE::smart_ptr<class ECS::gc_metadata, FE::RefType::_Observer> _metadata;
	typename ECS::gc_metadata::member_component_list_type::iterator _member_components_iterator;
};

class garbage_collector : public FE::internal::world::observer_base
{
	using base_type = FE::internal::world::observer_base;

	FE::internal::ECS::gc_root_getter m_root_getter;
	var::uint64 m_batch_count;
	var::float64 m_delta_ms;
	FE::framework::internal::processors::fiber_stack_allocator m_fiber_stack_allocator;
	std::atomic_bool m_should_terminate;
	boost::fibers::fiber m_garbage_collector;

	boost::thread m_garbage_investigator;
	boost::mutex m_mutex;
	boost::condition_variable m_condition_variable;

	std::vector<gc_metadata_stack, FE::cache_aligned_allocator<gc_metadata_stack>> m_metadata_stack;
	absl::node_hash_set<class FE::component_base*,
						FE::hash<class FE::component_base*>,
						std::equal_to<class FE::component_base*>,
						FE::cache_aligned_allocator<class FE::component_base*>> m_visited_components;

	FE::smart_ptr<death_note, FE::RefType::_Owner> m_death_note;

public:
	garbage_collector(FE::smart_ptr<FE::world, FE::RefType::_Observer> context_p, FE::uint64 batch_count_p, FE::size fiber_stack_size_p = FE::one_MiB) noexcept;
	virtual ~garbage_collector() noexcept override;

	_FE_FORCE_INLINE_ FE::float64 delta_ms() const noexcept { return m_delta_ms; }

private:
	static void __gc_main(garbage_collector* const gc_p) noexcept;
	// this routine runs periodically in a separate thread.
	static void __run_investigator(garbage_collector* const gc_p) noexcept; 
};


END_NAMESPACE
#endif