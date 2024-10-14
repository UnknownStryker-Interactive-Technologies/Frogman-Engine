// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/vector.hpp>




BEGIN_NAMESPACE(FE)


_FE_REGISTER_CALL_ vector4 cross_product_without_w(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    vector4 l_lhs, l_rhs;

    _mm_store_ps(reinterpret_cast<var::float32*>(&l_lhs),
                 _mm_mul_ps(_mm_set_ps(lhs_p._y, lhs_p._z, lhs_p._x, 0), 
                            _mm_set_ps(rhs_p._z, rhs_p._x, rhs_p._y, 0)
                           )
                );

    _mm_store_ps(reinterpret_cast<var::float32*>(&l_rhs),
                 _mm_mul_ps(_mm_set_ps(lhs_p._z, lhs_p._x, lhs_p._y, 0),
                            _mm_set_ps(rhs_p._y, rhs_p._z, rhs_p._x, 0)
                           ) 
                );

    _mm_store_ps(reinterpret_cast<var::float32*>(&l_rhs),
                 _mm_sub_ps(*reinterpret_cast<__m128*>(&l_lhs), 
                            *reinterpret_cast<__m128*>(&l_rhs)
                           )
                );
    return l_rhs;
}


_FE_REGISTER_CALL_ vector4 lerp(const vector4& lhs_p, const vector4& rhs_p, float32 t_p) noexcept
{
    return lhs_p + (t_p * (rhs_p - lhs_p));
}


_FE_REGISTER_CALL_ vector4 projection(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    return (dot_product(lhs_p, rhs_p) / dot_product(rhs_p, rhs_p)) * rhs_p;
}

_FE_REGISTER_CALL_ vector4 projection_without_w(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    return (dot_product_without_w(lhs_p, rhs_p) / dot_product_without_w(rhs_p, rhs_p)) * rhs_p;
}


END_NAMESPACE