#include <FE/world.hxx>
#include <FE/engine.hpp>
#include <boost/hash2/xxhash.hpp>




FE::world::world(world_tag_t world_tag_p) noexcept
	: m_registry(FE::get_concurrent_memory_resource()),
	m_world_tag(world_tag_p),
	m_delta_time(),
	m_tagged_entities(std::pmr::polymorphic_allocator<std::pair<const std::string_view, FE::entity>>{FE::get_concurrent_memory_resource()}),
	m_deferred_world_initialization_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_world_default_entity_initialization_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_world_begin_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_pre_world_tick_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_world_tick_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_post_world_tick_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_pre_entity_tick_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_entity_tick_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_post_entity_tick_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_pre_physics_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_start_physics_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_physics_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_end_physics_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_post_physics_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_post_update_work_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_pre_render_queue_commit_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_post_render_queue_commit_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_world_end_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_world_default_entity_deinitialization_cmd_queue(FE::get_concurrent_memory_resource()),
	m_deferred_world_deinitialization_cmd_queue(FE::get_concurrent_memory_resource())
{
}

FE::world::~world() noexcept
{
}


FE::world::world(world&& other_p) noexcept
	: m_registry(std::move(other_p.m_registry)),
	m_world_tag(std::exchange(other_p.m_world_tag, world_tag_t{})),
	m_delta_time(std::exchange(other_p.m_delta_time, FE::float64{})),
	m_tagged_entities(std::move(other_p.m_tagged_entities)),
	m_deferred_world_initialization_cmd_queue(std::move(other_p.m_deferred_world_initialization_cmd_queue)),
	m_deferred_world_default_entity_initialization_cmd_queue(std::move(other_p.m_deferred_world_default_entity_initialization_cmd_queue)),
	m_deferred_world_begin_cmd_queue(std::move(other_p.m_deferred_world_begin_cmd_queue)),
	m_deferred_pre_world_tick_cmd_queue(std::move(other_p.m_deferred_pre_world_tick_cmd_queue)),
	m_deferred_world_tick_cmd_queue(std::move(other_p.m_deferred_world_tick_cmd_queue)),
	m_deferred_post_world_tick_cmd_queue(std::move(other_p.m_deferred_post_world_tick_cmd_queue)),
	m_deferred_pre_entity_tick_cmd_queue(std::move(other_p.m_deferred_pre_entity_tick_cmd_queue)),
	m_deferred_entity_tick_cmd_queue(std::move(other_p.m_deferred_entity_tick_cmd_queue)),
	m_deferred_post_entity_tick_cmd_queue(std::move(other_p.m_deferred_post_entity_tick_cmd_queue)),
	m_deferred_pre_physics_cmd_queue(std::move(other_p.m_deferred_pre_physics_cmd_queue)),
	m_deferred_start_physics_cmd_queue(std::move(other_p.m_deferred_start_physics_cmd_queue)),
	m_deferred_physics_cmd_queue(std::move(other_p.m_deferred_physics_cmd_queue)),
	m_deferred_end_physics_cmd_queue(std::move(other_p.m_deferred_end_physics_cmd_queue)),
	m_deferred_post_physics_cmd_queue(std::move(other_p.m_deferred_post_physics_cmd_queue)),
	m_deferred_post_update_work_cmd_queue(std::move(other_p.m_deferred_post_update_work_cmd_queue)),
	m_deferred_pre_render_queue_commit_cmd_queue(std::move(other_p.m_deferred_pre_render_queue_commit_cmd_queue)),
	m_deferred_post_render_queue_commit_cmd_queue(std::move(other_p.m_deferred_post_render_queue_commit_cmd_queue)),
	m_deferred_world_end_cmd_queue(std::move(other_p.m_deferred_world_end_cmd_queue)),
	m_deferred_world_default_entity_deinitialization_cmd_queue(std::move(other_p.m_deferred_world_default_entity_deinitialization_cmd_queue)),
	m_deferred_world_deinitialization_cmd_queue(std::move(other_p.m_deferred_world_deinitialization_cmd_queue))
{
}

//FE::world& FE::world::operator=(world&& other_p) noexcept
//{
//	m_registry = std::move(other_p.m_registry);
//	m_world_tag = std::exchange(other_p.m_world_tag, world_tag_t{});
//	m_delta_time = std::exchange(other_p.m_delta_time, FE::float64{});
//	m_tagged_entities = std::move(other_p.m_tagged_entities);
//	m_deferred_world_initialization_cmd_queue = std::move(other_p.m_deferred_world_initialization_cmd_queue);
//	m_deferred_world_default_entity_initialization_cmd_queue = std::move(other_p.m_deferred_world_default_entity_initialization_cmd_queue);
//	m_deferred_world_begin_cmd_queue = std::move(other_p.m_deferred_world_begin_cmd_queue);
//	m_deferred_pre_world_tick_cmd_queue = std::move(other_p.m_deferred_pre_world_tick_cmd_queue);
//	m_deferred_world_tick_cmd_queue = std::move(other_p.m_deferred_world_tick_cmd_queue);
//	m_deferred_post_world_tick_cmd_queue = std::move(other_p.m_deferred_post_world_tick_cmd_queue);
//	m_deferred_pre_entity_tick_cmd_queue = std::move(other_p.m_deferred_pre_entity_tick_cmd_queue);
//	m_deferred_entity_tick_cmd_queue = std::move(other_p.m_deferred_entity_tick_cmd_queue);
//	m_deferred_post_entity_tick_cmd_queue = std::move(other_p.m_deferred_post_entity_tick_cmd_queue);
//	m_deferred_pre_physics_cmd_queue = std::move(other_p.m_deferred_pre_physics_cmd_queue);
//	m_deferred_start_physics_cmd_queue = std::move(other_p.m_deferred_start_physics_cmd_queue);
//	m_deferred_physics_cmd_queue = std::move(other_p.m_deferred_physics_cmd_queue);
//	m_deferred_end_physics_cmd_queue = std::move(other_p.m_deferred_end_physics_cmd_queue);
//	m_deferred_post_physics_cmd_queue = std::move(other_p.m_deferred_post_physics_cmd_queue);
//	m_deferred_post_update_work_cmd_queue = std::move(other_p.m_deferred_post_update_work_cmd_queue);
//	m_deferred_pre_render_queue_commit_cmd_queue = std::move(other_p.m_deferred_pre_render_queue_commit_cmd_queue);
//	m_deferred_post_render_queue_commit_cmd_queue = std::move(other_p.m_deferred_post_render_queue_commit_cmd_queue);
//	m_deferred_world_end_cmd_queue = std::move(other_p.m_deferred_world_end_cmd_queue);
//	m_deferred_world_default_entity_deinitialization_cmd_queue = std::move(other_p.m_deferred_world_default_entity_deinitialization_cmd_queue);
//	m_deferred_world_deinitialization_cmd_queue = std::move(other_p.m_deferred_world_deinitialization_cmd_queue);
//	m_deferred_engine_initialization_cmd_queue = std::move(other_p.m_deferred_engine_initialization_cmd_queue);
//	m_deferred_game_instance_initialization_cmd_queue = std::move(other_p.m_deferred_game_instance_initialization_cmd_queue);
//	m_deferred_game_instance_begin_cmd_queue = std::move(other_p.m_deferred_game_instance_begin_cmd_queue);
//	m_deferred_pre_game_instance_tick_cmd_queue = std::move(other_p.m_deferred_pre_game_instance_tick_cmd_queue);
//	m_deferred_game_instance_tick_cmd_queue = std::move(other_p.m_deferred_game_instance_tick_cmd_queue);
//	m_deferred_post_game_instance_tick_cmd_queue = std::move(other_p.m_deferred_post_game_instance_tick_cmd_queue);
//	m_deferred_game_instance_end_cmd_queue = std::move(other_p.m_deferred_game_instance_end_cmd_queue);
//	m_deferred_game_instance_deinitialization_cmd_queue = std::move(other_p.m_deferred_game_instance_deinitialization_cmd_queue);
//	m_deferred_engine_deinitialization_cmd_queue = std::move(other_p.m_deferred_engine_deinitialization_cmd_queue);
//	return *this;
//}


void FE::world::set_delta_time(const FE::world::auth&, FE::float64 delta_time_p) noexcept
{
	m_delta_time = delta_time_p;
}

FE::world::registry& FE::world::get_registry(const auth&) noexcept
{
	return m_registry;
}


FE::entity FE::world::spawn_entity(const std::string_view& tag_p) noexcept
{
	if (tag_p.empty())
	{
		return FE::entity{ m_registry.create() };
	}

	auto l_entity_handle = m_registry.create();
	FE::entity l_entity{ l_entity_handle };

	auto l_result = m_tagged_entities.find(tag_p);
	if (l_result == m_tagged_entities.end())
	{
		m_tagged_entities.emplace(tag_p, l_entity);
		return l_entity;
	}

	m_tagged_entities[tag_p] = l_entity;
	return l_entity;
}

std::optional<FE::entity> FE::world::find_entity(const std::string_view& tag_p) const noexcept
{
	auto l_it = m_tagged_entities.find(tag_p);
	if ( (l_it != m_tagged_entities.end()) && (m_registry.valid(l_it->second) == true) )
	{
		return l_it->second;
	}

	return std::nullopt;
}

void FE::world::despawn_entity(FE::entity entity_p) noexcept
{
	auto l_entity_handle = entity_p;
	FE_EXIT_IF(is_valid(entity_p) == false, FE::ErrorCode::_FatalGameError_1XXX_AttemptingToDoubleDeleteEntity, "FE::world::despawn_entity(FE::entity entity_p) - Entity '${%u64@0}' is not valid in world '${%u16@1}'.", &l_entity_handle, &m_world_tag);
	m_registry.destroy(l_entity_handle);
}

FE::boolean FE::world::is_valid(FE::entity entity_p) const noexcept
{
	return m_registry.valid(entity_p);
}


void FE::world::create_world(world_tag_t world_tag_p) noexcept
{
	auto& l_world_list = FE::engine::get_engine().get_worlds(FE::engine::auth{});
	l_world_list.emplace(world_tag_p, FE::make_owner<FE::world>(FE::get_concurrent_memory_resource(), world_tag_p));
}

void FE::world::enter_world(world_tag_t world_tag_p) noexcept
{
	FE::engine::get_engine().set_current_world(FE::engine::auth{}, FE::engine::get_engine().get_worlds(FE::engine::auth{}).at(world_tag_p));
}

void FE::world::enqueue_world_command(system system_p, WorldSystemCallPhase phase_p) noexcept
{
	switch (phase_p)
	{
	case WorldSystemCallPhase::_WorldInitialization:
		m_deferred_world_initialization_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_WorldDefaultEntityInitialization:
		m_deferred_world_default_entity_initialization_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_WorldBegin:
		m_deferred_world_begin_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_PreWorldTick:
		m_deferred_pre_world_tick_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_WorldTick:
		m_deferred_world_tick_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_PostWorldTick:
		m_deferred_post_world_tick_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_PreEntityTick:
		m_deferred_pre_entity_tick_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_EntityTick:
		m_deferred_entity_tick_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_PostEntityTick:
		m_deferred_post_entity_tick_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_PrePhysics:
		m_deferred_pre_physics_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_StartPhysics:
		m_deferred_start_physics_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_Physics:
		m_deferred_physics_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_EndPhysics:
		m_deferred_end_physics_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_PostPhysics:
		m_deferred_post_physics_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_PostUpdateWork:
		m_deferred_post_update_work_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_PreRenderQueueCommit:
		m_deferred_pre_render_queue_commit_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_PostRenderQueueCommit:
		m_deferred_post_render_queue_commit_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_WorldEnd:
		m_deferred_world_end_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_WorldDefaultEntityDeinitialization:
		m_deferred_world_default_entity_deinitialization_cmd_queue.push(system_p);
		break;
	case WorldSystemCallPhase::_WorldDeinitialization:
		m_deferred_world_deinitialization_cmd_queue.push(system_p);
		break;
	}
}