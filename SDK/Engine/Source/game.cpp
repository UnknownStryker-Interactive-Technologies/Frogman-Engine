#include <FE/game.hpp>

#include <FE/memory.hpp>




FE::game::game(	FE::framework::ECS& engine_ecs_p, 
				const std::pmr::string& entry_world_path_p, 
				const std::pmr::vector<std::pmr::string>& world_paths_p) noexcept
	:	base_type(engine_ecs_p),
		m_world_list(FE::memory::get_thread_local_memory_resource())
{
	// TO DO: load worlds from the specified paths.

	(entry_world_path_p);
	(world_paths_p);
}
