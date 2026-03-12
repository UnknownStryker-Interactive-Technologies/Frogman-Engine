#include <FE/game.hpp>
#include <FE/engine.hpp>




FE::game::game(FE::framework::ECS& engine_ecs_p) noexcept
	:	base_type(engine_ecs_p),
		m_world_list(get_ecs_memory_resource()),
		m_current_world()
{}

FE::smart_ptr<FE::world, FE::RefType::_Observer> FE::game::create_world(const std::pmr::string& path_to_world_file_p) noexcept
{
	(path_to_world_file_p);
	return FE::smart_ptr<FE::world, FE::RefType::_Observer>();
}

void FE::game::transition_world(const std::pmr::string& new_world_p) noexcept
{
	m_current_world = m_world_list[new_world_p];
	FE::engine::get_engine().notify_world_transition(*this);
}
