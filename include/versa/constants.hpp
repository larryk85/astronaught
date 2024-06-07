#pragma once

#include <cstdint>

#include <bit>
#include <stdexcept>

#include <versa/constants.inc>

namespace versa::info {
   enum class architectures : uint32_t {
      unknown = 0x0,    /**< Unknown architecture */
      x86     = 0x1,    /**< x86 architecture */
      x64     = 0x2,    /**< x64 architecture */
      arm32   = 0x4,    /**< ARM32 architecture */
      arm64   = 0x8,    /**< ARM64 architecture */
      sparc32 = 0x10,   /**< SPARC32 architecture */
      sparc64 = 0x20,   /**< SPARC64 architecture */
      mips32  = 0x40,   /**< MIPS32 architecture */
      mips64  = 0x80,   /**< MIPS64 architecture */
      ppc32   = 0x100,  /**< PPC32 architecture */
      ppc64   = 0x200,  /**< PPC64 architecture */
      riscv32 = 0x400,  /**< RISC-V 32-bit architecture */
      riscv64 = 0x800,  /**< RISC-V 64-bit architecture */
      s390    = 0x1000, /**< S390 architecture */
      s390x   = 0x2000, /**< S390X architecture */
      wasm32  = 0x4000, /**< WebAssembly 32-bit architecture */
      wasm64  = 0x8000, /**< WebAssembly 64-bit architecture */
   };

   enum class operating_systems : uint16_t {
      unknown = 0x0,  /**< Unknown operating system */
      windows = 0x1,  /**< Windows operating system */
      macos   = 0x2,  /**< macOS operating system */
      ios     = 0x4,  /**< iOS operating system */
      linux   = 0x8,  /**< Linux operating system */
      unix    = 0x10, /**< Unix operating system */
      bsd     = 0x20, /**< BSD operating system */
      android = 0x40, /**< Android operating system */
      wasi    = 0x80, /**< WebAssembly System Interface operating system */
      posix   = 0x100 /**< POSIX operating system */
   };

   enum class compilers : uint16_t {
      unknown = 0x0, /**< Unknown compiler */
      msvc    = 0x1, /**< Microsoft Visual C++ compiler */
      gcc     = 0x2, /**< GNU Compiler Collection */
      clang   = 0x4, /**< Clang compiler */
      cl430   = 0x8, /**< TI MSP430 C/C++ Compiler */
      intel   = 0x10 /**< Intel C++ Compiler */
   };

   enum class build_types : uint16_t {
      unknown                 = 0x0,  /**< Unknown build */
      debug                   = 0x1,  /**< Debug build */
      release                 = 0x2,  /**< Release build */
      release_with_debug_info = 0x4,  /**< Release with debug info build */
      profile                 = 0x8,  /**< Profiling build */
      trace                   = 0x10, /**< Tracing build */
      minimum_size            = 0x20  /**< Minimum size build */
   };

   enum class endianesses : uint8_t {
      unknown = 0x0, /**< Unknown endianness */
      little  = 0x1, /**< Little-endian */
      big     = 0x2  /**< Big-endian */
   };

   enum class languages : uint8_t {
      unknown = 0x0, /**< Unknown language */
      c       = 0x1, /**< C language */
      cpp     = 0x2  /**< C++ language */
   };

   struct build_info {
      architectures arch = architectures::unknown;
      endianesses endianess = endianesses::unknown;
      operating_systems os = operating_systems::unknown;
      compilers compiler = compilers::unknown;
      uint64_t compiler_version = 0;
      languages language = languages::unknown;
      build_types build = build_types::unknown;
   };

   struct version_t {
      uint64_t major : 16;
      uint64_t minor : 16;
      uint64_t patch : 16;
      uint64_t tweak : 16; 
   };

   struct version_info : public version_t {
      enum class parts : uint8_t {
         major,   /**< Major version */
         minor,   /**< Minor version */
         patch,   /**< Patch version */
         tweak,   /**< Tweak version */
         suffix,  /**< Version suffix */
         git_hash /**< Git hash */
      }; 

      std::string suffix;
      std::string git_hash;
      
      version_info() = default;
      version_info(const version_info&) = default;
      version_info(version_info&&) = default;

      constexpr inline version_info(std::uint16_t major=0, std::uint16_t minor=0, std::uint16_t patch=0, std::uint16_t tweak=0, std::string_view suffix="", std::string_view git_hash="")
         : version_t(major, minor, patch, tweak), suffix(suffix), git_hash(git_hash) {}

      constexpr inline std::strong_ordering operator<=>(const version_info& other) const noexcept {
         return number() <=> other.number();
      }

      constexpr inline uint64_t number() const {
         return (major * 1000000) + (minor * 10000) + (patch * 100) + tweak;
      }

      template <parts part>
      constexpr inline auto get() const noexcept {
         if constexpr (part == parts::major) {
            return major;
         } else if constexpr (part == parts::minor) {
            return minor;
         } else if constexpr (part == parts::patch) {
            return patch;
         } else if constexpr (part == parts::tweak) {
            return tweak;
         } else if constexpr (part == parts::suffix) {
            return suffix;
         } else if constexpr (part == parts::git_hash) {
            return git_hash;
         } else {
            throw std::runtime_error("Invalid part requested");
         }
      }

      template <parts part>
      inline std::string to_string() const {
         if constexpr (part == parts::major) {
            return std::to_string(major);
         } else if constexpr (part == parts::minor) {
            return std::to_string(minor);
         } else if constexpr (part == parts::patch) {
            return std::to_string(patch);
         } else if constexpr (part == parts::tweak) {
            return std::to_string(tweak);
         } else if constexpr (part == parts::suffix) {
            return std::string(suffix.data(), suffix.size());
         } else if constexpr (part == parts::git_hash) {
            return std::string(git_hash.data(), git_hash.size());
         } else {
            return std::string();
         }
      }

      inline std::string to_string() const {
         const auto suffix = to_string<parts::suffix>().empty() ? "" : "-" + to_string<parts::suffix>();
         const auto git_hash = to_string<parts::git_hash>().empty() ? "" : " (" + to_string<parts::git_hash>() + ")";
         return to_string<parts::major>() + "." + 
                to_string<parts::minor>() + "." + 
                to_string<parts::patch>() + "." + 
                to_string<parts::tweak>() +
                suffix + git_hash; 
      }
   };

} // namespace versa::info

#define VERSA_BUILD_INFO \
