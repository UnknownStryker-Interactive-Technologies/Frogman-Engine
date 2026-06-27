# Copyright © from 2023 to current, UNKNOWN STRYKER (Hojin Lee / Joey). All Rights Reserved.


IF(NOT DEFINED WWISE_SDK_PATH)
	SET(WWISE_SDK_PATH C:\\Audiokinetic\\Wwise2024.1.9.8920\\SDK)
ENDIF()

IF(EXISTS "${WWISE_SDK_PATH}/include")
	MESSAGE(STATUS "Found the Wwise SDK in ${WWISE_SDK_PATH}")
	INCLUDE_DIRECTORIES("${WWISE_SDK_PATH}/include")

	IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")
		
	FILE(GLOB_RECURSE WwiseSDK_DEBUG       "${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/*.lib")
	FILE(GLOB_RECURSE WwiseSDK_PROFILE     "${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/*.lib")
	FILE(GLOB_RECURSE WwiseSDK_RELEASE     "${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/*.lib")
	FILE(GLOB_RECURSE WwiseSDK_MINSIZEREL  "${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/*.lib")

	SET(WwiseSDK
		$<$<CONFIG:DEBUG>: WwiseSDK_DEBUG>
    	$<$<CONFIG:RELWITHDEBINFO>: WwiseSDK_PROFILE>
    	$<$<CONFIG:RELEASE>: WwiseSDK_RELEASE>
    	$<$<CONFIG:MINSIZEREL>: WwiseSDK_MINSIZEREL>
	)

	ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Android" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64")

	ELSE()
    	MESSAGE(FATAL_ERROR "System not selected.")

	ENDIF()

	
ELSE()
	MESSAGE(WARNING "Without the Wwise SDK, Frogman Engine games will not play any sound effects.")
	MESSAGE(NOTICE "Download the Wwise SDK installer from https://www.audiokinetic.com/en/download. Please check out the Wwise license before downloading.")
	MESSAGE(NOTICE "Available CMake options for the Wwise SDK integration:
	-DWWISE_SDK_PATH=<path>               Specify the path to the Wwise SDK installation directory (default: C:/Audiokinetic/Wwise2024.1.9.8920/SDK)")

ENDIF()

SET(WWISE_SDK
	${WwiseSDK}
)
