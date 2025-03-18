#pragma once

#include <mmintrin.h> // MMX
#include <xmmintrin.h> // SSE
#include <emmintrin.h> // SSE2
#include <pmmintrin.h> // SSE3
#include <tmmintrin.h> // SSSE3
#include <smmintrin.h> // SSE4.1
#include <nmmintrin.h> // SSE4.2
#include <ammintrin.h> // SSE4A
#include <wmmintrin.h> // AES
#include <immintrin.h> // AVX
#include <zmmintrin.h> // AVX512

#include <bit>

#include "../../utils/misc.hpp"

namespace astro::cryptid {
   /**
    * Calculates the carry bit of a 64-bit addition.
    *
    * @param a the first operand of the addition
    * @param b the second operand of the addition
    *
    * @return the carry bit of the addition, which is 1 if the sum is less than the first operand, and 0 otherwise
    */
   constexpr static inline add_64_bit_carry(uint64_t a, uint64_t b) noexcept {
      return ((a+b) < a) ? 1 : 0;
   }
} // namespace astro::cryptid