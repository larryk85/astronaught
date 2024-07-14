#pragma once

#include <cstdint>

namespace astro::cryptid {

   template <typename T>
   struct city_hash {
      constexpr static inline auto seed = 0x9e3779b9;
   };

} // namespace astro::cryptid