#include <FE/engine.hpp>

#include <FE/framework/ECS.hpp>
#include <FE/framework/processors.hpp>
#include <FE/framework/reflection.hpp>

#include <FE/pool/memory_resource.hpp>




FE::engine::engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept 
	:	FE::framework::framework_base(argc_p, argv_p), 
		m_gc_batch_count(100),
		m_fiber_stack_size(FE::one_MiB),
		m_renderer_system(),
		m_physics_system(),
		m_audio_system(),
		m_networking_system(),
		m_entity_list(),
		m_system_list()
{
}

FE::engine::~engine() noexcept
{
}

FE::int32 FE::engine::launch(FE::int32 argc_p, FE::ASCII** argv_p)
{
	(argc_p);
	(argv_p);
	__load_reflection_data();
	m_game_memory = std::make_unique<FE::memory_resource>();

	// Read the .froggy file and deserialize the m_entity_list and the m_system_list.
	m_ecs = std::make_unique<framework::ECS>(m_entity_list, m_system_list, m_game_memory.get());

	// Read the .froggy file and update the m_fiber_stack_size and m_gc_batch_count if specified in the file.
	m_processors = std::make_unique<framework::processors>(*m_ecs, m_program_options.get_max_concurrency(), m_gc_batch_count, m_fiber_stack_size);

	// Read the .froggy file and load the function pointer to the m_renderer_system, m_physics_system, m_audio_system, and m_networking_system using the reflection system.

	if (m_renderer_system == nullptr)
	{
		return -1; // Renderer system is mandatory.
	}
	if (m_physics_system == nullptr)
	{
		return -1; // Physics system is mandatory.
	}
	if (m_audio_system == nullptr)
	{
		return -1; // Audio system is mandatory.
	}
	if (m_networking_system == nullptr)
	{
		return -1; // Networking system is mandatory.
	}
	return 0;
}

FE::int32 FE::engine::run()
{
	m_processors->fork(m_renderer_system, m_physics_system, m_audio_system, m_networking_system);
	return 0;
}

FE::int32 FE::engine::shutdown()
{
	m_processors.reset();
	m_ecs.reset();
	m_game_memory.reset();
	return 0;
}
