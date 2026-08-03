#ifndef _FE_GAME_PROCESSOR_HXX_
#define _FE_GAME_PROCESSOR_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.hxx>

#include <FE/framework/ECS.hxx>
#include <FE/framework/fiber.hxx>
#include <FE/framework/smart_ptr.hxx>

#include <absl/container/node_hash_set.h>

#include <mutex>
#include <vector>




CLASS_FORWARD_DECLARATION(FE, engine)
CLASS_FORWARD_DECLARATION(FE, world)
CLASS_FORWARD_DECLARATION(FE, mutex)
CLASS_FORWARD_DECLARATION(FE, shared_mutex)


BEGIN_NAMESPACE(FE::framework)


struct gc_metadata
{
	FE::smart_ptr<class FE::internal::ECS::gc_metadata, FE::RefType::_Observer> _metadata;
	typename FE::internal::ECS::gc_metadata::member_component_list_type::iterator _member_components_iterator;
};

struct death_note
{
	FE::farray<class FE::archetype_base*, 511> _entity_kill_list;
	FE::farray<class FE::component_base*, 511> _component_kill_list;
	FE::farray<class FE::component_base*, 511> _cyclic_component_reference_list;
};


class game_processor
{
	friend class FE::engine;
	friend class FE::mutex;
	friend class FE::shared_mutex;

	FE::fiber_scheduler m_scheduler;
	FE::world* m_world;
	var::float64 m_game_delta_ms;


	std::atomic_bool m_should_terminate;


	var::uint64 m_gc_batch_count;
	std::vector<gc_metadata, FE::cache_aligned_allocator<gc_metadata>> m_metadata_stack;

	absl::node_hash_set<class FE::component_base*,
		absl::lts_20260107::DefaultHashContainerHash<class FE::component_base*>,
		absl::lts_20260107::DefaultHashContainerEq<class FE::component_base*>,
		FE::cache_aligned_allocator<class FE::component_base*>> m_visited_components;

	FE::smart_ptr<death_note, FE::RefType::_Owner> m_death_note;
	var::float64 m_gc_delta_ms;
	std::mutex m_mutex;
	std::condition_variable m_condition_variable;
	

public:
	game_processor(FE::world& world_p, FE::size fiber_stack_size_p = FE::one_MiB) noexcept;
	~game_processor() noexcept;

	void execute() noexcept;
	void terminate() noexcept;

	_FE_FORCE_INLINE_ FE::float64 get_delta_milliseconds() const noexcept { return m_game_delta_ms; }

private:
	static void __game_main_loop(FE::component_base* const arg_p) noexcept;

	static void __gc_main(FE::component_base* const arg_p) noexcept;

	// this routine runs periodically in a separate thread.
	static void __run_investigator(FE::component_base* const arg_p) noexcept;
};


END_NAMESPACE
#endif