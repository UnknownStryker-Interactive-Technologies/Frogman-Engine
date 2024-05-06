#ifndef _FE_CORE_FUNCTION_TABLE_HXX_
#define _FE_CORE_FUNCTION_TABLE_HXX_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/function.hxx>
#include <FE/core/fstring.hxx>
#include <FE/core/hash.hpp>
#include <FE/core/pool_allocator.hxx>
#include <memory>
#include <mutex>
#include <unordered_map>


#define _FUNCTION_TABLE_MEMORY_NAMESPACE_ "fn table"




BEGIN_NAMESPACE(FE::framework)

using method_signature_t = FE::fstring<64>;


// function_table is currently under development. It might not be suitable to use
class function_table
{
public:		
	constexpr static size_t function_pool_size = 8 MB;

	template<typename T>
	using smart_function_ptr = FE::generic_pool_ptr<T, function_pool_size, FE::align_CPU_L1_cache_line>;

	using underlying_container = std::unordered_map<method_signature_t,
													FE::task_base*, 
													FE::hash<method_signature_t>,
													std::equal_to<method_signature_t>,
													FE::namespace_pool_allocator<std::pair<const method_signature_t, FE::task_base*>, FE::align_CPU_L1_cache_line>
													>;
private:
	static std::unique_ptr<underlying_container> s_task_map;
	static std::mutex s_mutex;
	
public:
	function_table() noexcept = delete;
	~function_table() noexcept = delete;
	function_table(const function_table& other_cref_p) noexcept = delete;
	function_table(function_table&& rvalue_p) noexcept = delete;

	_FORCE_INLINE_ static void create_function_table() noexcept
	{
		FE::pool_allocator_base<FE::align_CPU_L1_cache_line>::create_pool_allocator_resource(1);
		s_task_map = std::make_unique<underlying_container>(typename underlying_container::allocator_type{ _FUNCTION_TABLE_MEMORY_NAMESPACE_ });
	}

	_FORCE_INLINE_ static void destroy_function_table() noexcept
	{
		s_task_map.reset();
		FE::pool_allocator_base<FE::align_CPU_L1_cache_line>::destroy_pool_allocator_resource();
	}

	template<class TaskType, typename FunctionPtr>
	_FORCE_INLINE_ static void register_method(const underlying_container::key_type& key_p, FunctionPtr function_p) noexcept
	{	
		FE_STATIC_ASSERT((std::is_pointer<FunctionPtr>::value == false), "An invalid function type detected.");
		FE_STATIC_ASSERT((std::is_base_of<FE::task_base, TaskType>::value == false), "An invalid function type detected.");
		static FE::generic_pool<function_pool_size, FE::align_CPU_L1_cache_line> l_s_pool;
		{
			std::lock_guard<std::mutex> l_lock(s_mutex);
			smart_function_ptr<TaskType> l_unique_ptr = l_s_pool.allocate<TaskType>(_FUNCTION_TABLE_MEMORY_NAMESPACE_, 1);
			*(l_unique_ptr.get()) = function_p;
			FE::task_base* const l_task_ptr = l_unique_ptr.release();
			s_task_map->emplace(key_p, l_task_ptr);
		}
	}

	_FORCE_INLINE_ static boolean check_presence(const underlying_container::key_type& key_p) noexcept { return s_task_map->find(key_p) != s_task_map->end(); }
	_FORCE_INLINE_ static FE::task_base* retrieve(const underlying_container::key_type& key_p) noexcept { return s_task_map->find(key_p)->second; }

	template<class... Arguments>
	_FORCE_INLINE_ static void invoke(const underlying_container::key_type& key_p) noexcept { s_task_map->find(key_p)->second->operator()(); }

	_FORCE_INLINE_ static void reserve(size_t size_p) noexcept { s_task_map->reserve(size_p); }
};


END_NAMESPACE
#endif
