#include <FE/framework/reflection.hxx>




BEGIN_NAMESPACE(FE::framework::reflection)


method_registry::method_registry(FE::size map_capacity_p, std::pmr::memory_resource* pool_p) noexcept
	: m_lock(),
	m_pool(pool_p),
	m_method_registry(map_capacity_p),
	m_system_table(syscall_phase_count, typename system_table::value_type(map_capacity_p, pool_p), pool_p)
{
}

FE::boolean method_registry::check_presence(const std::string_view& key_p) noexcept
{
	typename internal_map_type::key_type l_key(key_p, m_pool);
	boost::shared_lock_guard<lock_type> l_lock(m_lock);
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
	boost::shared_lock_guard<lock_type> l_lock(m_lock);
	for (auto it = m_method_registry.find(l_key); it != m_method_registry.end(); ++it)
	{
		if (it->first == key_p)
		{
			return it->second;
		}
	}
	return nullptr;
}




property_registry::property_registry(FE::size reflection_map_capacity_p, std::pmr::memory_resource* pool_p) noexcept
	: m_pool(pool_p), m_property_registry(reflection_map_capacity_p),
	m_class_layer(m_pool), m_scalable_container_size_record(m_pool),
	m_lock(), m_input_buffer(m_pool), m_position(),
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
	static std::string l_s_serialization_task_name;
	l_s_serialization_task_name = "FE::framework::reflection::property_registry::__serialize_by_foreach_mutually_recursive< >";
	l_s_serialization_task_name.replace(l_s_serialization_task_name.find(' '), 1, property_typename_p);
	return l_s_serialization_task_name.c_str();
}

std::string_view property_registry::__get_deserialization_task_name(const std::string_view& property_typename_p) noexcept
{
	static std::string l_s_deserialization_task_name;
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

