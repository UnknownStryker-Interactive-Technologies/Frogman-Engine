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
        "$WORKING_DIRECTORY/FE.core.prerequisites/FE.core.prerequisites_test"
        "$WORKING_DIRECTORY/FE.core.type_traits/FE.core.type_traits_test"
        "$WORKING_DIRECTORY/FE.core.algorithm.utility/FE.core.algorithm.utility_test"
        "$WORKING_DIRECTORY/FE.core.hash/FE.core.hash_test"
        "$WORKING_DIRECTORY/FE.core.log/FE.core.log_test"
        "$WORKING_DIRECTORY/FE.core.thread/FE.core.thread_test"
        "$WORKING_DIRECTORY/FE.core.algorithm.math/FE.core.algorithm.math_test"
        "$WORKING_DIRECTORY/FE.core.iterator/FE.core.iterator_test"
        "$WORKING_DIRECTORY/FE.core.memory/FE.core.memory_test"
        "$WORKING_DIRECTORY/FE.core.algorithm.string/FE.core.algorithm.string_test"
        "$WORKING_DIRECTORY/FE.core.fstring/FE.core.fstring_test"
        "$WORKING_DIRECTORY/FE.core.pool/FE.core.pool_test"
        "$WORKING_DIRECTORY/FE.core.managed/FE.core.managed_test"
        "$WORKING_DIRECTORY/FE.core.string/FE.core.string_test"
        "$WORKING_DIRECTORY/FE.core.string_view/FE.core.string_view_test"
        "$WORKING_DIRECTORY/FE.core.containers/FE.core.containers_test"
        "$WORKING_DIRECTORY/FE.framework.predefined_main/FE.framework.predefined_main_test"
        "$WORKING_DIRECTORY/FE.framework.reflection/FE.framework.reflection_test"
        "$WORKING_DIRECTORY/FE.core.adjacency_graph/FE.core.adjacency_graph_test"
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
    TEST="$WORKING_DIRECTORY/$SPECIFIED_TEST_NAME/$SPECIFIED_TEST_NAME$TEST_NAME_SUFFIX"

    echo "Running a test named: $SPECIFIED_TEST_NAME"
    chmod +x "$TEST"
    "$TEST" &
    wait
fi
