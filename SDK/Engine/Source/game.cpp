#include <FE/game.hxx>
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
#include <FE/memory.hpp>

#include <algorithm>




namespace
{
	// flush_pressed_keys() calls the release callbacks, and a callback can push or pop an input context:
	// the contexts are flushed from a copy of the stack (contexts_p is taken by value).
	void flush_pressed_keys_of(const auto contexts_p) noexcept
	{
		for (auto l_context : contexts_p)
		{
			l_context->second.flush_pressed_keys();
		}
	}
}




FE::game::game() noexcept
	: m_current_input_contexts(FE::get_large_thread_local_memory_resource())
{
}

FE::game::~game() noexcept
{
}


FE::input::context& FE::game::register_input_context(FE::input::context_id id_p) noexcept
{
	return m_input_contexts[id_p];
}

FE::input::context* FE::game::find_input_context(FE::input::context_id id_p) noexcept
{
	auto l_iterator = m_input_contexts.find(id_p);

	if (l_iterator == m_input_contexts.end())
	{
		return nullptr;
	}
	return &(l_iterator->second);
}

FE::boolean FE::game::is_input_context_registered(FE::input::context_id id_p) const noexcept
{
	return m_input_contexts.find(id_p) != m_input_contexts.end();
}

void FE::game::push_input_context(FE::input::context_id id_p) noexcept
{
	auto l_iterator = m_input_contexts.find(id_p);

	if (l_iterator == m_input_contexts.end())
	{
		return;
	}

	// The real release of anything held right now would land on the new top of the stack,
	// so every layer that received the press has to be closed out first.
	flush_pressed_keys_of(m_current_input_contexts);

	// front() is the top of the stack.
	m_current_input_contexts.insert(m_current_input_contexts.begin(), &(*l_iterator));
}

void FE::game::pop_input_context() noexcept
{
	if (m_current_input_contexts.empty() == true)
	{
		return;
	}

	const auto l_top = m_current_input_contexts.front();
	flush_pressed_keys_of(m_current_input_contexts);

	// The flush may have changed the stack: remove the context that was on top, wherever it is now.
	const auto l_position = std::find(m_current_input_contexts.begin(), m_current_input_contexts.end(), l_top);
	if (l_position != m_current_input_contexts.end())
	{
		m_current_input_contexts.erase(l_position);
	}
}

void FE::game::pop_input_context(FE::input::context_id id_p) noexcept
{
	for (auto l_iterator = m_current_input_contexts.begin(); l_iterator != m_current_input_contexts.end(); ++l_iterator)
	{
		if ((*l_iterator)->first == id_p)
		{
			const auto l_target = *l_iterator;
			flush_pressed_keys_of(m_current_input_contexts);

			// The flush may have changed the stack and moved the context: find it again.
			const auto l_position = std::find(m_current_input_contexts.begin(), m_current_input_contexts.end(), l_target);
			if (l_position != m_current_input_contexts.end())
			{
				m_current_input_contexts.erase(l_position);
			}
			return;
		}
	}
}

void FE::game::clear_input_contexts() noexcept
{
	if (m_current_input_contexts.empty() == true)
	{
		return;
	}

	flush_pressed_keys_of(m_current_input_contexts);
	m_current_input_contexts.clear();
}

FE::boolean FE::game::is_input_context_active(FE::input::context_id id_p) const noexcept
{
	for (auto l_iterator : m_current_input_contexts)
	{
		if (l_iterator->first == id_p)
		{
			return true;
		}
	}
	return false;
}