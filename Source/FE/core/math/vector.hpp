#ifndef _FE_CORE_MATH_VECTOR_HPP_
#define _FE_CORE_MATH_VECTOR_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/core/prerequisites.h>


/*
- Game world coordinate system scale - 

1000f32 == 1km 

1f32 == 1m

0.01f32 == 1cm 

0.001f32 == 1mm

0.000001f32 == 1um

*/

namespace var
{
    using scalar_t = var::float32;
}

BEGIN_NAMESPACE(FE)

using scalar_t = float32;


struct vector3
{
    var::float32 _x;
    var::float32 _y;
    var::float32 _z;
};

_FORCE_INLINE_ vector3 operator+(const vector3& lhs_p, const vector3& rhs_p) noexcept
{
    (void)lhs_p;
    (void)rhs_p;
    return vector3();
}




struct vector4
{
    var::float32 _x;
    var::float32 _y;
    var::float32 _z;
    var::float32 _w;
};

_FORCE_INLINE_ vector4 operator+(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    (void)lhs_p;
    (void)rhs_p;
    return vector4();
}


END_NAMESPACE
#endif