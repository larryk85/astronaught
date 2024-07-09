#pragma once

#include "sink.hpp"

#include <cstdio>

namespace astro::io {

   struct file_sink : sink<file_sink> {
      file_sink(FILE* f) noexcept 
         : file(f) {}

      constexpr inline int32_t write_impl(std::string_view s) noexcept {
         if (file == nullptr)
            return -1;
         return ::fwrite(s.data(), sizeof(char), s.size(), file);
      }

      FILE* file = nullptr;
   };

   namespace cstd {
      template <stdio Sink>
      constexpr inline auto get_stdio_sink() noexcept {
         if constexpr (Sink == stdio::stdout) {
            return file_sink{::stdout};
         } else if constexpr (Sink == stdio::stderr) {
            return file_sink{::stderr};
         } else if constexpr (Sink == stdio::stdlog) {
            return file_sink{::stderr};
         }
      }
   } // namespace cstd

} // namespace astro::io

#if ASTRO_OS == ASTRO_OS_WINDOWS
#include "win/native_file_sink.hpp"
#else
#include "unix/native_file_sink.hpp"
#endif