#include <FE/framework/processors.hpp>

#include <boost/fiber/all.hpp>




BEGIN_NAMESPACE(FE::framework)


processor::processor() noexcept
	:	m_processor(),
		m_fibers()
{
}

processor::~processor() noexcept
{
	if (m_processor.joinable())
	{
		m_processor.join();
	}
}

void processor::run(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
{
	m_processor = std::thread(__launch_fibers, argc_p, argv_p);
}

void processor::__launch_fibers(FE::int32 argc_p, FE::ASCII** argv_p) noexcept
{
}




processors::processors(FE::int32 argc_p, FE::ASCII** argv_p, FE::uint32 software_thread_count_p) noexcept
	:	m_argc(argc_p),
		m_argv(argv_p),
		m_software_thread_count(software_thread_count_p),
		m_fiber_count(software_thread_count_p*2),

		m_processors(std::make_unique<processor[]>(software_thread_count_p)),
		m_renderer_thread(),
		m_game_fibers()
{
	FE_ASSERT(software_thread_count_p >= 4, "Assertion failure: the software thread count must be greater than or equal to 4");
}

processors::~processors() noexcept
{
	for(boost::fibers::fiber& fiber : m_game_fibers)
	{
		if (fiber.joinable())
		{
			fiber.join();
		}
	}

	if (m_renderer_thread.joinable())
	{
		m_renderer_thread.join();
	}
}

void processors::run() noexcept
{
	boost::fibers::use_scheduling_algorithm<boost::fibers::algo::round_robin>();

	for (var::int32 i = 0; i < m_software_thread_count; ++i)
	{
		m_processors[i].run(m_argc, m_argv);
	}
}


END_NAMESPACE