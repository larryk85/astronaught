#pragma once

#include <cstdint>

#include <bit>
#include <stdexcept>

#include "build_info.pp" 

namespace astro::info {
   enum class architecture : uint32_t {
      unknown = 0x0,                 /**< Unknown architecture */
      x86     = ASTRO_X86_ARCH,      /**< x86 architecture */
      amd64   = ASTRO_AMD64_ARCH,    /**< x64 architecture */
      arm32   = ASTRO_ARM32_ARCH,    /**< ARM32 architecture */
      arm64   = ASTRO_ARM64_ARCH,    /**< ARM64 architecture */
      sparc32 = ASTRO_SPARC32_ARCH,  /**< SPARC32 architecture */
      sparc64 = ASTRO_SPARC64_ARCH,  /**< SPARC64 architecture */
      mips32  = ASTRO_MIPS32_ARCH,   /**< MIPS32 architecture */
      mips64  = ASTRO_MIPS64_ARCH,   /**< MIPS64 architecture */
      ppc32   = ASTRO_PPC32_ARCH,    /**< PPC32 architecture */
      ppc64   = ASTRO_PPC64_ARCH,    /**< PPC64 architecture */
      riscv32 = ASTRO_RISCV32_ARCH,  /**< RISC-V 32-bit architecture */
      riscv64 = ASTRO_RISCV64_ARCH,  /**< RISC-V 64-bit architecture */
      s390    = ASTRO_S390_ARCH,     /**< S390 architecture */
      s390x   = ASTRO_S390X_ARCH,    /**< S390X architecture */
      wasm32  = ASTRO_WASM32_ARCH,   /**< WebAssembly 32-bit architecture */
      wasm64  = ASTRO_WASM64_ARCH    /**< WebAssembly 64-bit architecture */
   };

   enum class operating_system : uint16_t {
      unknown = 0x0,                 /**< Unknown operating system */
      windows = ASTRO_WINDOWS_BUILD, /**< Windows operating system */
      macos   = ASTRO_MACOS_BUILD,   /**< macOS operating system */
      ios     = ASTRO_IOS_BUILD,     /**< iOS operating system */
      lnux    = ASTRO_LINUX_BUILD,   /**< Linux operating system */
      unx     = ASTRO_UNIX_BUILD,    /**< Unix operating system */
      bsd     = ASTRO_BSD_BUILD,     /**< BSD operating system */
      android = ASTRO_ANDROID_BUILD, /**< Android operating system */
      wasi    = ASTRO_WASI_BUILD,    /**< WebAssembly System Interface operating system */
      posix   = ASTRO_POSIX_BUILD    /**< POSIX operating system */
   };

   enum class compiler : uint16_t {
      unknown = 0x0,               /**< Unknown compiler */
      msvc    = ASTRO_MSVC_BUILD,  /**< Microsoft Visual C++ compiler */
      gcc     = ASTRO_GCC_BUILD,   /**< GNU Compiler Collection */
      clang   = ASTRO_CLANG_BUILD, /**< Clang compiler */
      cl430   = ASTRO_CL430_BUILD, /**< TI MSP430 C/C++ Compiler */
      intel   = ASTRO_INTEL_BUILD  /**< Intel C++ Compiler */
   };

   enum class build_type : uint16_t {
      unknown       = 0x0,                 /**< Unknown build */
      debug         = ASTRO_DEBUG_BUILD,   /**< Debug build */
      release       = ASTRO_RELEASE_BUILD, /**< Release build */
      profile       = ASTRO_PROFILE_BUILD, /**< Profiling build */
      trace         = ASTRO_TRACE_BUILD,   /**< Tracing build */
      minimum_size  = ASTRO_MIN_SIZE_BUILD /**< Minimum size build */
   };

   enum class byte_order : uint8_t {
      unknown = 0x0,                 /**< Unknown endianness */
      little  = ASTRO_LITTLE_ENDIAN, /**< Little-endian */
      big     = ASTRO_BIG_ENDIAN,    /**< Big-endian */
      pdp     = ASTRO_PDP_ENDIAN     /**< PDP-endian */
   };

   enum class language : uint8_t {
      unknown = 0x0,           /**< Unknown language */
      c       = ASTRO_C_LANG,  /**< C language */
      cpp     = ASTRO_CPP_LANG /**< C++ language */
   };

   struct build_info {
      architecture arch     = architecture::unknown;
      byte_order   order    = byte_order::unknown;
      operating_system os   = operating_system::unknown;
      compiler comp         = compiler::unknown;
      uint64_t comp_version = 0;
      language lang         = language::unknown;
      uint64_t lang_version = 0;
      build_type build      = build_type::unknown;
   };

   constexpr static inline build_info build_info_v = 
      {
         static_cast<architecture>(ASTRO_ARCH), 
         static_cast<byte_order>(ASTRO_BYTE_ORDER), 
         static_cast<operating_system>(ASTRO_OS), 
         static_cast<compiler>(ASTRO_COMPILER), 
         ASTRO_COMPILER_VERSION, 
         static_cast<language>(ASTRO_LANG), 
         ASTRO_LANG_VERSION, 
         static_cast<build_type>(ASTRO_BUILD)
      };
} // namespace astro::info