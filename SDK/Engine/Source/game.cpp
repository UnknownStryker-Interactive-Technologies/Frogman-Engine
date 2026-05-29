#include <FE/game.hpp>
#include <FE/engine.hpp>




FE::game::game(FE::framework::ECS& engine_ecs_p) noexcept
	:	base_type(engine_ecs_p),
		m_world_list( typename world_list::allocator_type{ FE::framework::framework_base::get_framework().get_memory_resource() } ),
		m_current_world()
{}

FE::smart_ptr<FE::world, FE::RefType::_Observer> FE::game::create_world(const FE::directory_string& path_to_world_file_p) noexcept
{
	(path_to_world_file_p);
	area_of_operation l_area_of_operation; // build from world file.
	//l_area_of_operation._target_gdk_version = 
	// FE_ASSERT( FE::engine::get_engine().get_engine_info()._version == l_area_of_operation._target_gdk_version );
	// area_of_operation._tag = hash(...);
	// area_of_operation._gravity = ...;
	l_area_of_operation._component_type_count_hint = FE::engine::get_engine().get_project_config()._max_engine_component_type_count_hint;

	auto l_result = m_world_list.emplace(l_area_of_operation._tag, FE::make_owner<world>(FE::framework::framework_base::get_framework().get_memory_resource(), l_area_of_operation));
	m_current_world = l_result.first->second;
	return m_current_world;
}
