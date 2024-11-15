#ifndef _FE_FRAMEWORK_REFLECTION_PROPERTY_HPP_
#define _FE_FRAMEWORK_REFLECTION_PROPERTY_HPP_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include <FE/prerequisites.h>
#include <FE/algorithm/utility.hxx>
#include <FE/fstream_guard.hxx>
#include <FE/hash.hpp>
#include <FE/pair.hxx>
#include <FE/fqueue.hxx>
#include <FE/fstack.hxx>
#include <FE/type_traits.hxx>

// FE.framework
#include <FE/framework/reflection/method.hpp>
#include <FE/framework/reflection/type_info.hpp>

// std
#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>

// boost::fast_pool_allocator
#include <boost/pool/pool_alloc.hpp>

// boost::shared_lock_guard
#include <boost/thread/shared_lock_guard.hpp>

// ronbin hood hash
#include <robin_hood.h>


/*
Frogman Engine serialization & deserialization system:

  Compatible component types: Plain old data, trivial fixed-sized arrays, FE smart pointers, std::basic_string, std::array, and std::vector.

	* C/C++ object that might contain a chain of derivation, components, other supported types, and a component's chain of derivation.

	* Raw pointers, non-trivial fixed-sized arrays, and references are not supported.
*/
BEGIN_NAMESPACE(FE::framework::reflection)
/*
memory layout reflection for serialization  																		      |--- etc...
																					  *
																					  |
	Shallower <----- Memory Hierarchy Depth ----> Deeper                      Lower Memory Address
													  |----------------|              |
													  | - FE::string - |              |
	|----------------------------------|    ----------|  FE::smart_ptr |              |
	|  Target Entry Non-Trivial Object |    |         |length, capacity|              |
	|- member variables -              |    |         |----------------|              |
	|  FE::string m_raw_name ----------|----|                                         |
	|  FE::vector<float64, 3> m_vector |-------|                                      |
	|----------------------------------|       |       |----------------|             |
											   |       | - FE::vector - |             |
											   |-------| x              |             |
													   | y              |             |
													   | z              |             |
													   |----------------|     Higher Memory Address
																					  |
																					  *

Memory Layer Traversal Order: Entry.FE::string m_raw_name -> FE::string.FE::smart_ptr -> FE::smart_ptr.m_smart_ptr data
																									|
													|-----------------------------------------------|
													|
													|--> FE::string.length -> FE::string.capacity
																					   |
																					   |
											   |---------------------------------------|
											   |
											   |--> FE::vector.x -> FE::vector.y -> FE::vector.z
*/

class framework_base;

struct property_meta_data
{
	FE::TypeTriviality _is_trivial; 
	var::boolean _is_serializable;
	var::uint32 _size_in_byte; 
	
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
	using reflection_map_type = robin_hood::unordered_map<FE::ASCII*, 
                                                              std::map<var::ptrdiff, property_meta_data, std::less<var::ptrdiff>, 
		                                                               boost::fast_pool_allocator< std::pair<FE::ptrdiff, property_meta_data>, boost::default_user_allocator_new_delete, boost::details::pool::null_mutex>
															          >,
		                                                      FE::hash<FE::ASCII*>>;
	using class_name_type = reflection_map_type::key_type;
	using class_property_list = reflection_map_type::mapped_type;
	using class_property_offset_type = typename class_property_list::key_type;
	using class_property_metadata_type = typename class_property_list::mapped_type;

	static constexpr size max_class_hierarchy_depth = 1024;
	using class_layer_stack = FE::fstack< FE::pair<class_property_list*, typename class_property_list::iterator>, max_class_hierarchy_depth >;
	
	static constexpr count_t number_of_max_container_per_instance = 1024;
	using data_on_heap_size_recorder = FE::fqueue<var::size, number_of_max_container_per_instance>;

	using lock_type = std::mutex;
	using file_handler = std::fstream;
	using input_buffer_type = std::string;
	using input_buffer_iterator_type = typename input_buffer_type::iterator;

private:
	reflection_map_type m_property_map;
	class_layer_stack m_class_layer;
	data_on_heap_size_recorder m_scalable_container_size_recorder;

	lock_type m_lock;
	file_handler m_fstream;
	input_buffer_type m_input_buffer;
	input_buffer_iterator_type m_position;

public:
	property(FE::size reflection_map_capacity_p) noexcept
		: m_property_map(reflection_map_capacity_p), m_class_layer(), m_scalable_container_size_recorder(), m_lock(), m_fstream(), m_input_buffer(), m_position() {}
	~property() noexcept = default;

	property(const property&) = delete;
	property(property&&) = delete;

	property& operator=(const property&) = delete;
	property& operator=(property&&) = delete;

	_FE_FORCE_INLINE_ void reserve(size size_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(m_lock);
		this->m_property_map.reserve(size_p);
	}

    template<class C, typename T>
	void register_property(const C& host_class_instance_p, const T& property_p, FE::ASCII* const property_name_p) noexcept
	{
		FE_NEGATIVE_STATIC_ASSERT(std::is_class<C>::value == false, "Primitive data types cannot be registered as the host classes/structs.");
		FE_NEGATIVE_STATIC_ASSERT((std::is_reference<T>::value == true) || (std::is_pointer<T>::value == true), "Static assertion failure: raw pointers and references cannot be serialized nor deserialized.");
		FE_NEGATIVE_STATIC_ASSERT((FE::is_trivial<T>::value == false) && (std::is_array<T>::value == true), "Static assertion failure: fixed-sized non-trivial arrays are not serializable nor deserializable.");
		FE_NEGATIVE_ASSERT((property_name_p == nullptr) || (*property_name_p == null), "Assertion failure: property name cannot be null.");

		property_meta_data l_property_meta_data;
		l_property_meta_data._is_trivial = static_cast<TypeTriviality>(FE::is_trivial<T>::value);
		l_property_meta_data._is_serializable = FE::is_serializable<T>::value;
		l_property_meta_data._size_in_byte = sizeof(T);
		l_property_meta_data._name = property_name_p;
		l_property_meta_data._typename = FE::framework::reflection::type_id<T>().name();

		// Find or register its host class.
		FE::ASCII* l_host_class_instance_typename = FE::framework::reflection::type_id<C>().name();

		std::lock_guard<lock_type> l_lock(m_lock);
		auto l_iterator = m_property_map.find(l_host_class_instance_typename);
		if(FE_UNLIKELY(l_iterator == m_property_map.end())) _FE_UNLIKELY_
		{
			auto l_result = m_property_map.emplace(l_host_class_instance_typename, class_property_list());
			FE_NEGATIVE_ASSERT(l_result.second == false, "Failed to robin_hood::unordered_map::emplace() while executing property::register_property().");
			l_iterator = l_result.first;
		}

		
		if constexpr ((FE::is_serializable<T>::value == true) && (FE::is_trivial<T>::value == false))
		{
			// This code section for serializing and deserializing complicated multidimensional container and the third-party containers.
			// It enables the system to serialize and deserialize the class instance without boilerplating the code with the Frogman Engine reflection macros.
			FE::framework::reflection_system::access()._method_reflection.register_task< FE::cpp_style_task<property, void(const void*)>>(__get_serialization_task_name(l_property_meta_data._typename), &property::__serialize_by_foreach_mutually_recursive<T>);
			FE::framework::reflection_system::access()._method_reflection.register_task< FE::cpp_style_task<property, void(void*)>>(__get_deserialization_task_name(l_property_meta_data._typename), &property::__deserialize_by_foreach_mutually_recursive<T>);

			if constexpr (FE::has_value_type<T>::value == true)
			{
				if constexpr ((FE::is_serializable<typename T::value_type>::value == true) &&
					          (FE::is_trivial<typename T::value_type>::value == false))
				{
					__push_multidimensional_container_serialization_task_recursive<T>();
					__push_multidimensional_container_deserialization_task_recursive<T>();
				}
			}
			else if constexpr (FE::has_element_type<T>::value == true)
			{
				if constexpr ((FE::is_serializable<typename T::element_type>::value == true) &&
					          (FE::is_trivial<typename T::element_type>::value == false))
				{
					__push_multidimensional_container_serialization_task_recursive<T>();
					__push_multidimensional_container_deserialization_task_recursive<T>();
				}
			}
		}

		l_iterator->second.emplace((reinterpret_cast<FE::byte* const>(&property_p) - reinterpret_cast<FE::byte*>(&host_class_instance_p)), l_property_meta_data); 
	}

    template<typename T>
    void serialize(std::filesystem::path path_p, directory_char_t* filename_p, const T& object_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(m_lock);

		FE_NEGATIVE_STATIC_ASSERT(std::is_class<T>::value == false, "Non-class/struct field variables cannot be serialized.");
		FE_NEGATIVE_STATIC_ASSERT((std::is_reference<T>::value == true) || (std::is_pointer<T>::value == true), "static assertion failure: raw pointers and references cannot be serialized nor deserialized.");
		
		FE_NEGATIVE_ASSERT(path_p.empty() == true, "Assertion failure: the target directory path to the file is nullptr.");
		FE_NEGATIVE_ASSERT((filename_p == nullptr) || (*filename_p == null), "The file name is null.");

		FE::ASCII* l_typename = FE::framework::reflection::type_id<T>().name();
		auto l_search_result = m_property_map.find(l_typename);
		FE_EXIT( (l_search_result == m_property_map.end()) || (l_search_result->second.size() == 0), ErrorCode::_FATAL_SERIALIZATION_ERROR_3XX_TYPE_NOT_FOUND, "Serialization failed: could not find the requested type information or the class/struct is empty");
		this->m_class_layer.push(typename class_layer_stack::value_type{ &(l_search_result->second), l_search_result->second.begin() } );

		if constexpr (FE::has_base_type<T>::value == true)
		{
			__push_parent_class_layers_recursive<T>();
		}

		if (std::filesystem::exists(path_p) == false)
    	{
        	std::filesystem::create_directory(path_p);
    	}

		FE::fstream_guard l_serialization_handler(m_fstream);
		l_serialization_handler.get_stream().open((path_p / filename_p).c_str(), std::ios::out | std::ios::binary);
		l_serialization_handler.get_stream() << l_typename << "{";
		__serialize_mutually_recursive<T>(object_p);
		l_serialization_handler.get_stream() << "};$-";

		while (this->m_scalable_container_size_recorder.is_empty() == false)
		{
			l_serialization_handler.get_stream() << this->m_scalable_container_size_recorder.pop() << "-";
		}
		l_serialization_handler.get_stream() << "\0";
	}

	// TODO: add file versioning.
    template<typename T>
	void deserialize(std::filesystem::path path_p, directory_char_t* filename_p, T& out_object_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(m_lock);

		FE_NEGATIVE_STATIC_ASSERT(std::is_class<T>::value == false, "Non-class/struct field variables cannot be deserialized.");
		FE_NEGATIVE_STATIC_ASSERT((std::is_reference<T>::value == true) || (std::is_pointer<T>::value == true), "static assertion failure: raw pointers and references cannot be serialized nor deserialized.");
		
		FE_NEGATIVE_ASSERT(std::filesystem::exists(path_p) == false, "Assertion failure: the target directory path to the file is nullptr.");
		FE_NEGATIVE_ASSERT((filename_p == nullptr) || (*filename_p == null), "The file name is null.");

		FE::ASCII* l_typename = FE::framework::reflection::type_id<T>().name();
		auto l_search_result = this->m_property_map.find(l_typename);
		FE_EXIT((l_search_result == m_property_map.end()) || (l_search_result->second.size() == 0), ErrorCode::_FATAL_SERIALIZATION_ERROR_3XX_TYPE_NOT_FOUND, "serialization failed: could not find the requested type information or the class/struct is empty");
		m_class_layer.push(typename class_layer_stack::value_type{ &(l_search_result->second), l_search_result->second.begin() });

		if constexpr (FE::has_base_type<T>::value == true)
		{
			__push_parent_class_layers_recursive<T>();
		}

		FE::fstream_guard l_serialization_handler(m_fstream);
		l_serialization_handler.get_stream().open((path_p / filename_p).c_str(), std::ios::in | std::ios::binary | std::ios::ate); // Open the file as a binary file and point at the end of the file to input to a buffer.
		FE_NEGATIVE_ASSERT(l_serialization_handler.get_stream().is_open() == false, "Assertion failure: the target directory path to the file is invalid.");
		
		this->m_input_buffer.reserve(l_serialization_handler.get_stream().tellg());
		this->m_input_buffer.assign(this->m_input_buffer.capacity()-1, '\0'); // Set the buffer with null.
		l_serialization_handler.get_stream().seekg(0, std::ios::beg); // Set the indicator to the begining of the file.
		l_serialization_handler.get_stream().read(this->m_input_buffer.data(), this->m_input_buffer.length()); // Load the serialized data onto the buffer.
		
		this->m_position = this->m_input_buffer.begin();
		this->m_position += this->m_input_buffer.find('{');
		FE_NEGATIVE_ASSERT(m_position == m_input_buffer.end(), "The serialization file is ill-formed or unsupported.");

		// Checks the class type name
		FE_EXIT(!algorithm::string::compare_ranged<char>(	l_typename, algorithm::string::range{0, std::strlen(l_typename)}, 
															m_input_buffer.c_str(), algorithm::string::range{0, static_cast<index_t>(m_position - m_input_buffer.begin()) }),
															FE::ErrorCode::_FATAL_SERIALIZATION_ERROR_3XX_TYPE_MISMATCH, "Unable to deserialize an instance with a different class name.");
		++m_position; // Point the first byte.

		auto l_size_indicator = this->m_input_buffer.begin();
		l_size_indicator += this->m_input_buffer.rfind("$-");
		l_size_indicator += 2; // to skip the "$-" and point to the first one.

		while (*l_size_indicator != '\0')
		{
			algorithm::utility::uint_info l_info = algorithm::utility::string_to_uint(FE::iterator_cast<FE::ASCII*>(l_size_indicator));
			FE_LOG_IF(l_info._value == 0, "Warning: the size of the container is zero. Please debug if the file is corrupted.");
			this->m_scalable_container_size_recorder.push(l_info._value);
			l_size_indicator += l_info._digit_length; // move to the next.
			++l_size_indicator; // to skip the '-'.
		}

		__deserialize_mutually_recursive<T>(out_object_p);
	}

private:
	template <class InnerContainer>
	_FE_FORCE_INLINE_ void __push_multidimensional_container_serialization_task_recursive() noexcept
	{
		FE::framework::reflection_system::access()._method_reflection.register_task< FE::cpp_style_task<property, void(const void*)> >(__get_serialization_task_name(FE::framework::reflection::type_id<InnerContainer>().name()), &property::__serialize_by_foreach_mutually_recursive<InnerContainer>);
		
		if constexpr (FE::has_value_type<InnerContainer>::value == true)
		{
			__push_multidimensional_container_serialization_task_recursive<typename InnerContainer::value_type>();
		}
		else if constexpr (FE::has_element_type<InnerContainer>::value == true)
		{
			__push_multidimensional_container_serialization_task_recursive<typename InnerContainer::element_type>();
		}
	}

	template <class InnerContainer>
	_FE_FORCE_INLINE_ void __push_multidimensional_container_deserialization_task_recursive() noexcept
	{
		FE::framework::reflection_system::access()._method_reflection.register_task< FE::cpp_style_task<property, void(void*)> >(__get_deserialization_task_name(FE::framework::reflection::type_id<InnerContainer>().name()), &property::__deserialize_by_foreach_mutually_recursive<InnerContainer>);

		if constexpr (FE::has_value_type<InnerContainer>::value == true)
		{
			__push_multidimensional_container_deserialization_task_recursive<typename InnerContainer::value_type>();
		}
		else if constexpr (FE::has_element_type<InnerContainer>::value == true)
		{
			__push_multidimensional_container_deserialization_task_recursive<typename InnerContainer::element_type>();
		}
	}

	/* 
	I do not like short functions but this is the way to make the code readable.
	It is hard to tell which 'first' and 'second' are which.
	*/
	_FE_FORCE_INLINE_ typename class_property_list::iterator& __get_the_top_class_property_list_iterator() noexcept
	{
		return this->m_class_layer.top()._second;
	}

	_FE_FORCE_INLINE_ FE::ptrdiff __get_memory_offset_of_the_property(typename class_property_list::iterator& to_the_property_p) noexcept
	{
		return to_the_property_p->first;
	}

	_FE_FORCE_INLINE_ property_meta_data& __get_metadata_of_the_property(typename class_property_list::iterator& to_the_property_p) noexcept
	{
		return to_the_property_p->second;
	}

	_FE_FORCE_INLINE_ class_property_list& __get_top_class_property_list() noexcept
	{
		return *(this->m_class_layer.top()._first);
	}

    template<class U>
	_FE_FORCE_INLINE_ void __push_parent_class_layers_recursive() noexcept
	{
		if constexpr ( FE::has_base_type<U>::value == true )
		{
			static typename  reflection_map_type::iterator l_s_search_result;
			l_s_search_result = this->m_property_map.find( FE::framework::reflection::type_id<typename U::base_type>().name() );
			if(l_s_search_result == this->m_property_map.end())
			{
				return;
			}
			this->m_class_layer.push(typename class_layer_stack::value_type{ &(l_s_search_result->second), l_s_search_result->second.begin() });
			__push_parent_class_layers_recursive<typename U::base_type>();
		}
	}

	_FE_FORCE_INLINE_ void __push_parent_class_layers_by_typename_string_recursive(FE::ASCII* typename_p) noexcept
	{
		static typename  reflection_map_type::iterator l_s_search_result;
		l_s_search_result = this->m_property_map.find( FE::framework::reflection::type_info::get_base_name_of(typename_p) );
		if(l_s_search_result == this->m_property_map.end())
		{
			return;
		}
		this->m_class_layer.push(typename class_layer_stack::value_type{ &(l_s_search_result->second), l_s_search_result->second.begin() });
		__push_parent_class_layers_by_typename_string_recursive( l_s_search_result->first );
	}

	template<typename T>
    void __serialize_mutually_recursive(const T& object_p) noexcept
	{
		var::ptrdiff l_offset_from_the_upmost_base_class_instance = 0;
		while(this->m_class_layer.is_empty() == false)
		{
			switch(__get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._is_trivial)
			{
			case TypeTriviality::_Trivial:
			{
				// Check if the field variable meta data is valid.
				FE_NEGATIVE_ASSERT(__get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._size_in_byte == 0, "Assertion failed: unable to serialize a zero-byte property.");

				this->m_fstream.write(reinterpret_cast<const char*>(&object_p) + (l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property( __get_the_top_class_property_list_iterator() )), __get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._size_in_byte);
	
				// Look for the next registered property of the class.
				++( __get_the_top_class_property_list_iterator() );

				// Pop the class layer if the iterator reached the end of the property list.
				if(__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
				{
					this->m_class_layer.pop();
				}
				break;
			}

			case TypeTriviality::_NotTrivial:
				
				if(__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end()) // Pop the class layer if the iterator reached the end of the property list.
				{
					this->m_class_layer.pop();
					break;
				}
				
				// Find the class/struct meta data that contains its memory layer.
				auto l_search_result = this->m_property_map.find(__get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._typename);
				
				// This is to serialize and deserialize containers and class instances that can be iterated through foreach. 
				FE::task_base* const l_foreach_task = FE::framework::reflection_system::access()._method_reflection.retrieve(__get_serialization_task_name(__get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._typename)); // Load method pointer.
				if(l_foreach_task != nullptr) // is serializable with foreach?
				{ 
					l_foreach_task->set_instance(this);
					FE::arguments<const void*> l_pointer_to_container; // Any containers with begin() and end() can be serialized and deserialized.
					l_pointer_to_container._first = reinterpret_cast<FE::byte*>(&object_p) + (l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property( __get_the_top_class_property_list_iterator() ));
					(*l_foreach_task)(&l_pointer_to_container); // The pointed task object_base knows what to do with the arguments type casting.
					
					// Move on to the next registered property of the class layer.
					++(__get_the_top_class_property_list_iterator());
				}
				else if(l_search_result != this->m_property_map.end()) // push the meta data onto the stack if found.
				{
					l_offset_from_the_upmost_base_class_instance = __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator());
					
					// Look for the next registered property of the class layer.
					++( __get_the_top_class_property_list_iterator() );

					// Push the member variable iterator and the class meta data to the class layer if the class meta data is found.
					this->m_class_layer.push(typename class_layer_stack::value_type{ &(l_search_result->second), l_search_result->second.begin() });
					__push_parent_class_layers_by_typename_string_recursive(l_search_result->first);
				}

				// if it reached the end.
				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
				{
					this->m_class_layer.pop();
				}
				break;
			}
		}
	}

    template<typename T>
	void __deserialize_mutually_recursive(T& out_object_p) noexcept
	{
		var::ptrdiff l_offset_from_the_upmost_base_class_instance = 0;
		while(this->m_class_layer.is_empty() == false)
		{
			switch(__get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._is_trivial)
			{
			case TypeTriviality::_Trivial:
				{
					// Check if the meta data is valid.
					FE_NEGATIVE_ASSERT(__get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._size_in_byte == 0, "Assertion failed: unable to serialize a zero-byte property.");

					// Write the bits to the object_base, from a buffer.	
					FE::memcpy(reinterpret_cast<var::byte*>(&out_object_p) + (l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property( __get_the_top_class_property_list_iterator() )), FE::iterator_cast<FE::ASCII*>(m_position), __get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._size_in_byte);
					this->m_position += __get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._size_in_byte; // Iterate to the next bits.
				
					// Look for the next registered property of the class layer.
					++(__get_the_top_class_property_list_iterator());

					// Pop the class layer if the iterator reached the end of the property list.
					if(__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
					{
						this->m_class_layer.pop();
					}
					break;
				}

			case TypeTriviality::_NotTrivial:
				
				if(__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end()) // Pop the class layer if the iterator reached the end of the property list.
				{
					this->m_class_layer.pop();
					break;
				}

				// Find the class/struct meta data that contains its memory layer.
				auto l_search_result = this->m_property_map.find(__get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._typename);

				// This is to serialize and deserialize containers and class instances that can be iterated through foreach. 
				FE::task_base* const l_foreach_task = FE::framework::reflection_system::access()._method_reflection.retrieve( __get_deserialization_task_name( __get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._typename ) ); // Load method pointer.
				if(l_foreach_task != nullptr) // is deserializable with foreach?
				{ 
					l_foreach_task->set_instance(this);
					FE::arguments<void*> l_pointer_to_container; // Any containers with begin() and end() can be serialized and deserialized.
					l_pointer_to_container._first = reinterpret_cast<var::byte*>(&out_object_p) + (l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator()));
					(*l_foreach_task)(&l_pointer_to_container); // The pointed task object_base knows what to do with the arguments type casting.
					
					// Look for the next registered property of the class layer.
					++(__get_the_top_class_property_list_iterator());
				}
				else if(l_search_result != this->m_property_map.end()) // push the meta data onto the stack if found.
				{
					l_offset_from_the_upmost_base_class_instance = __get_memory_offset_of_the_property( __get_the_top_class_property_list_iterator() );
				
					// Look for the next registered property of the class layer.
					++( __get_the_top_class_property_list_iterator() );

					// Push the member variable iterator and the class meta data to the class layer if the class meta data is found.
					this->m_class_layer.push(typename class_layer_stack::value_type{ &(l_search_result->second), l_search_result->second.begin() });
					__push_parent_class_layers_by_typename_string_recursive(l_search_result->first);
				}
				
				// if it reached the end.
				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
				{
					this->m_class_layer.pop();
				}
				break;
			}
		}
	}

    template<class Container>
	void __serialize_by_foreach_mutually_recursive(const void* const data_p) noexcept
	{
		//FE_NEGATIVE_STATIC_ASSERT(FE::is_trivial<Container>::value == true, "Incorrect template argument type: serializable containers are not trivially constructible and destructible.");
		FE_NEGATIVE_STATIC_ASSERT(FE::is_serializable<Container>::value == false, "The container is unable to be serialized: the container type is not supported and not compatible to this system.");
		FE_NEGATIVE_ASSERT(data_p == nullptr, "Aborting the serialization process: the pointer to the container is nullptr.");
		const Container* const l_container = static_cast<const Container* const>(data_p);

		if constexpr (FE::is_trivial<Container>::value == false)
		{
			if constexpr (FE::is_trivial<typename Container::value_type>::value == true)
			{
				if constexpr (std::is_array<Container>::value == true)
				{
					this->m_fstream.write(reinterpret_cast<const char*>(l_container), sizeof(Container));
				}
				else
				{
					this->m_scalable_container_size_recorder.push(l_container->size());
					this->m_fstream.write(reinterpret_cast<const char*>(l_container->data()), sizeof(typename Container::value_type) * l_container->size());
				}
			}
			else
			{
				this->m_scalable_container_size_recorder.push(l_container->size());
				for (auto& element : *l_container)
				{
					if constexpr (FE::is_reflective<typename Container::value_type>::value == true)
					{
						__serialize_mutually_recursive<typename Container::value_type>(element);
					}
					else if constexpr ((FE::is_serializable<typename Container::value_type>::value == true) && (FE::is_trivial<typename Container::value_type>::value == false))
					{
						__serialize_by_foreach_mutually_recursive<typename Container::value_type>(&element);
					}
					else
					{
						FE_LOG("Warning: could not perform __serialize_by_foreach_mutually_recursive() operation successfully.");
					}
				}
			}
		}
	}

	FE::ASCII* __get_serialization_task_name(FE::ASCII* property_typename_p) noexcept
	{
		static std::basic_string<char, std::char_traits<char>> l_s_serialization_task_name = "FE::framework::reflection::property::__serialize_by_foreach_mutually_recursive< >";
		l_s_serialization_task_name.replace(	l_s_serialization_task_name.cbegin() + (l_s_serialization_task_name.find('<') + 1), 
												l_s_serialization_task_name.cbegin() + (l_s_serialization_task_name.length() - 1),
												property_typename_p
												);
		return l_s_serialization_task_name.c_str();
	}

    template<class Container>
	void __deserialize_by_foreach_mutually_recursive(void* const data_p) noexcept
	{
		//FE_NEGATIVE_STATIC_ASSERT(FE::is_trivial<Container>::value == true, "Incorrect template argument type: serializable containers are not trivially constructible and destructible.");
		FE_NEGATIVE_STATIC_ASSERT(FE::is_serializable<Container>::value == false, "The container is unable to be deserialized: the container type is not supported and not compatible to this system.");
		FE_NEGATIVE_ASSERT(data_p == nullptr, "Aborting the deserialization process: the pointer to the container is nullptr.");
		Container* const l_container = static_cast<Container* const>(data_p);
		
		if constexpr (FE::is_trivial<Container>::value == false)
		{
			l_container->resize(this->m_scalable_container_size_recorder.pop());

			if constexpr (FE::is_trivial<typename Container::value_type>::value == true)
			{
				if constexpr (std::is_array<Container>::value == true)
				{
					FE::memcpy(reinterpret_cast<var::byte*>(l_container), FE::iterator_cast<FE::ASCII*>(m_position), sizeof(Container));
					this->m_position += sizeof(Container); // Iterate to the next bit.
				}
				else
				{
					FE::memcpy(reinterpret_cast<var::byte*>(l_container->data()), FE::iterator_cast<FE::ASCII*>(m_position), sizeof(typename Container::value_type) * l_container->size());
					this->m_position += sizeof(typename Container::value_type) * l_container->size(); // Iterate to the next bit.
				}
			}
			else
			{
				for (auto& element : *l_container)
				{
					if constexpr (FE::is_reflective<typename Container::value_type>::value == true)
					{
						__deserialize_mutually_recursive<typename Container::value_type>(element);
					}
					else if constexpr ((FE::is_serializable<typename Container::value_type>::value == true) && (FE::is_trivial<typename Container::value_type>::value == false))
					{
						__deserialize_by_foreach_mutually_recursive<typename Container::value_type>(&element);
					}
					else
					{
						FE_LOG("Warning: could not perform __deserialize_by_foreach_mutually_recursive() operation successfully.");
					}
				}
			}
		}
	}

	FE::ASCII* __get_deserialization_task_name(FE::ASCII* property_typename_p) noexcept
	{
		static std::basic_string<char, std::char_traits<char>> l_s_deserialization_task_name = "FE::framework::reflection::property::__deserialize_by_foreach_mutually_recursive< >";
		l_s_deserialization_task_name.replace(	l_s_deserialization_task_name.cbegin() + (l_s_deserialization_task_name.find('<') + 1), 
												l_s_deserialization_task_name.cbegin() + (l_s_deserialization_task_name.length() - 1),
												property_typename_p
												);
		return l_s_deserialization_task_name.c_str();
	}
};

END_NAMESPACE
#endif