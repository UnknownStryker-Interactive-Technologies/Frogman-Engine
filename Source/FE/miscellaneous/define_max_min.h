#ifndef _FE_DEFINE_MAX_MIN_H_
#define _FE_DEFINE_MAX_MIN_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/macros/macro_definitions.h>

#ifdef _MSVC_
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#endif

#endif