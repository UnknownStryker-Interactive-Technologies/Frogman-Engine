#ifndef _FE_CORE_MACROS_ATTRIBUTES_H_
#define _FE_CORE_MACROS_ATTRIBUTES_H_
#include <FE/core/macros/macro_definitions.h>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.


#define _NODISCARD_ [[nodiscard]]
#define _FALLTHROUGH_ [[fallthrough]]
#define _MAYBE_UNUSED_ [[maybe_unused]]
#define _DISCARD_ _MAYBE_UNUSED_
#define _NORETURN_ [[noreturn]]
#define _DEPRECATED_ [[deprecated]]
#define _IN_DEVELOPMENT_ _DEPRECATED_


#ifdef _HAS_CXX20_
	#define _LIKELY_ [[likely]]
	#define _UNLIKELY_ [[unlikely]]
		#ifdef _MSVC_
			#define _NO_UNIQUE_ADDRESS_ [[msvc::no_unique_address]]
		#else
			#define _NO_UNIQUE_ADDRESS_ [[no_unique_address]]
		#endif
#else
#define _LIKELY_
#define _UNLIKELY_
#define _NO_UNIQUE_ADDRESS_
#endif


#endif 