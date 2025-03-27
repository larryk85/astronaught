#pragma once

#include "../io/sink.hpp"

#include <cstdio>

namespace astro::fs {
   /**
    * @brief A sink that writes to a file.
    * @tparam Sink The type of the sink (e.g., stdout, stderr).
    */
   struct file_sink : io::sink<file_sink> {
      file_sink(FILE* f) noexcept 
         : file(f) {}

      constexpr inline int32_t write_impl(std::string_view s) noexcept {
         if (file == nullptr)
            return -1;
         return ::fwrite(s.data(), sizeof(char), s.size(), file);
      }

      FILE* file = nullptr;
   };
} // namespace astro::fs

#if ASTRO_OS == ASTRO_WINDOWS_BUILD
#include "win/native_file_sink.hpp"
#else
#include "unix/native_file_sink.hpp"
#endif