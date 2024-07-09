#pragma once

#include "../sink.hpp"

#include <windows.h>

namespace astro::io {

   struct native_file_sink : sink<native_file_sink> {
      native_file_sink(HANDLE handle) noexcept 
         : handle(handle) {}

      constexpr inline int32_t write_impl(std::string_view s) noexcept {
         int32_t result = 0;
         if (handle == INVALID_HANDLE_VALUE)
            return -1;
         if (::WriteFile(handle, s.data(), static_cast<DWORD>(s.size()), &result, nullptr))
            return -1;
         else
            return result;
      }

      HANDLE handle = INVALID_HANDLE_VALUE;
   };

   inline namespace native {
      template <stdio Sink>
      constexpr inline auto get_stdio_sink() noexcept {
         if constexpr (Sink == stdio::stdout) {
            return native_file_sink{GetStdHandle(STD_OUTPUT_HANDLE)};
         } else if constexpr (Sink == stdio::stderr) {
            return native_file_sink{GetStdHandle(STD_ERROR_HANDLE)};
         } else if constexpr (Sink == stdio::stdlog) {
            return native_file_sink{GetStdHandle(STD_ERROR_HANDLE)};
         }
      }
   }

} // namespace astro::io