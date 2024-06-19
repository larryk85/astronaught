#ifndef VERSA_DEFINTIONS_PP_

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

#endif // VERSA_DEFINITIONS_PP_