#include <FE/game.hpp>

#include <FE/memory.hpp>




FE::game::game(FE::framework::ECS& engine_ecs_p) noexcept
	:	base_type(engine_ecs_p)
{}

FE::smart_ptr<FE::world, FE::RefType::_Observer> FE::game::get_current_world() noexcept
{
	FE_ASSERT(m_world_list.empty() == false);
	return m_world_list[0];
}

FE::smart_ptr<FE::world, FE::RefType::_Observer> FE::game::create_world(const std::filesystem::path & path_to_world_file_p) noexcept
{
	(path_to_world_file_p);
	return FE::smart_ptr<FE::world, FE::RefType::_Observer>();
}

void FE::game::transition_world(const FE::smart_ptr<FE::world, FE::RefType::_Observer>& new_world_p) noexcept
{
	(new_world_p);
}
