#!/bin/bash
#https://llvm.org/docs/LibFuzzer.html?ref=jumpcrypto.com
WORKING_DIRECTORY=$(dirname "$0")
if [ "$WORKING_DIRECTORY" = '.' ];
    then
    WORKING_DIRECTORY=$(pwd)
fi


BUILD_FILES_FOLDER_NAME=build

SPECIFIED_TEST_NAME=$2
if [ -z "$SPECIFIED_TEST_NAME" ]; 
    then


    TESTS=(
        "$WORKING_DIRECTORY/binaries/FE.core.algorithm.string.count_chars"
        "$WORKING_DIRECTORY/binaries/FE.core.algorithm.string.capitalize"
        "$WORKING_DIRECTORY/binaries/FE.core.algorithm.string.to_lowercase"
        "$WORKING_DIRECTORY/binaries/FE.core.algorithm.string.capitalize_every_first_letter_of_words"
        "$WORKING_DIRECTORY/binaries/FE.core.algorithm.string.length"
        
        
    )


    if [ -d "$WORKING_DIRECTORY/crash-reports" ]; 
        then
        rm -r "$WORKING_DIRECTORY/crash-reports"
        echo "Frogman Engine Tests: re-building fuzz tests"
    fi
    mkdir "$WORKING_DIRECTORY/crash-reports"


    echo " "
    for TEST in "${TESTS[@]}"; do
        echo "Running a fuzz test named: $TEST"
        chmod +x "$TEST"
        "$TEST" "-artifact_prefix=/$WORKING_DIRECTORY/crash-reports/" "-max_total_time=10" "-only_ascii=1" "-max_len=4096" &
        wait
    done

else
    TEST_NAME_SUFFIX="_fuzz_test"
    TEST="$WORKING_DIRECTORY/$SPECIFIED_TEST_NAME/$SPECIFIED_TEST_NAME$TEST_NAME_SUFFIX"

    echo "Running a fuzz test named: $SPECIFIED_TEST_NAME"
    chmod +x "$TEST"
    "$TEST" "-artifact_prefix=/$WORKING_DIRECTORY/crash-reports/" "-max_total_time=10" "-only_ascii=1" "-max_len=4096" &
    wait
fi
