#ifndef _FE_CORE_REFLECTION_DEFINES_HXX_
#define _FE_CORE_REFLECTION_DEFINES_HXX_
/*
Copyright © from 2022 to present, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.

Licensed under the Frogman Engine License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#ifdef FE_CLASS
#error FE_CLASS is a reserved Frogman Engine macro keyword.
#else
#define FE_CLASS() // THis is an indicator for the FHT.
#endif

#ifdef FE_STRUCT
#error FE_STRUCT is a reserved Frogman Engine macro keyword.
#else
#define FE_STRUCT() // THis is an indicator for the FHT.
#endif

#ifdef FE_ENUM_STRUCT
#error FE_ENUM_STRUCT is a reserved Frogman Engine macro keyword.
#else
#define FE_ENUM_STRUCT() // THis is an indicator for the FHT.
#endif




#ifdef FE_CLASS_HAS_A_BASE
#error FE_CLASS_HAS_A_BASE is a reserved Frogman Engine macro keyword.
#else
#define FE_CLASS_HAS_A_BASE(base_class) \
    using base_type = base_class; 
#endif

#ifdef FE_ENABLE_SERIALIZATION
#error FE_ENABLE_SERIALIZATION is a reserved Frogman Engine macro keyword.
#else
#define FE_ENABLE_SERIALIZATION() using IsSerializable = decltype(true); // This is an indicator for the FE runtime reflection.
#endif


#ifdef FHT_GENERATED
#error FHT_GENERATED is a reserved Frogman Engine macro keyword.
#else
#define FHT_GENERATED_MACRO_IDENTIFIER(FilePathHash, LineNumber, TypeIdentifierHash) _##FilePathHash##_##LineNumber##_##TypeIdentifierHash##_
#define FHT_GENERATED_IMPL(FilePathHash, LineNumber, TypeIdentifierHash) FHT_GENERATED_MACRO_IDENTIFIER(FilePathHash, LineNumber, TypeIdentifierHash)
#define FHT_GENERATED() FHT_GENERATED_IMPL(_FE_REFLECTION_FILE_PATH_HASH_GENERATED_BY_FHT_, __LINE__, _FE_REFLECTION_TYPE_IDENTIFIER_HASH_GENERATED_BY_FHT_)
#endif

//FHT_GENERATED();
#endif