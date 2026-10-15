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
#include <cstdint>
#include <type_traits>
#include <FE/type_traits.hxx>




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
#define FHT_GENERATED_MACRO_IDENTIFIER(FilePathHash, LineNumber) _##FilePathHash##_##LineNumber##_
#define FHT_GENERATED_IMPL(FilePathHash, LineNumber) FHT_GENERATED_MACRO_IDENTIFIER(FilePathHash, LineNumber)
#define FHT_GENERATED() FHT_GENERATED_IMPL(_FE_REFLECTION_FILE_PATH_HASH_GENERATED_BY_FHT_, __LINE__)
#endif




using world_tag_t = uint16_t;

#ifdef FE_WORLD_TAG
	#error FE_WORLDS is a reserved Frogman Engine macro keyword.
#else
	#define FE_WORLD_TAG \
	enum FEWorldTag : world_tag_t
#endif

#ifdef FE_SYSTEM
	#error FE_SYSTEM is a reserved Frogman Engine macro keyword.
#else
	// This macro is an indicator for the Frogman Engine Header Tool.
	// Incorrectly placing the arguments will fail the generated code compilation.
	#define FE_SYSTEM(SysCallPhase, WorldTagEnum) \
	static_assert(::std::is_same_v<::std::remove_cv_t<decltype(SysCallPhase)>, ::FE::WorldSystemCallPhase>, "Frogman Engine C++: an incorrectly typed value passed to the SysCallPhase parameter."); \
	static_assert(static_cast<::std::underlying_type_t<::std::remove_cv_t<decltype(WorldTagEnum)>>>(WorldTagEnum) <= static_cast<::std::underlying_type_t<::std::remove_cv_t<decltype(WorldTagEnum)>>>(::FE::max_value<::FE::uint16>), "Frogman Engine C++: the WorldTagEnum value exceeds 2^16.");
#endif

#ifdef FE_GLOBAL_SYSTEM
	#error FE_GLOBAL_SYSTEM is a reserved Frogman Engine macro keyword.
#else
	// This macro is an indicator for the Frogman Engine Header Tool.
	// Incorrectly placing the arguments will fail the generated code compilation.
#define FE_GLOBAL_SYSTEM(SysCallPhase) \
	static_assert(::std::is_same_v<::std::remove_cv_t<decltype(SysCallPhase)>, ::FE::GlobalSystemCallPhase>, "Frogman Engine C++: an incorrectly typed value passed to the SysCallPhase parameter.");
#endif


CLASS_FORWARD_DECLARATION(FE, world);


#ifdef RUNS_BEFORE
	#error RUNS_BEFORE is a reserved Frogman Engine macro keyword.
#else
	// This macro is an indicator for the Frogman Engine Header Tool.
	#define RUNS_BEFORE(x)
#endif

#ifdef IS_RUNS_BEFORE_VALID
	#error IS_RUNS_BEFORE_VALID is a reserved Frogman Engine macro keyword.
#else
	#define IS_RUNS_BEFORE_VALID(x) \
	static_assert(::std::is_function_v<decltype(x)>, "Frogman Engine C++: an incorrectly typed value passed to the RUNS_BEFORE macro."); \
	static_assert(::std::is_same_v<::FE::function_argument_t<decltype(x), 0>, ::FE::world&>, "Frogman Engine C++: an incorrectly typed value passed to the RUNS_BEFORE macro."); \
	static_assert(::FE::function_arity_v<decltype(x)> == 1, "Frogman Engine C++: an incorrectly typed value passed to the RUNS_BEFORE macro."); \
	static_assert(::std::is_same_v<::FE::function_return_t<decltype(x)>, void>, "Frogman Engine C++: an incorrectly typed value passed to the RUNS_BEFORE macro.");
#endif


#ifdef RUNS_AFTER
	#error RUNS_AFTER is a reserved Frogman Engine macro keyword.
#else
	// This macro is an indicator for the Frogman Engine Header Tool.
	#define RUNS_AFTER(x)
#endif

#ifdef IS_RUNS_AFTER_VALID
	#error IS_RUNS_AFTER_VALID is a reserved Frogman Engine macro keyword.
#else
	#define IS_RUNS_AFTER_VALID(x) \
	static_assert(::std::is_function_v<decltype(x)>, "Frogman Engine C++: an incorrectly typed value passed to the RUNS_AFTER macro."); \
	static_assert(::std::is_same_v<::FE::function_argument_t<decltype(x), 0>, ::FE::world&>, "Frogman Engine C++: an incorrectly typed value passed to the RUNS_AFTER macro."); \
	static_assert(::FE::function_arity_v<decltype(x)> == 1, "Frogman Engine C++: an incorrectly typed value passed to the RUNS_AFTER macro."); \
	static_assert(::std::is_same_v<::FE::function_return_t<decltype(x)>, void>, "Frogman Engine C++: an incorrectly typed value passed to the RUNS_AFTER macro.");
#endif




namespace FE
{
	// The numerical values of this enum must not be changed, as they are used for indexing arrays; the engine make system calls in the order of the values! Do not change if unnecessary.
	enum WorldSystemCallPhase : uint16_t
	{
		_WorldInitialization = 0,
		_WorldDefaultEntityInitialization = 1,


		_WorldBegin = 2,


		_PreWorldTick = 3,
		_WorldTick = 4,
		_PostWorldTick = 5,


		_PreEntityTick = 6,
		_EntityTick = 7,
		_PostEntityTick = 8,


		_PrePhysics = 9,
		_StartPhysics = 10,
		_Physics = 11,
		_EndPhysics = 12,
		_PostPhysics = 13,


		_PostUpdateWork = 14,


		_PreRenderQueueCommit = 15,
		_PostRenderQueueCommit = 16,


		_WorldEnd = 17,


		_WorldDefaultEntityDeinitialization = 18,
		_WorldDeinitialization = 19,
	};
	constexpr static uint64_t world_syscall_phase_count = 20;

	// The numerical values of this enum must not be changed, as they are used for indexing arrays; the engine make system calls in the order of the values! Do not change if unnecessary.
	enum GlobalSystemCallPhase : uint16_t
	{
		_EngineInitialization = 0,
		_GameInstanceInitialization = 1,


		_GameInstanceBegin = 2,


		_PreGameInstanceTick = 3,
		_GameInstanceTick = 4,
		_PostGameInstanceTick = 5,


		_GameInstanceEnd = 6,


		_GameInstanceDeinitialization = 7,
		_EngineDeinitialization = 8
	};
	constexpr static uint64_t global_syscall_phase_count = 9;
}

#endif