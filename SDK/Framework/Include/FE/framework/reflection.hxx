#ifndef _FE_FRAMEWORK_REFLECTION_HXX_
#define _FE_FRAMEWORK_REFLECTION_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
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

#include <FE/algorithm/string.hxx>
#include <FE/algorithm/utility.hxx>

// FE.Core
#include <FE/do_once.hxx>
#include <FE/function.hxx>
#include <FE/fstream_guard.hxx>
#include <FE/hash.hxx>
#include <FE/type_traits.hxx>
#include <FE/pair.hxx>

#include <FE/framework/type_info.hxx>

// std
#include <filesystem>
#include <fstream>
#include <map>
#include <memory_resource>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

// boost::shared_lock_guard
#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/shared_lock_guard.hpp>

#include <absl/container/flat_hash_map.h>
#include <absl/container/node_hash_map.h>
#include <tsl/array-hash/array_map.h>




#ifdef FE_SYSTEM
#error FE_SYSTEM is a reserved Frogman Engine macro keyword.
#else
#define FE_SYSTEM(SysCallPhase, TargetComponentType, WorldTagEnumType) /* This macro is an indicator for the Frogman Engine Header Tool. Incorrectly placing the arguments will fail the generated code compilation. */ \
static_assert(::std::is_same_v<decltype(SysCallPhase), enum struct ::FE::SystemCallPhase>, "Frogman Engine C++: an incorrectly typed value passed to the SysCallPhase parameter."); \
static_assert(::std::is_base_of_v<class ::FE::component_base, TargetComponentType>, "Frogman Engine C++: the TargetComponentType must be derived from FE::component_base."); \
static_assert(::std::is_enum_v< decltype(WorldTagEnumType) >, "Frogman Engine C++: the WorldTagEnumType is not a valid enum value");
#endif

CLASS_FORWARD_DECLARATION(FE::framework, framework_base);


namespace FE
{
	// The numerical values of this enum must not be changed, as they are used for indexing arrays; the engine make system calls in the order of the values! Do not change if unnecessary.
	enum SystemCallPhase : var::uint32
	{
		_EngineInitialization = 0,
		_GameInitialization = 1,
		_WorldInitialization = 2,
		_WorldDefaultEntityInitialization = 3,

		_GameBegin = 4,
		_WorldBegin = 5,
		_EntitySetUp = 6,
		_EntityControllerSetUp = 7,

		_GameTick = 8,
		_WorldTick = 9,
		_EntityTick = 10,
		_EntityControllerTick = 11,
		_PrePhysics = 12,
		_StartPhysics = 13,
		_Physics = 14,
		_EndPhysics = 15,
		_PostPhysics = 16,
		_PostEntityTick = 17,

		//_NewlySpawnedEntities = 18,
		_PreRender = 18,
		_PostRender = 19,

		_EntityCleanUp = 20,
		_WorldCleanUp = 21,
		_GameCleanUp = 22,
		_EngineCleanUp = 23
	};

	constexpr static var::uint64 syscall_phase_count = 24;
}


BEGIN_NAMESPACE(FE::framework::reflection)


class method_registry
{
	friend class ::FE::framework::framework_base;
public:
	using alignment_type = FE::align_8bytes;

private:
	using lock_type = boost::shared_mutex;
	using internal_map_type = absl::flat_hash_map<std::pmr::string, FE::task_base*,
		FE::hash<std::pmr::string>,
		std::equal_to<std::pmr::string>,
		FE::cache_aligned_allocator<std::pmr::string>>;

	using world_tag_t = var::uint64;
	using component_typeid_t = var::size;
	using system_t = void(*)(class ::FE::component_base* const);

	using system_table = absl::node_hash_map<	world_tag_t,
												std::array<absl::node_hash_map<component_typeid_t, std::pmr::vector<system_t>,
																				FE::hash<component_typeid_t>,
																				std::equal_to<component_typeid_t>,
																				FE::polymorphic_allocator< std::pair<const component_typeid_t, std::pmr::vector<system_t>> >
																				>,
															syscall_phase_count
															>,
		FE::hash<world_tag_t>,
		std::equal_to<world_tag_t>,
		FE::polymorphic_allocator<std::pair<const world_tag_t,
											std::array<absl::node_hash_map<component_typeid_t, std::pmr::vector<system_t>,
																			FE::hash<component_typeid_t>,
																			std::equal_to<component_typeid_t>,
																			FE::polymorphic_allocator< std::pair<const component_typeid_t, std::pmr::vector<system_t>> >
																			>,
														syscall_phase_count
														>
											>
									>
	>;

	lock_type m_lock;
	std::pmr::memory_resource* m_pool;
	internal_map_type m_method_registry;
	system_table m_system_table;

public:
	method_registry(FE::size map_capacity_p, std::pmr::memory_resource* pool_p = std::pmr::get_default_resource()) noexcept;

	~method_registry() noexcept = default;

	method_registry(const method_registry&) = delete;
	method_registry(method_registry&&) = delete;

	method_registry& operator=(const method_registry&) = delete;
	method_registry& operator=(method_registry&&) = delete;

	_FE_FORCE_INLINE_ void reserve(FE::size size_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(m_lock);
		m_method_registry.reserve(size_p);
	}

	template<class TaskType>
	void register_task(const std::string_view& task_name_p, typename TaskType::task_type function_p) noexcept
	{
		static_assert(std::is_base_of_v<FE::task_base, TaskType>, "An invalid method type detected.");
		std::lock_guard<lock_type> l_lock(m_lock);
		TaskType* const l_task = std::pmr::polymorphic_allocator<TaskType>{ m_pool }.allocate(1);
		new(l_task) TaskType(function_p);
		typename internal_map_type::key_type l_key(task_name_p, m_pool);

		std::pair<typename internal_map_type::iterator, bool> l_result = m_method_registry.emplace(std::move(l_key), l_task);
		if (l_result.second == false) // If true, then the key already exists
		{
			// Are the keys identical? If not, then we have a collision.
			FE_EXIT_IF(task_name_p != l_result.first->first, FE::ErrorCode::_FatalError_TableInsertionFailure,
				"FE Runtime Reflection Metadata Registration Failure: the function name keys have collided.");
			std::pmr::polymorphic_allocator<TaskType>{ m_pool }.deallocate(l_task, 1);
		}
	}

	FE::boolean check_presence(const std::string_view& key_p) noexcept;

	// This method may return a nullptr.
	FE::task_base* retrieve(const std::string_view& key_p) noexcept;

	template<class TargetComponent>
	void associate_system(world_tag_t world_tag_p, SystemCallPhase syscall_phase_p, system_t system_function_p) noexcept
	{
		static_assert(std::is_base_of_v<::FE::component_base, TargetComponent>, "Static assertion failure: 'TargetComponent' must be derived from 'FE::component_base'.");
		FE_ASSERT(static_cast<var::uint32>(syscall_phase_p) < FE::syscall_phase_count, "Static assertion failure: 'syscall_phase_p' is out of range.");
		
		std::lock_guard<lock_type> l_lock(m_lock);

		if (m_system_table.find(world_tag_p) == m_system_table.end())
		{
			static typename system_table::mapped_type l_world_system_table;
			for (typename system_table::mapped_type::value_type& vec : l_world_system_table)
			{
				vec = typename system_table::mapped_type::value_type(m_pool);
				vec.reserve(256);
			}
			m_system_table[world_tag_p] = std::move(l_world_system_table);
		}

		static_assert(sizeof(syscall_phase_p) == sizeof(FE::uint32));

		if ( m_system_table[world_tag_p][(FE::uint32)syscall_phase_p].find( FE::framework::reflection::type_id<TargetComponent>().hash_code() ) == m_system_table[world_tag_p][(FE::uint32)syscall_phase_p].end() )
		{
			m_system_table[world_tag_p][(FE::uint32)syscall_phase_p][ FE::framework::reflection::type_id<TargetComponent>().hash_code() ] = typename system_table::mapped_type::value_type::mapped_type(m_pool);
		}
		m_system_table[world_tag_p][(FE::uint32)syscall_phase_p][ FE::framework::reflection::type_id<TargetComponent>().hash_code() ].emplace_back(system_function_p);
	}

	_FE_FORCE_INLINE_ typename system_table::mapped_type::value_type& get_systems(world_tag_t world_tag_p, SystemCallPhase syscall_phase_p) noexcept
	{
		static_assert(sizeof(syscall_phase_p) == sizeof(FE::uint32));
		return m_system_table[world_tag_p][(FE::uint32)syscall_phase_p];
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
	FE::ASCII* m_typename;
};


class instance_metadata
{
	friend class property_registry;

	using internal_map_type = tsl::array_map<var::ASCII, property_metadata*>;

	internal_map_type m_property_lut;

public:
	instance_metadata() noexcept = default;
	~instance_metadata() noexcept = default;

	template<typename T, class C>
	T* get_property_of(C& instance_p, const std::string_view& property_name_p) noexcept
	{
		static_assert(std::is_class_v<C>, "Static assertion failure: the typename 'class C' must be a class type.");
		for (typename internal_map_type::iterator it = m_property_lut.find(property_name_p); it != m_property_lut.end(); ++it)
		{
			if (property_name_p == it.value()->_name)
			{
				FE_ASSERT(sizeof(T) == it.value()->_size_in_byte, "Assertion Failure: interpreting bytes with the incorrect type 'typename T' is not allowed.");
				FE_ASSERT(algorithm::string::compare(it.value()->m_typename, reflection::type_id<T>().name()) == true, "Assertion Failure: interpreting bytes with the incorrect type 'typename T' is not allowed.");

				var::byte* l_address = reinterpret_cast<var::byte*>(&instance_p) + it.value()->_offset_from_this;
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
class property_registry
{
	friend class ::FE::framework::framework_base;
public:
	/*
	It is worth noting that, FE::string's contents can be allocated on a thread local memory pool
	by adding -DMEMORY_POOL_FE_STRINGS=1 option to cmake.
	*/
	using internal_map_type = absl::node_hash_map<std::pmr::string, std::pmr::map<var::ptrdiff, property_metadata>,
		FE::hash<std::pmr::string>,
		std::equal_to<std::pmr::string>,
		FE::cache_aligned_allocator<std::pmr::string>>;

	using class_name_type = internal_map_type::key_type;
	using class_property_list = internal_map_type::mapped_type;
	using class_property_offset_type = typename class_property_list::key_type;
	using class_property_metadata_type = typename class_property_list::mapped_type;

	using class_layer_stack = std::pmr::vector< FE::pair<class_property_list*, typename class_property_list::iterator> >;
	using data_on_heap_size_record = std::pmr::deque<var::size>; // Used std::pmr::deque because std::queue does not support pmr.

	using lock_type = std::mutex;
	using file_handler = std::fstream;
	using input_buffer_type = std::pmr::string;
	using input_buffer_iterator_type = typename input_buffer_type::iterator;

private:
	std::pmr::memory_resource* m_pool;
	internal_map_type m_property_registry;
	class_layer_stack m_class_layer;
	data_on_heap_size_record m_scalable_container_size_record;

	lock_type m_lock;
	input_buffer_type m_input_buffer;
	input_buffer_iterator_type m_position;

	absl::node_hash_map<std::pmr::string, instance_metadata> m_instance_metadata_lut; // This is used to retrieve the instance metadata of a class instance.

public:
	property_registry(FE::size reflection_map_capacity_p, std::pmr::memory_resource* pool_p = std::pmr::get_default_resource()) noexcept;

	~property_registry() noexcept = default;

	property_registry(const property_registry&) = delete;
	property_registry(property_registry&&) = delete;

	property_registry& operator=(const property_registry&) = delete;
	property_registry& operator=(property_registry&&) = delete;

	_FE_FORCE_INLINE_ void reserve(size size_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(m_lock);
		m_property_registry.reserve(size_p);
	}

	template<class C, typename T>
	void register_property(const C& host_class_instance_p, const T& property_p, const std::string_view& property_name_p) noexcept
	{
		static_assert(std::is_class_v<C>, "Primitive data types cannot be registered as the host classes/structs.");
		static_assert((std::is_reference_v<T> == false) && (std::is_pointer_v<T> == false), "Static assertion failure: raw pointers and references cannot be serialized nor deserialized.");
		static_assert(! ((FE::is_trivial<T>::value == false) && (std::is_array_v<T>)), "Static assertion failure: fixed-sized non-trivial arrays are not serializable nor deserializable.");
		FE_ASSERT(property_name_p.empty() != true, "Assertion failure: property name cannot be null.");
		
		property_metadata l_property_meta_data;
		l_property_meta_data._is_trivial = static_cast<TypeTriviality>(FE::is_trivial<T>::value);
		l_property_meta_data._is_serializable = FE::is_serializable<T>::value;
		static_assert(sizeof(T) <= FE::uint32_max, "Static assertion failure: the property instance size is too enormous.");
		l_property_meta_data._size_in_byte = sizeof(T);
		l_property_meta_data._offset_from_this = (reinterpret_cast<FE::byte* const>(&property_p) - reinterpret_cast<FE::byte*>(&host_class_instance_p));
		l_property_meta_data._name = property_name_p.data();
		l_property_meta_data.m_typename = reflection::type_id<T>().name();

		std::lock_guard<lock_type> l_lock(m_lock);
		std::pmr::string l_host_class_instance_typename(reflection::type_id<C>().name(), m_pool);
		auto l_iterator = m_property_registry.find(l_host_class_instance_typename);
		if (FE_UNLIKELY(l_iterator == m_property_registry.end())) _FE_UNLIKELY_
		{
			auto l_result = m_property_registry.emplace(l_host_class_instance_typename, class_property_list(m_pool));
			FE_NEGATIVE_ASSERT(l_result.second == false, "Failed to emplace() while executing property_registry::register_property().");
			l_iterator = l_result.first;
		}


		if constexpr ((FE::is_serializable<T>::value == true) && (FE::is_trivial<T>::value == false))
		{
			// This code section for serializing and deserializing a complicated multidimensional container and the third-party containers.
			// It enables the system to serialize and deserialize a class instance without Frogman Engine reflection macro boilerplates.
			framework_base::get_framework().get_method_reflection().register_task< FE::cpp_style_task<property_registry, void(std::pmr::string&, const void*)>>(__get_serialization_task_name(l_property_meta_data.m_typename), &property_registry::__serialize_by_foreach_mutually_recursive<T>);
			framework_base::get_framework().get_method_reflection().register_task< FE::cpp_style_task<property_registry, void(void*)>>(__get_deserialization_task_name(l_property_meta_data.m_typename), &property_registry::__deserialize_by_foreach_mutually_recursive<T>);

			if constexpr (FE::has_value_type<T>::value == true)
			{
				if constexpr ((FE::is_serializable<typename T::value_type>::value == true) &&
					(FE::is_trivial<typename T::value_type>::value == false))
				{
					__push_multidimensional_container_serialization_task_recursive<typename T::value_type>();
					__push_multidimensional_container_deserialization_task_recursive<typename T::value_type>();
				}
			}
			else if constexpr (FE::has_element_type<T>::value == true)
			{
				if constexpr ((FE::is_serializable<typename T::element_type>::value == true) &&
					(FE::is_trivial<typename T::element_type>::value == false))
				{
					__push_multidimensional_container_serialization_task_recursive<typename T::element_type>();
					__push_multidimensional_container_deserialization_task_recursive<typename T::element_type>();
				}
			}
		}

		auto l_prop_registry_insertion_result = l_iterator->second.emplace(l_property_meta_data._offset_from_this, l_property_meta_data);
		FE_EXIT_IF(l_prop_registry_insertion_result.second == false, FE::ErrorCode::_FatalError_TableInsertionFailure,
			    "FE Runtime Reflection Registration Error: failed to register a property metadata to the FE runtime reflection system.\nThis error might have ocurred in the FE runtime reflection metadata loader function, during the initialization.");


		auto l_lut_it = m_instance_metadata_lut.find(l_host_class_instance_typename);
		if ( l_lut_it == m_instance_metadata_lut.end() )
		{
			auto l_lut_insertion_result = m_instance_metadata_lut.emplace(l_host_class_instance_typename, instance_metadata());
			FE_EXIT_IF(l_lut_insertion_result.second == false, FE::ErrorCode::_FatalError_TableInsertionFailure,
				"FE Runtime Reflection Registration Error: failed to register a class instance metadata to the FE runtime reflection system lookup table.\nThis error might have ocurred in the FE runtime reflection metadata loader function, during the initialization.");
			l_lut_it = l_lut_insertion_result.first;
		}

		FE_EXIT_IF(l_lut_it->second.m_property_lut.emplace(l_property_meta_data._name, &(l_prop_registry_insertion_result.first->second)).second == false, FE::ErrorCode::_FatalError_TableInsertionFailure,
			"FE Runtime Reflection Registration Error: failed to associate a property metadata in the FE runtime reflection system lookup table.\nThis error might have ocurred in the FE runtime reflection metadata loader function, during the initialization.");
	}


	template<typename T>
	void serialize(std::pmr::string& out_ret_buffer_p, const T& object_p, FE::ASCII* const version_p) noexcept
	{
		static_assert(std::is_class_v<T>, "Non-class/struct field variables cannot be serialized.");
		static_assert((std::is_reference_v<T> == false) && (std::is_pointer_v<T> == false), "static assertion failure: raw pointers and references cannot be serialized nor deserialized.");
		out_ret_buffer_p.clear();
		out_ret_buffer_p.reserve(one_KiB); // Pre-allocate 1 KiB.

		std::lock_guard<lock_type> l_lock(m_lock);
		std::pmr::string l_typename(reflection::type_id<T>().name(), m_pool);
		auto l_search_result = m_property_registry.find(l_typename);
		if (l_search_result == m_property_registry.end())
		{
#ifdef _ENABLE_LOG_
			constexpr FE::ASCII* l_error_code = TO_STRING(ErrorCode::_FatalSerializationError_3XX_TypeNotFound);
			constexpr FE::ASCII* l_error_message = "serialization failed - could not find the requested type information";
			FE_LOG(FE::log::Severity::_Warning, "Frogman Engine ${%s@0}: ${%s@1}.", l_error_code, l_error_message);
#endif
			return;
		}
		m_class_layer.emplace_back(&(l_search_result->second), l_search_result->second.begin());

		if constexpr (FE::has_base_type<T>::value == true)
		{
			__push_parent_class_layers_recursive<T>();
		}
		out_ret_buffer_p += version_p;
		out_ret_buffer_p += "\n";
		out_ret_buffer_p += l_typename;
		out_ret_buffer_p += "{";
		__serialize_mutually_recursive<T>(out_ret_buffer_p, object_p);
		out_ret_buffer_p += "};$-";

		while (m_scalable_container_size_record.empty() == false)
		{
			std::pmr::string l_buffer(m_pool);
			l_buffer.reserve(FE::algorithm::utility::count_int_digit_length(m_scalable_container_size_record.front()));
			FE::algorithm::utility::uint_to_string(l_buffer.data(), l_buffer.capacity(), m_scalable_container_size_record.front());

			out_ret_buffer_p += l_buffer.c_str();
			out_ret_buffer_p += "-";
			m_scalable_container_size_record.pop_front();
		}
		out_ret_buffer_p += "EOF\0";
	}


	template<typename T>
	void deserialize(const std::pmr::string& data_p, T& out_object_p, _FE_MAYBE_UNUSED_ FE::ASCII* const version_p) noexcept
	{
		static_assert(std::is_class_v<T>, "Non-class/struct field variables cannot be serialized.");
		static_assert((std::is_reference_v<T> == false) && (std::is_pointer_v<T> == false), "static assertion failure: raw pointers and references cannot be serialized nor deserialized.");

		if (data_p.empty() == true)
		{
#ifdef _ENABLE_LOG_
			constexpr FE::ASCII* l_error_code = TO_STRING(ErrorCode::_FatalDeserializationError_3XX_FileBufferEmpty);
			constexpr FE::ASCII* l_error_message = "deserialization failed - the input data buffer is empty";
			FE_LOG(FE::log::Severity::_Warning, "Frogman Engine ${%s@0}: ${%s@1}.", l_error_code, l_error_message);
#endif
			return;
		}

		std::lock_guard<lock_type> l_lock(m_lock);
		std::pmr::string l_typename(reflection::type_id<T>().name(), m_pool);
		auto l_search_result = m_property_registry.find(l_typename);
		FE_EXIT_IF((l_search_result == m_property_registry.end()) || (l_search_result->second.size() == 0), ErrorCode::_FatalSerializationError_3XX_TypeNotFound, "serialization failed: could not find the requested type information or the class/struct is empty");
		m_class_layer.emplace_back(&(l_search_result->second), l_search_result->second.begin());

		if constexpr (FE::has_base_type<T>::value == true)
		{
			__push_parent_class_layers_recursive<T>();
		}

		m_input_buffer = data_p;
		// Checks the file version
		FE_EXIT_IF(!algorithm::string::compare_ranged<char>(m_input_buffer.data(), algorithm::string::range{ 0, std::strlen(version_p) },
															version_p, algorithm::string::range{ 0, std::strlen(version_p) }),
															FE::ErrorCode::_FatalSerializationError_3XX_FileVersionMismatch, "Assertion failure: the serialization file version is not supported.");

		m_position = m_input_buffer.begin();
		m_position += m_input_buffer.find('{');
		FE_NEGATIVE_ASSERT(m_position == m_input_buffer.end(), "The serialization file is ill-formed or unsupported.");

		// Checks the class type name
		FE_EXIT_IF(!algorithm::string::compare_ranged<char>(l_typename.data(), algorithm::string::range{0, std::strlen(l_typename.data())},
															m_input_buffer.c_str(), algorithm::string::range{ std::strlen(version_p) + sizeof('\n'), static_cast<uint64>(m_position - m_input_buffer.begin())}),
			FE::ErrorCode::_FatalSerializationError_3XX_TypeMismatch, "Unable to deserialize an instance with a different class name.");
		++m_position; // Point the first byte.

		auto l_size_indicator = m_input_buffer.begin();
		auto l_pos = m_input_buffer.rfind("$-");
		FE_ASSERT(l_pos != m_input_buffer.npos, "Assertion failure: the serialization file is ill-formed or unsupported.");

		l_size_indicator += l_pos;
		l_size_indicator += 2; // to skip the "$-" and point to the first one.

		std::pmr::string::size_type l_eof_pos = m_input_buffer.find("EOF\0");
		FE_ASSERT(l_eof_pos != m_input_buffer.npos, "Assertion failure: the serialization file is ill-formed or unsupported.");
		auto l_file_end = m_input_buffer.begin() + l_eof_pos;
		FE_ASSERT(l_file_end <= m_input_buffer.end(), "Assertion failure: the serialization file is ill-formed or unsupported.");
		while (l_size_indicator != l_file_end)
		{
			algorithm::utility::uint_info l_info = algorithm::utility::string_to_uint(FE::iterator_cast<FE::ASCII*>(l_size_indicator));
			FE_LOG_IF(l_info._value == 0, FE::log::Severity::_Warning, "Warning: the size of the container is zero. Please debug if the file is corrupted.");
			m_scalable_container_size_record.push_back(l_info._value);
			l_size_indicator += l_info._digit_length; // move to the next.
			++l_size_indicator; // to skip the '-'.
		}

		__deserialize_mutually_recursive<T>(out_object_p);
	}


	template<typename T>
	instance_metadata* get_instance_metadata() noexcept 	// This method may return a nullptr.
	{
		std::lock_guard<lock_type> l_lock(m_lock);
		for (auto it = m_instance_metadata_lut.find(reflection::type_id<T>().name()); it != m_instance_metadata_lut.end(); ++it)
		{
			if (reflection::type_id<T>().name() == it->first)
			{
				return &(it->second);
			}
		}
		return nullptr;
	}

private:
	template <class InnerContainer>
	_FE_FORCE_INLINE_ void __push_multidimensional_container_serialization_task_recursive() noexcept
	{
		framework_base::get_framework().get_method_reflection().register_task< FE::cpp_style_task<property_registry, void(std::pmr::string&, const void*)> >(__get_serialization_task_name(reflection::type_id<InnerContainer>().name()), &property_registry::__serialize_by_foreach_mutually_recursive<InnerContainer>);

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
		framework_base::get_framework().get_method_reflection().register_task< FE::cpp_style_task<property_registry, void(void*)> >(__get_deserialization_task_name(reflection::type_id<InnerContainer>().name()), &property_registry::__deserialize_by_foreach_mutually_recursive<InnerContainer>);

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
	It is hard to tell which 'first' or 'second' is which.
	*/
	_FE_FORCE_INLINE_ typename class_property_list::iterator& __get_the_top_class_property_list_iterator() noexcept
	{
		return m_class_layer.back()._second;
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
		return *(m_class_layer.back()._first);
	}

	template<class U>
	_FE_FORCE_INLINE_ void __push_parent_class_layers_recursive() noexcept
	{
		if constexpr (FE::has_base_type<U>::value == true)
		{
			static typename  internal_map_type::iterator l_s_search_result;
			static typename  internal_map_type::key_type l_s_typename;
			l_s_typename = reflection::type_id<typename U::base_type>().name();
			l_s_search_result = m_property_registry.find(l_s_typename);
			if (l_s_search_result == m_property_registry.end())
			{
				return;
			}
			m_class_layer.emplace_back(&(l_s_search_result->second), l_s_search_result->second.begin());
			__push_parent_class_layers_recursive<typename U::base_type>();
		}
	}

	void __push_parent_class_layers_by_typename_string_recursive(const std::string_view& typename_p) noexcept;

	template<typename T>
	void __serialize_mutually_recursive(std::pmr::string& out_ret_buffer_p, const T& object_p) noexcept
	{
		var::ptrdiff l_offset_from_the_upmost_base_class_instance = 0;
		while (m_class_layer.empty() == false)
		{
			switch (__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._is_trivial)
			{
			case TypeTriviality::_Trivial:
			{
				// Check if the field variable meta data is valid.
				FE_NEGATIVE_ASSERT(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._size_in_byte == 0, "Assertion failed: unable to serialize a zero-byte property.");

				out_ret_buffer_p.append(reinterpret_cast<const char*>(&object_p) + l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property( __get_the_top_class_property_list_iterator() ), 
										__get_metadata_of_the_property( __get_the_top_class_property_list_iterator() )._size_in_byte);

				// Look for the next registered property of the class.
				++(__get_the_top_class_property_list_iterator());

				// Pop the class layer if the iterator reached the end of the property list.
				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
				{
					m_class_layer.pop_back();
				}
				break;
			}

			case TypeTriviality::_NotTrivial:

				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end()) // Pop the class layer if the iterator reached the end of the property list.
				{
					m_class_layer.pop_back();
					break;
				}

				// Find the class/struct meta data that contains its memory layer.
				auto l_search_result = m_property_registry.find(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator()).m_typename);

				// This is to serialize and deserialize containers and class instances that can be iterated through foreach. 
				FE::task_base* const l_foreach_task = framework_base::get_framework().get_method_reflection().retrieve(__get_serialization_task_name(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator()).m_typename)); // Load method pointer.
				if (l_foreach_task != nullptr) // is serializable with foreach?
				{
					FE::arguments<std::pmr::string&, const void*> l_task_args; // Any containers with begin() and end() can be serialized and deserialized.
					l_task_args._first = out_ret_buffer_p;
					l_task_args._second = reinterpret_cast<FE::byte*>(&object_p) + (l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator()));
					(*l_foreach_task)(this, nullptr, &l_task_args); // The pointed task knows what to do with the arguments type casting.

					// Move on to the next registered property of the class layer.
					++(__get_the_top_class_property_list_iterator());
				}
				else if (l_search_result != m_property_registry.end()) // push the meta data onto the stack if found.
				{
					l_offset_from_the_upmost_base_class_instance = __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator());

					// Look for the next registered property of the class layer.
					++(__get_the_top_class_property_list_iterator());

					// Push the member variable iterator and the class meta data to the class layer if the class meta data is found.
					m_class_layer.emplace_back(&(l_search_result->second), l_search_result->second.begin());
					__push_parent_class_layers_by_typename_string_recursive(l_search_result->first);
				}

				// if it reached the end.
				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
				{
					m_class_layer.pop_back();
				}
				break;
			}
		}
	}

	template<typename T>
	void __deserialize_mutually_recursive(T& out_object_p) noexcept
	{
		var::ptrdiff l_offset_from_the_upmost_base_class_instance = 0;
		while (m_class_layer.empty() == false)
		{
			switch (__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._is_trivial)
			{
			case TypeTriviality::_Trivial:
			{
				// Check if the meta data is valid.
				FE_NEGATIVE_ASSERT(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._size_in_byte == 0, "Assertion failed: unable to serialize a zero-byte property.");

				// Write the bits to the object_base, from a buffer.	
				FE::memcpy(reinterpret_cast<var::byte*>(&out_object_p) + (l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator())), FE::iterator_cast<FE::ASCII*>(m_position), __get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._size_in_byte);
				m_position += __get_metadata_of_the_property(__get_the_top_class_property_list_iterator())._size_in_byte; // Iterate to the next bits.

				// Look for the next registered property of the class layer.
				++(__get_the_top_class_property_list_iterator());

				// Pop the class layer if the iterator reached the end of the property list.
				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
				{
					m_class_layer.pop_back();
				}
				break;
			}

			case TypeTriviality::_NotTrivial:

				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end()) // Pop the class layer if the iterator reached the end of the property list.
				{
					m_class_layer.pop_back();
					break;
				}

				// Find the class/struct meta data that contains its memory layer.
				auto l_search_result = m_property_registry.find(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator()).m_typename);

				// This is to serialize and deserialize containers and class instances that can be iterated through foreach. 
				FE::task_base* const l_foreach_task = framework_base::get_framework().get_method_reflection().retrieve(__get_deserialization_task_name(__get_metadata_of_the_property(__get_the_top_class_property_list_iterator()).m_typename)); // Load method pointer.
				if (l_foreach_task != nullptr) // is deserializable with foreach?
				{
					FE::arguments<void*> l_pointer_to_container; // Any containers with begin() and end() can be serialized and deserialized.
					l_pointer_to_container._first = reinterpret_cast<var::byte*>(&out_object_p) + (l_offset_from_the_upmost_base_class_instance + __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator()));
					(*l_foreach_task)(this, nullptr, &l_pointer_to_container); // The pointed task object_base knows what to do with the arguments type casting.

					// Look for the next registered property of the class layer.
					++(__get_the_top_class_property_list_iterator());
				}
				else if (l_search_result != m_property_registry.end()) // push the meta data onto the stack if found.
				{
					l_offset_from_the_upmost_base_class_instance = __get_memory_offset_of_the_property(__get_the_top_class_property_list_iterator());

					// Look for the next registered property of the class layer.
					++(__get_the_top_class_property_list_iterator());

					// Push the member variable iterator and the class meta data to the class layer if the class meta data is found.
					m_class_layer.emplace_back(&(l_search_result->second), l_search_result->second.begin());
					__push_parent_class_layers_by_typename_string_recursive(l_search_result->first);
				}

				// if it reached the end.
				if (__get_the_top_class_property_list_iterator() == __get_top_class_property_list().end())
				{
					m_class_layer.pop_back();
				}
				break;
			}
		}
	}

	template<class Container>
	void __serialize_by_foreach_mutually_recursive(std::pmr::string& out_ret_buffer_p, const void* const data_p) noexcept
	{
		//FE_NEGATIVE_STATIC_ASSERT(FE::is_trivial<Container>::value == true, "Incorrect template argument type: serializable containers are not trivially constructible and destructible.");
		static_assert(FE::is_serializable<Container>::value, "The container is unable to be serialized: the container type is not supported and not compatible to this system.");
		FE_NEGATIVE_ASSERT(data_p == nullptr, "Aborting the serialization process: the pointer to the container is nullptr.");
		const Container* const l_container = static_cast<const Container* const>(data_p);

		if constexpr (FE::is_trivial<Container>::value == false)
		{
			if constexpr (FE::is_trivial<typename Container::value_type>::value == true)
			{
				if constexpr (std::is_array<Container>::value == true)
				{
					out_ret_buffer_p.append(reinterpret_cast<const char*>(l_container), sizeof(Container));
				}
				else
				{
					m_scalable_container_size_record.push_back(l_container->size());
					out_ret_buffer_p.append(reinterpret_cast<const char*>(l_container->data()), sizeof(typename Container::value_type) * l_container->size());
				}
			}
			else
			{
				m_scalable_container_size_record.push_back(l_container->size());
				for (auto& element : *l_container)
				{
					if constexpr ((FE::is_serializable<typename Container::value_type>::value == true) && (FE::is_trivial<typename Container::value_type>::value == false))
					{
						__serialize_by_foreach_mutually_recursive<typename Container::value_type>(out_ret_buffer_p, &element);
					}
					else
					{
						__serialize_mutually_recursive<typename Container::value_type>(out_ret_buffer_p, element);
					}
				}
			}
		}
	}

	std::string_view __get_serialization_task_name(const std::string_view& property_typename_p) noexcept;

	template<class Container>
	void __deserialize_by_foreach_mutually_recursive(void* const data_p) noexcept
	{
		//FE_NEGATIVE_STATIC_ASSERT(FE::is_trivial<Container>::value == true, "Incorrect template argument type: serializable containers are not trivially constructible and destructible.");
		static_assert(FE::is_serializable<Container>::value, "The container is unable to be deserialized: the container type is not supported and not compatible to this system.");
		FE_NEGATIVE_ASSERT(data_p == nullptr, "Aborting the deserialization process: the pointer to the container is nullptr.");
		Container* const l_container = static_cast<Container* const>(data_p);

		if constexpr (FE::is_trivial<Container>::value == false)
		{
			l_container->resize(m_scalable_container_size_record.front());
			m_scalable_container_size_record.pop_front();

			if constexpr (FE::is_trivial<typename Container::value_type>::value == true)
			{
				if constexpr (std::is_array<Container>::value == true)
				{
					FE::memcpy(reinterpret_cast<var::byte*>(l_container), FE::iterator_cast<FE::ASCII*>(m_position), sizeof(Container));
					m_position += sizeof(Container); // Iterate to the next bit.
				}
				else
				{
					FE::memcpy(reinterpret_cast<var::byte*>(l_container->data()), FE::iterator_cast<FE::ASCII*>(m_position), sizeof(typename Container::value_type) * l_container->size());
					m_position += sizeof(typename Container::value_type) * l_container->size(); // Iterate to the next bit.
				}
			}
			else
			{
				for (auto& element : *l_container)
				{
					if constexpr ((FE::is_serializable<typename Container::value_type>::value == true) && (FE::is_trivial<typename Container::value_type>::value == false))
					{
						__deserialize_by_foreach_mutually_recursive<typename Container::value_type>(&element);
					}
					else
					{
						__deserialize_mutually_recursive<typename Container::value_type>(element);
					}
				}
			}
		}
	}

	std::string_view __get_deserialization_task_name(const std::string_view& property_typename_p) noexcept;
};




class enum_registry;


class enum_metadata
{
	friend class enum_registry;
public:
	constexpr static inline FE::uint32 field_max_size = 8;

private:
	std::string_view m_typename;
	absl::flat_hash_map< std::string_view, std::array<var::byte, field_max_size>,
		FE::hash<std::string_view>,
		std::equal_to<std::string_view>,
		FE::polymorphic_allocator<std::pair<const std::string_view, std::array<var::byte, field_max_size>>>> m_string_to_value_map;

	absl::flat_hash_map<std::array<var::byte, field_max_size>, std::string_view, 
						FE::hash<std::array<var::byte, field_max_size>>, 
						std::equal_to<std::array<var::byte, field_max_size>>,
						FE::polymorphic_allocator<std::pair<const std::array<var::byte, field_max_size>, std::string_view>>
						> m_value_to_string_map;

public:
	enum_metadata(std::pmr::memory_resource* const resource_p) noexcept;
	_FE_FORCE_INLINE_ FE::ASCII* get_typename() const noexcept { return m_typename.data(); }

	template<typename EnumStruct>
	std::optional<EnumStruct> string_to_enum(const std::string_view& enum_value_string_p) const noexcept
	{
		static_assert(sizeof(EnumStruct) <= field_max_size, "Static assertion failure: the enum size exceeds the maximum supported size.");

		auto l_result = m_string_to_value_map.find(enum_value_string_p);
		if (l_result == m_string_to_value_map.end())
		{
			return std::nullopt;
		}

		if (l_result->first == enum_value_string_p)
		{
			std::array<var::byte, field_max_size> l_result = l_result->second;
			EnumStruct l_ret;
			FE::memcpy(&l_ret, sizeof(EnumStruct), l_result.data(), l_result.size());
			return l_ret;
		}

		return std::nullopt;
	}

	template<typename EnumStruct>
	FE::ASCII* enum_to_string(const EnumStruct value_p) const noexcept
	{
		static_assert(sizeof(EnumStruct) <= field_max_size, "Static assertion failure: the enum size exceeds the maximum supported size.");

		std::array<var::byte, field_max_size> l_enum_bits = { 0 };
		FE::memcpy(l_enum_bits.data(), l_enum_bits.size(), &value_p, sizeof(EnumStruct));

		auto l_result = m_value_to_string_map.find(l_enum_bits);
		if (l_result == m_value_to_string_map.end())
		{
			return nullptr;
		}

		if (std::memcmp(l_enum_bits.data(), l_result->first.data(), l_enum_bits.size()) == 0)
		{
			return l_result->second.data();
		}

		return nullptr;
	}
};

// register enum with FHT.
class enum_registry
{
	tsl::array_map< var::ASCII, enum_metadata > m_enum_registry;
	std::pmr::memory_resource* m_resource;

public:
	enum_registry(std::pmr::memory_resource* const resource_p, FE::size capacity_p) noexcept;
	~enum_registry() noexcept = default;

	template<typename EnumStruct>
	void register_enum_struct(const std::string_view& enum_struct_name_p,
		                      std::initializer_list< FE::pair<EnumStruct, FE::ASCII*> >&& field_list_p)
	{
		enum_metadata l_enum_struct_metadata(m_resource);
		l_enum_struct_metadata.m_typename = enum_struct_name_p;
		for (const FE::pair<EnumStruct, FE::ASCII*>& field : field_list_p)
		{
			FE_ASSERT(field._second != nullptr, "Assertion failed: nullptr cannot be mapped to an enum value.");
			std::array<var::byte, enum_metadata::field_max_size> l_enum_bits{0};
			FE::memcpy(l_enum_bits.data(), l_enum_bits.size(), &field._first, sizeof(EnumStruct));
			l_enum_struct_metadata.m_string_to_value_map.emplace(field._second, l_enum_bits);
			l_enum_struct_metadata.m_value_to_string_map.emplace(l_enum_bits, field._second);
		}

		FE_EXIT_IF(m_enum_registry.insert(enum_struct_name_p, l_enum_struct_metadata).second == false, FE::ErrorCode::_FatalError_TableInsertionFailure,
			"FE Runtime Reflection Registration Error: failed to register a property metadata to the FE runtime reflection system.\nThis error might have occurred in the FE runtime reflection metadata loader function, during the initialization. Please reach out to the FE developer.");
	}

	// This method may return a nullptr.
	enum_metadata* retrieve_enum_struct_metadata(const std::string_view& enum_struct_name_p);
};


END_NAMESPACE




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
	static_assert(::FE::is_function<__VA_ARGS__>::value == true, "Static assertion failed: the type is not a function."); \
public: \
	_FE_FORCE_INLINE_ method_metadata_##method_name(auto* this_p) noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_,	m_method_name = __get_signature<::std::remove_pointer_t<::std::remove_const_t<decltype(this_p)>>>(); \
												::FE::framework::framework_base::get_framework().get_method_reflection() \
                                                .register_task< ::FE::cpp_style_task<::std::remove_pointer_t<::std::remove_const_t<decltype(this_p)>>, __VA_ARGS__> > \
			                                     ( m_method_name, &::std::remove_pointer_t<::std::remove_const_t<decltype(this_p)>>::method_name ) \
        ); \
	} \
private: \
	template <typename T = void> \
	::std::string& __get_signature() noexcept \
	{ \
		static ::std::string l_s_full_signature; \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, \
			::std::string l_signature_body = " "; \
			l_signature_body.reserve(128); \
			l_signature_body += ::FE::framework::reflection::type_id<T>().name(); \
			l_signature_body += "::"; \
			l_signature_body += #method_name; \
			::std::string l_full_signature = ::FE::framework::reflection::type_id<__VA_ARGS__>().name(); \
			auto l_prop_registry_insertion_result = ::FE::algorithm::string::find_the_last( l_full_signature.c_str(), '(' ); \
			l_full_signature.insert( l_prop_registry_insertion_result->_begin, l_signature_body ); \
			l_prop_registry_insertion_result = ::FE::algorithm::string::find_the_last( l_full_signature.c_str(), " __ptr64" ); \
			if (l_prop_registry_insertion_result != std::nullopt) \
			{ \
				l_full_signature.erase(l_prop_registry_insertion_result->_begin, l_prop_registry_insertion_result->_end - l_prop_registry_insertion_result->_begin); \
			} \
			l_s_full_signature = std::move(l_full_signature); \
		); \
		return l_s_full_signature; \
	} \
	::std::string_view m_method_name; \
public: \
	const ::std::string_view& get_method_name() const noexcept { return m_method_name; } \
}; \
_FE_NO_UNIQUE_ADDRESS_ method_metadata_##method_name method_name##_method_meta = this;
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
	_FE_FORCE_INLINE_ static_method_metadata_##method_name(auto* this_p) noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_,	m_method_name = __get_signature<::std::remove_pointer_t<::std::remove_const_t<decltype(this_p)>>>(); \
												::FE::framework::framework_base::get_framework().get_method_reflection() \
                                                .register_task< ::FE::c_style_task<__VA_ARGS__> > \
                                                 ( m_method_name, &::std::remove_pointer_t<::std::remove_const_t<decltype(this_p)>>::method_name ) \
        ); \
	} \
private: \
	template <typename T = void> \
	::std::string& __get_signature() noexcept \
	{ \
		static ::std::string l_s_full_signature; \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, \
			::std::string l_signature_body = " "; \
			l_signature_body.reserve(128); \
			l_signature_body += ::FE::framework::reflection::type_id<T>().name(); \
			l_signature_body += "::"; \
			l_signature_body += #method_name; \
			::std::string l_full_signature = ::FE::framework::reflection::type_id<__VA_ARGS__>().name(); \
			auto l_prop_registry_insertion_result = ::FE::algorithm::string::find_the_last( l_full_signature.c_str(), '(' ); \
			l_full_signature.insert( l_prop_registry_insertion_result->_begin, l_signature_body ); \
			l_prop_registry_insertion_result = ::FE::algorithm::string::find_the_last( l_full_signature.c_str(), " __ptr64" ); \
			if (l_prop_registry_insertion_result != std::nullopt) \
			{ \
				l_full_signature.erase(l_prop_registry_insertion_result->_begin, l_prop_registry_insertion_result->_end - l_prop_registry_insertion_result->_begin); \
			} \
			l_s_full_signature = std::move(l_full_signature); \
		); \
		return l_s_full_signature; \
	} \
	::std::string_view m_method_name; \
public: \
	const ::std::string_view& get_method_name() const noexcept { return m_method_name; } \
}; \
_FE_NO_UNIQUE_ADDRESS_ static_method_metadata_##method_name method_name##_static_method_meta = this;
#endif


#ifdef FE_PROPERTY
	#error FE_PROPERTY is a reserved Frogman Engine macro keyword.
#else
/*
The FE_PROPERTY macro defines a class for property reflection that registers a specified property of a given type with the framework's property reflection system
ensuring it is only registered once during the application's execution.
*/
#define FE_PROPERTY(property_name)  \
class property_metadata_##property_name : public ::FE::internal::ECS::gc_metadata_base \
{ \
public: \
	_FE_FORCE_INLINE_ property_metadata_##property_name(auto* this_p) noexcept \
	{ \
		FE_DO_ONCE(_DO_ONCE_PER_APP_EXECUTION_, ::FE::framework::framework_base::get_framework().get_property_reflection() \
                                                .register_property<::std::remove_pointer_t<::std::remove_const_t<decltype(this_p)>>, decltype(this_p->property_name)> \
                                                 ( *this_p, this_p->property_name, #property_name ) \
		); \
		if constexpr (::std::is_base_of_v< ::FE::component_base, ::std::remove_pointer_t<::std::remove_const_t<decltype(this_p)>> >) \
		{ \
			::FE::internal::ECS::gc_metadata_base::add_watch<decltype(this_p->property_name)>(this_p, this_p->property_name); \
		} \
	} \
}; \
_FE_NO_UNIQUE_ADDRESS_ property_metadata_##property_name property_name##_property_meta = this; \
friend class property_metadata_##property_name;
#endif


#endif