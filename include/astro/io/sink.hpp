#pragma once

#include <cstdio>

#include <string_view>

#include "../utils/misc.hpp"

namespace astro::io {

   enum class stdio : uint32_t {
      stdout = 1,
      stderr = 2,
      stdlog = 3
   };
   
   template <typename Derived>
   struct sink {
      constexpr inline int32_t write(std::string_view s) noexcept {
         return static_cast<Derived*>(this)->write_impl(s);
      }
   };

} // namespace astro::io