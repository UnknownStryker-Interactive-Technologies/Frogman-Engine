#ifndef _FE_FRAMEWORK_REFLECTION_PROPERTY_HPP_
#define _FE_FRAMEWORK_REFLECTION_PROPERTY_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/private/allocator_base.hpp>
#include <FE/core/adjacency_graph.hxx>
#include <FE/core/fstream_guard.hxx>
#include <FE/core/hash.hpp>
#include <FE/core/pair.hxx>
#include <FE/core/pool_allocator.hxx>
#include <FE/core/string.hxx>
#include <FE/core/type_traits.hxx>

// FE.framework
#include <FE/framework/reflection/function.hpp>
#include <FE/framework/reflection/type_info.hpp>

// std
#include <cstring>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <shared_mutex>
#include <stack>
#include <vector>

// boost 
#include <boost/thread/shared_lock_guard.hpp>

// ronbin hood hash
#include <robin_hood.h>


/*
Frogman Engine serialization & deserialization system supports:
	1. Plain old data serialization & deserialization. O
	2. Non-trivial object serialization & deserialization. O
	3. Fixed-sized non-trivial instance array serialization & deserialization. O
	4. Fixed-sized plain old data array serialization & deserialization. O

	5. Serializing & deserializing plain old data pointed by FE smart array pointer O
	6. Serializing & deserializing Non-trivial data pointed by FE smart array pointer O

	7. Plain old data Container serialization & deserialization. O
	8. non-trivial Container serialization & deserialization. O

    9. Base type serialization & deserialization. O
   10. Components' base types serialization & deserialization. ^
NOTE:
	Unregistered non Frogman Engine native types without begin() end() methods cannot be serialized nor deserialized. It will result in compilation failure.
*/
BEGIN_NAMESPACE(FE::framework::reflection)
// https://github.com/getnamo/7zip-cpp?tab=readme-ov-file
/* serialized object data format
3D models and assets -> 7z algorithm -> .froggy
*/

/* see references
https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/CoreUObject/UObject/UObject?application_version=5.4
https://app.creately.com/d/fZknuArLuyk/view 
*/
class property;

struct property_meta_data
{
	FE::TYPE_TRIVIALITY _is_trivial; 
	var::boolean _is_serializable;
	var::uint32 _total_size; 
	
	FE::ASCII* _name;
	FE::ASCII* _typename;
};


class property
{
public: 
/* 
It is worth noting that, FE::string's contents can be allocated on a thread local memory pool
by adding -DMEMORY_POOL_FE_STRINGS=1 option to cmake.
*/
	using property_meta_data_map_type = robin_hood::unordered_map<FE::ASCII*, 
                                                        		  FE::adjacency_graph<var::ptrdiff, property_meta_data>,
                                                        							  FE::hash<FE::ASCII*>>;
																					  
	using class_layer_stack = std::stack<FE::pair<typename  property_meta_data_map_type::mapped_type*, typename  property_meta_data_map_type::mapped_type::iterator>, 
									 	 std::deque<FE::pair<typename  property_meta_data_map_type::mapped_type*, typename  property_meta_data_map_type::mapped_type::iterator>, 
									 	 FE::pool_allocator<FE::pair<typename  property_meta_data_map_type::mapped_type*, typename  property_meta_data_map_type::mapped_type::iterator>, 
															FE::size_in_bytes<1 MB>>>>;

	using lock_type = std::shared_mutex;
	using file_handler = std::fstream;
	using input_buffer_type = std::string;
	using input_buffer_iterator_type = typename input_buffer_type::iterator;

    static constexpr count_t initial_capacity = 1000;

private:
	static property_meta_data_map_type* s_property_map;
	thread_local static class_layer_stack tl_s_class_layer;

	static lock_type s_lock;
	thread_local static file_handler tl_s_fstream;
	thread_local static input_buffer_type tl_s_input_buffer;
	thread_local static input_buffer_iterator_type tl_s_position;

public:
	property() noexcept = default;
	~property() noexcept = default;

	_FORCE_INLINE_ static boolean initialize() noexcept
	{
		std::lock_guard<lock_type> l_mutex(s_lock);
		FE_ASSERT(s_property_map != nullptr, "Assertion failure: cannot initialize FE::framework::reflection::property more than once.");
		if(s_property_map == nullptr)
		{
			s_property_map = ( property_meta_data_map_type*)FE_ALIGNED_ALLOC(sizeof( property_meta_data_map_type), FE::align_CPU_L1_cache_line::size);
			new(s_property_map)  property_meta_data_map_type(property::initial_capacity);
			__load_default_class_metadata();
			return true;
		}

		return false;
	}
	
	_FORCE_INLINE_ static boolean clean_up() noexcept
	{
		std::lock_guard<lock_type> l_mutex(s_lock);
		FE_ASSERT(s_property_map == nullptr, "Assertion failure: unable to clean_up() FE::framework::reflection::property. It is null.");
		if(s_property_map != nullptr)
		{
			s_property_map->~property_meta_data_map_type();
			FE_ALIGNED_FREE(s_property_map);
			s_property_map = nullptr;
			return true;
		}

		return false;
	}

	_FORCE_INLINE_ static boolean is_available() noexcept
	{
		return s_property_map != nullptr;
	}

	_FORCE_INLINE_ static void reserve(size size_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(s_lock);
		s_property_map->reserve(size_p);
	}

    template<class C, typename T>
	static void register_property(const C& host_class_instance_p, const T& property_p, FE::ASCII* const property_name_p) noexcept
	{
		FE_STATIC_ASSERT(std::is_class<C>::value == false, "Primitive data types cannot be registered as the host classes/structs.");
		FE_STATIC_ASSERT((std::is_reference<T>::value == true) || (std::is_pointer<T>::value == true), "static assertion failure: raw pointers and references cannot be serialized nor deserialized.");
		FE_ASSERT(s_property_map == nullptr, "variable map not initialized: please property::initialize() and register the property meta data to the map, before invoking register_property().");	
		FE_ASSERT(FE::framework::reflection::function::is_available() == false, "function table not initialized: please function::initialize() and register the property meta data to the map, before invoking register_property().");
		FE_ASSERT((property_name_p == nullptr) || (*property_name_p == null), "Assertion failure: property name cannot be null.");

		property_meta_data l_property_meta_data;
		l_property_meta_data._is_trivial = static_cast<TYPE_TRIVIALITY>(FE::is_trivial<T>::value);
		l_property_meta_data._is_serializable = FE::is_serializable<T>::value;
		l_property_meta_data._total_size = sizeof(T);
		l_property_meta_data._name = property_name_p;
		l_property_meta_data._typename = FE::framework::reflection::type_id<T>().name();

		// Find or register its host class.
		FE::ASCII* l_host_class_instance_typename = FE::framework::reflection::type_id<C>().name();

		std::lock_guard<lock_type> l_mutex(s_lock);
		auto l_iterator = s_property_map->find(l_host_class_instance_typename);
		if(UNLIKELY(l_iterator == s_property_map->end())) _UNLIKELY_
		{
			(*s_property_map)[l_host_class_instance_typename];
			l_iterator = s_property_map->find(l_host_class_instance_typename);
		}

		if constexpr (FE::has_iterator<T>::value == true)
		{
			FE::framework::reflection::function::register_task< FE::c_style_task<void(T*)> >(__get_serialization_task_name(l_property_meta_data._typename), &property::__serialize_by_foreach_mutually_recursive<T>);
			FE::framework::reflection::function::register_task< FE::c_style_task<void(T*)> >(__get_deserialization_task_name(l_property_meta_data._typename), &property::__deserialize_by_foreach_mutually_recursive<T>);
		}

		if constexpr ((std::is_array<T>::value == true) && (FE::is_trivial<T>::value == false))
		{
			count_t l_array_size = l_property_meta_data._total_size / sizeof( std::remove_extent_t< std::remove_pointer_t<T> > );

			for(var::count_t i = 0; i < l_array_size; ++i)
			{
				l_iterator->second.insert( (reinterpret_cast<FE::byte* const>(&property_p) + (i * sizeof( std::remove_extent_t< std::remove_pointer_t<T> > ))) - reinterpret_cast<FE::byte*>(&host_class_instance_p),
				                           l_property_meta_data); // insert fixed-sized N of a non-trivial array.
			}
			return;
		}

		l_iterator->second.insert((reinterpret_cast<FE::byte* const>(&property_p) - reinterpret_cast<FE::byte*>(&host_class_instance_p)), l_property_meta_data); 
	}

 	// component's base class iteration not implemented.
    template<typename T>
    static void serialize(std::filesystem::path path_p, directory_char_t* filename_p, const T& object_p) noexcept
	{
		boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
#ifdef _DEBUG_
		_MAYBE_UNUSED_ class_layer_stack* l_tl_s_class_layer_watcher = &tl_s_class_layer;
#endif
		FE_STATIC_ASSERT(std::is_class<T>::value == false, "Non-class/struct field variables cannot be serialized.");
		FE_STATIC_ASSERT((std::is_reference<T>::value == true) || (std::is_pointer<T>::value == true), "static assertion failure: raw pointers and references cannot be serialized nor deserialized.");
		FE_ASSERT(path_p.empty() == true, "Assertion failure: the target directory path to the file is nullptr.");
		FE_ASSERT(s_property_map == nullptr, "variable map not initialized: please property::initialize() before invoking serialize().");
		FE_ASSERT(FE::framework::reflection::function::is_available() == false, "function table not initialized: please function::initialize() before invoking serialize().");
   		FE_ASSERT((filename_p == nullptr) || (*filename_p == null), "The file name is null.");
		
		FE::ASCII* l_typename = FE::framework::reflection::type_id<T>().name();
		auto l_search_result = s_property_map->find(l_typename);
		if((l_search_result == s_property_map->end()) || (l_search_result->second.size() == 0)) // return if it is empty or not found.
		{
			FE_LOG("serialization failed: could not find the requested type information or the class/struct is empty");
			return;
		}
		tl_s_class_layer.push(typename class_layer_stack::value_type{ &(l_search_result->second), l_search_result->second.begin()});

		if constexpr (FE::has_base_type<T>::value == true)
		{
			__push_parent_class_layers_recursive<T>();
		}

		if (std::filesystem::exists(path_p) == false)
    	{
        	std::filesystem::create_directory(path_p);
    	}

		FE::fstream_guard l_serialization_handler(tl_s_fstream);
		l_serialization_handler.get_stream().open((path_p / filename_p).c_str(), std::ios::out | std::ios::binary);
		l_serialization_handler.get_stream() << l_typename << "{";
		__serialize_mutually_recursive<T>(object_p);
		l_serialization_handler.get_stream() << "};\0";
	}

	// TODO: add file versioning.
    template<typename T>
	static void deserialize(std::filesystem::path path_p, directory_char_t* filename_p, T& out_object_p) noexcept
	{
		boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
#ifdef _DEBUG_
		_MAYBE_UNUSED_ class_layer_stack* l_tl_s_class_layer_watcher = &tl_s_class_layer;
#endif
		FE_STATIC_ASSERT(std::is_class<T>::value == false, "Non-class/struct field variables cannot be deserialized.");
		FE_STATIC_ASSERT((std::is_reference<T>::value == true) || (std::is_pointer<T>::value == true), "static assertion failure: raw pointers and references cannot be serialized nor deserialized.");
		FE_ASSERT(std::filesystem::exists(path_p) == false, "Assertion failure: the target directory path to the file is nullptr.");
		FE_ASSERT(s_property_map == nullptr, "variable map not initialized: please property::initialize() before invoking deserialize().");
		FE_ASSERT(FE::framework::reflection::function::is_available() == false, "function table not initialized: please function::initialize() before invoking deserialize().");
		FE_ASSERT((filename_p == nullptr) || (*filename_p == null), "The file name is null.");

		FE::ASCII* l_typename = FE::framework::reflection::type_id<T>().name();
		auto l_search_result = s_property_map->find(l_typename);
		if((l_search_result == s_property_map->end()) || (l_search_result->second.size() == 0)) // return if it is empty or not found.
		{
			FE_LOG("deserialization failed: could not find the requested type information or the class/struct is empty");
			return;
		}
		tl_s_class_layer.push(typename class_layer_stack::value_type{ &(l_search_result->second), l_search_result->second.begin()});

		if constexpr (FE::has_base_type<T>::value == true)
		{
			__push_parent_class_layers_recursive<T>();
		}

		FE::fstream_guard l_serialization_handler(tl_s_fstream);
		l_serialization_handler.get_stream().open((path_p / filename_p).c_str(), std::ios::in | std::ios::binary | std::ios::ate); // Open the file as a binary file and point at the end of the file to input to a buffer.
		FE_ASSERT(l_serialization_handler.get_stream().is_open() == false, "Assertion failure: the target directory path to the file is invalid.");
		
		tl_s_input_buffer.reserve(l_serialization_handler.get_stream().tellg()); 
		tl_s_input_buffer.assign(tl_s_input_buffer.capacity()-1, '\0'); // Set the buffer with null.
		l_serialization_handler.get_stream().seekg(0, std::ios::beg); // Set the indicator to the begining of the file.
		l_serialization_handler.get_stream().read(tl_s_input_buffer.data(), tl_s_input_buffer.length()); // Load the serialized data onto the buffer.
		
		tl_s_position = tl_s_input_buffer.begin();
		tl_s_position += tl_s_input_buffer.find('{');
		FE_ASSERT(tl_s_position == tl_s_input_buffer.end(), "The serialization file is ill-formed or unsupported.");

		// Checks the class type name
		FE_EXIT(!algorithm::string::compare_ranged<char>(	l_typename, algorithm::string::range{0, std::strlen(l_typename)}, 
															tl_s_input_buffer.c_str(), algorithm::string::range{0, static_cast<index_t>(tl_s_position - tl_s_input_buffer.begin()) }),
															FE::ERROR_CODE::_FATAL_SERIALIZATION_ERROR_3XX_TYPE_MISMATCH, "Unable to deserialize an instance with a different class name.");
		++tl_s_position; // Point the first byte.

		__deserialize_mutually_recursive<T>(out_object_p);
	}

private:

    template<class U>
	_FORCE_INLINE_ static void __push_parent_class_layers_recursive() noexcept
	{
		if constexpr ( FE::has_base_type<U>::value == true )
		{
			thread_local static typename  property_meta_data_map_type::iterator tl_s_search_result;
			tl_s_search_result = s_property_map->find( FE::framework::reflection::type_id<typename U::base_type>().name() );
			if(tl_s_search_result == s_property_map->end())
			{
				return;
			}
			tl_s_class_layer.push(typename class_layer_stack::value_type{ &(tl_s_search_result->second), tl_s_search_result->second.begin()});
			__push_parent_class_layers_recursive<typename U::base_type>();
		}
	}

	_FORCE_INLINE_ static void __push_parent_class_layers_by_typename_string_recursive(FE::ASCII* typename_p) noexcept
	{
		thread_local static typename  property_meta_data_map_type::iterator tl_s_search_result;
		tl_s_search_result = s_property_map->find( FE::framework::reflection::type_info::get_base_name_of(typename_p) );
		if(tl_s_search_result == s_property_map->end())
		{
			return;
		}
		tl_s_class_layer.push( typename class_layer_stack::value_type{ &(tl_s_search_result->second), tl_s_search_result->second.begin() } );
		__push_parent_class_layers_by_typename_string_recursive( tl_s_search_result->first );
	}

	template<typename T>
    static void __serialize_mutually_recursive(const T& object_p) noexcept
	{
		var::ptrdiff l_offset_from_this = 0;
		while(tl_s_class_layer.empty() == false)
		{
			switch(tl_s_class_layer.top()._second->_value._is_trivial)
			{
			case TYPE_TRIVIALITY::_TRIVIAL:
			{
				// Acquire an iterator to the top class layer.
				auto l_property_meta_data = tl_s_class_layer.top()._second; // The first is the data structure that represents the class memory layer and the second is the property meta data iterator.
				
				// Check if the field variable meta data is valid.
				FE_ASSERT(l_property_meta_data->_value._total_size == 0, "Assertion failed: unable to serialize a zero-byte property.");

				tl_s_fstream.write(reinterpret_cast<const char*>(&object_p) + (l_offset_from_this + l_property_meta_data->get_key()), l_property_meta_data->_value._total_size);

				// Look for the next registered property of the class layer.
				++(tl_s_class_layer.top()._second);

				// Pop the class layer if the iterator reached the end of the property list.
				if(tl_s_class_layer.top()._second == tl_s_class_layer.top()._first->end())
				{
					tl_s_class_layer.pop();
				}
				break;
			}

			case TYPE_TRIVIALITY::_NOT_TRIVIAL:
				
				if(tl_s_class_layer.top()._second == tl_s_class_layer.top()._first->end()) // Pop the class layer if the iterator reached the end of the property list.
				{
					tl_s_class_layer.pop();
					break;
				}

				// Find the class/struct meta data that contains its memory layer.
				auto l_search_result = s_property_map->find(tl_s_class_layer.top()._second->_value._typename);
				
				FE::task_base* const l_foreach_task = FE::framework::reflection::function::retrieve(__get_serialization_task_name(tl_s_class_layer.top()._second->_value._typename)); // Load function pointer.
				if(l_foreach_task != nullptr) // This is to serialize and deserialize containers and class instances that can be iterated through foreach. 
				{ 
					FE::arguments<void*> l_pointer_to_data; // Any containers with begin() and end() can be serialized and deserialized.
					(*l_foreach_task)(&l_pointer_to_data); // The pointed task object knows what to do with the arguments type casting.
					// std::any can be used instead of FE::arguments_base* for safer type conversion. 
				}

				if(l_search_result != s_property_map->end())
				{
					l_offset_from_this = tl_s_class_layer.top()._second->get_key();
					
					// Look for the next registered property of the class layer.
					++(tl_s_class_layer.top()._second);

					// Push the member variable iterator and the class meta data to the class layer if the class meta data is found.
					tl_s_class_layer.push(typename class_layer_stack::value_type{ &(l_search_result->second), l_search_result->second.begin()});
					__push_parent_class_layers_by_typename_string_recursive(l_search_result->first);
				}
				else
				{
					++(tl_s_class_layer.top()._second);
				}
				break;
			}
		}
	}

    template<typename T>
	static void __deserialize_mutually_recursive(T& out_object_p) noexcept
	{
		var::ptrdiff l_offset_from_this = 0;
		while(tl_s_class_layer.empty() == false)
		{
			switch(tl_s_class_layer.top()._second->_value._is_trivial)
			{
			case TYPE_TRIVIALITY::_TRIVIAL:
				{
					// Acquire an iterator to the top class layer.
					auto l_property_meta_data = tl_s_class_layer.top()._second;
				
					// Check if the meta data is valid.
					FE_ASSERT(l_property_meta_data->_value._total_size == 0, "Assertion failed: unable to serialize a zero-byte property.");

					// Write the bits to the object, from a buffer.	
					FE::memcpy(reinterpret_cast<var::byte*>(&out_object_p) + (l_offset_from_this + l_property_meta_data->get_key()), FE::iterator_cast<FE::ASCII*>(tl_s_position), l_property_meta_data->_value._total_size);
					tl_s_position += l_property_meta_data->_value._total_size; // Iterate to the next bits.
				
					// Look for the next registered property of the class layer.
					++(tl_s_class_layer.top()._second);

					// Pop the class layer if the iterator reached the end of the property list.
					if(tl_s_class_layer.top()._second == tl_s_class_layer.top()._first->end())
					{
						tl_s_class_layer.pop();
					}
					break;
				}

			case TYPE_TRIVIALITY::_NOT_TRIVIAL:
				
				if(tl_s_class_layer.top()._second == tl_s_class_layer.top()._first->end()) // Pop the class layer if the iterator reached the end of the property list.
				{
					tl_s_class_layer.pop();
					break;
				}

				// Find the class/struct meta data that contains its memory layer.
				auto l_search_result = s_property_map->find(tl_s_class_layer.top()._second->_value._typename);

				FE::task_base* const l_foreach_task = FE::framework::reflection::function::retrieve(__get_deserialization_task_name(tl_s_class_layer.top()._second->_value._typename)); // Load function pointer.
				if(l_foreach_task != nullptr) // This is to serialize and deserialize containers and class instances that can be iterated through foreach. 
				{ 
					FE::arguments<void*> l_pointer_to_data; // Any containers with begin() and end() can be serialized and deserialized.
					(*l_foreach_task)(&l_pointer_to_data); // The pointed task object knows what to do with the arguments type casting.
					// std::any can be used instead of FE::arguments_base* for safer type conversion. 
				}

				if(l_search_result != s_property_map->end())
				{
					l_offset_from_this = tl_s_class_layer.top()._second->get_key();
				
					// Look for the next registered property of the class layer.
					++(tl_s_class_layer.top()._second);

					// Push the member variable iterator and the class meta data to the class layer if the class meta data is found.
					tl_s_class_layer.push(typename class_layer_stack::value_type{ &(l_search_result->second), l_search_result->second.begin()});
					__push_parent_class_layers_by_typename_string_recursive(l_search_result->first);
				}
				else
				{
					// Look for the next registered property of the class layer.
					++(tl_s_class_layer.top()._second);
				}
				break;
			}
		}
	}

    template<class Data>
	_FORCE_INLINE_ static void __serialize_by_foreach_mutually_recursive(Data* const data_p) noexcept
	{
		for(auto& element : *data_p)
		{
			if constexpr (FE::has_value_type<Data>::value == true)
			{
				if constexpr (FE::is_reflectable<typename Data::value_type>::value == true)
				{
					__serialize_mutually_recursive<typename Data::value_type>(element);
				}
				else if constexpr (FE::has_iterator<typename Data::value_type>::value == true)
				{
					__serialize_by_foreach_mutually_recursive<typename Data::value_type>(&element);
				}
				else if constexpr (FE::is_trivial<typename Data::value_type>::value == true)
				{
					tl_s_fstream.write(reinterpret_cast<const char*>(&element), sizeof(typename Data::value_type));
				}
			}
			else if constexpr (FE::has_element_type<Data>::value == true)
			{
				if constexpr (FE::is_reflectable<typename Data::element_type>::value == true)
				{
					__serialize_mutually_recursive<typename Data::element_type>(element);
				}
				else if constexpr (FE::has_iterator<typename Data::element_type>::value == true)
				{
					__serialize_by_foreach_mutually_recursive<typename Data::element_type>(&element);
				}
				else if constexpr (FE::is_trivial<typename Data::element_type>::value == true)
				{
					tl_s_fstream.write(reinterpret_cast<const char*>(&element), sizeof(typename Data::element_type));
				}
			}
		}
	}

	_FORCE_INLINE_ static FE::ASCII* __get_serialization_task_name(FE::ASCII* property_typename_p) noexcept
	{
		thread_local static std::string tl_s_serialization_task_name = "FE::framework::reflection::property::__serialize_by_foreach_mutually_recursive< >";
		tl_s_serialization_task_name.replace(	tl_s_serialization_task_name.cbegin() + (tl_s_serialization_task_name.find('<') + 1), 
												tl_s_serialization_task_name.cbegin() + (tl_s_serialization_task_name.length() - 1),
												property_typename_p
												);
		return tl_s_serialization_task_name.c_str();
	}

    template<class Data>
	_FORCE_INLINE_ static void __deserialize_by_foreach_mutually_recursive(Data* const data_p) noexcept
	{
		for(auto& element : *data_p)
		{
			if constexpr (FE::has_value_type<Data>::value == true)
			{
				if constexpr (FE::is_reflectable<typename Data::value_type>::value == true)
				{
					__deserialize_mutually_recursive<typename Data::value_type>(element);
				}
				else if constexpr (FE::has_iterator<typename Data::value_type>::value == true)
				{
					__deserialize_by_foreach_mutually_recursive<typename Data::value_type>(&element);
				}
				else if constexpr (FE::is_trivial<typename Data::value_type>::value == true)
				{
					// Write the bits to the object, from a buffer.	
					FE::memcpy(reinterpret_cast<var::byte*>(&element), FE::iterator_cast<FE::ASCII*>(tl_s_position), sizeof(typename Data::value_type));
					tl_s_position += sizeof(typename Data::value_type); // Iterate to the next bits.
				}
			}
			else if constexpr (FE::has_element_type<Data>::value == true)
			{
				if constexpr (FE::is_reflectable<typename Data::element_type>::value == true)
				{
					__deserialize_mutually_recursive<typename Data::element_type>(element);
				}
				else if constexpr (FE::has_iterator<typename Data::element_type>::value == true)
				{
					__deserialize_by_foreach_mutually_recursive<typename Data::element_type>(&element);
				}
				else if constexpr (FE::is_trivial<typename Data::element_type>::value == true)
				{
					// Write the bits to the object, from a buffer.	
					FE::memcpy(reinterpret_cast<var::byte*>(&element), FE::iterator_cast<FE::ASCII*>(tl_s_position), sizeof(typename Data::element_type));
					tl_s_position += sizeof(typename Data::element_type); // Iterate to the next bits.
				}
			}
		}
	}

	_FORCE_INLINE_ static FE::ASCII* __get_deserialization_task_name(FE::ASCII* property_typename_p) noexcept
	{
		thread_local static std::string tl_s_deserialization_task_name = "FE::framework::reflection::property::__deserialize_by_foreach_mutually_recursive<>";
		tl_s_deserialization_task_name.replace(	tl_s_deserialization_task_name.cbegin() + (tl_s_deserialization_task_name.find('<') + 1), 
												tl_s_deserialization_task_name.cbegin() + (tl_s_deserialization_task_name.length() - 1),
												property_typename_p
												);
		return tl_s_deserialization_task_name.c_str();
	}

	static void __load_default_class_metadata() noexcept {}
};

END_NAMESPACE
#endif