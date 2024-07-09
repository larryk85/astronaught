#pragma once

#include "../info/build_info.hpp"

#if ASTRO_OS == ASTRO_WINDOWS_BUILD
#include "win/demangle_impl.hpp"
#else
#include "unix/demangle_impl.hpp"
#endif

namespace astro::debug {

   /**
    * Demangles a C++ symbol name.
    *
    * @param name The symbol name to demangle.
    *
    * @return The demangled symbol name if successful, an empty string otherwise.
    */
   static std::string demangle(std::string_view name) noexcept {
      return demangle_impl(name);
   }

   /**
    * Demangles the type name of the given template type.
    *
    * @param T The template type to demangle.
    *
    * @return The demangled type name.
    */
   template <typename T>
   static inline std::string demangle() noexcept {
      return demangle_impl(typeid(T).name());
   }

} // namespace astro::utils