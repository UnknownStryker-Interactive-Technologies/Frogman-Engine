#ifndef _FROGMAN_ENGINE_GAME_PROCESSOR_HXX_
#define _FROGMAN_ENGINE_GAME_PROCESSOR_HXX_
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
#include <FE/framework/ECS.hxx>
#include <FE/framework/processors.hxx>
#include <FE/world.hpp>




BEGIN_NAMESPACE(FE::internal)


class game_processor
{
	FE::internal::world::proxy_getter m_world_proxy_getter;
	FE::framework::internal::processors::fiber_stack_allocator m_fiber_stack_allocator;
	std::atomic_bool m_should_terminate;

	boost::fibers::fiber m_game_fiber;
	var::float64 m_delta_ms;

	boost::fibers::fiber m_gc_fiber;
	var::float64 m_gc_delta_ms;
	var::uint64 m_gc_iter_per_frame;

public:
	game_processor(FE::smart_ptr<FE::world, FE::RefType::_Observer> context_p, var::uint64 gc_iter_count_p = 30, FE::size fiber_stack_size_p = FE::one_MiB) noexcept;
	~game_processor() noexcept = default;

public:
	void run() noexcept;
	void shutdown() noexcept;

private:
	_FE_FORCE_INLINE_ FE::float64 get_delta_milliseconds() const noexcept { return m_delta_ms; }
	_FE_FORCE_INLINE_ FE::float64 get_gc_delta_milliseconds() const noexcept { return m_gc_delta_ms; }

	static void __game_main(game_processor* const host_p) noexcept;
	static void __gc_main(game_processor* const host_p) noexcept;

};


END_NAMESPACE
#endif