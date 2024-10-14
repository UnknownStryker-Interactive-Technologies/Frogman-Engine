// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>
#include <FE/algorithm/utility.hxx>
//std
#include <array>
#include <memory>


/*
ls: lists all folders in the current working directory
ls filename: checks the presence of files with the specified name.
*/
int main()
{
    using namespace FE;

    FE_LOG("Compilation test of FE.core.log source code is successful.\n");

    _FE_MAYBE_UNUSED_ var::int32 i = 7;
    FE_LOG("This is how an integer can be logged. ${%s@0} = ${%d@1};\n", TO_STRING(var::int32 i), &i);

    _FE_MAYBE_UNUSED_ var::float32 f = 3.14f;
    FE_LOG("This is how a float can be logged. ${%s@0} = ${%f@1};\n", TO_STRING(var::float32 i), &f);

    _FE_MAYBE_UNUSED_ var::boolean b = true;
    FE_LOG("This is how a boolean can be logged. ${%s@0} = ${%b@1};\n", TO_STRING(var::boolean b), &b);

    return 0;
}