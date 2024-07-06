#pragma once

#include <cstdint>

#include <compare>

namespace astro::cryptid {
   struct m64 {
      std::uint64_t value = 0;
      char          carry = 0;
   };

   struct alignas(16) m128 {
      constexpr m128() noexcept = default;
      constexpr m128(const m128&) noexcept = default;
      constexpr m128(m128&&) noexcept = default;
      constexpr m128(std::uint64_t low, std::uint64_t high) noexcept
         : low(low), high(high) {}
      constexpr m128(m64 low, m64 high) noexcept
         : low(low.value), high(high.value+low.carry) {}
      constexpr m128& operator=(const m128&) noexcept = default;
      constexpr m128& operator=(m128&&) noexcept = default;
      constexpr auto operator<=>(const m128&) const noexcept = default;

      std::uint64_t low;
      std::uint64_t high;
   };

   inline namespace generic {
      constexpr static inline m64 add_m64(uint64_t a, uint64_t b) noexcept {
         m64 result;
         result.value = a + b;
         result.carry = (result.value < a) ? 1 : 0;
         return result;
      }

      constexpr static inline m128 add_m128(m128 a, m128 b) noexcept {
         m64 low = add_m64(a.low, b.low);
         m64 high = add_m64(a.high, b.high);
         return {low, high};
      }

      constexpr static inline m64 sub_m64(uint64_t a, uint64_t b) noexcept {
         m64 result;
         result.value = a - b;
         result.carry = (result.value > a) ? 1 : 0;
         return result;
      }

      constexpr static inline m128 sub_m128(m128 a, m128 b) noexcept {
         m64 low = sub_m64(a.low, b.low);
         m64 high = sub_m64(a.high, b.high);
         return {low, high};
      }
   }
} // namespace astro::cryptid