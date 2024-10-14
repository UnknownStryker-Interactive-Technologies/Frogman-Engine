#ifndef _FE_CORE_VECTOR_HPP_
#define _FE_CORE_VECTOR_HPP_
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.
#include <FE/prerequisites.h>

#ifdef __SSE__
#include <xmmintrin.h>
#endif

// std
#include <cmath>




namespace var
{
    using scalar_t = var::float32;
}

BEGIN_NAMESPACE(FE)

using scalar_t = float32;


_FE_MAYBE_UNUSED_ constexpr static FE::float32 one_km = 10.0f;
_FE_MAYBE_UNUSED_ constexpr static FE::float32 one_m = 1.0f;
_FE_MAYBE_UNUSED_ constexpr static FE::float32 one_cm = 0.1f;
_FE_MAYBE_UNUSED_ constexpr static FE::float32 one_mm = 0.01f;
_FE_MAYBE_UNUSED_ constexpr static FE::float32 one_um = 0.001f;


struct alignas(16) vector4
{
    var::float32 _x;
    var::float32 _y;
    var::float32 _z;
    var::float32 _w;
};


_FE_MAYBE_UNUSED_ constexpr static vector4 forward_vector = { 0.0f, 0.0f, 1.0f, 0.0f };
_FE_MAYBE_UNUSED_ constexpr static vector4 backward_vector = { 0.0f, 0.0f, -1.0f, 0.0f };
_FE_MAYBE_UNUSED_ constexpr static vector4 rightward_vector = { 1.0f, 0.0f, 0.0f, 0.0f };
_FE_MAYBE_UNUSED_ constexpr static vector4 leftward_vector = { -1.0f, 0.0f, 0.0f, 0.0f };
_FE_MAYBE_UNUSED_ constexpr static vector4 upward_vector = { 0.0f, 1.0f, 0.0f, 0.0f };
_FE_MAYBE_UNUSED_ constexpr static vector4 downward_vector = { 0.0f, -1.0f, 0.0f, 0.0f };


// Component-wise addition
_FE_FORCE_INLINE_ vector4 operator+(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result), 
                 _mm_add_ps(_mm_load_ps( reinterpret_cast<float32*>(&lhs_p) ), 
                            _mm_load_ps( reinterpret_cast<float32*>(&rhs_p) )
                           )
                );
    return l_result;
}


// Component-wise subtraction
_FE_FORCE_INLINE_ vector4 operator-(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result), 
                 _mm_sub_ps(_mm_load_ps( reinterpret_cast<float32*>(&lhs_p) ), 
                            _mm_load_ps( reinterpret_cast<float32*>(&rhs_p) )
                           )
                 );
    return l_result;
}


// Component-wise multiplication
_FE_FORCE_INLINE_ vector4 operator*(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result), 
                 _mm_mul_ps(_mm_load_ps( reinterpret_cast<float32*>(&lhs_p) ), 
                            _mm_load_ps( reinterpret_cast<float32*>(&rhs_p) )
                           )
                 );
    return l_result;
}


// Scalar multiplication
_FE_FORCE_INLINE_ vector4 operator*(const vector4& lhs_p, float32 value_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result), 
                 _mm_mul_ps(_mm_load_ps( reinterpret_cast<float32*>(&lhs_p) ), 
                            _mm_set1_ps( value_p )
                           )
                );
    return l_result;
}

_FE_FORCE_INLINE_ vector4 operator*(float32 value_p, const vector4& rhs_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result), 
                 _mm_mul_ps(_mm_load_ps( reinterpret_cast<float32*>(&rhs_p) ), 
                            _mm_set1_ps( value_p )
                           )
                 );
    return l_result;
}


_FE_FORCE_INLINE_ boolean operator==(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result), 
                 _mm_cmpeq_ps(_mm_load_ps(reinterpret_cast<float32*>(&lhs_p)), 
                              _mm_load_ps(reinterpret_cast<float32*>(&rhs_p))
                             )
                 );
    return l_result._x != 0;
}

_FE_FORCE_INLINE_ boolean operator!=(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result), 
                 _mm_cmpeq_ps(_mm_load_ps(reinterpret_cast<float32*>(&lhs_p)), 
                              _mm_load_ps(reinterpret_cast<float32*>(&rhs_p))
                             )
                );
    return l_result._x == 0;
}


_FE_FORCE_INLINE_ float dot_product(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result), 
                 _mm_mul_ps(_mm_load_ps( reinterpret_cast<float32*>(&lhs_p) ), 
                            _mm_load_ps( reinterpret_cast<float32*>(&rhs_p) )
                           )
                );
    return l_result._x + l_result._y + l_result._z + l_result._w;
}

_FE_FORCE_INLINE_ float dot_product_without_w(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result), 
                 _mm_mul_ps(_mm_load_ps( reinterpret_cast<float32*>(&lhs_p) ), 
                            _mm_load_ps( reinterpret_cast<float32*>(&rhs_p) )
                           )
                );
    return l_result._x + l_result._y + l_result._z;
}


/* Cross product:
   it does not produce a bivector instead it treats the vectors as 3D vectors. 
*/
_FE_REGISTER_CALL_ vector4 cross_product_without_w(const vector4& lhs_p, const vector4& rhs_p) noexcept;


// Disregards the w component
_FE_FORCE_INLINE_ float32 compute_length_without_w(const vector4& vector_p) noexcept
{
    vector4 l_result = vector_p * vector_p;
    return std::sqrt(l_result._x + l_result._y + l_result._z);
}

_FE_FORCE_INLINE_ float32 compute_length_of(const vector4& vector_p) noexcept
{
    vector4 l_result = vector_p * vector_p;
    return std::sqrt(l_result._x + l_result._y + l_result._z + l_result._w);
}


_FE_FORCE_INLINE_ vector4 normalize(const vector4& vector_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result),
                 _mm_div_ps(_mm_load_ps(reinterpret_cast<float32*>(&vector_p)),
                            _mm_set1_ps( compute_length_of(vector_p) )
                           )
                );
    return l_result;
}

// Disregards the w component
_FE_FORCE_INLINE_ vector4 normalize_without_w(const vector4& vector_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result),
                 _mm_div_ps(_mm_load_ps(reinterpret_cast<float32*>(&vector_p)),
                            _mm_set1_ps(compute_length_without_w(vector_p))
                           )
                );
    return l_result;
}


_FE_REGISTER_CALL_ vector4 lerp(const vector4& lhs_p, const vector4& rhs_p, float32 t_p) noexcept;


_FE_REGISTER_CALL_ vector4 projection(const vector4& lhs_p, const vector4& rhs_p) noexcept;

_FE_REGISTER_CALL_ vector4 projection_without_w(const vector4& lhs_p, const vector4& rhs_p) noexcept;


_FE_FORCE_INLINE_ float32 distance(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    vector4 l_result = rhs_p - lhs_p;
    l_result = l_result * l_result;
    return std::sqrt(l_result._x + l_result._y + l_result._z + l_result._w);
}

_FE_FORCE_INLINE_ float32 distance_without_w(const vector4& lhs_p, const vector4& rhs_p) noexcept
{
    vector4 l_result = rhs_p - lhs_p;
    l_result = l_result * l_result;
    return std::sqrt(l_result._x + l_result._y + l_result._z);
}

// It forces the sign-bit(most significant bit, aka leftmost bit) of the four-packed single precision floating points to be zero.
_FE_FORCE_INLINE_ vector4 abs(const vector4& vector_p) noexcept
{
    vector4 l_result;
    _mm_store_ps(reinterpret_cast<var::float32*>(&l_result), 
                 _mm_andnot_ps(_mm_set1_ps(-0.0f), 
                               _mm_load_ps(reinterpret_cast<float32*>(&vector_p))
                              )
                );
    return l_result;
}

END_NAMESPACE
#endif