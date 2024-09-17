#!/bin/bash
WORKING_DIRECTORY=$(dirname "$0")
if [ "$WORKING_DIRECTORY" = '.' ];
    then
    WORKING_DIRECTORY=$(pwd)
fi

BUILD_FILES_FOLDER_NAME=build

SPECIFIED_TEST_NAME=$1
if [ -z "$SPECIFIED_TEST_NAME" ]; 
    then

    TESTS=(
        "$WORKING_DIRECTORY/FE.algorithm.math/Debug/FE.algorithm.math_test"
        "$WORKING_DIRECTORY/FE.algorithm.string/Debug/FE.algorithm.string_test"
        "$WORKING_DIRECTORY/FE.algorithm.utility/Debug/FE.algorithm.utility_test"
        "$WORKING_DIRECTORY/FE.containers/Debug/FE.containers_test"
        
        "$WORKING_DIRECTORY/FE.framework.predefined_main/Debug/FE.framework.predefined_main_test"
        "$WORKING_DIRECTORY/FE.framework.reflection/Debug/FE.framework.reflection_test"

        "$WORKING_DIRECTORY/FE.hash/Debug/FE.hash_test"
        "$WORKING_DIRECTORY/FE.iterator/Debug/FE.iterator_test"
        "$WORKING_DIRECTORY/FE.log/Debug/FE.log_test"
        "$WORKING_DIRECTORY/FE.managed/Debug/FE.managed_test"
        "$WORKING_DIRECTORY/FE.memory/Debug/FE.memory_test"
        "$WORKING_DIRECTORY/FE.pool/Debug/FE.pool_test"
        "$WORKING_DIRECTORY/FE.string/Debug/FE.string_test"
        "$WORKING_DIRECTORY/FE.string_view/Debug/FE.string_view_test"
        "$WORKING_DIRECTORY/FE.task/Debug/FE.task_test"
    )

    echo " "
    for TEST in "${TESTS[@]}"; do
        echo "Running a test named: $TEST"
        chmod +x "$TEST"
        "$TEST" &
        wait
    done

else
    TEST_NAME_SUFFIX="_test"
    TEST="$WORKING_DIRECTORY/$SPECIFIED_TEST_NAME/Debug/$SPECIFIED_TEST_NAME$TEST_NAME_SUFFIX"

    echo "Running a test named: $SPECIFIED_TEST_NAME"
    chmod +x "$TEST"
    "$TEST" &
    wait
fi
