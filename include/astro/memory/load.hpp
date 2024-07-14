#pragma once

#include <cstdint>

#include "../utils/misc.hpp"

namespace astro::memory {
   constexpr static inline uint64_t unaligned_load(const void* ptr) noexcept {
      uint64_t result;
      std::memcpy(&result, ptr, sizeof(result));
      return result;
   }

   constexpr static inline uint32_t unaligned_load32(const void* ptr) noexcept {
      uint32_t result;
      std::memcpy(&result, ptr, sizeof(result));
      return result;
   }

} // namespace astro::memory 