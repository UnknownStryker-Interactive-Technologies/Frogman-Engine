#ifndef _FE_SUPPRESS_WARNINGS_H_
#define _FE_SUPPRESS_WARNINGS_H_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#if defined(_CLANG_)
#pragma clang diagnostic ignored "-Wambiguous-reversed-operator" // Silence, there is no obscurity in the FE.core.iterator comparison operators definition.
#elif defined(_V143_)
#pragma warning (disable: 4702) // 1>C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Core\Source\format_string.cpp(213,1): warning C4702: unreachable code
#endif

#endif