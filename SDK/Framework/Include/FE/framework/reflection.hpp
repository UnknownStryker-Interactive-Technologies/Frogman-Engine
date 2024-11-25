#ifndef _FE_FRAMEWORK_REFLECTION_HPP_
#define _FE_FRAMEWORK_REFLECTION_HPP_
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

#include <FE/algorithm/string.hxx>
#include <FE/algorithm/utility.hxx>

// FE.Core
#include <FE/do_once.hxx>
#include <FE/fqueue.hxx>
#include <FE/function.hxx>
#include <FE/fstack.hxx>
#include <FE/fstream_guard.hxx>
#include <FE/hash.hpp>
#include <FE/type_traits.hxx>
#include <FE/pair.hxx>
#include <FE/pool/block_pool.hxx>

#include <FE/framework/reflection/type_info.hpp>

// std
#include <filesystem>
#include <fstream>
#include <map>
#include <memory_resource>
#include <mutex>
#include <shared_mutex>
#include <string>

// boost::shared_lock_guard
#include <boost/thread/shared_lock_guard.hpp>

// boost::fast_pool_allocator
#include <boost/pool/pool_alloc.hpp>

// ronbin hood hash map
#include <robin_hood.h>




CLASS_FORWARD_DECLARATION(FE::framework, framework_base);




BEGIN_NAMESPACE(FE::framework::reflection)


template<typename T>
_FE_FORCE_INLINE_ void construct_object(void* address_p) noexcept
{
	new( static_cast<T*>(address_p) ) T();
}

template<typename T>
_FE_FORCE_INLINE_ void destruct_object(void* address_p) noexcept
{
	static_cast<T*>(address_p)->~T();
}

class method
{
	friend class ::FE::framework::framework_base;
public:
	using alignment_type = FE::align_8bytes;

private:
	using lock_type = std::shared_mutex;
	using reflection_map_type = robin_hood::unordered_map<std::pmr::string, FE::task_base*, FE::hash<std::pmr::string>>;

	lock_type m_lock;
	reflection_map_type m_task_map;
	std::pmr::monotonic_buffer_resource m_pool;

private:
	method(FE::size reflection_map_capacity_p) noexcept
		: m_lock(), m_task_map(reflection_map_capacity_p), m_pool()
	{}
	~method() noexcept = default;

public:
	method(const method&) = delete;
	method(method&&) = delete;

	method& operator=(const method&) = delete;
	method& operator=(method&&) = delete;

	_FE_FORCE_INLINE_ void reserve(FE::size size_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(m_lock);
		this->m_task_map.reserve(size_p);
	}

	template<class TaskType, typename FunctionPtr>
	_FE_FORCE_INLINE_ void register_task(const char* task_name_p, FunctionPtr function_p) noexcept
	{
		FE_NEGATIVE_STATIC_ASSERT((std::is_base_of<FE::task_base, TaskType>::value == false), "An invalid method type detected.");

		std::pmr::polymorphic_allocator<TaskType> l_allocator(&m_pool);
		std::pmr::polymorphic_allocator<const char*> l_string_allocator(&m_pool);

		std::lock_guard<lock_type> l_lock(m_lock);

		TaskType* const l_task = l_allocator.allocate(1);
		new(l_task) TaskType(function_p);

		typename reflection_map_type::key_type l_key(l_string_allocator);
		l_key = task_name_p;

		this->m_task_map.emplace(l_key, l_task);
	}

	_FE_FORCE_INLINE_ FE::boolean check_presence(const typename reflection_map_type::key_type& key_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(m_lock);
		return this->m_task_map.find(key_p) != this->m_task_map.end();
	}

	/*
		This method may return nullptr.
	*/
	_FE_NODISCARD_ _FE_FORCE_INLINE_ FE::task_base* retrieve(const typename reflection_map_type::key_type& key_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(m_lock);
		auto l_result = this->m_task_map.find(key_p);
		return (l_result == this->m_task_map.end()) ? nullptr : l_result->second;
	}
};




/*
Frogman Engine serialization & deserialization system:

  Compatible component types: Plain old data, trivial fixed-sized arrays, FE smart pointers, std::basic_string, std::array, and std::vector.

	* C/C++ object that might contain a chain of derivation, components, other supported types, and a component's chain of derivation.

	* Raw pointers, non-trivial fixed-sized arrays, and references are not supported.
*/
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
	friend class ::FE::framework::framework_base;
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

private:
	property(FE::size reflection_map_capacity_p) noexcept
		: m_property_map(reflection_map_capacity_p), m_class_layer(), m_scalable_container_size_recorder(), m_lock(), m_fstream(), m_input_buffer(), m_position() {
	}
	~property() noexcept = default;

public:
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
		l_property_meta_data._typename = reflection::type_id<T>().name();

		// Find or register its host class.
		FE::ASCII* l_host_class_instance_typename = reflection::type_id<C>().name();

		std::lock_guard<lock_type> l_lock(m_lock);
		auto l_iterator = m_property_map.find(l_host_class_instance_typename);
		if (FE_UNLIKELY(l_iterator == m_property_map.end())) _FE_UNLIKELY_
		{
			auto l_result = m_property_map.emplace(l_host_class_instance_typename, class_property_list());
			FE_NEGATIVE_ASSERT(l_result.second == false, "Failed to robin_hood::unordered_map::emplace() while executing property::register_property().");
			l_iterator = l_result.first;
		}


			if constexpr ((FE::is_serializable<T>::value == true) && (FE::is_trivial<T>::value == false))
			{
				// This code section for serializing and deserializing a complicated multidimensional container and the third-party containers.
				// It enables the system to serialize and deserialize a class instance without Frogman Engine reflection macro boilerplates.
				framework_base::get_engine().access_method_reflection().register_task< FE::cpp_style_task<property, void(const void*)>>(__get_serialization_task_name(l_property_meta_data._typename), &property::__serialize_by_foreach_mutually_recursive<T>);
				framework_base::get_engine().access_method_reflection().register_task< FE::cpp_style_task<property, void(void*)>>(__get_deserialization_task_name(l_property_meta_data._typename), &property::__deserialize_by_foreach_mutually_recursive<T>);

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

		FE::ASCII* l_typename = reflection::type_id<T>().name();
		auto l_search_result = m_property_map.find(l_typename);
		FE_EXIT((l_search_result == m_property_map.end()) || (l_search_result->second.size() == 0), ErrorCode::_FATAL_SERIALIZATION_ERROR_3XX_TYPE_NOT_FOUND, "Serialization failed: could not find the requested type information or the class/struct is empty");
		this->m_class_layer.push(typename class_layer_stack::value_type{ &(l_search_result->second), l_search_result->second.begin() });

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

		FE::ASCII* l_typename = reflection::type_id<T>().name();
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
		this->m_input_buffer.assign(this->m_input_buffer.capacity() - 1, '\0'); // Set the buffer with null.
		l_serialization_handler.get_stream().seekg(0, std::ios::beg); // Set the indicator to the begining of the file.
		l_serialization_handler.get_stream().read(this->m_input_buffer.data(), this->m_input_buffer.length()); // Load the serialized data onto the buffer.

		this->m_position = this->m_input_buffer.begin();
		this->m_position += this->m_input_buffer.find('{');
		FE_NEGATIVE_ASSERT(m_position == m_input_buffer.end(), "The serialization file is ill-formed or unsupported.");

		// Checks the class type name
		FE_EXIT(!algorithm::string::compare_ranged<char>(l_typename, algorithm::string::range{ 0, std::strlen(l_typename) },
			m_input_buffer.c_str(), algorithm::string::range{ 0, static_cast<index_t>(m_position - m_input_buffer.begin()) }),
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
		framework_base::get_engine().access_method_reflection().register_task< FE::cpp_style_task<property, void(const void*)> >(__get_serialization_task_name(reflection::type_id<InnerContainer>().name()), &property::__serialize_by_foreach_mutually_recursive<InnerContainer>);

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
		framework_base::get_engine().access_method_reflection().register_task< FE::cpp_style_task<property, void(void*)> >(__get_deserialization_task_name(reflection::type_id<InnerContainer>().name()), &property::__deserialize_by_foreach_mutually_recursive<InnerContainer>);

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
		if constexpr (FE::has_base_type<U>::value == true)
		{
			static typename  reflection_map_type::iterator l_s_search_result;
			l_s_search_result = this->m_property_map.find(reflection::type_id<typename U::base_type>().name());
			if (l_s_search_result == this->m_property_map.end())
			{
				return;
			}
			this->m_class_layer.push(typename class_layer_stack::value_type{ &(l_s_search_result->second), l_s_search_result->second.begin() });
			__push_parent_class_layers_recursive<typename U::base_type>();
		}
	}

	void __push_parent_class_layers_by_typename_string_recursive(FE::ASCII* typename_p) noexcept
	{
		static typename  reflection_map_type::iterator l_s_search_result;
		l_s_search_result = this->m_property_map.find(reflection::type_info::get_base_name_of(typename_p));
		if (l_s_search_result == this->m_property_map.end())
		{
			return;
		}
		this->m_class_layer.push(typename class_layer_stack::value_type{ &(l_s_search_result->second), l_s_search_result->second.begin() });
		__push_parent_class_layers_by_typename_string_recursive(l_s_search_result->first);
	}

	template<typename T>
	void __serialize_mutually_recursive(const T& object_p) noexcept
	{
		var::ptrdiff l_offset_from_the_upmost_base_class_instance = 0;
		while (this->m_class_layer.is_empty() == false)
		{
			switch (__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._is_trivial)
			{
			case TypeTriviality::_Trivial:
			{
				// Check if the field variable meta data is valid.
				FE_NEGATIVE_ASSERT(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._size_in_byte == 0, "Assertion failed: unable to serialize a zero-byte property.");

				this->m_fstream.write(reinterpret_cast<const char*>(&object_p) + (l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator())), __get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._size_in_byte);

				// Look for the next registered property of the class.
				++(__get_the_top_class_property_list_iterator());

				// Pop the class layer if the iterator reached the end of the property list.
				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
				{
					this->m_class_layer.pop();
				}
				break;
			}

			case TypeTriviality::_NotTrivial:

				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end()) // Pop the class layer if the iterator reached the end of the property list.
				{
					this->m_class_layer.pop();
					break;
				}

				// Find the class/struct meta data that contains its memory layer.
				auto l_search_result = this->m_property_map.find(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._typename);

				// This is to serialize and deserialize containers and class instances that can be iterated through foreach. 
				FE::task_base* const l_foreach_task = framework_base::get_engine().access_method_reflection().retrieve(__get_serialization_task_name(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._typename)); // Load method pointer.
				if (l_foreach_task != nullptr) // is serializable with foreach?
				{
					FE::arguments<const void*> l_pointer_to_container; // Any containers with begin() and end() can be serialized and deserialized.
					l_pointer_to_container._first = reinterpret_cast<FE::byte*>(&object_p) + (l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator()));
					(*l_foreach_task)(this, nullptr, &l_pointer_to_container); // The pointed task object_base knows what to do with the arguments type casting.

					// Move on to the next registered property of the class layer.
					++(__get_the_top_class_property_list_iterator());
				}
				else if (l_search_result != this->m_property_map.end()) // push the meta data onto the stack if found.
				{
					l_offset_from_the_upmost_base_class_instance = __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator());

					// Look for the next registered property of the class layer.
					++(__get_the_top_class_property_list_iterator());

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
		while (this->m_class_layer.is_empty() == false)
		{
			switch (__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._is_trivial)
			{
			case TypeTriviality::_Trivial:
			{
				// Check if the meta data is valid.
				FE_NEGATIVE_ASSERT(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._size_in_byte == 0, "Assertion failed: unable to serialize a zero-byte property.");

				// Write the bits to the object_base, from a buffer.	
				FE::memcpy(reinterpret_cast<var::byte*>(&out_object_p) + (l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator())), FE::iterator_cast<FE::ASCII*>(m_position), __get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._size_in_byte);
				this->m_position += __get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._size_in_byte; // Iterate to the next bits.

				// Look for the next registered property of the class layer.
				++(__get_the_top_class_property_list_iterator());

				// Pop the class layer if the iterator reached the end of the property list.
				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
				{
					this->m_class_layer.pop();
				}
				break;
			}

			case TypeTriviality::_NotTrivial:

				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end()) // Pop the class layer if the iterator reached the end of the property list.
				{
					this->m_class_layer.pop();
					break;
				}

				// Find the class/struct meta data that contains its memory layer.
				auto l_search_result = this->m_property_map.find(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._typename);

				// This is to serialize and deserialize containers and class instances that can be iterated through foreach. 
				FE::task_base* const l_foreach_task = framework_base::get_engine().access_method_reflection().retrieve(__get_deserialization_task_name(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._typename)); // Load method pointer.
				if (l_foreach_task != nullptr) // is deserializable with foreach?
				{
					FE::arguments<void*> l_pointer_to_container; // Any containers with begin() and end() can be serialized and deserialized.
					l_pointer_to_container._first = reinterpret_cast<var::byte*>(&out_object_p) + (l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator()));
					(*l_foreach_task)(this, nullptr, &l_pointer_to_container); // The pointed task object_base knows what to do with the arguments type casting.

					// Look for the next registered property of the class layer.
					++(__get_the_top_class_property_list_iterator());
				}
				else if (l_search_result != this->m_property_map.end()) // push the meta data onto the stack if found.
				{
					l_offset_from_the_upmost_base_class_instance = __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator());

					// Look for the next registered property of the class layer.
					++(__get_the_top_class_property_list_iterator());

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
		l_s_serialization_task_name.replace(l_s_serialization_task_name.cbegin() + (l_s_serialization_task_name.find('<') + 1),
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
		l_s_deserialization_task_name.replace(l_s_deserialization_task_name.cbegin() + (l_s_deserialization_task_name.find('<') + 1),
			l_s_deserialization_task_name.cbegin() + (l_s_deserialization_task_name.length() - 1),
			property_typename_p
		);
		return l_s_deserialization_task_name.c_str();
	}
};

END_NAMESPACE




#ifdef FE_CLASS_HAS_A_BASE
#error FE_CLASS_HAS_A_BASE is a reserved Frogman Engine macro keyword.
#else
#define FE_CLASS_HAS_A_BASE(base_class) \
using base_type = base_class; 
#endif


#ifdef FE_CLASS
#error REGISTER_FE_CLASS is a reserved Frogman Engine macro keyword.
#else
#define FE_CLASS(class_name) \
using is_reflective = decltype(true); \
class class_meta_data \
{ \
public: \
	using type = class_name; \
	class_meta_data() noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, std::string l_class_name = #class_name; l_class_name += "()"; ::FE::framework::framework_base::get_engine().access_method_reflection().register_task< ::FE::c_style_task<void(void*), typename ::FE::function<void(void*)>::arguments_type> >(l_class_name.c_str(), &::FE::framework::reflection::construct_object<class_name>) ); \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, std::string l_class_name = "~"; l_class_name += #class_name; l_class_name += "()"; ::FE::framework::framework_base::get_engine().access_method_reflection().register_task< ::FE::c_style_task<void(void*), typename ::FE::function<void(void*)>::arguments_type> >(l_class_name.c_str(), &::FE::framework::reflection::destruct_object<class_name>) ); \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ class_meta_data class_reflection_instance_##class_name;
#endif


#ifdef FE_METHOD
	#error FE_METHOD is a reserved Frogman Engine macro keyword.
#else
#define FE_METHOD(namespace, method_name, ...) \
class method_reflection_##method_name \
{ \
public: \
	_FE_FORCE_INLINE_ method_reflection_##method_name() noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, std::string l_full_signature = get_signature(); ::FE::framework::framework_base::get_engine().access_method_reflection().register_task<::FE::cpp_style_task<class_meta_data::type, __VA_ARGS__, typename FE::method<class_meta_data::type, __VA_ARGS__>::arguments_type>>(l_full_signature.c_str(), &class_meta_data::type::method_name)); \
	} \
	static ::std::string get_signature() noexcept \
	{ \
		::std::string l_full_signature = #__VA_ARGS__; \
		auto l_result = ::FE::algorithm::string::find_the_last(l_full_signature.c_str(), '('); \
		if (l_full_signature[l_result->_begin - 1] != ' ') \
		{ \
			l_full_signature.insert(l_result->_begin, " "); \
		} \
		::std::string l_signature = #namespace; \
		l_signature += "::"; l_signature += #method_name; \
		l_result = ::FE::algorithm::string::find_the_last(l_full_signature.c_str(), '('); \
		l_full_signature.insert(l_result->_begin, l_signature.c_str()); \
		return l_full_signature; \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ method_reflection_##method_name method_reflection_instance_##method_name;
#endif


#ifdef FE_PROPERTY
	#error REGISTER_FE_PROPERTY is a reserved Frogman Engine macro keyword.
#else
#define FE_PROPERTY(property_name)  \
class property_reflection_##property_name \
{ \
public: \
	_FE_FORCE_INLINE_ property_reflection_##property_name(typename class_meta_data::type* const this_p) noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, ::FE::framework::framework_base::get_engine().access_property_reflection().register_property<typename class_meta_data::type, decltype(property_name)>(*this_p, this_p->property_name, #property_name)); \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ property_reflection_##property_name property_reflection_instance_##property_name = this;
#endif


#endif