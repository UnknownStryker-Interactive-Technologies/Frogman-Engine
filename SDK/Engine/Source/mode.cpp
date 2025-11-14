#include <FE/mode.hpp>
#include <FE/memory.hpp>




FE::mode::mode(framework::ECS& host_p, ControllerType type_p) noexcept
	:	base_type(host_p),
		m_controller(host_p, type_p)
{}

FE::mode::~mode() noexcept
{}