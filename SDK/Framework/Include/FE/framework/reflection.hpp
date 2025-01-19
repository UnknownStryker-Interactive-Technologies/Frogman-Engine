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
#include <FE/function.hxx>
#include <FE/fstream_guard.hxx>
#include <FE/hash.hpp>
#include <FE/type_traits.hxx>
#include <FE/pair.hxx>

#include <FE/framework/reflection/type_info.hpp>

// std
#include <deque>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory_resource>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <vector>

// boost::shared_lock_guard
#include <boost/thread/shared_lock_guard.hpp>

// ronbin hood hash map
#include <robin_hood.h>

// array hash map
#include <tsl/array-hash/array_map.h>




CLASS_FORWARD_DECLARATION(FE::framework, framework_base);

namespace FE::framework::serialization
{
	enum struct FileExtension : FE::int8
	{
		_Froggy,
		_Toc,
		_Aar,
		_Ao,
		_Fassetpack,
		_Fasset,
		Fdata
	};

	constexpr FE::pair<FileExtension, FE::ASCII*> froggy_file_extension{ FileExtension::_Froggy, ".froggy" };
	constexpr FE::pair<FileExtension, FE::ASCII*> toc_file_extension{ FileExtension::_Toc, ".toc" };
	constexpr FE::pair<FileExtension, FE::ASCII*> aar_file_extension{ FileExtension::_Aar, ".aar" };
	constexpr FE::pair<FileExtension, FE::ASCII*> ao_file_extension{ FileExtension::_Ao, ".ao" };
	constexpr FE::pair<FileExtension, FE::ASCII*> fassetpack_file_extension{ FileExtension::_Fassetpack, ".fassetpack" };
	constexpr FE::pair<FileExtension, FE::ASCII*> fasset_file_extension{ FileExtension::_Fasset, ".fasset" };
	constexpr FE::pair<FileExtension, FE::ASCII*> fdata_file_extension{ FileExtension::Fdata, ".fdata" };
}




BEGIN_NAMESPACE(FE::framework::reflection)


template<typename T>
_FE_FORCE_INLINE_ void construct_object(void* address_p)
{
	new( static_cast<T*>(address_p) ) T();
}

template<typename T>
_FE_FORCE_INLINE_ void destruct_object(void* address_p)
{
	static_cast<T*>(address_p)->~T();
}


class method_map
{
	friend class ::FE::framework::framework_base;
public:
	using alignment_type = FE::align_8bytes;

private:
	using lock_type = std::shared_mutex;
	using internal_map_type = robin_hood::unordered_map<std::pmr::string, FE::task_base*>;

	lock_type m_lock;
	internal_map_type m_task_map;
	std::pmr::synchronized_pool_resource m_pool;

private:
	method_map(FE::size map_capacity_p) noexcept
		: m_lock(), m_task_map(map_capacity_p), m_pool()
	{}
	~method_map() noexcept = default;

public:
	method_map(const method_map&) = delete;
	method_map(method_map&&) = delete;

	method_map& operator=(const method_map&) = delete;
	method_map& operator=(method_map&&) = delete;

	_FE_FORCE_INLINE_ void reserve(FE::size size_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(m_lock);
		this->m_task_map.reserve(size_p);
	}

	template<class TaskType, typename FunctionPtr>
	_FE_FORCE_INLINE_ void register_task(const std::string_view& task_name_p, FunctionPtr function_p) noexcept
	{
		FE_NEGATIVE_STATIC_ASSERT((std::is_base_of<FE::task_base, TaskType>::value == false), "An invalid method type detected.");

		TaskType* const l_task = std::pmr::polymorphic_allocator<TaskType>{ &m_pool }.allocate(1);
		new(l_task) TaskType(function_p);
		typename internal_map_type::key_type l_key(task_name_p, &m_pool);
		std::lock_guard<lock_type> l_lock(m_lock);
		this->m_task_map.emplace(std::move(l_key), l_task);
	}

	_FE_FORCE_INLINE_ FE::boolean check_presence(const std::string_view& key_p) noexcept
	{
		typename internal_map_type::key_type l_key(key_p, &m_pool);
		std::lock_guard<lock_type> l_lock(m_lock);
		for (auto it = this->m_task_map.find(l_key); it != this->m_task_map.end(); ++it)
		{
			if (it->first == key_p)
			{
				return true;
			}
		}
		return false;
	}

	/*
		This method may return nullptr.
	*/
	_FE_NODISCARD_ _FE_FORCE_INLINE_ FE::task_base* retrieve(const std::string_view& key_p) noexcept
	{
		typename internal_map_type::key_type l_key(key_p, &m_pool);
		std::lock_guard<lock_type> l_lock(m_lock);
		for (auto it = this->m_task_map.find(l_key); it != this->m_task_map.end(); ++it)
		{
			if (it->first == key_p)
			{
				return it->second;
			}
		}
		return nullptr;
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
struct property_metadata
{
	FE::TypeTriviality _is_trivial;
	var::boolean _is_serializable;
	var::uint32 _size_in_byte;
	var::ptrdiff _offset_from_this;

	FE::ASCII* _name;
	FE::ASCII* _typename;
};


class instance_layout
{
	tsl::array_map<var::ASCII, property_metadata> m_property_map;

public:
	instance_layout(const std::pmr::map<var::ptrdiff, property_metadata>& layout_p) noexcept
		: m_property_map()
	{
		for (auto [key, value] : layout_p)
		{
			this->m_property_map.emplace(value._name, value);
		}
	}
	~instance_layout() noexcept = default;


	template<typename T, class C>
	_FE_FORCE_INLINE_ T* get_property_of(C& instance_p, const std::string_view& property_name_p) noexcept
	{
		FE_STATIC_ASSERT(std::is_class<C>::value == true, "Static assertion failure: the typename 'class C' must be a class type.");
		for (auto it = this->m_property_map.find(property_name_p); it != this->m_property_map.end(); ++it)
		{
			if (property_name_p == it->_name)
			{
				FE_ASSERT(sizeof(T) == it->_size_in_byte, "Assertion Failure: interpreting bytes with the incorrect type 'typename T' is not allowed.");
				FE_ASSERT(algorithm::string::compare(it->_typename, reflection::type_id<T>().name()) == true, "Assertion Failure: interpreting bytes with the incorrect type 'typename T' is not allowed.");

				var::byte* l_address = reinterpret_cast<var::byte*>(&instance_p) + it->_offset_from_this;
				return reinterpret_cast<T*>(l_address);
			}
		}
		return nullptr;
	}
};
/*
The property_map class in the FE::framework::reflection namespace manages the registration and metadata of properties associated with host class instances
utilizing a custom memory pool and thread-safe operations for efficient reflection and serialization.
*/
class property_map
{
	friend class ::FE::framework::framework_base;
public:
	/*
	It is worth noting that, FE::string's contents can be allocated on a thread local memory pool
	by adding -DMEMORY_POOL_FE_STRINGS=1 option to cmake.
	*/
	using internal_map_type = robin_hood::unordered_map<std::pmr::string, std::pmr::map<var::ptrdiff, property_metadata>>;
	using class_name_type = internal_map_type::key_type;
	using class_property_list = internal_map_type::mapped_type;
	using class_property_offset_type = typename class_property_list::key_type;
	using class_property_metadata_type = typename class_property_list::mapped_type;

	using class_layer_stack = std::pmr::vector< FE::pair<class_property_list*, typename class_property_list::iterator> >;
	using data_on_heap_size_record = std::pmr::deque<var::size>; // Used std::pmr::deque beause std::queue does not support pmr.

	using lock_type = std::mutex;
	using file_handler = std::fstream;
	using input_buffer_type = std::pmr::string;
	using input_buffer_iterator_type = typename input_buffer_type::iterator;

private:
	internal_map_type m_property_map;

	std::pmr::synchronized_pool_resource m_pool;
	class_layer_stack m_class_layer;
	data_on_heap_size_record m_scalable_container_size_record;

	lock_type m_lock;
	file_handler m_fstream;
	input_buffer_type m_input_buffer;
	input_buffer_iterator_type m_position;

private:
	property_map(FE::size reflection_map_capacity_p) noexcept
		: m_property_map(reflection_map_capacity_p), 
		  m_pool(), m_class_layer(&m_pool), m_scalable_container_size_record(&m_pool),
		  m_lock(), m_fstream(), m_input_buffer(&m_pool), m_position() {
	}
	~property_map() noexcept = default;

public:
	property_map(const property_map&) = delete;
	property_map(property_map&&) = delete;

	property_map& operator=(const property_map&) = delete;
	property_map& operator=(property_map&&) = delete;

	_FE_FORCE_INLINE_ void reserve(size size_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(m_lock);
		this->m_property_map.reserve(size_p);
	}

	template<class C, typename T>
	void register_property(const C& host_class_instance_p, const T& property_p, const std::string_view& property_name_p) noexcept
	{
		FE_NEGATIVE_STATIC_ASSERT(std::is_class<C>::value == false, "Primitive data types cannot be registered as the host classes/structs.");
		FE_NEGATIVE_STATIC_ASSERT((std::is_reference<T>::value == true) || (std::is_pointer<T>::value == true), "Static assertion failure: raw pointers and references cannot be serialized nor deserialized.");
		FE_NEGATIVE_STATIC_ASSERT((FE::is_trivial<T>::value == false) && (std::is_array<T>::value == true), "Static assertion failure: fixed-sized non-trivial arrays are not serializable nor deserializable.");
		FE_ASSERT(property_name_p.empty() != true, "Assertion failure: property name cannot be null.");
		
		property_metadata l_property_meta_data;
		l_property_meta_data._is_trivial = static_cast<TypeTriviality>(FE::is_trivial<T>::value);
		l_property_meta_data._is_serializable = FE::is_serializable<T>::value;
		FE_STATIC_ASSERT(sizeof(T) <= FE::uint32_max, "Static assertion failure: the property instance size is too enormous.");
		l_property_meta_data._size_in_byte = sizeof(T);
		l_property_meta_data._offset_from_this = (reinterpret_cast<FE::byte* const>(&property_p) - reinterpret_cast<FE::byte*>(&host_class_instance_p));
		l_property_meta_data._name = property_name_p.data();
		l_property_meta_data._typename = reflection::type_id<T>().name();

		// Find or register its host class.
		std::pmr::string l_host_class_instance_typename(reflection::type_id<C>().name(), &m_pool);
		std::lock_guard<lock_type> l_lock(m_lock);
		auto l_iterator = m_property_map.find(l_host_class_instance_typename);
		if (FE_UNLIKELY(l_iterator == m_property_map.end())) _FE_UNLIKELY_
		{
			auto l_result = m_property_map.emplace(l_host_class_instance_typename, class_property_list(&m_pool));
			FE_NEGATIVE_ASSERT(l_result.second == false, "Failed to robin_hood::unordered_map::emplace() while executing property_map::register_property().");
			l_iterator = l_result.first;
		}


		if constexpr ((FE::is_serializable<T>::value == true) && (FE::is_trivial<T>::value == false))
		{
			// This code section for serializing and deserializing a complicated multidimensional container and the third-party containers.
			// It enables the system to serialize and deserialize a class instance without Frogman Engine reflection macro boilerplates.
			framework_base::get_engine().get_method_reflection().register_task< FE::cpp_style_task<property_map, void(const void*)>>(__get_serialization_task_name(l_property_meta_data._typename), &property_map::__serialize_by_foreach_mutually_recursive<T>);
			framework_base::get_engine().get_method_reflection().register_task< FE::cpp_style_task<property_map, void(void*)>>(__get_deserialization_task_name(l_property_meta_data._typename), &property_map::__deserialize_by_foreach_mutually_recursive<T>);

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

		l_iterator->second.emplace(l_property_meta_data._offset_from_this, l_property_meta_data);
	}

	/*
	The serialize function template in the FE::framework::reflection::property_map class is designed to serialize an object of type T into a specified file within a given directory
	enforcing constraints on the type to ensure it is neither a class nor a struct, and disallowing raw pointers and references.
	*/
	template<typename T>
	void serialize(const std::filesystem::path& path_p, directory_char_t* filename_p, const T& object_p) noexcept
	{
		FE_NEGATIVE_STATIC_ASSERT(std::is_class<T>::value == false, "Non-class/struct field variables cannot be serialized.");
		FE_NEGATIVE_STATIC_ASSERT((std::is_reference<T>::value == true) || (std::is_pointer<T>::value == true), "static assertion failure: raw pointers and references cannot be serialized nor deserialized.");

		FE_NEGATIVE_ASSERT(path_p.empty() == true, "Assertion failure: the target directory path to the file is nullptr.");
		FE_NEGATIVE_ASSERT((filename_p == nullptr) || (*filename_p == null), "The file name is null.");

		std::pmr::string l_typename(reflection::type_id<T>().name(), &m_pool);
		std::lock_guard<lock_type> l_lock(m_lock);
		auto l_search_result = m_property_map.find(l_typename);
		FE_EXIT((l_search_result == m_property_map.end()) || (l_search_result->second.size() == 0), ErrorCode::_FatalSerializationError_3XX_TypeNotFound, "Serialization failed: could not find the requested type information or the class/struct is empty");
		this->m_class_layer.emplace_back(&(l_search_result->second), l_search_result->second.begin());

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

		while (this->m_scalable_container_size_record.empty() == false)
		{
			l_serialization_handler.get_stream() << this->m_scalable_container_size_record.front() << "-";
			this->m_scalable_container_size_record.pop_front();
		}
		l_serialization_handler.get_stream() << "\0";
	}

	// TODO: add file versioning.
	template<typename T>
	void deserialize(const std::filesystem::path& path_p, directory_char_t* filename_p, T& out_object_p) noexcept
	{
		FE_NEGATIVE_STATIC_ASSERT(std::is_class<T>::value == false, "Non-class/struct field variables cannot be deserialized.");
		FE_NEGATIVE_STATIC_ASSERT((std::is_reference<T>::value == true) || (std::is_pointer<T>::value == true), "static assertion failure: raw pointers and references cannot be serialized nor deserialized.");

		FE_NEGATIVE_ASSERT(std::filesystem::exists(path_p) == false, "Assertion failure: the target directory path to the file is nullptr.");
		FE_NEGATIVE_ASSERT((filename_p == nullptr) || (*filename_p == null), "The file name is null.");

		std::pmr::string l_typename(reflection::type_id<T>().name(), &m_pool);
		std::lock_guard<lock_type> l_lock(m_lock);
		auto l_search_result = this->m_property_map.find(l_typename);
		FE_EXIT((l_search_result == m_property_map.end()) || (l_search_result->second.size() == 0), ErrorCode::_FatalSerializationError_3XX_TypeNotFound, "serialization failed: could not find the requested type information or the class/struct is empty");
		this->m_class_layer.emplace_back(&(l_search_result->second), l_search_result->second.begin());

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
		FE_EXIT(!algorithm::string::compare_ranged<char>(l_typename.data(), algorithm::string::range{0, std::strlen(l_typename.data())},
			m_input_buffer.c_str(), algorithm::string::range{ 0, static_cast<uint64>(m_position - m_input_buffer.begin()) }),
			FE::ErrorCode::_FatalSerializationError_3XX_TypeMismatch, "Unable to deserialize an instance with a different class name.");
		++m_position; // Point the first byte.

		auto l_size_indicator = this->m_input_buffer.begin();
		l_size_indicator += this->m_input_buffer.rfind("$-");
		l_size_indicator += 2; // to skip the "$-" and point to the first one.

		while (*l_size_indicator != '\0')
		{
			algorithm::utility::uint_info l_info = algorithm::utility::string_to_uint(FE::iterator_cast<FE::ASCII*>(l_size_indicator));
			FE_LOG_IF(l_info._value == 0, "Warning: the size of the container is zero. Please debug if the file is corrupted.");
			this->m_scalable_container_size_record.push_back(l_info._value);
			l_size_indicator += l_info._digit_length; // move to the next.
			++l_size_indicator; // to skip the '-'.
		}

		__deserialize_mutually_recursive<T>(out_object_p);
	}

	template<typename T>
	_FE_FORCE_INLINE_ std::optional<instance_layout> get_instance_layout() noexcept
	{
		std::pmr::string l_class_name(reflection::type_id<T>().name(), &m_pool);
		std::lock_guard<lock_type> l_lock(m_lock);
		for (auto it = m_property_map.find(l_class_name); it != m_property_map.end(); ++it)
		{
			if (l_class_name == it->first)
			{
				return instance_layout(it->second);
			}
		}
		return std::nullopt;
	}

private:
	template <class InnerContainer>
	_FE_FORCE_INLINE_ void __push_multidimensional_container_serialization_task_recursive() noexcept
	{
		framework_base::get_engine().get_method_reflection().register_task< FE::cpp_style_task<property_map, void(const void*)> >(__get_serialization_task_name(reflection::type_id<InnerContainer>().name()), &property_map::__serialize_by_foreach_mutually_recursive<InnerContainer>);

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
		framework_base::get_engine().get_method_reflection().register_task< FE::cpp_style_task<property_map, void(void*)> >(__get_deserialization_task_name(reflection::type_id<InnerContainer>().name()), &property_map::__deserialize_by_foreach_mutually_recursive<InnerContainer>);

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
		return this->m_class_layer.back()._second;
	}

	_FE_FORCE_INLINE_ FE::ptrdiff __get_memory_offset_of_the_property(typename class_property_list::iterator& to_the_property_p) noexcept
	{
		return to_the_property_p->first;
	}

	_FE_FORCE_INLINE_ property_metadata& __get_metadata_of_the_property(typename class_property_list::iterator& to_the_property_p) noexcept
	{
		return to_the_property_p->second;
	}

	_FE_FORCE_INLINE_ class_property_list& __get_top_class_property_list() noexcept
	{
		return *(this->m_class_layer.back()._first);
	}

	template<class U>
	_FE_FORCE_INLINE_ void __push_parent_class_layers_recursive() noexcept
	{
		if constexpr (FE::has_base_type<U>::value == true)
		{
			static typename  internal_map_type::iterator l_s_search_result;
			static typename  internal_map_type::key_type l_s_typename;
			l_s_typename = reflection::type_id<typename U::base_type>().name();
			l_s_search_result = this->m_property_map.find(l_s_typename);
			if (l_s_search_result == this->m_property_map.end())
			{
				return;
			}
			this->m_class_layer.emplace_back(&(l_s_search_result->second), l_s_search_result->second.begin());
			__push_parent_class_layers_recursive<typename U::base_type>();
		}
	}

	void __push_parent_class_layers_by_typename_string_recursive(const std::string_view& typename_p) noexcept
	{
		static typename  internal_map_type::iterator l_s_search_result;
		static typename  internal_map_type::key_type l_s_typename;
		l_s_typename = reflection::type_info::get_base_name_of(typename_p);
		l_s_search_result = this->m_property_map.find(l_s_typename);
		if (l_s_search_result == this->m_property_map.end())
		{
			return;
		}
		this->m_class_layer.emplace_back(&(l_s_search_result->second), l_s_search_result->second.begin());
		__push_parent_class_layers_by_typename_string_recursive(l_s_search_result->first);
	}

	template<typename T>
	void __serialize_mutually_recursive(const T& object_p) noexcept
	{
		var::ptrdiff l_offset_from_the_upmost_base_class_instance = 0;
		while (this->m_class_layer.empty() == false)
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
					this->m_class_layer.pop_back();
				}
				break;
			}

			case TypeTriviality::_NotTrivial:

				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end()) // Pop the class layer if the iterator reached the end of the property list.
				{
					this->m_class_layer.pop_back();
					break;
				}

				// Find the class/struct meta data that contains its memory layer.
				auto l_search_result = this->m_property_map.find(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._typename);

				// This is to serialize and deserialize containers and class instances that can be iterated through foreach. 
				FE::task_base* const l_foreach_task = framework_base::get_engine().get_method_reflection().retrieve(__get_serialization_task_name(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._typename)); // Load method pointer.
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
					this->m_class_layer.emplace_back(&(l_search_result->second), l_search_result->second.begin());
					__push_parent_class_layers_by_typename_string_recursive(l_search_result->first);
				}

				// if it reached the end.
				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
				{
					this->m_class_layer.pop_back();
				}
				break;
			}
		}
	}

	template<typename T>
	void __deserialize_mutually_recursive(T& out_object_p) noexcept
	{
		var::ptrdiff l_offset_from_the_upmost_base_class_instance = 0;
		while (this->m_class_layer.empty() == false)
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
					this->m_class_layer.pop_back();
				}
				break;
			}

			case TypeTriviality::_NotTrivial:

				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end()) // Pop the class layer if the iterator reached the end of the property list.
				{
					this->m_class_layer.pop_back();
					break;
				}

				// Find the class/struct meta data that contains its memory layer.
				auto l_search_result = this->m_property_map.find(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._typename);

				// This is to serialize and deserialize containers and class instances that can be iterated through foreach. 
				FE::task_base* const l_foreach_task = framework_base::get_engine().get_method_reflection().retrieve(__get_deserialization_task_name(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._typename)); // Load method pointer.
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
					this->m_class_layer.emplace_back(&(l_search_result->second), l_search_result->second.begin());
					__push_parent_class_layers_by_typename_string_recursive(l_search_result->first);
				}

				// if it reached the end.
				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
				{
					this->m_class_layer.pop_back();
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
					this->m_scalable_container_size_record.push_back(l_container->size());
					this->m_fstream.write(reinterpret_cast<const char*>(l_container->data()), sizeof(typename Container::value_type) * l_container->size());
				}
			}
			else
			{
				this->m_scalable_container_size_record.push_back(l_container->size());
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

	std::string_view __get_serialization_task_name(const std::string_view& property_typename_p) noexcept
	{
		static std::string l_s_serialization_task_name("FE::framework::reflection::property_map::__serialize_by_foreach_mutually_recursive< >");
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
			l_container->resize(this->m_scalable_container_size_record.front());
			this->m_scalable_container_size_record.pop_front();

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

	std::string_view __get_deserialization_task_name(const std::string_view& property_typename_p) noexcept
	{
		static std::string l_s_deserialization_task_name("FE::framework::reflection::property_map::__deserialize_by_foreach_mutually_recursive< >");
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
#error FE_CLASS is a reserved Frogman Engine macro keyword.
#else
/*
The FE_CLASS macro defines a class metadata structure for a given class name
enabling reflection capabilities by registering construction and destruction tasks with a framework's reflection system.
*/
#define FE_CLASS(class_name) \
using is_reflective = decltype(true); \
class class_metadata \
{ \
public: \
	using type = class_name; \
	class_metadata() noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, std::string l_class_name = #class_name; l_class_name += "()"; ::FE::framework::framework_base::get_engine().get_method_reflection().register_task< ::FE::c_style_task<void(void*), typename ::FE::function<void(void*)>::arguments_type> >(l_class_name.c_str(), &::FE::framework::reflection::construct_object<class_name>) ); \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, std::string l_class_name = "~"; l_class_name += #class_name; l_class_name += "()"; ::FE::framework::framework_base::get_engine().get_method_reflection().register_task< ::FE::c_style_task<void(void*), typename ::FE::function<void(void*)>::arguments_type> >(l_class_name.c_str(), &::FE::framework::reflection::destruct_object<class_name>) ); \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ class_metadata class_name##_class_meta;
#endif

#ifdef FE_STRUCT
#error FE_STRUCT is a reserved Frogman Engine macro keyword.
#else
	#define FE_STRUCT(struct_name) FE_CLASS(struct_name)
#endif


#ifdef FE_METHOD
	#error FE_METHOD is a reserved Frogman Engine macro keyword.
#else
/*
The FE_METHOD macro defines a class for method reflection in a specified namespace
automatically registering the method's signature and its associated metadata upon instantiation.
*/
#define FE_METHOD(method_name, ...) \
class method_metadata_##method_name \
{ \
public: \
	_FE_FORCE_INLINE_ method_metadata_##method_name() noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, ::FE::framework::framework_base::get_engine().get_method_reflection().register_task<::FE::cpp_style_task<class_metadata::type, __VA_ARGS__, typename FE::method<class_metadata::type, __VA_ARGS__>::arguments_type>>(get_signature(), &class_metadata::type::method_name)); \
	} \
	static ::std::string& get_signature() noexcept \
	{ \
		static ::std::string l_s_full_signature; \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, \
			::std::string l_full_signature = #__VA_ARGS__; \
			auto l_result = ::FE::algorithm::string::find_the_last(l_full_signature.c_str(), '('); \
			if (l_full_signature[l_result->_begin - 1] != ' ') \
			{ \
				l_full_signature.insert(l_result->_begin, " "); \
			} \
			::std::string l_signature = ::FE::framework::reflection::type_id<typename class_metadata::type>().name(); \
			l_signature += "::"; l_signature += #method_name; \
			l_result = ::FE::algorithm::string::find_the_last(l_full_signature.c_str(), '('); \
			l_full_signature.insert(l_result->_begin, l_signature.c_str()); \
			l_s_full_signature = std::move(l_full_signature); \
		); \
		return l_s_full_signature; \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ method_metadata_##method_name method_name##_method_meta;
#endif

#ifdef FE_STATIC_METHOD
	#error FE_STATIC_METHOD is a reserved Frogman Engine macro keyword.
#else
/*
The FE_STATIC_METHOD macro defines a class that registers a static method for reflection in a specified namespace
capturing its signature and associating it with a task for runtime method invocation.
*/
#define FE_STATIC_METHOD(method_name, ...) \
class static_method_metadata_##method_name \
{ \
public: \
	_FE_FORCE_INLINE_ static_method_metadata_##method_name() noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, ::FE::framework::framework_base::get_engine().get_method_reflection().register_task<::FE::c_style_task<__VA_ARGS__, typename FE::function<__VA_ARGS__>::arguments_type>>(get_signature(), &class_metadata::type::method_name)); \
	} \
	static ::std::string& get_signature() noexcept \
	{ \
		static ::std::string l_s_full_signature; \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, \
			::std::string l_full_signature = #__VA_ARGS__; \
			auto l_result = ::FE::algorithm::string::find_the_last(l_full_signature.c_str(), '('); \
			if (l_full_signature[l_result->_begin - 1] != ' ') \
			{ \
				l_full_signature.insert(l_result->_begin, " "); \
			} \
			::std::string l_signature = ::FE::framework::reflection::type_id<typename class_metadata::type>().name(); \
			l_signature += "::"; l_signature += #method_name; \
			l_result = ::FE::algorithm::string::find_the_last(l_full_signature.c_str(), '('); \
			l_full_signature.insert(l_result->_begin, l_signature.c_str()); \
			l_s_full_signature = std::move(l_full_signature); \
		); \
		return l_s_full_signature; \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ static_method_metadata_##method_name method_name##_static_method_meta;
#endif


#ifdef FE_PROPERTY
	#error FE_PROPERTY is a reserved Frogman Engine macro keyword.
#else
/*
The FE_PROPERTY macro defines a class for property reflection that registers a specified property of a given type with the framework's property reflection system
ensuring it is only registered once during the application's execution.
*/
#define FE_PROPERTY(property_name)  \
class property_metadata_##property_name \
{ \
public: \
	_FE_FORCE_INLINE_ property_metadata_##property_name(typename class_metadata::type* const this_p) noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, ::FE::framework::framework_base::get_engine().get_property_reflection().register_property<typename class_metadata::type, decltype(property_name)>(*this_p, this_p->property_name, #property_name)); \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ property_metadata_##property_name property_name##_property_meta = this;
#endif


#endif