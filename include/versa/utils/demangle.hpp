#pragma once

#include "misc.hpp"

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#define VERSA_HAS_CXXABI_H
#elif __has_include(<dbghelp.h>)
#define NOMINMAX
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#undef NOMINMAX
#define VERSA_HAS_DBGHELP_H
#else
#error "cxxabi.h or dbghelp.h is required for demangling"
#endif

namespace versa::util {

#if defined(VERSA_HAS_CXXABI_H)
   static std::string demangle(std::string_view name) noexcept {
      int status;
      std::size_t len;
      char* nm = abi::__cxa_demangle(name.data(), nullptr, len, &status);
      if (status) {
         return {nm, len};
      } else {
         return {};
      }
   }


#elif defined(VERSA_HAS_DBGHELP_H)
   static std::string demangle(std::string_view name) noexcept {
      static char demangled[1024];
      if (UnDecorateSymbolName(name.data(), demangled, 1024, UNDNAME_COMPLETE)) {
         return {demangled};
      } else {
         return {};
      }
   }
#endif

   template <typename T>
   static inline std::string demangle() noexcept {
      return demangle(typeid(T).name());
   }

} // namespace versa::utils

#undef VERSA_HAS_CXXABI_H
#undef VERSA_HAS_DBGHELP_H