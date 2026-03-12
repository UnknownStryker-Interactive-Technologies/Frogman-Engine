#include <FE/controller.hpp>
#include <FE/engine.hpp>




FE::controller::controller(framework::ECS& host_p, ControllerType device_type_p) noexcept
	:	base_type(host_p),
		m_device_type(device_type_p),
		m_player(),

		m_keyboard_view(),
		m_mouse_view()
{
	switch (device_type_p)
	{
	case ControllerType::_KeyboardAndMouse:
		m_keyboard_view = add_component<input_device::keyboard>();
		m_mouse_view = add_component<input_device::mouse>();
		break;

	default:
		break;
	}
}

FE::controller::~controller() noexcept
{}