#pragma once

#include <cstdint>

#include <bit>
#include <concepts>

#include "../utils.hpp"

namespace astro::types {
   template <std::floating_point F> struct floating_bytes; /// forward declaration for the concept

   template <typename T>
   concept float_type =  std::is_floating_point_v<T> || 
                         std::is_same_v<T, floating_bytes<float>> || 
                         std::is_same_v<T, floating_bytes<double>>;

   template <std::floating_point F>
   struct floating_bytes {
      using float_t = F;
      using bytes_t = std::conditional_t<sizeof(float_t) == 4, uint32_t, uint64_t>;

      static_assert(sizeof(float_t) == 4 || sizeof(float_t) == 8, "Floating point type must be 32 or 64 bits.");

      floating_bytes() = default;
      floating_bytes(const floating_bytes& other) = default;
      floating_bytes(floating_bytes&& other) = default;
      constexpr inline floating_bytes(float_t v) noexcept
         : _data(std::bit_cast<bytes_t>(v)) {
      }

      ~floating_bytes() = default;

      floating_bytes& operator=(const floating_bytes& other) = default;
      floating_bytes& operator=(floating_bytes&& other) = default;
      constexpr inline floating_bytes& operator=(float_t v) noexcept {
         _data = std::bit_cast<bytes_t>(v);
         return *this;
      }

      constexpr inline float_t value() const noexcept {
         return std::bit_cast<float_t>(_data);
      }

      constexpr inline operator float_t() const noexcept {
         return std::bit_cast<float_t>(_data);
      }

      constexpr inline operator bytes_t() const noexcept {
         return _data;
      }

      constexpr inline float_t operator*() const noexcept {
         return std::bit_cast<float_t>(_data);
      }

      constexpr inline floating_bytes operator-() const noexcept {
         return floating_bytes(-value());
      }

#define gen_op(OP)                                                          \
      template <float_type V>                                               \
      constexpr inline floating_bytes operator OP(V other) const noexcept { \
         return floating_bytes{value() OP floating_bytes{other}.value()};   \
      }                                                                     \
      template <float_type V>                                               \
      constexpr inline floating_bytes& operator OP ## =(V other) noexcept { \
         float_t res = value() OP floating_bytes{other}.value();            \
         _data = std::bit_cast<bytes_t>(res);                               \
         return *this;                                                      \
      }

      gen_op(+)
      gen_op(-)
      gen_op(*)
      gen_op(/)
      gen_op(%)

#undef gen_op

      template <float_type T>
      constexpr inline bool operator==(T other) const noexcept {
         return value() == floating_bytes{other}.value();
      }

      template <float_type T>
      constexpr inline bool operator<(T other) const noexcept {
         return value() < floating_bytes{other}.value();
      }

      bytes_t _data;
   };

   using float_bytes  = floating_bytes<float>;
   using double_bytes = floating_bytes<double>;
} // namespace astro::types

namespace astro::literals {
   constexpr static inline types::float_bytes operator""_fb(long double v) noexcept {
      return static_cast<float>(v);
   }

   constexpr static inline types::double_bytes operator""_db(long double v) noexcept {
      return static_cast<double>(v);
   }
};