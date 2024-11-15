#ifndef _FE_FRAMEWORK_REFLECTION_METHOD_HPP_
#define _FE_FRAMEWORK_REFLECTION_METHOD_HPP_
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
#include <FE/function.hxx>
#include <FE/hash.hpp>
#include <FE/pool/block_pool.hxx>

// std
#include <mutex>
#include <memory_resource>
#include <shared_mutex>
#include <string>

// boost::shared_lock_guard
#include <boost/thread/shared_lock_guard.hpp>

// ronbin hood hash
#include <robin_hood.h>




BEGIN_NAMESPACE(FE::framework::reflection)

template<typename T>
_FE_CONSTEXPR20_ void construct_object(void* const address_p) noexcept
{
	new(static_cast<T*>(address_p)) T();
}

class method
{
public:	
	using alignment_type = FE::align_8bytes;

private:
	using lock_type = std::shared_mutex;
	using reflection_map_type = robin_hood::unordered_map<std::string, FE::task_base*, FE::hash<std::string>>;

	lock_type m_lock;
	reflection_map_type m_task_map;
	std::pmr::monotonic_buffer_resource m_function_pool;

public:
	method(FE::size reflection_map_capacity_p) noexcept
		: m_lock(), m_task_map(reflection_map_capacity_p), m_function_pool(alignment_type::size* reflection_map_capacity_p)
	{}
	~method() noexcept = default;

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
	_FE_FORCE_INLINE_ void register_task(const typename reflection_map_type::key_type& key_p, FunctionPtr function_p) noexcept
	{	
		FE_NEGATIVE_STATIC_ASSERT((std::is_base_of<FE::task_base, TaskType>::value == false), "An invalid method type detected.");
		std::lock_guard<lock_type> l_lock(m_lock);
		std::pmr::polymorphic_allocator<TaskType> l_allocator(&m_function_pool);
		TaskType* const l_task = l_allocator.allocate(1);
		new(l_task) TaskType(function_p);
		this->m_task_map.emplace(key_p, l_task);
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
		std::lock_guard<lock_type> l_lock(this->m_lock);
		auto l_result = this->m_task_map.find(key_p);
		return (l_result == this->m_task_map.end()) ? nullptr : l_result->second;
	}

	template<typename ReturnType = void, class Arguments = FE::arguments<void>>
	_FE_FORCE_INLINE_ ReturnType invoke(const typename reflection_map_type::key_type& key_p, Arguments arguments_p = FE::arguments<void>()) noexcept
	{
		boost::shared_lock_guard<lock_type> l_shared_mutex(m_lock);

		auto l_result = this->m_task_map.find(key_p);
		FE_ASSERT(l_result != this->m_task_map.end(), "Assertion failure: unable to retrieve an unregistered task object_base pointer.");
		
		if constexpr (std::is_same<ReturnType, void>::value == true)
		{
			l_result->second->operator()(&arguments_p);
		}
		else
		{
			return std::any_cast<ReturnType>(l_result->second->operator()(&arguments_p));
		}
	}

	template<typename ReturnType = void, class C, class Arguments = FE::arguments<void>>
	_FE_FORCE_INLINE_ ReturnType invoke(C& in_out_instance_p, const typename reflection_map_type::key_type& key_p, Arguments arguments_p = FE::arguments<void>()) noexcept
	{
		FE_NEGATIVE_STATIC_ASSERT((std::is_class<C>::value == false), "Static Assertion Failure: The template typename C is not a class type.");
		boost::shared_lock_guard<lock_type> l_shared_mutex(m_lock);

		auto l_result = this->m_task_map.find(key_p);
		FE_ASSERT(l_result != this->m_task_map.end(), "Assertion failure: unable to retrieve an unregistered task object_base pointer.");
		
		l_result->second->set_instance(&in_out_instance_p);

		if constexpr (std::is_same<ReturnType, void>::value == true)
		{
			l_result->second->operator()(&arguments_p);
		}
		else
		{
			return std::any_cast<ReturnType>(l_result->second->operator()(&arguments_p));
		}
	}
};


END_NAMESPACE
#endif