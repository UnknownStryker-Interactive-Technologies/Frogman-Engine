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
#include <FE/framework/reflection.hxx>
#include <FE/shared_lock_guard.hpp>




BEGIN_NAMESPACE(FE::framework::reflection)


method_registry::method_registry(FE::size map_capacity_p, std::pmr::memory_resource* pool_p) noexcept
	: m_lock(),
	m_pool(pool_p),
	m_method_registry(map_capacity_p),
	m_world_system_table(pool_p),
	m_global_system_table()
{
	for (typename global_system_table::value_type& vec : m_global_system_table)
	{
		vec = typename global_system_table::value_type(m_pool);
		vec.reserve(512);
	}
}

void method_registry::reserve(FE::size size_p) noexcept
{
	std::lock_guard<lock_type> l_lock(m_lock);
	m_method_registry.reserve(size_p);
}

FE::boolean method_registry::check_presence(const std::string_view& key_p) noexcept
{
	typename internal_map_type::key_type l_key(key_p, m_pool);
	FE::shared_lock_guard<lock_type> l_lock(m_lock);
	for (auto it = m_method_registry.find(l_key); it != m_method_registry.end(); ++it)
	{
		if (it->first == key_p)
		{
			return true;
		}
	}
	return false;
}

FE::task_base* method_registry::retrieve(const std::string_view& key_p) noexcept
{
	typename internal_map_type::key_type l_key(key_p, m_pool);
	FE::shared_lock_guard<lock_type> l_lock(m_lock);
	for (auto it = m_method_registry.find(l_key); it != m_method_registry.end(); ++it)
	{
		if (it->first == key_p)
		{
			return it->second;
		}
	}
	return nullptr;
}


void method_registry::associate_system(world_tag_t world_tag_p, FE::WorldSystemCallPhase syscall_phase_p, system system_function_p) noexcept
{
	static_assert(::std::is_same_v<::std::remove_cv_t<decltype(syscall_phase_p)>, ::FE::WorldSystemCallPhase>, "Frogman Engine C++: an incorrectly typed value passed to the SysCallPhase parameter.");
	static_assert(sizeof(syscall_phase_p) == sizeof(FE::uint16));

	if (m_world_system_table.find(world_tag_p) == m_world_system_table.end())
	{
		typename world_system_table::mapped_type l_world_system_table;
		for (typename world_system_table::mapped_type::value_type& vec : l_world_system_table)
		{
			vec = typename world_system_table::mapped_type::value_type(m_pool);
			vec.reserve(512);
		}
		m_world_system_table[world_tag_p] = std::move(l_world_system_table);
	}

	m_world_system_table[world_tag_p][(FE::uint16)syscall_phase_p].emplace_back(system_function_p);
}

void method_registry::associate_global_system(FE::GlobalSystemCallPhase syscall_phase_p, global_system system_function_p) noexcept
{
	static_assert(::std::is_same_v<::std::remove_cv_t<decltype(syscall_phase_p)>, ::FE::GlobalSystemCallPhase>, "Frogman Engine C++: an incorrectly typed value passed to the SysCallPhase parameter.");
	static_assert(sizeof(syscall_phase_p) == sizeof(FE::uint16));

	m_global_system_table[(FE::uint16)syscall_phase_p].emplace_back(system_function_p);
}


void method_registry::sort_systems_topologically(world_tag_t world_tag_p, FE::WorldSystemCallPhase syscall_phase_p, std::initializer_list<system_dependency> ilist_p) noexcept
{
	static_assert(::std::is_same_v<::std::remove_cv_t<decltype(syscall_phase_p)>, ::FE::WorldSystemCallPhase> || ::std::is_same_v<::std::remove_cv_t<decltype(syscall_phase_p)>, ::FE::GlobalSystemCallPhase>, "Frogman Engine C++: an incorrectly typed value passed to the SysCallPhase parameter.");
	static_assert(sizeof(syscall_phase_p) == sizeof(FE::uint16));

	if (ilist_p.size() == 0)
	{
		return;
	}

	std::unordered_map<	world_tag_t, 
						std::unordered_map<	WorldSystemCallPhase, 
											std::unordered_map<FE::system, var::int64>
											>
						> l_system_symbol_table;
	// initialize the symbol table with all systems
	for (const system_dependency& dependency_object : ilist_p)
	{
		l_system_symbol_table[world_tag_p][syscall_phase_p][dependency_object._current_system] = 0;

		for (const system& current_runs_before_this_system : dependency_object._runs_before)
		{
			l_system_symbol_table[world_tag_p][syscall_phase_p][current_runs_before_this_system] = 0;
		}

		for (const system& current_runs_after_this_system : dependency_object._runs_after)
		{
			l_system_symbol_table[world_tag_p][syscall_phase_p][current_runs_after_this_system] = 0;
		}
	}

	
	auto& l_systems = m_world_system_table[world_tag_p][(FE::uint16)syscall_phase_p];
	for (const auto& [world_tag, phases] : l_system_symbol_table)
	{
		for (const auto& [phase, systems] : phases)
		{
			for (const auto& [system, reference_count] : systems)
			{
				auto l_it = std::find(l_systems.begin(), l_systems.end(), system);
				FE_EXIT_IF(l_it == l_systems.end(), FE::ErrorCode::_FatalError_ReflexcodeExecutionFailure, "FE Runtime Reflection Metadata Registration Failure: the system function was not registered in the system table.");
				l_systems.erase(l_it);
			}
		}
	}

	// count the number of dependencies for each system
	for (const system_dependency& dependency_object : ilist_p)
	{
		for (const system& current_runs_before_this_system : dependency_object._runs_before)
		{
			l_system_symbol_table[world_tag_p][syscall_phase_p][current_runs_before_this_system] += 1;
		}
		l_system_symbol_table[world_tag_p][syscall_phase_p][dependency_object._current_system] += dependency_object._runs_after.size();
	}

	// topological sort
	auto& l_table = l_system_symbol_table[world_tag_p][syscall_phase_p];

	// a system named only in another system's RUNS_BEFORE/RUNS_AFTER has no dependency object of its own: give it an empty one, so that the loop below puts it back as well.
	std::pmr::vector<system_dependency> l_dependency_objects(ilist_p.begin(), ilist_p.end(), m_pool);
	for (const system_dependency& dependency_object : ilist_p)
	{
		for (const std::pmr::vector<system>* const linked_systems : { &dependency_object._runs_before, &dependency_object._runs_after })
		{
			for (const system& linked_system : *linked_systems)
			{
				if (std::none_of(l_dependency_objects.begin(), l_dependency_objects.end(), [linked_system](const system_dependency& object_p) { return object_p._current_system == linked_system; }))
				{
					l_dependency_objects.push_back(system_dependency{ linked_system, std::pmr::vector<system>(m_pool), std::pmr::vector<system>(m_pool) });
				}
			}
		}
	}

	while (l_table.size() > 0)
	{
		bool l_has_made_progress = false;

		for (const system_dependency& dependency_object : l_dependency_objects)
		{
			auto l_it = l_table.find(dependency_object._current_system);
			if (l_it == l_table.end() || l_it->second != 0)
			{
				continue;
			}

			l_has_made_progress = true;
			l_systems.push_back(dependency_object._current_system);
			l_table.erase(l_it);

			for (const system& current_runs_before_this : dependency_object._runs_before)
			{
				l_table[current_runs_before_this] -= 1;
			}

			for (const system_dependency& another_dependency_object : ilist_p)
			{
				for (const system& current_of_another_dependency_object_runs_after_this : another_dependency_object._runs_after) // another_dependency_object._current_system runs after 'current_of_another_dependency_object_runs_after_this'
				{
					if (current_of_another_dependency_object_runs_after_this == dependency_object._current_system) // is current_of_another_dependency_object_runs_after_this the same as the one that has been erased?
					{
						l_table[another_dependency_object._current_system] -= 1; // dependency_object._current_system -> another_dependency_object._current_system
					}
				}
			}
		}

		// a whole pass emitted nothing: cyclic dependency
		FE_EXIT_IF(l_has_made_progress == false, FE::ErrorCode::_FatalError_ReflexcodeExecutionFailure, "FE Runtime Reflection Metadata Registration Failure: a circular dependency_object was found among the systems.");
	}
}

void method_registry::sort_global_systems_topologically(FE::GlobalSystemCallPhase syscall_phase_p, std::initializer_list<global_system_dependency> ilist_p) noexcept
{
	static_assert(::std::is_same_v<::std::remove_cv_t<decltype(syscall_phase_p)>, ::FE::WorldSystemCallPhase> || ::std::is_same_v<::std::remove_cv_t<decltype(syscall_phase_p)>, ::FE::GlobalSystemCallPhase>, "Frogman Engine C++: an incorrectly typed value passed to the SysCallPhase parameter.");
	static_assert(sizeof(syscall_phase_p) == sizeof(FE::uint16));

	if (ilist_p.size() == 0)
	{
		return;
	}

	std::unordered_map<	GlobalSystemCallPhase,
		std::unordered_map<FE::global_system, var::int64>
	> l_system_symbol_table;
	// initialize the symbol table with all systems
	for (const global_system_dependency& dependency_object : ilist_p)
	{
		l_system_symbol_table[syscall_phase_p][dependency_object._current_system] = 0;

		for (const global_system& current_runs_before_this_system : dependency_object._runs_before)
		{
			l_system_symbol_table[syscall_phase_p][current_runs_before_this_system] = 0;
		}

		for (const global_system& current_runs_after_this_system : dependency_object._runs_after)
		{
			l_system_symbol_table[syscall_phase_p][current_runs_after_this_system] = 0;
		}
	}


	auto& l_systems = m_global_system_table[(FE::uint16)syscall_phase_p];
	for (const auto& [phase, systems] : l_system_symbol_table)
	{
		for (const auto& [system, reference_count] : systems)
		{
			auto l_it = std::find(l_systems.begin(), l_systems.end(), system);
			FE_EXIT_IF(l_it == l_systems.end(), FE::ErrorCode::_FatalError_ReflexcodeExecutionFailure, "FE Runtime Reflection Metadata Registration Failure: the system function was not registered in the system table.");
			l_systems.erase(l_it);
		}
	}

	// count the number of dependencies for each system
	for (const global_system_dependency& dependency_object : ilist_p)
	{
		for (const global_system& current_runs_before_this_system : dependency_object._runs_before)
		{
			l_system_symbol_table[syscall_phase_p][current_runs_before_this_system] += 1;
		}
		l_system_symbol_table[syscall_phase_p][dependency_object._current_system] += dependency_object._runs_after.size();
	}

	// topological sort
	auto& l_table = l_system_symbol_table[syscall_phase_p];

	// a system named only in another system's RUNS_BEFORE/RUNS_AFTER has no dependency object of its own: give it an empty one, so that the loop below puts it back as well.
	std::pmr::vector<global_system_dependency> l_dependency_objects(ilist_p.begin(), ilist_p.end(), m_pool);
	for (const global_system_dependency& dependency_object : ilist_p)
	{
		for (const std::pmr::vector<global_system>* const linked_systems : { &dependency_object._runs_before, &dependency_object._runs_after })
		{
			for (const global_system& linked_system : *linked_systems)
			{
				if (std::none_of(l_dependency_objects.begin(), l_dependency_objects.end(), [linked_system](const global_system_dependency& object_p) { return object_p._current_system == linked_system; }))
				{
					l_dependency_objects.push_back(global_system_dependency{ linked_system, std::pmr::vector<global_system>(m_pool), std::pmr::vector<global_system>(m_pool) });
				}
			}
		}
	}

	while (l_table.size() > 0)
	{
		bool l_has_made_progress = false;

		for (const global_system_dependency& dependency_object : l_dependency_objects)
		{
			auto l_it = l_table.find(dependency_object._current_system);
			if (l_it == l_table.end() || l_it->second != 0)
			{
				continue;
			}

			l_has_made_progress = true;
			l_systems.push_back(dependency_object._current_system);
			l_table.erase(l_it);

			for (const global_system& current_runs_before_this : dependency_object._runs_before)
			{
				l_table[current_runs_before_this] -= 1;
			}

			for (const global_system_dependency& another_dependency_object : ilist_p)
			{
				for (const global_system& current_of_another_dependency_object_runs_after_this : another_dependency_object._runs_after) // another_dependency_object._current_system runs after 'current_of_another_dependency_object_runs_after_this'
				{
					if (current_of_another_dependency_object_runs_after_this == dependency_object._current_system) // is current_of_another_dependency_object_runs_after_this the same as the one that has been erased?
					{
						l_table[another_dependency_object._current_system] -= 1; // dependency_object._current_system -> another_dependency_object._current_system
					}
				}
			}
		}

		// a whole pass emitted nothing: cyclic dependency
		FE_EXIT_IF(l_has_made_progress == false, FE::ErrorCode::_FatalError_ReflexcodeExecutionFailure, "FE Runtime Reflection Metadata Registration Failure: a circular dependency_object was found among the systems.");
	}
}


typename method_registry::world_system_table::mapped_type::value_type& method_registry::get_world_systems(world_tag_t world_tag_p, WorldSystemCallPhase syscall_phase_p) noexcept
{
	static_assert(sizeof(syscall_phase_p) == sizeof(FE::uint16));
	return m_world_system_table[world_tag_p][(FE::uint16)syscall_phase_p];
}

typename method_registry::global_system_table::value_type& method_registry::get_global_systems(GlobalSystemCallPhase syscall_phase_p) noexcept
{
	static_assert(sizeof(syscall_phase_p) == sizeof(FE::uint16));
	return m_global_system_table[(FE::uint16)syscall_phase_p];
}




property_registry::property_registry(FE::size reflection_map_capacity_p, std::pmr::memory_resource* pool_p) noexcept
	:	m_pool(pool_p), 
		m_property_registry(reflection_map_capacity_p),
		m_class_layer(m_pool),
		m_lock(),
		m_key_buffer(m_pool),
		m_buffer(m_pool),
		m_instance_metadata_lut(reflection_map_capacity_p) 
{
}

void property_registry::__push_parent_class_layers_by_typename_string_recursive(const std::string_view& typename_p) noexcept
{
	static typename  internal_map_type::iterator l_s_search_result;
	static typename  internal_map_type::key_type l_s_typename;
	l_s_typename = reflection::type_info::get_base_name_of(typename_p);
	l_s_search_result = m_property_registry.find(l_s_typename);
	if (l_s_search_result == m_property_registry.end())
	{
		return;
	}
	m_class_layer.emplace_back(&(l_s_search_result->second), l_s_search_result->second.begin());
	__push_parent_class_layers_by_typename_string_recursive(l_s_search_result->first);
}

std::string_view property_registry::__get_serialization_task_name(const std::string_view& property_typename_p) noexcept
{
	static std::basic_string<char, std::char_traits<char>, FE::cache_aligned_allocator<char>> l_s_serialization_task_name;
	l_s_serialization_task_name = "FE::framework::reflection::property_registry::__serialize_by_foreach_mutually_recursive< >";
	l_s_serialization_task_name.replace(l_s_serialization_task_name.find(' '), 1, property_typename_p);
	return l_s_serialization_task_name.c_str();
}

std::string_view property_registry::__get_deserialization_task_name(const std::string_view& property_typename_p) noexcept
{
	static std::basic_string<char, std::char_traits<char>, FE::cache_aligned_allocator<char>> l_s_deserialization_task_name;
	l_s_deserialization_task_name = "FE::framework::reflection::property_registry::__deserialize_by_foreach_mutually_recursive< >";
	l_s_deserialization_task_name.replace(l_s_deserialization_task_name.find(' '), 1, property_typename_p);
	return l_s_deserialization_task_name.c_str();
}




enum_metadata::enum_metadata(std::pmr::memory_resource* const resource_p) noexcept
	: m_typename(),
	m_string_to_value_map(resource_p),
	m_value_to_string_map(resource_p)
{
	FE_ASSERT(resource_p != nullptr, "Assertion failed: memory resource cannot be a nullptr.");
}

enum_registry::enum_registry(std::pmr::memory_resource* const resource_p, FE::size capacity_p) noexcept
	:	m_enum_registry(capacity_p),
		m_resource(resource_p)
{
	FE_ASSERT(resource_p != nullptr, "Assertion failed: memory resource cannot be a nullptr.");
}




enum_metadata* enum_registry::retrieve_enum_struct_metadata(const std::string_view& enum_struct_name_p)
{
	for (auto it = m_enum_registry.find(enum_struct_name_p); it != m_enum_registry.end(); ++it)
	{
		if (it.key() == enum_struct_name_p)
		{
			return &(it.value());
		}
	}
	return nullptr;
}


END_NAMESPACE

