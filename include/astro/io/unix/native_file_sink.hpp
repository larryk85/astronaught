#pragma once

#include "../sink.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace astro::io {

   struct native_file_sink : sink<native_file_sink> {
      native_file_sink(int32_t fd) noexcept 
         : fd(fd) {}

      inline int32_t write_impl(std::string_view s) noexcept {
         return ::write(fd, s.data(), s.size());
      }

      int32_t fd = -1;
   };

   inline namespace native {
      template <stdio Sink>
      constexpr inline auto get_stdio_sink() noexcept {
         if constexpr (Sink == stdio::stdout) {
            return native_file_sink{::stdout};
         } else if constexpr (Sink == stdio::stderr) {
            return native_file_sink{::stderr};
         } else if constexpr (Sink == stdio::stdlog) {
            return native_file_sink{::stderr};
         }
      }
   } // namespace native

} // namespace astro::io