#ifndef _FE_FRAMEWORK_VULKAN_RENDERER_HPP_
#define _FE_FRAMEWORK_VULKAN_RENDERER_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>




BEGIN_NAMESPACE(FE::framework)

class vulkan_renderer
{

public:
	void run() noexcept;
	void shutdown() noexcept;
};

END_NAMESPACE
#endif