#include <FE/world.hxx>
#include <FE/engine.hpp>
#include <boost/hash2/xxhash.hpp>




FE::world::world(world_tag_t world_tag_p) noexcept
	: m_registry(std::pmr::polymorphic_allocator<entt::entity>{FE::get_large_thread_local_memory_resource()}),
	  m_world_tag(world_tag_p),
	  m_deferred_ecs_syscall_queue(std::pmr::polymorphic_allocator<void(*)(class ::FE::world&)>{FE::get_large_thread_local_memory_resource()})
{
}

FE::world::~world() noexcept
{
}


void FE::world::__set_delta_time(const FE::world::auth&, FE::float64 delta_time_p) noexcept
{
	m_delta_time = delta_time_p;
}

FE::world::registry& FE::world::__get_registry(const auth&) noexcept
{
	return m_registry;
}


FE::entity FE::world::spawn_entity()
{
	return FE::entity();
}

void FE::world::despawn_entity(FE::entity entity_p)
{
}

bool FE::world::is_valid(FE::entity entity_p) const noexcept
{
	return false;
}


world_tag_t FE::world::get_world_tag() const noexcept
{
	return m_world_tag;
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