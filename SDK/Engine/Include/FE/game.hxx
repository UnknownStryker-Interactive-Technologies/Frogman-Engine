#ifndef _FROGMAN_ENGINE_GAME_HPP_
#define _FROGMAN_ENGINE_GAME_HPP_
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
#include <FE/reflection.hpp>
#include <FE/memory.hpp>
#include <FE/input_contexts.hxx>

#include <absl/container/node_hash_map.h>




BEGIN_NAMESPACE(FE)


class game
{
	using input_context_table =
		absl::node_hash_map<input::context_id, input::context,
		absl::DefaultHashContainerHash<input::context_id>,
		absl::DefaultHashContainerEq<input::context_id>,
		std::pmr::polymorphic_allocator<std::pair<const input::context_id, input::context>>>;

	input_context_table m_input_contexts;

	std::pmr::vector<typename input_context_table::iterator> m_current_input_contexts;

public:
	game() noexcept;
	~game() noexcept;

	std::pmr::vector<typename input_context_table::iterator>& get_current_input_contexts() noexcept { return m_current_input_contexts; }

	input::context& register_input_context(input::context_id id_p) noexcept;
	input::context* find_input_context(input::context_id id_p) noexcept;
	FE::boolean is_input_context_registered(input::context_id id_p) const noexcept;

	void push_input_context(input::context_id id_p) noexcept;
	void pop_input_context() noexcept;        
	void pop_input_context(input::context_id id_p) noexcept; 
	void clear_input_contexts() noexcept;

	FE::boolean is_input_context_active(input::context_id id_p) const noexcept;
};


END_NAMESPACE
#endif