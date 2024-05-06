#ifndef _FE_CONFIGURATION_H_
#define _FE_CONFIGURATION_H_
#pragma warning(disable: 4530)
#pragma warning(disable: 4099)
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
// To enable or disable a certain feature, remove the macro.
// 
// Modifying the macro will not immediately affect your project.
// The binary files e.g. (.lib, .a, .dll, or .so) need to be rebuilt to apply changes that have been made.


constexpr inline auto allowed_directory_string_length = 512;


#ifdef _LEAVE_THIRD_PARTY_EXCEPTIONS_OUT_

	#ifdef _HAS_EXCEPTIONS
		#undef _HAS_EXCEPTIONS
		#define _HAS_EXCEPTIONS 0
	#endif

	#ifdef TBB_USE_EXCEPTIONS
		#undef TBB_USE_EXCEPTIONS
		#define TBB_USE_EXCEPTIONS 0
	#endif

	#ifndef BOOST_NO_EXCEPTIONS
		#define BOOST_NO_EXCEPTIONS
	#endif

#endif

#endif 