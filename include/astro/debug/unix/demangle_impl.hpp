#pragma once

#include <string>
#include <string_view>

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#else
#error "cxxabi.h is required for demangling"
#endif

namespace astro::debug {

   /**
    * Demangles a C++ symbol name on Unix systems using the abi::__cxa_demangle function.
    *
    * @param name The symbol name to demangle.
    *
    * @return The demangled symbol name if successful, an empty string otherwise.
    */
   static std::string demangle_impl(std::string_view name) noexcept {
      int status;
      std::size_t len;
      char* nm = abi::__cxa_demangle(name.data(), nullptr, &len, &status);
      if (status) {
         return {nm, len};
      } else {
         return {};
      }
   }

} // namespace astro::utils