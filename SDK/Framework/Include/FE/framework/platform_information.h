#ifndef _FE_FRAMEWORK_PLATFORM_INFORMATION_H_
#define _FE_FRAMEWORK_PLATFORM_INFORMATION_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>




BEGIN_NAMESPACE(FE::framework)

struct platform_information
{

};

void get_current_platform_information(platform_information& out_platform_info_p) noexcept;

END_NAMESPACE
#endif