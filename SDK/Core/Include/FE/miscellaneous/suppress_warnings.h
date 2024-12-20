#ifndef _FE_SUPPRESS_WARNINGS_H_
#define _FE_SUPPRESS_WARNINGS_H_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#if defined(_CLANG_)
#pragma clang diagnostic ignored "-Wambiguous-reversed-operator" // Silence, there is no obscurity in the FE.core.iterator comparison operators definition.
#elif defined(_V143_)
#pragma warning (disable: 4702) // 1>C:\Users\leeho\OneDrive\문서\GitHub\Frogman-Engine\SDK\Core\Source\format_string.cpp(213,1): warning C4702: unreachable code
#endif

#endif