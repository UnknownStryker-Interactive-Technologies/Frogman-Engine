#ifndef _FE_CORE_PREREQUISITES_H_
#define _FE_CORE_PREREQUISITES_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/types.hxx>
#include <FE/log/log.h>

#ifdef FE_STATIC_ASSERT
#error FE_STATIC_ASSERT is a reserved Frogman Engine macro keyword.
#endif 
// statically asserts the condition, that must not be true at compile time.
#define FE_STATIC_ASSERT(expression, message) static_assert(!(expression), message)

#endif