#ifndef ASTRO_INFO_PP_

/**
 * @file constants.inc
 * @brief This file contains constants and macros related to the library version, platform, compiler, architecture, and byte order.
 */

/**
 * @def ASTRO_C_LANG
 * @brief Flag indicating C language.
 */

/**
 * @def ASTRO_CPP_LANG
 * @brief Flag indicating C++ language.
 */

/**
 * @def ASTRO_LANG
 * @brief The language used in the library.
 */

/**
 * @def ASTRO_LANG_VERSION
 * @brief The version of the language used in the library.
 */

/**
 * @def ASTRO_WINDOWS_BUILD
 * @brief Flag indicating Windows build.
 */

/**
 * @def ASTRO_MACOS_BUILD
 * @brief Flag indicating macOS build.
 */

/**
 * @def ASTRO_IOS_BUILD
 * @brief Flag indicating iOS build.
 */

/**
 * @def ASTRO_LINUX_BUILD
 * @brief Flag indicating Linux build.
 */

/**
 * @def ASTRO_UNIX_BUILD
 * @brief Flag indicating Unix build.
 */

/**
 * @def ASTRO_BSD_BUILD
 * @brief Flag indicating BSD build.
 */

/**
 * @def ASTRO_ANDROID_BUILD
 * @brief Flag indicating Android build.
 */

/**
 * @def ASTRO_WASI_BUILD
 * @brief Flag indicating WASI build.
 */

/**
 * @def ASTRO_POSIX_BUILD
 * @brief Flag indicating POSIX build.
 */

/**
 * @def ASTRO_OS
 * @brief The operating system used in the library.
 */

/**
 * @def ASTRO_GCC_BUILD
 * @brief Flag indicating GCC build.
 */

/**
 * @def ASTRO_MSVC_BUILD
 * @brief Flag indicating MSVC build.
 */

/**
 * @def ASTRO_CLANG_BUILD
 * @brief Flag indicating Clang build.
 */

/**
 * @def ASTRO_INTEL_BUILD
 * @brief Flag indicating Intel build.
 */

/**
 * @def ASTRO_CL430_BUILD
 * @brief Flag indicating CL430 build.
 */

/**
 * @def ASTRO_COMPILER
 * @brief The compiler used in the library.
 */

/**
 * @def ASTRO_COMPILER_VERSION
 * @brief The version of the compiler used in the library.
 */

/**
 * @def ASTRO_LITTLE_ENDIAN
 * @brief Flag indicating little endian byte order.
 */

/**
 * @def ASTRO_BIG_ENDIAN
 * @brief Flag indicating big endian byte order.
 */

/**
 * @def ASTRO_PDP_ENDIAN
 * @brief Flag indicating PDP endian byte order.
 */

/**
 * @def ASTRO_BYTE_ORDER
 * @brief The byte order used in the library.
 */

/**
 * @def ASTRO_X86_ARCH
 * @brief Flag indicating x86 architecture.
 */

/**
 * @def ASTRO_AMD64_ARCH
 * @brief Flag indicating AMD64 architecture.
 */

/**
 * @def ASTRO_ARM32_ARCH
 * @brief Flag indicating ARM32 architecture.
 */

/**
 * @def ASTRO_ARM64_ARCH
 * @brief Flag indicating ARM64 architecture.
 */

/**
 * @def ASTRO_SPARC32_ARCH
 * @brief Flag indicating SPARC32 architecture.
 */

/**
 * @def ASTRO_SPARC64_ARCH
 * @brief Flag indicating SPARC64 architecture.
 */

/**
 * @def ASTRO_MIPS32_ARCH
 * @brief Flag indicating MIPS32 architecture.
 */

/**
 * @def ASTRO_MIPS64_ARCH
 * @brief Flag indicating MIPS64 architecture.
 */

/**
 * @def ASTRO_PPC32_ARCH
 * @brief Flag indicating PowerPC32 architecture.
 */

/**
 * @def ASTRO_PPC64_ARCH
 * @brief Flag indicating PowerPC64 architecture.
 */

/**
 * @def ASTRO_RISCV32_ARCH
 * @brief Flag indicating RISC-V32 architecture.
 */

/**
 * @def ASTRO_RISCV64_ARCH
 * @brief Flag indicating RISC-V64 architecture.
 */

/**
 * @def ASTRO_S390_ARCH
 * @brief Flag indicating S390 architecture.
 */

/**
 * @def ASTRO_S390X_ARCH
 * @brief Flag indicating S390X architecture.
 */

/**
 * @def ASTRO_WASM32_ARCH
 * @brief Flag indicating WebAssembly32 architecture.
 */

/**
 * @def ASTRO_WASM64_ARCH
 * @brief Flag indicating WebAssembly64 architecture.
 */

/**
 * @def ASTRO_ARCH
 * @brief The architecture used in the library.
 */

#define ASTRO_C_LANG      0x1
#define ASTRO_CPP_LANG    0x2

#if defined(__cplusplus)
   #define ASTRO_LANG         ASTRO_CPP_LANG
   #define ASTRO_LANG_VERSION __cplusplus
#elif defined(__STDC_VERSION__)
   #define ASTRO_LANG         ASTRO_C_LANG
   #define ASTRO_LANG_VERSION __STDC_VERSION__
#endif

#define ASTRO_WINDOWS_BUILD 0x1
#define ASTRO_MACOS_BUILD   0x2
#define ASTRO_IOS_BUILD     0x4
#define ASTRO_LINUX_BUILD   0x8
#define ASTRO_UNIX_BUILD    0x10
#define ASTRO_BSD_BUILD     0x20
#define ASTRO_ANDROID_BUILD 0x40
#define ASTRO_WASI_BUILD    0x80
#define ASTRO_POSIX_BUILD   0x100

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || defined(__NT__) || defined(WIN32) || defined(WIN64) || defined(_WIN32_WCE) || defined(_WIN32_WCE_EMULATION) || defined(_WIN32_WCE_PSPC)
   #define ASTRO_OS ASTRO_WINDOWS_BUILD
#elif defined(__APPLE__)
   #include <TargetConditional.h>
   #if defined(TARGET_OS_MAC)
      #define ASTRO_OS ASTRO_MACOS_BUILD
   #elif defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
      #define ASTRO_OS ASTRO_IOS_BUILD
   #endif
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
   #define ASTRO_OS ASTRO_LINUX_BUILD
#elif defined(__unix__) || defined(__unix) || defined(unix) || defined(_POSIX_VERSION)
   #define ASTRO_OS ASTRO_UNIX_BUILD
#elif defined(__FREEBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__) || defined(__BSD__)
   #define ASTRO_OS ASTRO_BSD_BUILD
#elif defined(__ANDROID__) || defined(ANDROID)
   #define ASTRO_OS ASTRO_ANDROID_BUILD
#elif defined(__wasi__) || defined(__wasi)
   #define ASTRO_OS ASTRO_WASI_BUILD
#elif defined(_POSIX_VERSION)
   #define ASTRO_OS ASTRO_POSIX_BUILD
#endif

#define ASTRO_GCC_BUILD 0x1
#define ASTRO_MSVC_BUILD 0x2
#define ASTRO_CLANG_BUILD 0x4
#define ASTRO_INTEL_BUILD 0x8
#define ASTRO_CL430_BUILD 0x10

#if defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER) || defined(__ICC) || defined(__INTEL_COMPILER_UPDATE)
   #define ASTRO_COMPILER_ ASTRO_INTEL_BUILD
#else
   #define ASTRO_COMPILER_ 0
#endif  

#if defined(__GNUC__) || defined(__GNUG__) || defined(__MINGW32__) || defined(__MINGW64__)
   #define ASTRO_COMPILER ASTRO_GCC_BUILD | ASTRO_COMPILER_
#elif defined(__clang__)
   #define ASTRO_COMPILER ASTRO_CLANG_BUILD | ASTRO_COMPILER_
#elif defined(__TI_COMPILER_VERSION__)
   #define ASTRO_COMPILER ASTRO_CL430_BUILD
#elif defined(_MSC_VER) || defined(_MSC_FULL_VER) || defined(_MSC_BUILD)
   #define ASTRO_COMPILER ASTRO_MSVC_BUILD
#else
   #define ASTRO_COMPILER ASTRO_COMPILER_
#endif

#if (ASTRO_COMPILER & ASTRO_GCC_BUILD) == ASTRO_GCC_BUILD
   #define ASTRO_COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif (ASTRO_COMPILER & ASTRO_MSVC_BUILD) == ASTRO_MSVC_BUILD
   #define ASTRO_COMPILER_VERSION _MSC_FULL_VER
#elif (ASTRO_COMPILER & ASTRO_CLANG_BUILD) == ASTRO_CLANG_BUILD
   #define ASTRO_COMPILER_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif (ASTRO_COMPILER & ASTRO_CL430_BUILD) == ASTRO_CL430_BUILD
   #define ASTRO_COMPILER_VERSION (__TI_COMPILER_VERSION__)
#elif (ASTRO_COMPILER & ASTRO_INTEL_BUILD) == ASTRO_INTEL_BUILD
   #if (ASTRO_COMPILER & ASTRO_CLANG_BUILD)  == ASTRO_CLANG_BUILD
      #define ASTRO_COMPILER_VERSION (__INTEL_COMPILER * 10000 + __INTEL_COMPILER_UPDATE * 100)
   #else
      #define ASTRO_COMPILER_VERSION (__ICC * 10000 + __INTEL_COMPILER_UPDATE * 100)
   #endif
#endif

#define ASTRO_LITTLE_ENDIAN 0x1
#define ASTRO_BIG_ENDIAN    0x2
#define ASTRO_PDP_ENDIAN    0x4

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
   #define ASTRO_BYTE_ORDER ASTRO_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
   #define ASTRO_BYTE_ORDER ASTRO_BIG_ENDIAN
#else
   #define ASTRO_BYTE_ORDER ASTRO_PDP_ENDIAN
#endif


#define ASTRO_X86_ARCH 0x1
#define ASTRO_AMD64_ARCH 0x2
#define ASTRO_ARM32_ARCH 0x4
#define ASTRO_ARM64_ARCH 0x8
#define ASTRO_SPARC32_ARCH 0x10
#define ASTRO_SPARC64_ARCH 0x20
#define ASTRO_MIPS32_ARCH 0x40
#define ASTRO_MIPS64_ARCH 0x80
#define ASTRO_PPC32_ARCH 0x100
#define ASTRO_PPC64_ARCH 0x200
#define ASTRO_RISCV32_ARCH 0x400
#define ASTRO_RISCV64_ARCH 0x800
#define ASTRO_S390_ARCH 0x1000
#define ASTRO_S390X_ARCH 0x2000
#define ASTRO_WASM32_ARCH 0x4000
#define ASTRO_WASM64_ARCH 0x8000

#if defined(__i386__) || defined(__i386) || defined(i386) || defined(__i486__) || defined(__i486) || defined(i486) || defined(__i586__) || defined(__i586) || defined(i586) || defined(__i686__) || defined(__i686) || defined(i686) || defined(__IA32__) || defined(__IA32) || defined(IA32) || defined(__X86__) || defined(__X86) || defined(X86) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__) || defined(__386)
   #define ASTRO_ARCH ASTRO_X64_ARCH
#elif defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(__amd64__) || defined(__amd64) || defined(amd64) || defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(_M_AMD64) 
   #define ASTRO_ARCH ASTRO_AMD64_ARCH
#elif defined(__arm__) || defined(__arm) || defined(arm) || defined(__ARM__) || defined(__ARM) || defined(ARM) || defined(__thumb__) || defined(__thumb) || defined(thumb) || defined(__THUMB__) || defined(__THUMB) || defined(THUMB) || defined(_M_ARM) || defined(_M_ARM_ARMV7VE) /**< Flag indicating whether the target architecture is ARM. */
   #define ASTRO_ARCH ASTRO_ARM32_ARCH
#elif defined(__aarch64__) || defined(__aarch64) || defined(aarch64) || defined(__ARM64__) || defined(__ARM64) || defined(ARM64) || defined(__arm64__) || defined(__arm64) || defined(arm64) || defined(_M_ARM64) || defined(_M_ARM64EC) /**< Flag indicating whether the target architecture is ARM64. */
   #define ASTRO_ARCH ASTRO_ARM64_ARCH
#elif defined(__sparc__) || defined(__sparc) || defined(sparc) /**< Flag indicating whether the target architecture is SPARC32. */
   #define ASTRO_ARCH ASTRO_SPARC32_ARCH
#elif defined(__sparc64__) || defined(__sparc64) || defined(sparc64) /**< Flag indicating whether the target architecture is SPARC64. */
   #define ASTRO_ARCH ASTRO_SPARC64_ARCH
#elif defined(__mips__) || defined(__mips) || defined(mips) /**< Flag indicating whether the target architecture is MIPS32. */
   #define ASTRO_ARCH ASTRO_MIPS32_ARCH
#elif defined(__mips64__) || defined(__mips64) || defined(mips64) /**< Flag indicating whether the target architecture is MIPS64. */
   #define ASTRO_ARCH ASTRO_MIPS64_ARCH
#elif defined(__powerpc__) || defined(__powerpc) || defined(powerpc) /**< Flag indicating whether the target architecture is PowerPC. */
   #define ASTRO_ARCH ASTRO_PPC32_ARCH
#elif defined(__powerpc64__) || defined(__powerpc64) || defined(powerpc64) /**< Flag indicating whether the target architecture is PowerPC64. */
   #define ASTRO_ARCH ASTRO_PPC64_ARCH
#elif defined(__riscv__) || defined(__riscv) || defined(riscv) /**< Flag indicating whether the target architecture is RISC-V32. */
   #define ASTRO_ARCH ASTRO_RISCV32_ARCH
#elif defined(__riscv64__) || defined(__riscv64) || defined(riscv64) /**< Flag indicating whether the target architecture is RISC-V64. */
   #define ASTRO_ARCH ASTRO_RISCV64_ARCH
#elif defined(__s390__) || defined(__s390) || defined(s390) /**< Flag indicating whether the target architecture is S390. */
   #define ASTRO_ARCH ASTRO_S390_ARCH
#elif defined(__s390x__) || defined(__s390x) || defined(s390x) /**< Flag indicating whether the target architecture is S390X. */
   #define ASTRO_ARCH ASTRO_S390X_ARCH
#elif defined(__EMSCRIPTEN__) || defined(__wasi__) || defined(__asmjs__) || defined(__wasm32__) /**< Flag indicating whether the target architecture is WebAssembly. */
   #define ASTRO_ARCH ASTRO_WASM32_ARCH
#elif defined(__wasm64__) /**< Flag indicating whether the target architecture is WebAssembly. */
   #define ASTRO_ARCH ASTRO_WASM64_ARCH
#endif


#define ASTRO_RELEASE_BUILD  0x1
#define ASTRO_DEBUG_BUILD    0x2
#define ASTRO_MIN_SIZE_BUILD 0x4
#define ASTRO_PROFILE_BUILD  0x8
#define ASTRO_TRACE_BUILD    0x10

#if defined(RELEASE) || defined(RELEASE_BUILD) || defined(NDEBUG) || defined(__NDEBUG) || defined(__NDEBUG__) || defined(_NDEBUG_)
   #define ASTRO_BUILD ASTRO_RELEASE_BUILD
#elif defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG) || defined(__DEBUG__) || defined(_DEBUG_)
   #define ASTRO_BUILD ASTRO_DEBUG_BUILD
#elif defined(RELWITHDEBINFO) || defined(RELEASE_WITH_DEBUG_INFO) 
   #define ASTRO_BUILD ASTRO_RELEASE_BUILD | ASTRO_DEBUG_BUILD
#elif defined(MINSIZEREL) || defined(MINSIZEREL_BUILD) || defined(__MINSIZEREL) || defined(__MINSIZEREL__) || defined(_MINSIZEREL_)
   #define ASTRO_BUILD ASTRO_RELEASE_BUILD | ASTRO_MIN_SIZE_BUILD
#elif defined(PROFILE) || defined(PROFILE_BUILD) || defined(__PROFILE) || defined(__PROFILE__) || defined(_PROFILE_)
   #define ASTRO_BUILD ASTRO_PROFILE_BUILD
#elif defined(TRACE) || defined(TRACE_BUILD) || defined(__TRACE) || defined(__TRACE__) || defined(_TRACE_)
   #define ASTRO_BUILD ASTRO_TRACE_BUILD
#else
   #define ASTRO_BUILD ASTRO_RELEASE_BUILD
#endif

#endif // ASTRO_INFO_PP_