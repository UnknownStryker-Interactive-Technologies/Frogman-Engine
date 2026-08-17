#include <FE/memory.hpp>




std::pmr::memory_resource* FE::get_large_thread_local_memory_resource() noexcept
{
	return FE::framework::framework_base::get_framework().get_large_memory_resource();
}

std::pmr::memory_resource* FE::get_thread_local_memory_resource() noexcept
{
	return FE::framework::framework_base::get_framework().get_memory_resource();
}
