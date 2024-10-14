#ifndef _FE_FRAMEWORK_INPUT_MANAGER_HPP_
#define _FE_FRAMEWORK_INPUT_MANAGER_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>




BEGIN_NAMESPACE(FE::framework)

class input_manager
{

public:
	void run() noexcept;
	void shutdown() noexcept;
};

END_NAMESPACE
#endif