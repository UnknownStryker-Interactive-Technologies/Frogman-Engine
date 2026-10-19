    SET(RETURN_VALUE_FROM_TOOL)
    SET(TOOL_STDOUT)
    SET(TOOL_STDERR)
        
    MESSAGE("========== Frogman Engine Header Tool ==========")

        # Print paths for debugging
        SET(PATH_TO_HEADER_TOOL ${FROGMAN_ENGINE_CMAKE_DIR}/../Header-Tool/Binaries/RelWithDebInfo/FE.HeaderTool.exe)

        MESSAGE(STATUS "The path to Frogman Engine Header Tool is: ${PATH_TO_HEADER_TOOL}")
        MESSAGE(STATUS "The target header files are: ${HEADER_FILES_PATHS}")
        MESSAGE(STATUS "The Header Tool program options are: ${HEADER_TOOL_PROGRAM_OPTIONS}")

        # Verify the existence of the header tool executable
        IF(NOT EXISTS "${PATH_TO_HEADER_TOOL}")
            MESSAGE(FATAL_ERROR "Frogman Engine Header Tool executable not found at ${PATH_TO_HEADER_TOOL}")
        ENDIF()

        # Verify the existence of the target header files
        FOREACH(HEADER_FILE IN LISTS HEADER_FILES_PATHS)
            IF(NOT EXISTS "${HEADER_FILE}")
                MESSAGE(FATAL_ERROR "Target header file not found: ${HEADER_FILE}")
            ENDIF()
        ENDFOREACH()

        # The header tool rewrites every target header, every generated-includes/*.generated.h and generated.cpp
        # on each run, even when their content did not change. That bumps their timestamps and forces a full recompile.
        # To keep incremental builds working, snapshot those files (file(COPY) preserves timestamps) before running
        # the tool, and afterwards restore the snapshot for every file whose content is unchanged.
        SET(FHT_SNAPSHOT_DIR "${PROJECT_DIR}/generated-cmake/fht-snapshot")
        FILE(REMOVE_RECURSE "${FHT_SNAPSHOT_DIR}")

        SET(FHT_TRACKED_FILES ${HEADER_FILES_PATHS})
        FILE(GLOB_RECURSE FHT_GENERATED_HEADERS "${PROJECT_DIR}/generated-includes/*.generated.h")
        LIST(APPEND FHT_TRACKED_FILES ${FHT_GENERATED_HEADERS})
        IF(EXISTS "${PROJECT_DIR}/generated.cpp")
            LIST(APPEND FHT_TRACKED_FILES "${PROJECT_DIR}/generated.cpp")
        ENDIF()

        # Each file is snapshotted into its own sub-folder keyed by a hash of its path, because the headers usually
        # live outside this directory (../Include) and may share base names.
        FOREACH(TRACKED_FILE IN LISTS FHT_TRACKED_FILES)
            STRING(SHA1 TRACKED_KEY "${TRACKED_FILE}")
            FILE(COPY "${TRACKED_FILE}" DESTINATION "${FHT_SNAPSHOT_DIR}/${TRACKED_KEY}")
            FILE(SHA256 "${TRACKED_FILE}" FHT_HASH_BEFORE_${TRACKED_KEY})
        ENDFOREACH()

        # Execute the header tool.
        EXECUTE_PROCESS(
            COMMAND ${CMAKE_COMMAND} -E env LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8 ${PATH_TO_HEADER_TOOL}
            ARGS ${HEADER_TOOL_PROGRAM_OPTIONS} "-frequire-reflection-marker" "-path-to-project=${PROJECT_DIR}" "${HEADER_FILES_PATHS}"
            RESULT_VARIABLE RETURN_VALUE_FROM_TOOL
            OUTPUT_VARIABLE TOOL_STDOUT
            ERROR_VARIABLE TOOL_STDERR
        )

        # Restore the original timestamp of every file the tool rewrote without changing its content.
        FOREACH(TRACKED_FILE IN LISTS FHT_TRACKED_FILES)
            IF(NOT EXISTS "${TRACKED_FILE}")
                CONTINUE()
            ENDIF()
            STRING(SHA1 TRACKED_KEY "${TRACKED_FILE}")
            FILE(SHA256 "${TRACKED_FILE}" FHT_HASH_AFTER)
            IF(FHT_HASH_AFTER STREQUAL "${FHT_HASH_BEFORE_${TRACKED_KEY}}")
                GET_FILENAME_COMPONENT(TRACKED_NAME "${TRACKED_FILE}" NAME)
                GET_FILENAME_COMPONENT(TRACKED_DIR "${TRACKED_FILE}" DIRECTORY)
                FILE(COPY "${FHT_SNAPSHOT_DIR}/${TRACKED_KEY}/${TRACKED_NAME}" DESTINATION "${TRACKED_DIR}")
            ENDIF()
        ENDFOREACH()
        FILE(REMOVE_RECURSE "${FHT_SNAPSHOT_DIR}")

    MESSAGE("${TOOL_STDOUT}")
    MESSAGE("${TOOL_STDERR}")

    # Print tool output and error for debugging
    MESSAGE("Frogman Engine Header Tool returned the exit code '${RETURN_VALUE_FROM_TOOL}'.")

    IF(NOT RETURN_VALUE_FROM_TOOL EQUAL 0)
        IF(NOT RETURN_VALUE_FROM_TOOL EQUAL 1006)
            MESSAGE(FATAL_ERROR "------ FE HT: compilation failed! Please, check the messages above! ------")
        ENDIF()
    ENDIF()

    MESSAGE("========== Frogman Engine Header Tool Successfully Processed the Target Header Files ==========")

