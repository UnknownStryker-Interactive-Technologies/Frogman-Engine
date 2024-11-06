#ifndef _FE_FRAMEWORK_REFLECTION_FUNCTION_HPP_
#define _FE_FRAMEWORK_REFLECTION_FUNCTION_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/framework/string.hxx>

#include <FE/prerequisites.h>
#include <FE/function.hxx>
#include <FE/hash.hpp>
#include <FE/pool/block_pool.hxx>

// std
#include <mutex>
#include <memory_resource>
#include <shared_mutex>

// boost::shared_lock_guard
#include <boost/thread/shared_lock_guard.hpp>

// ronbin hood hash
#include <robin_hood.h>




BEGIN_NAMESPACE(FE::framework::reflection)

/* 
It is worth noting that, FE::string's contents can be allocated on a thread local memory pool
by adding -DMEMORY_POOL_FE_STRINGS=1 option to cmake.
*/
// TO DO: needs some rework.
class function
{
public:	
	using alignment_type = FE::align_32bytes;
	using lock_type = std::shared_mutex;

	static constexpr size initial_size_in_bytes = (65536);
	static constexpr size initial_capacity = initial_size_in_bytes / alignment_type::size;

	using underlying_container = robin_hood::unordered_map<FE::string, FE::task_base*, FE::hash<FE::string>>;
	using function_pool_type = FE::block_pool<alignment_type::size, initial_capacity, alignment_type>;

private:
	static lock_type s_lock;
	static underlying_container* s_task_map;
	static function_pool_type s_function_pool;

public:
	function() noexcept = default;
	~function() noexcept = default;

	_FE_FORCE_INLINE_ static FE::boolean __initialize() noexcept
	{
		FE_NEGATIVE_ASSERT(s_task_map != nullptr, "Assertion failure: cannot initialize FE::framework::reflection::function more than once.");
		if(s_task_map == nullptr)
		{
			s_task_map = (underlying_container*)FE_ALIGNED_ALLOC(sizeof(underlying_container), FE::align_CPU_L1_cache_line::size);
			new(s_task_map) underlying_container(function::initial_capacity);
			return true;
		}

		return false;
	}
	
	_FE_FORCE_INLINE_ static FE::boolean __shutdown() noexcept
	{
		FE_NEGATIVE_ASSERT(s_task_map == nullptr, "Assertion failure: unable to clean_up() FE::framework::reflection::function. It is null.");
		if(s_task_map != nullptr)
		{
			s_task_map->~underlying_container();
			FE_ALIGNED_FREE(s_task_map);
			s_task_map = nullptr;
			return true;
		}

		return false;
	}

public:
	_FE_FORCE_INLINE_ static FE::boolean is_available() noexcept
	{
		return s_task_map != nullptr;
	}

	_FE_FORCE_INLINE_ static void reserve(size size_p) noexcept
	{
		std::lock_guard<lock_type> l_lock(s_lock);
		s_task_map->reserve(size_p);
	}

	template<class TaskType, typename FunctionPtr>
	_FE_FORCE_INLINE_ static void register_task(const typename underlying_container::key_type& key_p, FunctionPtr function_p) noexcept
	{	
		FE_NEGATIVE_STATIC_ASSERT((std::is_base_of<FE::task_base, TaskType>::value == false), "An invalid function type detected.");
		std::lock_guard<lock_type> l_lock(s_lock);

		TaskType* const l_task = s_function_pool.allocate<TaskType>();
		l_task->set_task(function_p);
		s_task_map->emplace(key_p, l_task);
	}

	_FE_FORCE_INLINE_ static FE::boolean check_presence(const typename underlying_container::key_type& key_p) noexcept 
	{
		std::lock_guard<lock_type> l_lock(s_lock);
		return s_task_map->find(key_p) != s_task_map->end(); 
	}

/*
	This function may return nullptr.
*/
	_FE_NODISCARD_ _FE_FORCE_INLINE_ static FE::task_base* retrieve(const typename underlying_container::key_type& key_p) noexcept 
	{
		std::lock_guard<lock_type> l_lock(s_lock);
		auto l_result = s_task_map->find(key_p);
		return (l_result == s_task_map->end()) ? nullptr : l_result->second;
	}

	template<class Arguments = FE::arguments<void>>
	_FE_FORCE_INLINE_ static std::any invoke(const typename underlying_container::key_type& key_p, Arguments arguments_p = FE::arguments<void>()) noexcept
	{
		boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
		auto l_result = s_task_map->find(key_p);
		FE_NEGATIVE_ASSERT(l_result == s_task_map->end(), "Assertion failure: unable to retrieve an unregistered task object_base pointer.");
		return l_result->second->operator()(&arguments_p);
	}

	template<class C, class Arguments = FE::arguments<void>>
	_FE_FORCE_INLINE_ static std::any invoke(C& in_out_instance_p, const typename underlying_container::key_type& key_p, Arguments arguments_p = FE::arguments<void>()) noexcept
	{
		boost::shared_lock_guard<lock_type> l_shared_mutex(s_lock);
		FE_NEGATIVE_STATIC_ASSERT((std::is_class<C>::value == false), "Static Assertion Failure: The template typename C is not a class type.");
		auto l_result = s_task_map->find(key_p);
		FE_NEGATIVE_ASSERT(l_result == s_task_map->end(), "Assertion failure: unable to retrieve an unregistered task object_base pointer.");
		l_result->second->set_instance(&in_out_instance_p);
		return l_result->second->operator()(&arguments_p);
	}
};


END_NAMESPACE
#endif