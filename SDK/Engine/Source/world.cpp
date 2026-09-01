#include <FE/world.hxx>
#include <FE/engine.hpp>
#include <boost/hash2/xxhash.hpp>




FE::world::world(world_tag_t world_tag_p) noexcept
	: m_registry(std::pmr::polymorphic_allocator<entt::entity>{FE::get_large_thread_local_memory_resource()}),
	  m_world_tag(world_tag_p),
	  m_delta_time(),
	  m_deferred_ecs_syscall_queue(std::pmr::polymorphic_allocator<void(*)(class ::FE::world&)>{FE::get_large_thread_local_memory_resource()}),
	  m_tagged_entities(std::pmr::polymorphic_allocator<std::pair<const std::pmr::string, FE::entity>>{FE::get_large_thread_local_memory_resource()})
{
}

FE::world::~world() noexcept
{
}


FE::world::world(world&& other_p) noexcept
	: m_registry(std::move(other_p.m_registry)),
	  m_world_tag(std::exchange(other_p.m_world_tag, world_tag_t{})),
	  m_delta_time(std::exchange(other_p.m_delta_time, FE::float64{})),
	  m_deferred_ecs_syscall_queue(std::move(other_p.m_deferred_ecs_syscall_queue)),
	  m_tagged_entities(std::move(other_p.m_tagged_entities))
{
}

FE::world& FE::world::operator=(world&& other_p) noexcept
{
	m_registry = std::move(other_p.m_registry);
	m_world_tag = std::exchange(other_p.m_world_tag, world_tag_t{});
	m_delta_time = std::exchange(other_p.m_delta_time, FE::float64{});
	m_deferred_ecs_syscall_queue = std::move(other_p.m_deferred_ecs_syscall_queue);
	m_tagged_entities = std::move(other_p.m_tagged_entities);
	return *this;
}


void FE::world::set_delta_time(const FE::world::auth&, FE::float64 delta_time_p) noexcept
{
	m_delta_time = delta_time_p;
}

FE::world::registry& FE::world::get_registry(const auth&) noexcept
{
	return m_registry;
}


FE::entity FE::world::spawn_entity(FE::ASCII* const tag_p) noexcept
{
	if (tag_p == NULL)
	{
		return FE::entity{ m_registry.create() };
	}

	auto l_entity_handle = m_registry.create();
	FE::entity l_entity{ l_entity_handle };
	m_tagged_entities.emplace(tag_p, l_entity);
	return l_entity;
}

std::optional<FE::entity> FE::world::find_entity(FE::ASCII* const tag_p) const noexcept
{
	auto l_it = m_tagged_entities.find(tag_p);
	if (l_it != m_tagged_entities.end())
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
	l_world_list.emplace(world_tag_p, FE::make_owner<FE::world>(FE::engine::get_engine().get_large_memory_resource(), world_tag_p));
}

void FE::world::enter_world(world_tag_t world_tag_p) noexcept
{
	FE::engine::get_engine().set_current_world(FE::engine::auth{}, FE::engine::get_engine().get_worlds(FE::engine::auth{}).at(world_tag_p));
}