#ifndef _FE_FRAMEWORK_REFLECTION_VARIABLE_MAP_HPP_
#define _FE_FRAMEWORK_REFLECTION_VARIABLE_MAP_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/private/allocator_base.hpp>
#include <FE/core/adjacency_graph.hxx>
#include <FE/core/fstream_guard.hxx>
#include <FE/core/hash.hpp>
#include <FE/core/pair.hxx>
#include <FE/core/string.hxx>
#include <FE/core/type_traits.hxx>

// std
#include <cstring>
#include <fstream>
#include <mutex>
#include <stack>
#include <shared_mutex>
#include <vector>

// boost 
#include <boost/thread/shared_lock_guard.hpp>

// ronbin hood hash
#include <robin_hood.h>




BEGIN_NAMESPACE(FE::framework::reflection)
// https://github.com/getnamo/7zip-cpp?tab=readme-ov-file
/* serialized object data format
3D models and assets -> 7z algorithm -> .froggy
*/

struct property_meta_data
{
	FE::TYPE_TRIVIALITY _is_trivial = FE::TYPE_TRIVIALITY::_TRIVIAL; 
	var::boolean _is_pointer = false; 
	var::uint64 _size_of_type = 0; 

	FE::character* _name = nullptr;

	FE::string _typename;
};


class variable_map
{
public:
    using underlying_container = robin_hood::unordered_map< FE::string, 
                                                            FE::adjacency_graph<var::ptrdiff, property_meta_data>,
                                                            FE::hash<FE::string>
                                                            >;
	using lock_type = std::shared_mutex;

    static constexpr count_t initial_capacity = 1000;

private:
	static underlying_container* s_variable_map;
	static lock_type s_lock; 

public:
	variable_map() noexcept = default;
	~variable_map() noexcept = default;

	static boolean initialize() noexcept
	{
		std::lock_guard<lock_type> l_mutex(s_lock);
		FE_ASSERT(s_variable_map != nullptr, "Assertion failure: cannot initialize FE::framework::reflection::variable_map more than once.");
		if(s_variable_map == nullptr)
		{
			s_variable_map = (underlying_container*)FE_ALIGNED_ALLOC(sizeof(underlying_container), FE::align_CPU_L1_cache_line::size);
			new(s_variable_map) underlying_container(variable_map::initial_capacity);
			return true;
		}

		return false;
	}
	
	static boolean clean_up() noexcept
	{
		std::lock_guard<lock_type> l_mutex(s_lock);
		FE_ASSERT(s_variable_map == nullptr, "Assertion failure: unable to clean_up() FE::framework::reflection::variable_map. It is null.");
		if(s_variable_map != nullptr)
		{
			s_variable_map->~underlying_container();
			FE_ALIGNED_FREE(s_variable_map);
			s_variable_map = nullptr;
			return true;
		}

		return false;
	}

    template<class C, typename T>
	static void register_property(const C& host_class_instance_p, const T& property_p, FE::character* const property_name_p) noexcept
	{
		FE_STATIC_ASSERT(std::is_class<C>::value == false, "Primitive data types cannot be registered as the host classes/structs.");
		FE_STATIC_ASSERT(std::is_reference<T>::value == true, "static assertion failure: references cannot be serialized nor deserialized.");
		FE_ASSERT(s_variable_map == nullptr, "variable map not initialized: please initialize and register the property meta data to the map, before invoking register_property().");	
		FE_ASSERT(property_name_p == nullptr, "Assertion failure: property name cannot be nullptr.");
		FE_ASSERT(*property_name_p == null, "Assertion failure: property name cannot be nullptr.");

		property_meta_data l_property_meta_data;
		l_property_meta_data._is_trivial = FE::is_trivial<typename std::remove_pointer<T>::type>::value;
		l_property_meta_data._is_pointer = std::is_pointer<T>::value;
		l_property_meta_data._size_of_type = sizeof(typename std::remove_pointer<T>::type);
		l_property_meta_data._name = property_name_p;
		l_property_meta_data._typename = FE::demangle_type_name<::FE::string>(typeid(typename std::remove_pointer<T>::type).name());
		// append the parent class name of C with \n to l_host_class_instance_typename.
		FE::string l_host_class_instance_typename = FE::demangle_type_name<::FE::string>(typeid(C).name());
		std::lock_guard<lock_type> l_mutex(s_lock);
		auto l_iterator = s_variable_map->find(l_host_class_instance_typename);
		if(UNLIKELY(l_iterator == s_variable_map->end())) _UNLIKELY_
		{
			(*s_variable_map)[l_host_class_instance_typename];
			l_iterator = s_variable_map->find(l_host_class_instance_typename);
		}
		l_iterator->second.insert((reinterpret_cast<FE::byte* const>(&property_p) - reinterpret_cast<FE::byte*>(&host_class_instance_p)), std::move(l_property_meta_data));
	}

    template<typename T>
    static void serialize(std::filesystem::path path_p, directory_char_t* filename_p, const T& object_p) noexcept
	{
		boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
		FE_STATIC_ASSERT(std::is_class<T>::value == false, "Non-class/struct field variables cannot be serialized.");
		FE_ASSERT(path_p.empty() == true, "Assertion failure: the target directory path to the file is nullptr.");
		FE_ASSERT(s_variable_map == nullptr, "variable map not initialized: please initialize and register the property meta data to the map, before invoking serialize().");
   		FE_ASSERT((filename_p == nullptr) || (*filename_p == null), "The file name is null.");

		if (std::filesystem::exists(path_p) == false)
    	{
        	std::filesystem::create_directory(path_p);
    	}
		std::ofstream l_ofstream;
		FE::ofstream_guard l_serialization_handler(l_ofstream);
		l_serialization_handler.get_stream().open((path_p / filename_p).c_str(), std::ios::out | std::ios::binary);
		FE::string l_typename = FE::demangle_type_name<FE::string>(typeid(object_p).name());
		l_serialization_handler.get_stream() << l_typename.c_str() << "{";
		auto l_search_result = s_variable_map->find(l_typename);
		FE_ASSERT(l_search_result == s_variable_map->end(), "Failed to find the requested type meta data from the property reflection system. Please check if the type is registered to the variable map.");
		
		using class_layer_type = std::stack< FE::pair<typename underlying_container::mapped_type*, typename underlying_container::mapped_type::iterator> >;
		class_layer_type l_class_layer;
		l_class_layer.push(typename class_layer_type::value_type{ &(l_search_result->second), l_search_result->second.begin()});

		using indirection_layer_type = std::stack<FE::byte*>;
		indirection_layer_type l_indirection_layer;
		l_indirection_layer.push(reinterpret_cast<FE::byte*>(&object_p));


		while(l_class_layer.empty() == false)
		{
			switch(l_class_layer.top()._second->_value._is_trivial)
			{
			case TYPE_TRIVIALITY::_TRIVIAL:
			{
				// Acquire an iterator to the top class layer.
				auto l_property_meta_data = l_class_layer.top()._second; // The first is the data structure that represents the class memory layer and the second is the property meta data iterator.
				
				// Check if the field variable meta data is valid.
				FE_ASSERT(l_property_meta_data->_value._size_of_type == 0, "Assertion failed: unable to serialize a zero-byte property.");
				
				// Check if the field variable is a pointer to a member variable.
				if(l_property_meta_data->_value._is_pointer == true)
				{
					FE::byte* const* l_ptr_to_ptr = reinterpret_cast<FE::byte* const*>(l_indirection_layer.top() + l_property_meta_data->get_key());
					// serialize indirected data.
					if(l_ptr_to_ptr != nullptr)
					{
						l_serialization_handler.get_stream().write(reinterpret_cast<const char*>(*l_ptr_to_ptr), l_property_meta_data->_value._size_of_type);
					}
				}
				else
				{
					// Copy the bits onto the buffer.
					l_serialization_handler.get_stream().write(reinterpret_cast<const char*>(l_indirection_layer.top() + l_property_meta_data->get_key()), l_property_meta_data->_value._size_of_type);
				}

				// Look for the next registered property of the class layer.
				++(l_class_layer.top()._second);

				// Pop the class layer if the iterator reached the end of the property list.
				if(l_class_layer.top()._second == l_class_layer.top()._first->end())
				{
					l_class_layer.pop();
				}
				break;
			}

			case TYPE_TRIVIALITY::_NOT_TRIVIAL:
/*
					// Find the class/struct meta data that contains its memory layer.
					l_search_result = s_variable_map->find(l_property_meta_data->_value._typename);
					FE_ASSERT(l_search_result == s_variable_map->end(), "Failed to find the requested type meta data from the property reflection system. Please check if the type is registered to the variable map.");
					
					// Push the member variable iterator and the class meta data to the class layer if the class meta data is found.
					l_class_layer.push(typename class_layer_type::value_type{ &(l_search_result->second), l_search_result->second.begin()});

					l_property_meta_data = l_class_layer.top()._second;
*/
				break;
			}
		}

		l_serialization_handler.get_stream() << "};\0";
	}

	// TODO: add file versioning.
    template<typename T>
	static void deserialize(std::filesystem::path path_p, directory_char_t* filename_p, T& out_object_p) noexcept
	{
		boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
		FE_STATIC_ASSERT(std::is_class<T>::value == false, "Non-class/struct field variables cannot be deserialized.");
		FE_ASSERT(std::filesystem::exists(path_p) == false, "Assertion failure: the target directory path to the file is nullptr.");
		FE_ASSERT(s_variable_map == nullptr, "variable map not initialized: please initialize and register the property meta data to the map, before invoking deserialize().");
		FE_ASSERT((filename_p == nullptr) || (*filename_p == null), "The file name is null.");

		std::ifstream l_ifstream;
		FE::ifstream_guard l_serialization_handler(l_ifstream);
		l_serialization_handler.get_stream().open((path_p / filename_p).c_str(), std::ios::in | std::ios::binary | std::ios::ate); // Open the file as a binary file and point at the end of the file to input to a buffer.
		FE_ASSERT(l_serialization_handler.get_stream().is_open() == false, "Assertion failure: the target directory path to the file is invalid.");
		FE::string l_buffer;
		l_buffer.reserve(l_serialization_handler.get_stream().tellg()); // FE::string does not allow implicit memory allocation and re-allocation.
		l_buffer.assign(l_buffer.capacity()-1, '\0'); // Set the buffer with null.
		l_serialization_handler.get_stream().seekg(0, std::ios::beg); // Set the indicator to the begining of the file.
		l_serialization_handler.get_stream().read(l_buffer.data(), l_buffer.length()); // Load the serialized data onto the buffer.

		FE::string::iterator l_position = l_buffer.find('{');
		FE_ASSERT(l_position == l_buffer.end(), "The serialization file is ill-formed or unsupported.");
		_MAYBE_UNUSED_ length_t l_typename_length = (l_position - l_buffer.begin());
		++l_position; // Point the first byte.

		FE::string l_typename = FE::demangle_type_name<FE::string>(typeid(out_object_p).name());
		auto l_search_result = s_variable_map->find(l_typename);
		FE_ASSERT(l_search_result == s_variable_map->end(), "Failed to find the requested type meta data from the property reflection system. Please check if the type is registered to the variable map.");
		
		FE_EXIT(!algorithm::string::compare_ranged<char>(	l_typename.c_str(), algorithm::string::range{0, l_typename.length()}, 
															l_buffer.c_str(), algorithm::string::range{0, l_typename_length}),
															FE::ERROR_CODE::_FATAL_SERIALIZATION_ERROR_3XX_TYPE_MISMATCH, "Unable to deserialize an instance with a different class name.");

		using class_layer_type = std::stack< FE::pair<typename underlying_container::mapped_type*, typename underlying_container::mapped_type::iterator> >;
		class_layer_type l_class_layer;
		l_class_layer.push(typename class_layer_type::value_type{ &(l_search_result->second), l_search_result->second.begin()});

		using indirection_layer_type = std::stack<var::byte*>;
		indirection_layer_type l_indirection_layer;
		l_indirection_layer.push(reinterpret_cast<var::byte*>(&out_object_p));


		while(l_class_layer.empty() == false)
		{
			switch(l_class_layer.top()._second->_value._is_trivial)
			{
			case TYPE_TRIVIALITY::_TRIVIAL:
			{
				// Acquire an iterator to the top class layer.
				auto l_property_meta_data = l_class_layer.top()._second;
				
				// Check if the meta data is valid.
				FE_ASSERT(l_property_meta_data->_value._size_of_type == 0, "Assertion failed: unable to serialize a zero-byte property.");

				// Check if the field variable is a pointer to a member variable.
				if(l_property_meta_data->_value._is_pointer == true)
				{
					var::byte** l_ptr_to_ptr = reinterpret_cast<var::byte**>(l_indirection_layer.top() + l_property_meta_data->get_key());
					if(l_ptr_to_ptr != nullptr)
					{
						// Write the bits to the object, from a buffer.
						FE::memcpy(*l_ptr_to_ptr, FE::iterator_cast<FE::character*>(l_position), l_property_meta_data->_value._size_of_type);
					}
				}
				else
				{
					// Write the bits to the object, from a buffer.
					FE::memcpy((l_indirection_layer.top() + l_property_meta_data->get_key()), FE::iterator_cast<FE::character*>(l_position), l_property_meta_data->_value._size_of_type);
				}
				l_position += l_property_meta_data->_value._size_of_type; // Iterate to the next bits.
				// Look for the next registered property of the class layer.
				++(l_class_layer.top()._second);

				// Pop the class layer if the iterator reached the end of the property list.
				if(l_class_layer.top()._second == l_class_layer.top()._first->end())
				{
					l_class_layer.pop();
				}
				break;
			}

			case TYPE_TRIVIALITY::_NOT_TRIVIAL:

				break;
			}
		}
	}
};

END_NAMESPACE
#endif