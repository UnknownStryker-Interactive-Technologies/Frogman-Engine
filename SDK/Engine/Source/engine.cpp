#include <FE/engine.hpp>

#include <FE/framework/ECS.hpp>
#include <FE/framework/processors.hpp>
#include <FE/framework/reflection.hpp>

#include <FE/pool/memory_resource.hpp>




void nullsys(FE::component_base*) noexcept {}




FE::engine::engine(FE::int32 argc_p, FE::ASCII** argv_p) noexcept 
	:	FE::framework::framework_base(argc_p, argv_p), 
		m_gc_batch_count(100),
		m_fiber_stack_size(FE::one_MiB),
		m_entity_list(),
		m_system_list(),
		m_window_config(),
		m_renderer(),
		m_this_pointer(this)
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

	// Read the .froggy file and deserialize the m_entity_list and the m_system_list.
	m_ecs = std::make_unique<framework::ECS>();
	m_ecs->initialize(m_entity_list, m_system_list);

	// Read the .froggy file and update the m_fiber_stack_size and m_gc_batch_count if specified in the file.
	m_processors = std::make_unique<framework::processors>(*m_ecs, m_program_options.get_max_concurrency(), m_gc_batch_count, m_fiber_stack_size);

	m_window_config = std::make_unique<window_config>(); // the .froggy file for config
	m_renderer = std::make_unique<FE::renderer>(*m_window_config);

	glfwSetWindowCloseCallback(m_renderer->get_window(), &FE::engine::__on_window_close);
	glfwSetKeyCallback(m_renderer->get_window(), &FE::engine::__key_callback);
	glfwSetMouseButtonCallback(m_renderer->get_window(), &FE::engine::__mouse_button_callback);
	glfwSetCursorPosCallback(m_renderer->get_window(), &FE::engine::__cursor_position_callback);
	return 0;
}

FE::int32 FE::engine::run()
{
	m_processors->fork(	__renderer_main, &m_this_pointer, 
						nullsys, nullptr,
						nullsys, nullptr, 
						nullsys, nullptr);
	return 0;
}

FE::int32 FE::engine::shutdown()
{
	m_processors->join();
	return 0;
}

void FE::engine::__renderer_main(FE::component_base* engine_reference_p) noexcept
{
	FE::engine::reference* const l_engine_reference = FE::polymorphic_cast<FE::engine::reference* const>(engine_reference_p);

	while (l_engine_reference->_engine->m_processors->is_running() == true)
	{
		l_engine_reference->_engine->m_renderer->render_frame();
	}
}

void FE::engine::__on_window_close(GLFWwindow* window_p) noexcept
{
	FE::engine::__get_engine().m_processors->shutdown();
	glfwSetWindowShouldClose(window_p, GLFW_TRUE);
}

void FE::engine::__key_callback(GLFWwindow* const window_p, FE::int32 key_p, FE::int32 scancode_p, FE::int32 action_p, FE::int32 mods_p) noexcept
{
	(window_p);
	(key_p);
	(scancode_p);
	(action_p);
	(mods_p);
}

void FE::engine::__mouse_button_callback(GLFWwindow* const window_p, FE::int32 button_p, FE::int32 action_p, FE::int32 mods_p) noexcept
{
	(window_p);
	(button_p);
	(action_p);
	(mods_p);
}

void FE::engine::__cursor_position_callback(GLFWwindow* const window_p, double x_p, double y_p) noexcept
{
#ifdef _DEBUG_
	std::cout << "x: " << x_p << "; " << "y: " << y_p << ".\n";
#endif
	(window_p);
	(x_p);
	(y_p);
}
