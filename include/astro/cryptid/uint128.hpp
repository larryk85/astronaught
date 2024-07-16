#pragma once

#include <array>
#include <bit>
#include <iomanip>
#include <sstream>

//#include "../utils.hpp"

#include "ops.hpp"

namespace astro::cryptid {
   struct high_value {
      std::uint64_t value;
   };

   struct low_value {
      std::uint64_t value;
   };

   class alignas(16) uint128 {
      public:
         uint128() = default;
         uint128(const uint128&) = default;
         uint128(uint128&&) = default;
         constexpr inline uint128(std::uint64_t low, std::uint64_t high) noexcept 
            : _value(low,high) {}

         constexpr inline uint128(low_value low) noexcept
            : _value(low.value, 0) {}
         
         constexpr inline uint128(high_value high) noexcept
            : _value(0, high.value) {}

         constexpr inline uint128(m128 value) noexcept
            : _value(value) {}
         
         template <std::size_t N, typename B>
         constexpr inline uint128(const B(&bytes)[N]) noexcept {
            static_assert(N == sizeof(uint128), "Invalid byte array size");
            std::memcpy(this, bytes, N);
         }

         template <std::size_t N, typename B>
         constexpr inline uint128(const std::array<B,N>& bytes) noexcept {
            static_assert(N == sizeof(uint128), "Invalid byte array size");
            std::memcpy(this, bytes.data(), N);
         }

         uint128& operator=(const uint128&) = default;
         uint128& operator=(uint128&&) = default;
         ~uint128() = default;

         constexpr inline uint128& operator=(low_value v) noexcept {
            _value.low = v.value;
            return *this;
         }

         constexpr inline uint128& operator=(high_value v) noexcept {
            _value.high = v.value;
            return *this;
         }

         template <std::size_t N, typename B>
         constexpr inline uint128& operator=(const B(&bytes)[N]) noexcept {
            static_assert(N == sizeof(uint128), "Invalid byte array size");
            std::memcpy(this, bytes, N);
            return *this;
         }

         template <std::size_t N, typename B>
         constexpr inline uint128& operator=(const std::array<B,N>& bytes) noexcept {
            static_assert(N == sizeof(uint128), "Invalid byte array size");
            std::memcpy(this, bytes.data(), N);
            return *this;
         }

         constexpr inline std::uint64_t low() const noexcept { return _value.low; }
         constexpr inline std::uint64_t high() const noexcept { return _value.high; }
         constexpr inline std::uint64_t& low() noexcept { return _value.low; }
         constexpr inline std::uint64_t& high() noexcept { return _value.high; }

         constexpr inline std::uint64_t* data() noexcept { return &_value.low; }
         constexpr inline const std::uint64_t* data() const noexcept { return &_value.low; }

         auto operator<=>(const uint128&) const noexcept = default;

         constexpr inline uint128& operator+=(const uint128& other) noexcept {
            _value = add_m128(_value, other._value);
            return *this;
         }

         constexpr inline uint128& operator+=(uint64_t other) noexcept {
            _value = add_m128(_value, {other, 0});
            return *this;
         }

         constexpr inline uint128 operator+(const uint128& other) const noexcept {
            return {add_m128(_value, other._value)};
         }

         constexpr inline uint128 operator+(uint64_t other) const noexcept {
            return {add_m128(_value, {other, 0})};
         }

         constexpr inline uint128& operator-=(const uint128& other) noexcept {
            _value = sub_m128(_value, other._value);
            return *this;
         }

         constexpr inline uint128& operator-=(uint64_t other) noexcept {
            _value = sub_m128(_value, {other, 0});
            return *this;
         }

         constexpr inline uint128 operator-(const uint128& other) const noexcept {
            return {sub_m128(_value, other._value)};
         }

         constexpr inline uint128 operator-(uint64_t other) const noexcept {
            return {sub_m128(_value, {other, 0})};
         }

         constexpr inline uint128 operator*=(const uint128& other) noexcept {
            uint64_t cross1 = _value.high * other._value.low;
            uint64_t cross2 = _value.low * other._value.high;

            _value.low *= other._value.low;
            _value.high *= other._value.high;
            _value.high += (cross1 >> 32) + (cross2 >> 32);

            uint64_t carry = (cross1 & 0xFFFFFFFF) + (cross2 & 0xFFFFFFFF);
            if (carry >> 32) {
               _value.high += carry >> 32;
            }

            return *this;
         }

         constexpr inline uint128 operator*=(uint64_t other) noexcept {
            uint64_t cross1 = _value.high * other;

            _value.low *= other;
            _value.high += (cross1 >> 32);

            uint64_t carry = (cross1 & 0xFFFFFFFF);
            if (carry >> 32) {
               _value.high += carry >> 32;
            }

            return *this;
         }


         constexpr inline uint128 operator*(const uint128& other) const noexcept {
            uint128 result = *this;
            result *= other;
            return result;
         }

         constexpr inline uint128 operator/=(const uint128& other) {
            if (other == uint128{0,0}) {
               throw std::runtime_error("Division by zero");
            }

            uint128 quotient = {0, 0};
            uint128 remainder = {0, 0};
            uint128 dividend = *this;

            for (int i = 127; i >= 0; --i) {
               remainder._value.high = (remainder._value.high << 1) | (remainder._value.low >> 63);
               remainder._value.low = (remainder._value.low << 1) | ((dividend._value.high >> i) & 1);
               if (remainder._value.high > other._value.high || (remainder._value.high == other._value.high && remainder._value.low >= other._value.low)) {
                  remainder -= other;
                  if (i >= 64) {
                     quotient._value.high |= 1ull << (i - 64);
                  } else {
                     quotient._value.low |= 1ull << i;
                  }
               }
            }

            *this = quotient;
            return *this;
         }


         constexpr inline uint128 operator/(const uint128& other) const noexcept {
            uint128 result = *this;
            result /= other;
            return result;
         }

         constexpr inline uint128 operator%(const uint128& other) const noexcept {
            return { _value.low % other._value.low, _value.high % other._value.high };
         }

         std::string to_string() const noexcept {
            std::ostringstream oss;
            oss << "0x" << std::hex << std::setw(16) << std::setfill('0') << _value.high 
                << std::setw(16) << std::setfill('0') << _value.low;
            return oss.str();
         }

      private:
         m128 _value = {0,0};
   }; 

   using uint128_t = uint128;
} // namespace astro::cryptid

namespace astro::literals {
   consteval static inline astro::cryptid::uint128_t operator""_ui128(const char* x) {
      uint64_t high = 0;
      uint64_t low = 0;
      bool high_set = true;
      int shift = 0;

      for (int i = 2; x[i] != '\0'; ++i) {
         uint8_t value;
         if ('0' <= x[i] && x[i] <= '9')
            value = x[i] - '0';
         else if ('A' <= x[i] && x[i] <= 'F')
            value = x[i] - 'A' + 10;
         else if ('a' <= x[i] && x[i] <= 'f')
            value = x[i] - 'a' + 10;
         
         if (high_set) {
            high = (high << 4) | value;
            shift += 4;
            if (shift == 64) {
               high_set = false;
               shift = 0;
            }
         } else {
            low = (low << 4) | value;
         }
      }

      return {low, high};
   }

   consteval static inline astro::cryptid::low_value operator""_low(unsigned long long v) noexcept {
      return {static_cast<std::uint64_t>(v)};
   }

   consteval static inline astro::cryptid::high_value operator""_high(unsigned long long v) noexcept {
      return {static_cast<std::uint64_t>(v)};
   }
}