# Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.


IF(NOT DEFINED WWISE_SDK_PATH)
	SET(WWISE_SDK_PATH C:\\Audiokinetic\\Wwise2024.1.9.8920\\SDK)
ENDIF()

IF(EXISTS "${WWISE_SDK_PATH}/include")
	MESSAGE(STATUS "Found the Wwise SDK in ${WWISE_SDK_PATH}")
	INCLUDE_DIRECTORIES("${WWISE_SDK_PATH}/include")

	IF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")
    	SET(Ak3DAudioBedMixerFX
        	$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/Ak3DAudioBedMixerFX.lib>
        	$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/Ak3DAudioBedMixerFX.lib>
        	$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/Ak3DAudioBedMixerFX.lib>
        	$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/Ak3DAudioBedMixerFX.lib>
		)

		SET(AkAudioInputSource
        	$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkAudioInputSource.lib>
        	$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkAudioInputSource.lib>
        	$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkAudioInputSource.lib>
        	$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkAudioInputSource.lib>
		)

		SET(AkAutobahn
        	$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkAutobahn.lib>
        	$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkAutobahn.lib>
        	$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkAutobahn.lib>
        	$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkAutobahn.lib>
		)

		SET(AkCompressorFX
        	$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkCompressorFX.lib>
        	$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkCompressorFX.lib>
        	$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkCompressorFX.lib>
        	$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkCompressorFX.lib>
		)

		SET(AkDelayFX
        	$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkDelayFX.lib>
        	$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkDelayFX.lib>
        	$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkDelayFX.lib>
        	$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkDelayFX.lib>
		)

		SET(AkExpanderFX
        	$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkExpanderFX.lib>
        	$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkExpanderFX.lib>
        	$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkExpanderFX.lib>
        	$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkExpanderFX.lib>
		)

		SET(AkFlangerFX
        	$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkFlangerFX.lib>
        	$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkFlangerFX.lib>
        	$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkFlangerFX.lib>
        	$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkFlangerFX.lib>
		)

		SET(AkGainFX
        	$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkGainFX.lib>
        	$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkGainFX.lib>
        	$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkGainFX.lib>
        	$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkGainFX.lib>
		)

		SET(AkGuitarDistortionFX
        	$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkGuitarDistortionFX.lib>
        	$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkGuitarDistortionFX.lib>
        	$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkGuitarDistortionFX.lib>
        	$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkGuitarDistortionFX.lib>
		)

		SET(AkHarmonizerFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkHarmonizerFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkHarmonizerFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkHarmonizerFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkHarmonizerFX.lib>
		)

		SET(AkMatrixReverbFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkMatrixReverbFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkMatrixReverbFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkMatrixReverbFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkMatrixReverbFX.lib>
		)

		SET(AkMemoryMgr
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkMemoryMgr.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkMemoryMgr.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkMemoryMgr.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkMemoryMgr.lib>
		)
		
		SET(AkMeterFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkMeterFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkMeterFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkMeterFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkMeterFX.lib>
		)

		SET(AkMusicEngine
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkMusicEngine.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkMusicEngine.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkMusicEngine.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkMusicEngine.lib>
		)

		SET(AkOpusDecoder
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkOpusDecoder.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkOpusDecoder.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkOpusDecoder.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkOpusDecoder.lib>
		)

		SET(AkParametricEQFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkParametricEQFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkParametricEQFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkParametricEQFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkParametricEQFX.lib>
		)

		SET(AkPeakLimiterFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkPeakLimiterFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkPeakLimiterFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkPeakLimiterFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkPeakLimiterFX.lib>
		)	

		SET(AkPitchShifterFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkPitchShifterFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkPitchShifterFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkPitchShifterFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkPitchShifterFX.lib>
		)

		SET(AkRecorderFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkRecorderFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkRecorderFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkRecorderFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkRecorderFX.lib>
		)

		SET(AkReflectFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkReflectFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkReflectFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkReflectFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkReflectFX.lib>
		)

		SET(AkRoomVerbFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkRoomVerbFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkRoomVerbFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkRoomVerbFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkRoomVerbFX.lib>
		)

		SET(AkSilenceSource
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkSilenceSource.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkSilenceSource.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSilenceSource.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSilenceSource.lib>
		)

		SET(AkSineSource
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkSineSource.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkSineSource.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSineSource.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSineSource.lib>
		)

		SET(AkSoundEngine
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkSoundEngine.lib>	
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkSoundEngine.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSoundEngine.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSoundEngine.lib>
		)

		SET(AkSoundSeedWindSource
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkSoundSeedWindSource.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkSoundSeedWindSource.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSoundSeedWindSource.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSoundSeedWindSource.lib>
		)

		SET(AkSoundSeedWooshSource
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkSoundSeedWooshSource.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkSoundSeedWooshSource.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSoundSeedWooshSource.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSoundSeedWooshSource.lib>
		)

		SET(AkSpatialAudio
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkSpatialAudio.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkSpatialAudio.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSpatialAudio.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSpatialAudio.lib>
		)
		
		SET(AkStereoDelayFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkStereoDelayFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkStereoDelayFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkStereoDelayFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkStereoDelayFX.lib>
		)

		SET(AkStreamMgr
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkStreamMgr.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkStreamMgr.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkStreamMgr.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkStreamMgr.lib>
		)

		SET(AkSynthOneSource
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkSynthOneSource.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkSynthOneSource.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSynthOneSource.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkSynthOneSource.lib>
		)

		SET(AkTimeStretchFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkTimeStretchFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkTimeStretchFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkTimeStretchFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkTimeStretchFX.lib>
		)

		SET(AkToneSource
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkToneSource.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkToneSource.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkToneSource.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkToneSource.lib>
		)

		SET(AkTremoloFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkTremoloFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkTremoloFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkTremoloFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkTremoloFX.lib>
		)

		SET(AkVorbisDecoder
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AkVorbisDecoder.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AkVorbisDecoder.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkVorbisDecoder.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AkVorbisDecoder.lib>
		)

		SET(AuroHeadphoneFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/AuroHeadphoneFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/AuroHeadphoneFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AuroHeadphoneFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/AuroHeadphoneFX.lib>
		)

		SET(iZHybridReverbFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/iZHybridReverbFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/iZHybridReverbFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZHybridReverbFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZHybridReverbFX.lib>
		)

		SET(iZTrashBoxModelerFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/iZTrashBoxModelerFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/iZTrashBoxModelerFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashBoxModelerFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashBoxModelerFX.lib>
		)

		SET(iZTrashDelayFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/iZTrashDelayFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/iZTrashDelayFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashDelayFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashDelayFX.lib>
		)

		SET(iZTrashDistortionFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/iZTrashDistortionFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/iZTrashDistortionFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashDistortionFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashDistortionFX.lib>
		)

		SET(iZTrashDynamicsFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/iZTrashDynamicsFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/iZTrashDynamicsFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashDynamicsFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashDynamicsFX.lib>
		)

		SET(iZTrashFiltersFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/iZTrashFiltersFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/iZTrashFiltersFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashFiltersFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashFiltersFX.lib>
		)

		SET(iZTrashMultibandDistortionFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/iZTrashMultibandDistortionFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/iZTrashMultibandDistortionFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashMultibandDistortionFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/iZTrashMultibandDistortionFX.lib>
		)

		SET(MasteringSuiteFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/MasteringSuiteFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/MasteringSuiteFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/MasteringSuiteFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/MasteringSuiteFX.lib>
		)

		SET(McDSPFutzBoxFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/McDSPFutzBoxFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/McDSPFutzBoxFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/McDSPFutzBoxFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/McDSPFutzBoxFX.lib>
		)

		SET(McDSPLimiterFX
			$<$<CONFIG:DEBUG>:			${WWISE_SDK_PATH}/x64_vc170/Debug(StaticCRT)/lib/McDSPLimiterFX.lib>
			$<$<CONFIG:RELWITHDEBINFO>: ${WWISE_SDK_PATH}/x64_vc170/Profile(StaticCRT)/lib/McDSPLimiterFX.lib>
			$<$<CONFIG:RELEASE>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/McDSPLimiterFX.lib>
			$<$<CONFIG:MINSIZEREL>:		${WWISE_SDK_PATH}/x64_vc170/Release(StaticCRT)/lib/McDSPLimiterFX.lib>
		)

	ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Windows" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64")

	ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x64")

	ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "ARM64")

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
	${Ak3DAudioBedMixerFX}
	${AkAudioInputSource}
	${AkAutobahn}
	${AkCompressorFX}
	${AkDelayFX}
	${AkExpanderFX}
	${AkFlangerFX}
	${AkGainFX}
	${AkGuitarDistortionFX}
	${AkHarmonizerFX}
	${AkMatrixReverbFX}
	${AkMemoryMgr}
	${AkMeterFX}
	${AkMusicEngine}
	${AkOpusDecoder}
	${AkParametricEQFX}
	${AkPeakLimiterFX}
	${AkPitchShifterFX}
	${AkRecorderFX}
	${AkReflectFX}
	${AkRoomVerbFX}
	${AkSilenceSource}
	${AkSineSource}
	${AkSoundEngine}
	${AkSoundSeedWindSource}
	${AkSoundSeedWooshSource}
	${AkSpatialAudio}
	${AkStereoDelayFX}
	${AkStreamMgr}
	${AkSynthOneSource}
	${AkTimeStretchFX}
	${AkToneSource}
	${AkTremoloFX}
	${AkVorbisDecoder}
	${AuroHeadphoneFX}
	${iZHybridReverbFX}
	${iZTrashBoxModelerFX}
	${iZTrashDelayFX}
	${iZTrashDistortionFX}
	${iZTrashDynamicsFX}
	${iZTrashFiltersFX}
	${iZTrashMultibandDistortionFX}
	${MasteringSuiteFX}
	${McDSPFutzBoxFX}
	${McDSPLimiterFX}
)
