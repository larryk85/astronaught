/**
 * @file constants.h.in
 * @brief Header file containing constants for the library version.
 *
 * This file defines various constants related to the library version, such as the namespace,
 * major version, minor version, patch version, tweak version, suffix, and git hash.
 * These constants are used throughout the library for versioning purposes.
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

/**
 * This file defines the VERSA_VERSION_STRING based on the values of LV_USE_SUFFIX and LV_USE_GIT_HASH constants.
 * If both VERSA_USE_SUFFIX and LV_USE_GIT_HASH are set to 1, the version string will include the suffix and git hash.
 * If only VERSA_USE_SUFFIX is set to 1, the version string will include the suffix.
 * If only VERSA_USE_GIT_HASH is set to 1, the version string will include the git hash.
 * If neither VERSA_USE_SUFFIX nor LV_USE_GIT_HASH are set to 1, the version string will only include the major, minor, patch, and tweak numbers.
 */
#if (VERSA_USE_SUFFIX == 1 && LV_USE_GIT_HASH == 1)
   #define VERSA_VERSION_STRING "@LV_MAJOR@.@LV_MINOR@.@LV_PATCH@.@LV_TWEAK@-@LV_SUFFIX@ (@LV_GIT_HASH@)"
#elif (VERSA_USE_SUFFIX == 1)
   #define VERSA_VERSION_STRING "@LV_MAJOR@.@LV_MINOR@.@LV_PATCH@.@LV_TWEAK@-@LV_SUFFIX@"
#elif (VERSA_USE_GIT_HASH == 1)
   #define VERSA_VERSION_STRING "@LV_MAJOR@.@LV_MINOR@.@LV_PATCH@.@LV_TWEAK@ (@LV_GIT_HASH@)"
#else
   #define VERSA_VERSION_STRING "@LV_MAJOR@.@LV_MINOR@.@LV_PATCH@.@LV_TWEAK@"
#endif

/**
 * The constants defined in this file are used throughout the project to control
 * the build behavior based on whether the code is being compiled as C++ or not.
 * If the code is being compiled as C++, the macro VERSA_CPP_BUILD is defined as 1,
 * otherwise it is defined as 0.
 */
#define VERSA_C_BUILD !defined(__cplusplus)
#define VERSA_CPP_BUILD defined(__cplusplus)

/**
 * The constants defined in this file indicate the target operating system for the build.
 * These constants can be used to conditionally compile different sections of code based on the target platform.
 * The constants are defined as follows:
 * - VERSA_WINDOWS_BUILD: Set to 1 if the target platform is Windows, otherwise set to 0.
 * - VERSA_APPLE_BUILD: Set to 1 if the target platform is Apple (macOS, iOS), otherwise set to 0.
 * - VERSA_LINUX_BUILD: Set to 1 if the target platform is Linux, otherwise set to 0.
 * - VERSA_UNIX_BUILD: Set to 1 if the target platform is Unix-like (Linux, macOS, BSD), otherwise set to 0.
 * - VERSA_BSD_BUILD: Set to 1 if the target platform is BSD (FreeBSD, NetBSD, OpenBSD, DragonFlyBSD), otherwise set to 0.
 * - VERSA_ANDROID_BUILD: Set to 1 if the target platform is Android, otherwise set to 0.
 * - VERSA_WASI_BUILD: Set to 1 if the target platform is WASI, otherwise set to 0.
 * - VERSA_POSIX_BUILD: Set to 1 if the target platform is POSIX, otherwise set to 0.
 * - VERSA_UNKNOWN_BUILD: Set to 1 if the target platform is unknown, otherwise set to 0.
 *
 * The constants are defined using preprocessor directives based on the target operating system.
 * If none of the preprocessor directives match, the VERSA_UNKNOWN_BUILD constant is set to 1.
 */
#define VERSA_WINDOWS_BUILD defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__) || defined(__NT__) || defined(WIN32) || defined(WIN64) || defined(_WIN32_WCE) || defined(_WIN32_WCE_EMULATION) || defined(_WIN32_WCE_PSPC)
#if defined(__APPLE__)
   #include <TargetConditionals.h>
#endif
#define VERSA_MACOS_BUILD #defined(__APPLE__) && defined(TARGET_OS_MAC)
#define VERSA_IOS_BUILD #defined(__APPLE__) && (defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR))
#define VERSA_LINUX_BUILD defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#define VERSA_UNIX_BUILD defined(__unix__) || defined(__unix) || defined(unix) || defined(_POSIX_VERSION)
#define VERSA_BSD_BUILD defined(__FREEBSD__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__) || defined(__BSD__)
#define VERSA_ANDROID_BUILD defined(__ANDROID__) || defined(ANDROID)
#define VERSA_WASI_BUILD defined(__wasi__) || defined(__wasi)
#define VERSA_POSIX_BUILD defined(_POSIX_VERSION)
#define VERSA_UNKNOWN_OS !defined(VERSA_WINDOWS_BUILD) && !defined(VERSA_MACOS_BUILD) && !defined(VERSA_IOS_BUILD) && !defined(VERSA_LINUX_BUILD) && !defined(VERSA_UNIX_BUILD) && !defined(VERSA_BSD_BUILD) && !defined(VERSA_ANDROID_BUILD) && !defined(VERSA_WASI_BUILD) && !defined(VERSA_POSIX_BUILD)

/**
 * This file defines various constants based on the compiler being used.
 * It checks for the presence of specific compiler macros and sets the corresponding
 * constants accordingly.
 */
#define VERSA_GCC_BUILD defined(__GNUC__) || defined(__GNUG__) || defined(__MINGW32__) || defined(__MINGW64__)
#define VERSA_MSVC_BUILD defined(_MSC_VER) || defined(_MSC_FULL_VER) || defined(_MSC_BUILD)
#define VERSA_CLANG_BUILD defined(__clang__) || defined(__INTEL_LLVM_COMPILER)
#define VERSA_INTEL_BUILD defined(__INTEL_COMPILER) || defined(__ICC) || defined(__INTEL_COMPILER_UPDATE)
#define VERSA_CL430_BUILD defined(__TI_COMPILER_VERSION__)
#define VERSA_UNKNOWN_COMPILER !defined(VERSA_GCC_BUILD) && !defined(VERSA_MSVC_BUILD) && !defined(VERSA_CLANG_BUILD) && !defined(VERSA_INTEL_BUILD) && !defined(VERSA_CL430_BUILD)

#ifdef VERSA_GCC_BUILD
   #define VERSA_COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif VERSA_MSVC_BUILD
   #define VERSA_COMPILER_VERSION _MSC_FULL_VER
#elif VERSA_CLANG_BUILD
   #define VERSA_COMPILER_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif VERSA_CL430_BUILD
   #define VERSA_COMPILER_VERSION (__TI_COMPILER_VERSION__)
#elif VERSA_INTEL_BUILD
   #if defined(__INTEL_COMPILER)
      #define VERSA_COMPILER_VERSION (__INTEL_COMPILER * 10000 + __INTEL_COMPILER_UPDATE * 100)
   #else
      #define VERSA_COMPILER_VERSION (__ICC * 10000 + __INTEL_COMPILER_UPDATE * 100)
   #endif
#else
   #define VERSA_COMPILER_VERSION 0
#endif



/**
 * This file contains preprocessor directives that define constants for
 * determining the endianness of the system. It checks the value of the
 * __BYTE_ORDER__ macro and sets the VERSA_LITTLE_ENDIAN and LV_BIG_ENDIAN
 * macros accordingly.
 *
 * If the system is little-endian, VERSA_LITTLE_ENDIAN is set to 1 and
 * VERSA_BIG_ENDIAN is set to 0. Otherwise, if the system is big-endian,
 * VERSA_LITTLE_ENDIAN is set to 0 and LV_BIG_ENDIAN is set to 1.
 */
#define VERSA_LITTLE_ENDIAN (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)*0x1 /**< Flag indicating whether the system is little-endian. */  
#define VERSA_BIG_ENDIAN (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)*0x2 /**< Flag indicating whether the system is big-endian. */
#define VERSA_BYTE_ORDER VERSA_LITTLE_ENDIAN + VERSA_BIG_ENDIAN /**< The byte order of the system. */

#define VERSA_C_VERSION (__STDC_VERSION__) /**< The version of the C standard used by the compiler. */
#define VERSA_CPP_VERSION (__cplusplus) /**< The version of the C++ standard used by the compiler. */

/**
 * This file contains a series of conditional statements that define build flags
 * based on the target architecture. Each architecture has a set of flags that
 * are set to 1 if the architecture matches, and 0 otherwise. These flags can be
 * used to conditionally compile different parts of the code based on the target
 * architecture.
 *
 * The following architectures are supported:
 * - x86
 * - x64
 * - ARM
 * - ARM64
 * - SPARC32
 * - SPARC64
 * - MIPS32
 * - MIPS64
 * - PowerPC
 * - PowerPC64
 * - RISC-V32
 * - RISC-V64
 * - S390
 * - S390X
 * - WebAssembly (WASM32 and WASM64)
 * - Unknown
 *
 * The build flags are defined as macros, which can be used in conditional
 * compilation statements. For example, to conditionally compile code for x86
 * architecture, you can use the macro VERSA_X86_BUILD.
 *
 * Example usage:
 * ```
 * #ifdef VERSA_X86_BUILD
 *     // Code specific to x86 architecture
 * #endif
 * ```
 */
#define VERSA_X86_BUILD defined(__i386__) || defined(__i386) || defined(i386) || defined(__i486__) || defined(__i486) || defined(i486) || defined(__i586__) || defined(__i586) || defined(i586) || defined(__i686__) || defined(__i686) || defined(i686) || defined(__IA32__) || defined(__IA32) || defined(IA32) || defined(__X86__) || defined(__X86) || defined(X86) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__) || defined(__386) /**< Flag indicating whether the target architecture is x86. */
#define VERSA_X64_BUILD defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(__amd64__) || defined(__amd64) || defined(amd64) || defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(__x86_64__) || defined(__x86_64) || defined(x86_64) || defined(__x86_64__) || defined(__x86_64) || defined(x86_64) /**< Flag indicating whether the target architecture is x64. */
#define VERSA_ARM32_BUILD defined(__arm__) || defined(__arm) || defined(arm) || defined(__ARM__) || defined(__ARM) || defined(ARM) || defined(__thumb__) || defined(__thumb) || defined(thumb) || defined(__THUMB__) || defined(__THUMB) || defined(THUMB) /**< Flag indicating whether the target architecture is ARM. */
#define VERSA_ARM64_BUILD defined(__aarch64__) || defined(__aarch64) || defined(aarch64) || defined(__ARM64__) || defined(__ARM64) || defined(ARM64) || defined(__arm64__) || defined(__arm64) || defined(arm64) /**< Flag indicating whether the target architecture is ARM64. */
#define VERSA_SPARC32_BUILD defined(__sparc__) || defined(__sparc) || defined(sparc) /**< Flag indicating whether the target architecture is SPARC32. */
#define VERSA_SPARC64_BUILD defined(__sparc64__) || defined(__sparc64) || defined(sparc64) /**< Flag indicating whether the target architecture is SPARC64. */
#define VERSA_MIPS32_BUILD defined(__mips__) || defined(__mips) || defined(mips) /**< Flag indicating whether the target architecture is MIPS32. */
#define VERSA_MIPS64_BUILD defined(__mips64__) || defined(__mips64) || defined(mips64) /**< Flag indicating whether the target architecture is MIPS64. */
#define VERSA_PPC32_BUILD defined(__powerpc__) || defined(__powerpc) || defined(powerpc) /**< Flag indicating whether the target architecture is PowerPC. */
#define VERSA_PPC64_BUILD defined(__powerpc64__) || defined(__powerpc64) || defined(powerpc64) /**< Flag indicating whether the target architecture is PowerPC64. */
#define VERSA_RISCV32_BUILD defined(__riscv__) || defined(__riscv) || defined(riscv) /**< Flag indicating whether the target architecture is RISC-V32. */
#define VERSA_RISCV64_BUILD defined(__riscv64__) || defined(__riscv64) || defined(riscv64) /**< Flag indicating whether the target architecture is RISC-V64. */
#define VERSA_S390_BUILD defined(__s390__) || defined(__s390) || defined(s390) /**< Flag indicating whether the target architecture is S390. */
#define VERSA_S390X_BUILD defined(__s390x__) || defined(__s390x) || defined(s390x) /**< Flag indicating whether the target architecture is S390X. */
#define VERSA_WASM32_BUILD defined(__EMSCRIPTEN__) || defined(__wasi__) || defined(__asmjs__) || defined(__wasm32__) /**< Flag indicating whether the target architecture is WebAssembly. */
#define VERSA_WASM64_BUILD defined(__wasm64__) /**< Flag indicating whether the target architecture is WebAssembly. */)
#define VERSA_UNKNOWN_BUILD !defined(VERSA_X86_BUILD) && !defined(VERSA_X64_BUILD) && !defined(VERSA_ARM32_BUILD) && !defined(VERSA_ARM64_BUILD) && !defined(VERSA_SPARC32_BUILD) && !defined(VERSA_SPARC64_BUILD) && !defined(VERSA_MIPS32_BUILD) && !defined(VERSA_MIPS64_BUILD) && !defined(VERSA_PPC32_BUILD) && !defined(VERSA_PPC64_BUILD) && !defined(VERSA_RISCV32_BUILD) && !defined(VERSA_RISCV64_BUILD) && !defined(VERSA_S390_BUILD) && !defined(VERSA_S390X_BUILD) && !defined(VERSA_WASM32_BUILD) && !defined(VERSA_WASM64_BUILD)

/**
 * The defined macros represent different build configurations for the Versa library.
 * These configurations include release builds, debug builds, release builds with debug info,
 * minimum size release builds, profile builds, and trace builds.
 *
 * The macros are defined based on various preprocessor definitions such as RELEASE, DEBUG, RELWITHDEBINFO, MINSIZEREL, PROFILE, and TRACE.
 * These definitions are commonly used in build systems to specify the desired build configuration.
 *
 * @note This file is included in the Versa library and should not be modified directly.
 */
#define VERSA_RELEASE_BUILD defined(RELEASE) || defined(RELEASE_BUILD) || defined(NDEBUG) || defined(__NDEBUG) || defined(__NDEBUG__) || defined(_NDEBUG_)
#define VERSA_DEBUG_BUILD defined(DEBUG) || defined(_DEBUG) || defined(__DEBUG) || defined(__DEBUG__) || defined(_DEBUG_)
#define VERSA_RELEASE_WITH_DEBUG_INFO defined(RELWITHDEBINFO) || defined(RELEASE_WITH_DEBUG_INFO) 
#define VERSA_MIN_SIZE_RELEASE_BUILD defined(MINSIZEREL) || defined(MINSIZEREL_BUILD) || defined(__MINSIZEREL) || defined(__MINSIZEREL__) || defined(_MINSIZEREL_)
#define VERSA_PROFILE_BUILD defined(PROFILE) || defined(PROFILE_BUILD) || defined(__PROFILE) || defined(__PROFILE__) || defined(_PROFILE_)
#define VERSA_TRACE_BUILD defined(TRACE) || defined(TRACE_BUILD) || defined(__TRACE) || defined(__TRACE__) || defined(_TRACE_)
