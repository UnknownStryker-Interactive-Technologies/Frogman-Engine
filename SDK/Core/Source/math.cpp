#include <FE/algorithm/math.hpp>
// Copyright © from 2023 to current, UNKNOWN STRYKER. All Rights Reserved.

#ifdef _FE_ON_X86_64_
	#ifdef __AVX__
		// x86-64 AVX
		#include <immintrin.h>
	#else
		#error "Please check if the current CPU supports AVX SIMD instruction set and has ymm registers."
	#endif

	#ifdef __SSE2__
		// x86-64 SSE2
		#include <emmintrin.h>
	#else
		#error "Please check if the current CPU supports SSE2 SIMD instruction set and has xmm registers."
	#endif
#endif




BEGIN_NAMESPACE(FE::algorithm::math)


uint64 to_higher_prime(uint64 number_p) noexcept
{
	constexpr uint64 l_magical_seven = 7;

	switch (number_p)
	{
	case 0:
		_FE_FALLTHROUGH_;
	case 1:
		return smallest_prime_number;

	case 3:
		_FE_FALLTHROUGH_;
	case 5:
		_FE_FALLTHROUGH_;
	case 7:
		_FE_FALLTHROUGH_;
	case 11:
		_FE_FALLTHROUGH_;
	case 13:
		return number_p;

	default:
		{
			var::uint64 l_number = number_p;
	
			if (FE_MODULO_BY_2(l_number) == 0)
			{
				l_number ^= l_magical_seven;
	
				if (FE_MODULO_BY_2(l_number) == 0)
				{
					++l_number;
				}
			}
	
			while (is_prime(l_number) == false)
			{
				++l_number;
			}
			return l_number;
		}
	}
}

uint64 to_lower_prime(uint64 number_p) noexcept
{
	constexpr uint64 l_magical_seven = 7;

	switch (number_p)
	{
	case 0:
		_FE_FALLTHROUGH_;
	case 1:
		return smallest_prime_number;

	case 3:
		_FE_FALLTHROUGH_;
	case 5:
		_FE_FALLTHROUGH_;
	case 7:
		_FE_FALLTHROUGH_;
	case 11:
		_FE_FALLTHROUGH_;
	case 13:
		return number_p;

	default:
		{
			var::uint64 l_number = number_p;

			if (FE_MODULO_BY_2(l_number) == 0)
			{
				l_number ^= l_magical_seven;

				if (FE_MODULO_BY_2(l_number) == 0)
				{
					++l_number;
				}
			}

			while (is_prime(l_number) == false)
			{
				--l_number;
			}
			return l_number;
		}
	}
}


/*
    Taylor's series of sin(𝑥)

    sin(𝑥) = 𝑥 - (𝑥^3 / 3!) + (𝑥^5 / 5!) - (𝑥^7 / 7!) + (𝑥^9 / 9!)
               - (𝑥^11 / 11!) + (𝑥^13 / 13!) - (𝑥^15 / 15!) + (𝑥^17 / 17!)

    180°(angle) = π(radian)
*/
FE::float64 sin(::var::float64 radian_p) noexcept
{
    // number of pies == ( static_cast<FE::float64>(static_cast<FE::int64>(radian_to_degree(radian_p)) / 180) * pi );
    radian_p -= ( static_cast<FE::float64>(static_cast<FE::int64>(radian_to_degree(radian_p)) / 180) * pi );
    // radian is now less than a pi.

    // factorial lookups.
    constexpr FE::float64 factorial_of_3 = 6.0;
    constexpr FE::float64 factorial_of_5 = 120.0;
    constexpr FE::float64 factorial_of_7 = 5040.0;
    constexpr FE::float64 factorial_of_9 = 362880.0;
    constexpr FE::float64 factorial_of_11 = 39916800.0;
    constexpr FE::float64 factorial_of_13 = 6227020800.0;
    constexpr FE::float64 factorial_of_15 = 1307674368000.0;
    constexpr FE::float64 factorial_of_17 = 355687428096000.0;

#ifdef _FE_ON_X86_64_
    // load look up tables onto ymm registers.
    static const __m256d l_s_factorial_of_11_to_17 = _mm256_set_pd(factorial_of_17, factorial_of_15, factorial_of_13, factorial_of_11);
    static const __m256d l_s_factorial_of_3_to_9 = _mm256_set_pd(factorial_of_9, factorial_of_7, factorial_of_5, factorial_of_3);

    alignas(32) ::var::float64 l_terms[8];
    // exponent calculation.
    l_terms[0] = radian_p * radian_p * radian_p;    
    l_terms[1] = l_terms[0] * radian_p * radian_p; 
    l_terms[2] = l_terms[1] * radian_p * radian_p; 
    l_terms[3] = l_terms[2] * radian_p * radian_p; 
    
    l_terms[4] = l_terms[3] * radian_p * radian_p;
    l_terms[5] = l_terms[4] * radian_p * radian_p;
    l_terms[6] = l_terms[5] * radian_p * radian_p;
    l_terms[7] = l_terms[6] * radian_p * radian_p;

    // divide 0:31 bits by factorial of 3 to 9.
    _mm256_store_pd(static_cast<::var::float64*>(l_terms), 
                    _mm256_div_pd(_mm256_load_pd( static_cast<::var::float64* const>(l_terms) ),
                                  l_s_factorial_of_3_to_9)
                   );

    // divide 32:63 bits by factorial of 11 to 17.
    _mm256_store_pd(static_cast<::var::float64*>(l_terms) + 4, 
                    _mm256_div_pd(_mm256_load_pd( static_cast<::var::float64* const>(l_terms) + 4 ),
                                  l_s_factorial_of_11_to_17)
                   );

    // negate terms.
    l_terms[0] = -l_terms[0];
    l_terms[2] = -l_terms[2];
    l_terms[4] = -l_terms[4];
    l_terms[6] = -l_terms[6];

    // add 0:31 bits with 32:63 bits and store in 0:31 bits.
    _mm256_store_pd(static_cast<::var::float64*>(l_terms),
                    _mm256_add_pd(_mm256_load_pd( static_cast<::var::float64* const>(l_terms) ),
                                  _mm256_load_pd( static_cast<::var::float64* const>(l_terms) + 4 ))
                   );

    // add 0:15 bits with 16:31 bits and store in 0:15 bits.
    _mm_store_pd(static_cast<::var::float64*>(l_terms),
                 _mm_add_pd(_mm_load_pd( static_cast<::var::float64* const>(l_terms) ),
                            _mm_load_pd( static_cast<::var::float64* const>(l_terms) + 2 ))
                );
#endif

    // sum up the rest and ret.
    return radian_p + l_terms[0] + l_terms[1];
}


/*
    Taylor's series of cos(𝑥)

    cos(𝑥) = 1.0 - (𝑥^2 / 2!) + (𝑥^4 / 4!) - (𝑥^6 / 6!) + (𝑥^8 / 8!)
                 - (𝑥^10 / 10!) + (𝑥^12 / 12!) - (𝑥^14 / 14!) + (𝑥^16 / 16!)

    180°(angle) = π(radian)
*/
FE::float64 cos(::var::float64 radian_p) noexcept
{
    // number of pies == ( static_cast<FE::int64>(radian_to_degree(radian_p) / 360) * (2 * pi) );
    radian_p -= ( static_cast<FE::float64>(static_cast<FE::int64>(radian_to_degree(radian_p)) / 360) * (2.0 * pi) );
    // radian is now less than a pi.

    if(radian_p > pi)
    {
        radian_p -= (radian_p - pi);
    }

    // factorial lookups.
    constexpr FE::float64 factorial_of_2 = 2.0;
    constexpr FE::float64 factorial_of_4 = 24.0;
    constexpr FE::float64 factorial_of_6 = 720.0;
    constexpr FE::float64 factorial_of_8 = 40320.0;
    constexpr FE::float64 factorial_of_10 = 3628800.0;
    constexpr FE::float64 factorial_of_12 = 479001600.0;
    constexpr FE::float64 factorial_of_14 = 87178291200.0;
    constexpr FE::float64 factorial_of_16 = 20922789888000.0;

#ifdef _FE_ON_X86_64_
    // load look up tables onto ymm registers.
    static const __m256d l_s_factorial_of_10_to_16 = _mm256_set_pd(factorial_of_16, factorial_of_14, factorial_of_12, factorial_of_10);
    static const __m256d l_s_factorial_of_2_to_8 = _mm256_set_pd(factorial_of_8, factorial_of_6, factorial_of_4, factorial_of_2);
    
    ::var::float64 l_terms[8];
    // exponent calculation.
    l_terms[0] = radian_p * radian_p;    
    l_terms[1] = l_terms[0] * radian_p * radian_p; 
    l_terms[2] = l_terms[1] * radian_p * radian_p; 
    l_terms[3] = l_terms[2] * radian_p * radian_p; 
    
    l_terms[4] = l_terms[3] * radian_p * radian_p;
    l_terms[5] = l_terms[4] * radian_p * radian_p;
    l_terms[6] = l_terms[5] * radian_p * radian_p;
    l_terms[7] = l_terms[6] * radian_p * radian_p;

    // divide 0:31 bits by factorial of 2 to 8.
    _mm256_store_pd(static_cast<::var::float64*>(l_terms), 
                    _mm256_div_pd(_mm256_load_pd( static_cast<::var::float64* const>(l_terms) ),
                                  l_s_factorial_of_2_to_8)
                   );
    
    // divide 32:63 bits by factorial of 10 to 16.
    _mm256_store_pd(static_cast<::var::float64*>(l_terms) + 4, 
                    _mm256_div_pd(_mm256_load_pd( static_cast<::var::float64* const>(l_terms) + 4 ),
                                  l_s_factorial_of_10_to_16)
                   );

    // negate terms.
    l_terms[0] = -l_terms[0];
    l_terms[2] = -l_terms[2];
    l_terms[4] = -l_terms[4];
    l_terms[6] = -l_terms[6];

    // add 0:31 bits with 32:63 bits and store in 0:31 bits.
    _mm256_store_pd(static_cast<::var::float64*>(l_terms),
                    _mm256_add_pd(_mm256_load_pd( static_cast<::var::float64* const>(l_terms) ),
                                  _mm256_load_pd( static_cast<::var::float64* const>(l_terms) + 4 ))
                   );

    // add 0:15 bits with 16:31 bits and store in 0:15 bits.
    _mm_store_pd(static_cast<::var::float64*>(l_terms),
                 _mm_add_pd(_mm_load_pd( static_cast<::var::float64* const>(l_terms) ),
                            _mm_load_pd( static_cast<::var::float64* const>(l_terms) + 2 ))
                );
#endif
    
    // sum up the rest and ret.
    return 1.0 + l_terms[0] + l_terms[1];
}


END_NAMESPACE