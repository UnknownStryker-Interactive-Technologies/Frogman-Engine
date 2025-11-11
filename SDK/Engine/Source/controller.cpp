#include <FE/controller.hpp>




FE::controller::controller(ControllerType device_type_p, framework::ECS& host_p) noexcept
	:	base_type(host_p),
		m_device_type(device_type_p),
		m_player()
{
	switch (device_type_p)
	{
	case ControllerType::_KeyboardAndMouse:
		add_component<input_device::keyboard>();
		add_component<input_device::mouse>();
		break;

	default:
		break;
	}
}

FE::controller::~controller() noexcept
{
	switch (m_device_type)
	{
	case ControllerType::_KeyboardAndMouse:
		destroy_component<input_device::keyboard>();
		destroy_component<input_device::mouse>();
		break;

	default:
		break;
	}
}
