#ifndef _FE_HEADER_TOOL_PREPROCESSOR_PURGE_TEST_HPP_
#define _FE_HEADER_TOOL_PREPROCESSOR_PURGE_TEST_HPP_
/*
Copyright © from 2024 to present, UNKNOWN STRYKER. All Rights Reserved.

Licensed under the Frogman Engine Apache License (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	https://github.com/UnknownStryker-Interactive-Technologies/Frogman-Engine-License/blob/release/LICENSE.md

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License */ #include <string>
#include <string_view>

// Deeply nested defines test cases (not assigned to constexpr)
#ifdef LEVEL_1
	#ifdef LEVEL_2
		#ifdef LEVEL_3
			#define NESTED_VALUE_A 100
			#define NESTED_VALUE_B 200
		#endif
	#endif
#endif

#ifdef PLATFORM_WINDOWS
	#ifdef ARCHITECTURE_X64
		#ifdef COMPILER_MSVC
			#define PLATFORM_SPECIFIC_VALUE 1
			#ifdef VERSION_2022
				#define COMPILER_VERSION 2022
				#ifdef FEATURE_CPP20
					#define SUPPORTS_CONCEPTS 1
					#define SUPPORTS_MODULES 1
				#endif
			#endif
		#endif
	#endif
#endif

#ifdef GRAPHICS_API
	#ifdef API_DIRECTX
		#define DX_VERSION 12
		#ifdef FEATURE_RAYTRACING
			#define RT_TIER 1_1
			#ifdef HARDWARE_ACCELERATED
				#define USE_HW_RT 1
				#ifdef SHADER_MODEL_6_6
					#define ADVANCED_RT_FEATURES 1
					#define MESH_SHADERS_ENABLED 1
					#ifdef PERFORMANCE_MODE
						#define INLINE_RAYTRACING 1
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

#ifdef BUILD_CONFIG
	#ifdef CONFIG_DEBUG
		#define DEBUG_LEVEL 3
		#define LOGGING_ENABLED 1
		#ifdef VERBOSE_LOGGING
			#define LOG_LEVEL_TRACE 1
			#ifdef MEMORY_TRACKING
				#define TRACK_ALLOCATIONS 1
				#define LEAK_DETECTION 1
				#ifdef CALLSTACK_CAPTURE
					#define CAPTURE_DEPTH 16
					#define SYMBOL_RESOLUTION 1
				#endif
			#endif
		#endif
	#endif
#endif

#ifdef FEATURE_SET_A
	void feature_a_init();
	#ifdef FEATURE_SET_B
		void feature_b_init();
		#ifdef FEATURE_SET_C
			#define FEATURE_C_VERSION 3
			void feature_c_init();
			#ifdef FEATURE_SET_D
				#define FEATURE_D_VERSION 4
				void feature_d_init();
				#ifdef FEATURE_SET_E
					#define FEATURE_E_VERSION 5
					void feature_e_init();
					#ifdef FEATURE_SET_F
						#define FEATURE_F_VERSION 6
						void feature_f_init();
						#ifdef FEATURE_SET_G
							#define FEATURE_G_VERSION 7
							void feature_g_init();
						#endif
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

#ifdef ABC
	#ifdef DEF
		#define GHI 456
		#define JKL 567 \
		345
	#endif
#endif

#endif // _FE_HEADER_TOOL_PREPROCESSOR_PURGE_TEST_HPP_