#ifndef _FE_LOAD_ALL_CLASS_REFLECTION_DATA_FROM_DLL_H_
#define _FE_LOAD_ALL_CLASS_REFLECTION_DATA_FROM_DLL_H_
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

#ifdef _FE_DLL_EXPORT_
#error _FE_DLL_EXPORT_ is a reserved Frogman Engine macro.
#else
#define _FE_DLL_EXPORT_ __declspec(dllexport)
#endif

#ifdef _FE_DLL_IMPORT_
    #error _FE_DLL_IMPORT_ is a reserved Frogman Engine macro.
#else
    #define _FE_DLL_IMPORT_ __declspec(dllimport)
#endif


extern "C" 
{
#ifdef _IMPORT_LOAD_ALL_CLASS_REFLECTION_DATA_FROM_DLL_
    _FE_DLL_IMPORT_ void load_all_class_reflection_data_from_dll();
#else
    _FE_DLL_EXPORT_ void load_all_class_reflection_data_from_dll();
#endif
}

#endif