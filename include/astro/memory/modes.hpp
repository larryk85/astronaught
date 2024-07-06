#pragma once

#include <cstdint>

namespace astro::memory {
      enum class access_mode : std::uint8_t {
      none = 0,
      read = 1,
      write = 2,
      execute = 4,
      read_write = read | write,
      read_execute = read | execute,
      write_execute = write | execute,
      read_write_execute = read | write | execute
   };

   enum class protection_mode : std::uint8_t {

   };
} // namespace astro::memory