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
   constexpr static inline add_64_bit_carry(uint64_t a, uint64_t b) noexcept {
      uint64_t sum = a + b;
      return (sum < a) ? 1 : 0;
   }
} // namespace astro::cryptid