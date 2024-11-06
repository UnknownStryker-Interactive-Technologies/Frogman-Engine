#ifndef _FE_FRAMEWORK_PLATFORM_INFO_H_
#define _FE_FRAMEWORK_PLATFORM_INFO_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>




BEGIN_NAMESPACE(FE::framework)

// https://github.com/lfreist/hwinfo
struct platform_info
{

};

void get_current_platform_information(platform_info& out_platform_info_p) noexcept;

END_NAMESPACE
#endif