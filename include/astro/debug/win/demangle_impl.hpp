#pragma once

#include <string>
#include <string_view>

#if __has_include(<dbghelp.h>)
#define NOMINMAX
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#undef NOMINMAX
#define ASTRO_HAS_DBGHELP_H
#else
#error "dbghelp.h is required for demangling"
#endif

namespace astro::debug {

   /**
    * Demangles a C++ symbol name using the UnDecorateSymbolName function from the Windows Debug Help Library.
    *
    * @param name The symbol name to demangle.
    *
    * @return The demangled symbol name if successful, an empty string otherwise.
    */
   static inline std::string demangle_impl(std::string_view name) noexcept {
      static char demangled[1024];
      if (UnDecorateSymbolName(name.data(), demangled, 1024, UNDNAME_COMPLETE)) {
         return {demangled};
      } else {
         return {};
      }
   }

} // namespace astro::utils