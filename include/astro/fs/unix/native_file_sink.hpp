#pragma once

#include "../../io/sink.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace astro::fs {

   struct native_file_sink : io::sink<native_file_sink> {
      native_file_sink(int32_t fd) noexcept 
         : fd(fd) {}

      inline int32_t write_impl(std::string_view s) noexcept {
         return ::write(fd, s.data(), s.size());
      }

      int32_t fd = -1;
   };

   inline namespace native {
      template <io::stdio Sink>
      constexpr inline auto get_stdio_sink() noexcept {
         if constexpr (Sink == io::stdio::out) {
            return native_file_sink{fileno(::stdout)};
         } else if constexpr (Sink == io::stdio::err) {
            return native_file_sink{fileno(::stderr)};
         } else if constexpr (Sink == io::stdio::log) {
            return native_file_sink{fileno(::stderr)};
         }
      }
   } // namespace native

} // namespace astro::fs