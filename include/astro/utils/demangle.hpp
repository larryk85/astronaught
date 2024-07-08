#pragma once

#include "misc.hpp"

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#define ASTRO_HAS_CXXABI_H
#elif __has_include(<dbghelp.h>)
#define NOMINMAX
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#undef NOMINMAX
#define ASTRO_HAS_DBGHELP_H
#else
#error "cxxabi.h or dbghelp.h is required for demangling"
#endif

namespace astro::util {

#if defined(ASTRO_HAS_CXXABI_H)
   /**
    * Demangles a C++ symbol name.
    *
    * @param name The symbol name to demangle.
    *
    * @return The demangled symbol name if successful, an empty string otherwise.
    *
    * @throws None
    */
   static std::string demangle(std::string_view name) noexcept {
      int status;
      std::size_t len;
      char* nm = abi::__cxa_demangle(name.data(), nullptr, &len, &status);
      if (status) {
         return {nm, len};
      } else {
         return {};
      }
   }


#elif defined(ASTRO_HAS_DBGHELP_H)
/**
 * Demangles a C++ symbol name using the UnDecorateSymbolName function from the Windows Debug Help Library.
 *
 * @param name The symbol name to demangle.
 *
 * @return The demangled symbol name if successful, an empty string otherwise.
 *
 * @throws None
 */
   static inline std::string demangle(std::string_view name) noexcept {
      static char demangled[1024];
      if (UnDecorateSymbolName(name.data(), demangled, 1024, UNDNAME_COMPLETE)) {
         return {demangled};
      } else {
         return {};
      }
   }
#endif

/**
 * Demangles the type name of the given template type.
 *
 * @param T The template type to demangle.
 *
 * @return The demangled type name.
 *
 * @throws None
 */
   template <typename T>
   static inline std::string demangle() noexcept {
      return demangle(typeid(T).name());
   }

} // namespace astro::utils

#undef ASTRO_HAS_CXXABI_H
#undef ASTRO_HAS_DBGHELP_H