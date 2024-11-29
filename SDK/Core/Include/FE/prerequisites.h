#ifndef _FE_CORE_PREREQUISITES_H_
#define _FE_CORE_PREREQUISITES_H_
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
#include <FE/types.hxx>
#include <FE/log/log.h>

#ifdef FE_NEGATIVE_STATIC_ASSERT
    #error FE_NEGATIVE_STATIC_ASSERT is a reserved Frogman Engine macro keyword.
#endif 

// statically asserts the condition, that must not be true at the compile time.
#define FE_NEGATIVE_STATIC_ASSERT(expression, message) static_assert(!(expression), message)


#ifdef FE_STATIC_ASSERT
    #error FE_STATIC_ASSERT is a reserved Frogman Engine macro keyword.
#endif 

// statically asserts the condition, that must be true at compile time.
#define FE_STATIC_ASSERT(expression, message) static_assert((expression), message)


#endif