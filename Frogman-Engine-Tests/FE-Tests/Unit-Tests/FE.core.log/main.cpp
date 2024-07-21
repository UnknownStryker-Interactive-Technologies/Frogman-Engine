// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>
#include <FE/core/algorithm/utility.hxx>
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

    _MAYBE_UNUSED_ var::int32 i = 7;
    FE_LOG("This is how an integer can be logged. ${%s@0} = ${%d@1};\n", TO_STRING(var::int32 i), &i);

    _MAYBE_UNUSED_ var::float32 f = 3.14f;
    FE_LOG("This is how a float can be logged. ${%s@0} = ${%f@1};\n", TO_STRING(var::float32 i), &f);

    _MAYBE_UNUSED_ var::boolean b = true;
    FE_LOG("This is how a boolean can be logged. ${%s@0} = ${%b@1};\n", TO_STRING(var::boolean b), &b);

    {
        using object_t = std::array<var::int32, 10>;
        constexpr FE::size l_buffer_size = sizeof(object_t) * 100;
        std::unique_ptr<char[]> u = std::make_unique<char[]>(l_buffer_size);

        _MAYBE_UNUSED_ object_t arr = {1,2,3,4,5,6,7,8,9,10};
        FE::algorithm::utility::any_object_binary_representation<char>(u.get(), l_buffer_size, arr);
        FE_LOG("This is how an object's binary can be logged. ${%s@0} :\n${%s@1}\n", "object_t arr = {1,2,3,4,5,6,7,8,9,10} can be represented as", u.get());
    }

    return 0;
}