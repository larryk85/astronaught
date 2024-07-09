#pragma once

#include <cstdio>

#include <string_view>

#include "../utils/misc.hpp"

namespace astro::io {

   enum class stdio : uint32_t {
      out = 1,
      err = 2,
      log = 3
   };
   
   template <typename Derived>
   struct sink {
      constexpr inline int32_t write(std::string_view s) noexcept {
         return static_cast<Derived*>(this)->write_impl(s);
      }
   };

} // namespace astro::io