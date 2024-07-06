#pragma once
#include <cstdint>
#if (ASTRO_COMPILER & ASTRO_MSVC_BUILD) == ASTRO_MSVC_BUILD
   #define ASTRO_PRETTY_FUNCTION __FUNCSIG__
   #define ASTRO_ALWAYS_INLINE __forceinline
#else
   #define ASTRO_PRETTY_FUNCTION __PRETTY_FUNCTION__
   #define ASTRO_ALWAYS_INLINE __attribute__((always_inline))
#endif

#if defined(ASTRO_COMPILE_TIME_CONSTEVAL)
   #define ASTRO_CT_CONST consteval
#else
   #define ASTRO_CT_CONST constexpr
#endif

namespace astro::util {
   template <std::size_t Num, std::size_t Den>
   constexpr static inline int64_t lower_bound_v = Num/Den;

   template <std::size_t Num, std::size_t Den>
   constexpr static inline int64_t upper_bound_v = Num/Den + !(Num%Den == 0);
} // namespace astro::util

#ifndef ASTRO_ENUM_MAX_ELEMS
   #define ASTRO_ENUM_MAX_ELEMS 1024ll
#endif

#ifndef ASTRO_ENUM_UPPER_BOUND
   #ifndef ASTRO_ENUM_LOWER_BOUND
      #define ASTRO_ENUM_UPPER_BOUND astro::util::upper_bound_v<ASTRO_ENUM_MAX_ELEMS, 2>
      #define ASTRO_ENUM_LOWER_BOUND (-astro::util::lower_bound_v<ASTRO_ENUM_MAX_ELEMS, 2>)
   #else
      #define ASTRO_ENUM_UPPER_BOUND ASTRO_ENUM_LOWER_BOUND + ASTRO_ENUM_MAX_ELEMS
   #endif
#else
   #ifndef ASTRO_ENUM_LOWER_BOUND
      #define ASTRO_ENUM_LOWER_BOUND ASTRO_ENUM_UPPER_BOUND - ASTRO_ENUM_MAX_ELEMS
   #else
      #undef ASTRO_ENUM_MAX_ELEMS
      #define ASTRO_ENUM_MAX_ELEMS ASTRO_ENUM_UPPER_BOUND + (-ASTRO_ENUM_LOWER_BOUND)
   #endif
#endif

// min clang v12
// min gcc v10
// min icc v2021
// min icx v2021
// min kvx 4.10.0 gcc(10) __KVX__