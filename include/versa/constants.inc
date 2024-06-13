/**
 * @file constants.inc
 * @brief This file contains constants and macros related to the library version, platform, compiler, architecture, and byte order.
 */

/**
 * @def VERSA_NAMESPACE
 * @brief The namespace used in the library.
 */

/**
 * @def VERSA_NAMESPACE_X(NM)
 * @brief Macro to concatenate the namespace with another name.
 * @param NM The name to concatenate with the namespace.
 */

/**
 * @def VERSA_MAJOR_VERSION
 * @brief The major version number of the library.
 */

/**
 * @def VERSA_MINOR_VERSION
 * @brief The minor version number of the library.
 */

/**
 * @def VERSA_PATCH_VERSION
 * @brief The patch version number of the library.
 */

/**
 * @def VERSA_TWEAK_VERSION
 * @brief The tweak version number of the library.
 */

/**
 * @def VERSA_USE_SUFFIX
 * @brief Flag indicating whether a suffix is used in the version number.
 */

/**
 * @def VERSA_USE_GIT_HASH
 * @brief Flag indicating whether the git hash is used in the version number.
 */

/**
 * @def VERSA_VERSION_SUFFIX
 * @brief The suffix used in the version number.
 */

/**
 * @def VERSA_GIT_HASH
 * @brief The git hash of the library.
 */

/**
 * @def VERSA_VERSION_STRING
 * @brief The version string of the library, including the major, minor, patch, tweak, suffix, and git hash.
 */

/**
 * @def VERSA_C_LANG
 * @brief Flag indicating C language.
 */

/**
 * @def VERSA_CPP_LANG
 * @brief Flag indicating C++ language.
 */

/**
 * @def VERSA_LANG
 * @brief The language used in the library.
 */

/**
 * @def VERSA_LANG_VERSION
 * @brief The version of the language used in the library.
 */

/**
 * @def VERSA_WINDOWS_BUILD
 * @brief Flag indicating Windows build.
 */

/**
 * @def VERSA_MACOS_BUILD
 * @brief Flag indicating macOS build.
 */

/**
 * @def VERSA_IOS_BUILD
 * @brief Flag indicating iOS build.
 */

/**
 * @def VERSA_LINUX_BUILD
 * @brief Flag indicating Linux build.
 */

/**
 * @def VERSA_UNIX_BUILD
 * @brief Flag indicating Unix build.
 */

/**
 * @def VERSA_BSD_BUILD
 * @brief Flag indicating BSD build.
 */

/**
 * @def VERSA_ANDROID_BUILD
 * @brief Flag indicating Android build.
 */

/**
 * @def VERSA_WASI_BUILD
 * @brief Flag indicating WASI build.
 */

/**
 * @def VERSA_POSIX_BUILD
 * @brief Flag indicating POSIX build.
 */

/**
 * @def VERSA_OS
 * @brief The operating system used in the library.
 */

/**
 * @def VERSA_GCC_BUILD
 * @brief Flag indicating GCC build.
 */

/**
 * @def VERSA_MSVC_BUILD
 * @brief Flag indicating MSVC build.
 */

/**
 * @def VERSA_CLANG_BUILD
 * @brief Flag indicating Clang build.
 */

/**
 * @def VERSA_INTEL_BUILD
 * @brief Flag indicating Intel build.
 */

/**
 * @def VERSA_CL430_BUILD
 * @brief Flag indicating CL430 build.
 */

/**
 * @def VERSA_COMPILER
 * @brief The compiler used in the library.
 */

/**
 * @def VERSA_COMPILER_VERSION
 * @brief The version of the compiler used in the library.
 */

/**
 * @def VERSA_LITTLE_ENDIAN
 * @brief Flag indicating little endian byte order.
 */

/**
 * @def VERSA_BIG_ENDIAN
 * @brief Flag indicating big endian byte order.
 */

/**
 * @def VERSA_PDP_ENDIAN
 * @brief Flag indicating PDP endian byte order.
 */

/**
 * @def VERSA_BYTE_ORDER
 * @brief The byte order used in the library.
 */

/**
 * @def VERSA_X86_ARCH
 * @brief Flag indicating x86 architecture.
 */

/**
 * @def VERSA_AMD64_ARCH
 * @brief Flag indicating AMD64 architecture.
 */

/**
 * @def VERSA_ARM32_ARCH
 * @brief Flag indicating ARM32 architecture.
 */

/**
 * @def VERSA_ARM64_ARCH
 * @brief Flag indicating ARM64 architecture.
 */

/**
 * @def VERSA_SPARC32_ARCH
 * @brief Flag indicating SPARC32 architecture.
 */

/**
 * @def VERSA_SPARC64_ARCH
 * @brief Flag indicating SPARC64 architecture.
 */

/**
 * @def VERSA_MIPS32_ARCH
 * @brief Flag indicating MIPS32 architecture.
 */

/**
 * @def VERSA_MIPS64_ARCH
 * @brief Flag indicating MIPS64 architecture.
 */

/**
 * @def VERSA_PPC32_ARCH
 * @brief Flag indicating PowerPC32 architecture.
 */

/**
 * @def VERSA_PPC64_ARCH
 * @brief Flag indicating PowerPC64 architecture.
 */

/**
 * @def VERSA_RISCV32_ARCH
 * @brief Flag indicating RISC-V32 architecture.
 */

/**
 * @def VERSA_RISCV64_ARCH
 * @brief Flag indicating RISC-V64 architecture.
 */

/**
 * @def VERSA_S390_ARCH
 * @brief Flag indicating S390 architecture.
 */

/**
 * @def VERSA_S390X_ARCH
 * @brief Flag indicating S390X architecture.
 */

/**
 * @def VERSA_WASM32_ARCH
 * @brief Flag indicating WebAssembly32 architecture.
 */

/**
 * @def VERSA_WASM64_ARCH
 * @brief Flag indicating WebAssembly64 architecture.
 */

/**
 * @def VERSA_ARCH
 * @brief The architecture used in the library.
 */

#define VERSA_NAMESPACE       _VERSA_PROJECT_NAMESPACE        /**< The namespace used in the library. */
#define VERSA_NAMESPACE_X(NM) VERSA_NAMESPACE ## _ ## NM      /**< Macro to concatenate the namespace with another name. */
#define VERSA_MAJOR_VERSION   _VERSA_PROJECT_MAJOR_VERSION    /**< The major version number of the library. */
#define VERSA_MINOR_VERSION   _VERSA_PROJECT_MINOR_VERSION    /**< The minor version number of the library. */
#define VERSA_PATCH_VERSION   _VERSA_PROJECT_PATCH_VERSION    /**< The patch version number of the library. */
#define VERSA_TWEAK_VERSION   _VERSA_PROJECT_TWEAK_VERSION    /**< The tweak version number of the library. */
#define VERSA_USE_SUFFIX      _VERSA_PROJECT_USE_SUFFIX       /**< Flag indicating whether a suffix is used in the version number. */
#define VERSA_USE_GIT_HASH    _VERSA_PROJECT_USE_GIT_HASH     /**< Flag indicating whether the git hash is used in the version number. */
#define VERSA_VERSION_SUFFIX  _VERSA_PROJECT_VERSION_SUFFIX   /**< The suffix used in the version number. */
#define VERSA_GIT_HASH        _VERSA_PROJECT_VERSION_GIT_HASH /**< The git hash of the library. */

#if (VERSA_USE_SUFFIX == 1 && LV_USE_GIT_HASH == 1)
   #define VERSA_VERSION_STRING "@LV_MAJOR@.@LV_MINOR@.@LV_PATCH@.@LV_TWEAK@-@LV_SUFFIX@ (@LV_GIT_HASH@)"
#elif (VERSA_USE_SUFFIX == 1)
   #define VERSA_VERSION_STRING "@LV_MAJOR@.@LV_MINOR@.@LV_PATCH@.@LV_TWEAK@-@LV_SUFFIX@"
#elif (VERSA_USE_GIT_HASH == 1)
   #define VERSA_VERSION_STRING "@LV_MAJOR@.@LV_MINOR@.@LV_PATCH@.@LV_TWEAK@ (@LV_GIT_HASH@)"
#else
   #define VERSA_VERSION_STRING "@LV_MAJOR@.@LV_MINOR@.@LV_PATCH@.@LV_TWEAK@"
#endif

#define VERSA_C_LANG      0x1
#define VERSA_CPP_LANG    0x2

#if defined(__cplusplus)
   #define VERSA_LANG         VERSA_CPP_LANG
   #define VERSA_LANG_VERSION __cplusplus
#elif defined(__STDC_VERSION__)
   #define VERSA_LANG         VERSA_C_LANG
   #define VERSA_LANG_VERSION __STDC_VERSION__
#endif

#define VERSA_WINDOWS_BUILD 0x1
#define VERSA_MACOS_BUILD   0x2
#define VERSA_IOS_BUILD     0x4
#define VERSA_LINUX_BUILD   0x8
#define VERSA_UNIX_BUILD    0x10
#define VERSA_BSD_BUILD     0x20
#define VERSA_ANDROID_BUILD 0x40
#define VERSA_WASI_BUILD    0x80
#define VERSA_POSIX_BUILD   0x100

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || defined(__NT__) || defined(WIN32) || defined(WIN64) || defined(_WIN32_WCE) || defined(_WIN32_WCE_EMULATION) || defined(_WIN32_WCE_PSPC)
   #define VERSA_OS VERSA_WINDOWS_BUILD
#elif defined(__APPLE__)
   #include <TargetConditional.h>
   #if defined(TARGET_OS_MAC)
      #define VERSA_OS VERSA_MACOS_BUILD
   #elif defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
      #define VERSA_OS VERSA_IOS_BUILD
   #endif
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
   #define VERSA_OS VERSA_LINUX_BUILD
#elif defined(__unix__) || defined(__unix) || defined(unix) || defined(_POSIX_VERSION)
   #define VERSA_OS VERSA_UNIX_BUILD
#elif defined(__FREEBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__) || defined(__BSD__)
   #define VERSA_OS VERSA_BSD_BUILD
#elif defined(__ANDROID__) || defined(ANDROID)
   #define VERSA_OS VERSA_ANDROID_BUILD
#elif defined(__wasi__) || defined(__wasi)
   #define VERSA_OS VERSA_WASI_BUILD
#elif defined(_POSIX_VERSION)
   #define VERSA_OS VERSA_POSIX_BUILD
#endif

#define VERSA_GCC_BUILD 0x1
#define VERSA_MSVC_BUILD 0x2
#define VERSA_CLANG_BUILD 0x4
#define VERSA_INTEL_BUILD 0x8
#define VERSA_CL430_BUILD 0x10

#if defined(__INTEL_LLVM_COMPILER) || defined(__INTEL_COMPILER) || defined(__ICC) || defined(__INTEL_COMPILER_UPDATE)
   #define VERSA_COMPILER_ VERSA_INTEL_BUILD
#else
   #define VERSA_COMPILER_ 0
#endif  

#if defined(__GNUC__) || defined(__GNUG__) || defined(__MINGW32__) || defined(__MINGW64__)
   #define VERSA_COMPILER VERSA_GCC_BUILD | VERSA_COMPILER_
#elif defined(__clang__)
   #define VERSA_COMPILER VERSA_CLANG_BUILD | VERSA_COMPILER_
#elif defined(__TI_COMPILER_VERSION__)
   #define VERSA_COMPILER VERSA_CL430_BUILD
#elif defined(_MSC_VER) || defined(_MSC_FULL_VER) || defined(_MSC_BUILD)
   #define VERSA_COMPILER VERSA_MSVC_BUILD
#else
   #define VERSA_COMPILER VERSA_COMPILER_
#endif

#if (VERSA_COMPILER & VERSA_GCC_BUILD) == VERSA_GCC_BUILD
   #define VERSA_COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif (VERSA_COMPILER & VERSA_MSVC_BUILD) == VERSA_MSVC_BUILD
   #define VERSA_COMPILER_VERSION _MSC_FULL_VER
#elif (VERSA_COMPILER & VERSA_CLANG_BUILD) == VERSA_CLANG_BUILD
   #define VERSA_COMPILER_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif (VERSA_COMPILER & VERSA_CL430_BUILD) == VERSA_CL430_BUILD
   #define VERSA_COMPILER_VERSION (__TI_COMPILER_VERSION__)
#elif (VERSA_COMPILER & VERSA_INTEL_BUILD) == VERSA_INTEL_BUILD
   #if (VERSA_COMPILER & VERSA_CLANG_BUILD)  == VERSA_CLANG_BUILD
      #define VERSA_COMPILER_VERSION (__INTEL_COMPILER * 10000 + __INTEL_COMPILER_UPDATE * 100)
   #else
      #define VERSA_COMPILER_VERSION (__ICC * 10000 + __INTEL_COMPILER_UPDATE * 100)
   #endif
#endif

#define VERSA_LITTLE_ENDIAN 0x1
#define VERSA_BIG_ENDIAN    0x2
#define VERSA_PDP_ENDIAN    0x4

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
   #define VERSA_BYTE_ORDER VERSA_LITTLE_ENDIAN
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
   #define VERSA_BYTE_ORDER VERSA_BIG_ENDIAN
#else
   #define VERSA_BYTE_ORDER VERSA_PDP_ENDIAN
#endif


#define VERSA_X86_ARCH 0x1
#define VERSA_AMD64_ARCH 0x2
#define VERSA_ARM32_ARCH 0x4
#define VERSA_ARM64_ARCH 0x8
#define VERSA_SPARC32_ARCH 0x10
#define VERSA_SPARC64_ARCH 0x20
#define VERSA_MIPS32_ARCH 0x40
#define VERSA_MIPS64_ARCH 0x80
#define VERSA_PPC32_ARCH 0x100
#define VERSA_PPC64_ARCH 0x200
#define VERSA_RISCV32_ARCH 0x400
#define VERSA_RISCV64_ARCH 0x800
#define VERSA_S390_ARCH 0x1000
#define VERSA_S390X_ARCH 0x2000
#define VERSA_WASM32_ARCH 0x4000
#define VERSA_WASM64_ARCH 0x8000

#if defined(__i386__) || defined(__i386) || defined(i386) || defined(__i486__) || defined(__i486) || defined(i486) || defined(__i586__) || defined(__i586) || defined(i586) || defined(__i686__) || defined(__i686) || defined(i686) || defined(__IA32__) || defined(__IA32) || defined(IA32) || defined(__X86__) || defined(__X86) || defined(X86) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__) || defined(__386)
   #define VERSA_ARCH VERSA_X64_ARCH
#elif defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(__amd64__) || defined(__amd64) || defined(amd64) || defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(_M_AMD64) 
   #define VERSA_ARCH VERSA_AMD64_ARCH
#elif defined(__arm__) || defined(__arm) || defined(arm) || defined(__ARM__) || defined(__ARM) || defined(ARM) || defined(__thumb__) || defined(__thumb) || defined(thumb) || defined(__THUMB__) || defined(__THUMB) || defined(THUMB) || defined(_M_ARM) || defined(_M_ARM_ARMV7VE) /**< Flag indicating whether the target architecture is ARM. */
   #define VERSA_ARCH VERSA_ARM32_ARCH
#elif defined(__aarch64__) || defined(__aarch64) || defined(aarch64) || defined(__ARM64__) || defined(__ARM64) || defined(ARM64) || defined(__arm64__) || defined(__arm64) || defined(arm64) || defined(_M_ARM64) || defined(_M_ARM64EC) /**< Flag indicating whether the target architecture is ARM64. */
   #define VERSA_ARCH VERSA_ARM64_ARCH
#elif defined(__sparc__) || defined(__sparc) || defined(sparc) /**< Flag indicating whether the target architecture is SPARC32. */
   #define VERSA_ARCH VERSA_SPARC32_ARCH
#elif defined(__sparc64__) || defined(__sparc64) || defined(sparc64) /**< Flag indicating whether the target architecture is SPARC64. */
   #define VERSA_ARCH VERSA_SPARC64_ARCH
#elif defined(__mips__) || defined(__mips) || defined(mips) /**< Flag indicating whether the target architecture is MIPS32. */
   #define VERSA_ARCH VERSA_MIPS32_ARCH
#elif defined(__mips64__) || defined(__mips64) || defined(mips64) /**< Flag indicating whether the target architecture is MIPS64. */
   #define VERSA_ARCH VERSA_MIPS64_ARCH
#elif defined(__powerpc__) || defined(__powerpc) || defined(powerpc) /**< Flag indicating whether the target architecture is PowerPC. */
   #define VERSA_ARCH VERSA_PPC32_ARCH
#elif defined(__powerpc64__) || defined(__powerpc64) || defined(powerpc64) /**< Flag indicating whether the target architecture is PowerPC64. */
   #define VERSA_ARCH VERSA_PPC64_ARCH
#elif defined(__riscv__) || defined(__riscv) || defined(riscv) /**< Flag indicating whether the target architecture is RISC-V32. */
   #define VERSA_ARCH VERSA_RISCV32_ARCH
#elif defined(__riscv64__) || defined(__riscv64) || defined(riscv64) /**< Flag indicating whether the target architecture is RISC-V64. */
   #define VERSA_ARCH VERSA_RISCV64_ARCH
#elif defined(__s390__) || defined(__s390) || defined(s390) /**< Flag indicating whether the target architecture is S390. */
   #define VERSA_ARCH VERSA_S390_ARCH
#elif defined(__s390x__) || defined(__s390x) || defined(s390x) /**< Flag indicating whether the target architecture is S390X. */
   #define VERSA_ARCH VERSA_S390X_ARCH
#elif defined(__EMSCRIPTEN__) || defined(__wasi__) || defined(__asmjs__) || defined(__wasm32__) /**< Flag indicating whether the target architecture is WebAssembly. */
   #define VERSA_ARCH VERSA_WASM32_ARCH
#elif defined(__wasm64__) /**< Flag indicating whether the target architecture is WebAssembly. */
   #define VERSA_ARCH VERSA_WASM64_ARCH
#endif


#define VERSA_RELEASE_BUILD  0x1
#define VERSA_DEBUG_BUILD    0x2
#define VERSA_MIN_SIZE_BUILD 0x4
#define VERSA_PROFILE_BUILD  0x8
#define VERSA_TRACE_BUILD    0x10

#if defined(RELEASE) || defined(RELEASE_BUILD) || defined(NDEBUG) || defined(__NDEBUG) || defined(__NDEBUG__) || defined(_NDEBUG_)
   #define VERSA_BUILD VERSA_RELEASE_BUILD
#elif defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG) || defined(__DEBUG__) || defined(_DEBUG_)
   #define VERSA_BUILD VERSA_DEBUG_BUILD
#elif defined(RELWITHDEBINFO) || defined(RELEASE_WITH_DEBUG_INFO) 
   #define VERSA_BUILD VERSA_RELEASE_BUILD | VERSA_DEBUG_BUILD
#elif defined(MINSIZEREL) || defined(MINSIZEREL_BUILD) || defined(__MINSIZEREL) || defined(__MINSIZEREL__) || defined(_MINSIZEREL_)
   #define VERSA_BUILD VERSA_RELEASE_BUILD | VERSA_MIN_SIZE_BUILD
#elif defined(PROFILE) || defined(PROFILE_BUILD) || defined(__PROFILE) || defined(__PROFILE__) || defined(_PROFILE_)
   #define VERSA_BUILD VERSA_PROFILE_BUILD
#elif defined(TRACE) || defined(TRACE_BUILD) || defined(__TRACE) || defined(__TRACE__) || defined(_TRACE_)
   #define VERSA_BUILD VERSA_TRACE_BUILD
#else
   #define VERSA_BUILD VERSA_RELEASE_BUILD
#endif