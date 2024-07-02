#pragma once
#include <cstdint>
#if (VERSA_COMPILER & VERSA_MSVC_BUILD) == VERSA_MSVC_BUILD
   #define VERSA_PRETTY_FUNCTION __FUNCSIG__
   #define VERSA_ALWAYS_INLINE __forceinline
#else
   #define VERSA_PRETTY_FUNCTION __PRETTY_FUNCTION__
   #define VERSA_ALWAYS_INLINE __attribute__((always_inline))
#endif

#if defined(VERSA_COMPILE_TIME_CONSTEVAL)
   #define VERSA_CT_CONST consteval
#else
   #define VERSA_CT_CONST constexpr
#endif

namespace versa::util {
   template <std::size_t Num, std::size_t Den>
   constexpr static inline int64_t lower_bound_v = Num/Den;

   template <std::size_t Num, std::size_t Den>
   constexpr static inline int64_t upper_bound_v = Num/Den + !(Num%Den == 0);
} // namespace versa::util

#ifndef VERSA_ENUM_MAX_ELEMS
   #define VERSA_ENUM_MAX_ELEMS 1024ll
#endif

#ifndef VERSA_ENUM_UPPER_BOUND
   #ifndef VERSA_ENUM_LOWER_BOUND
      #define VERSA_ENUM_UPPER_BOUND versa::util::upper_bound_v<VERSA_ENUM_MAX_ELEMS, 2>
      #define VERSA_ENUM_LOWER_BOUND (-versa::util::lower_bound_v<VERSA_ENUM_MAX_ELEMS, 2>)
   #else
      #define VERSA_ENUM_UPPER_BOUND VERSA_ENUM_LOWER_BOUND + VERSA_ENUM_MAX_ELEMS
   #endif
#else
   #ifndef VERSA_ENUM_LOWER_BOUND
      #define VERSA_ENUM_LOWER_BOUND VERSA_ENUM_UPPER_BOUND - VERSA_ENUM_MAX_ELEMS
   #else
      #undef VERSA_ENUM_MAX_ELEMS
      #define VERSA_ENUM_MAX_ELEMS VERSA_ENUM_UPPER_BOUND + (-VERSA_ENUM_LOWER_BOUND)
   #endif
#endif

// min clang v12
// min gcc v10
// min icc v2021
// min icx v2021
// min kvx 4.10.0 gcc(10) __KVX__