#ifndef _FE_FRAMEWORK_OBJECT_BASE_HPP_
#define _FE_FRAMEWORK_OBJECT_BASE_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>




BEGIN_NAMESPACE(FE::framework)

class object_base
{
public:
	object_base() noexcept = default;
	virtual ~object_base() = 0;

protected:
	var::boolean m_is_active = false;
	var::boolean m_is_tick_enabled = false;

	virtual void on_activate() = 0;
	virtual void tick(FE::float64 delta_p) = 0;
	virtual void on_inactivate() = 0;
};

END_NAMESPACE
#endif