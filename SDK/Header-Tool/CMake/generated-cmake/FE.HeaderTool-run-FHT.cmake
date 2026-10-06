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

        # Execute the header tool. 
        EXECUTE_PROCESS(
            COMMAND ${CMAKE_COMMAND} -E env LANG=en_US.UTF-8 LC_ALL=en_US.UTF-8 ${PATH_TO_HEADER_TOOL} 
            ARGS ${HEADER_TOOL_PROGRAM_OPTIONS} "-frequire-reflection-marker" "-path-to-project=${CMAKE_CURRENT_SOURCE_DIR}" "${HEADER_FILES_PATHS}"
            RESULT_VARIABLE RETURN_VALUE_FROM_TOOL
            OUTPUT_VARIABLE TOOL_STDOUT
            ERROR_VARIABLE TOOL_STDERR
        )

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

