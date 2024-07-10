#pragma once

#include "floating_bytes.hpp"
#include "float_ops.hpp"

namespace astro::types {
   template <typename T>
   concept float_bytes_type = std::is_same_v<float_bytes, T> || std::is_same_v<double_bytes, T>;

   template <auto V>
   requires(float_bytes_type<std::remove_cvref_t<decltype(V)>>)
   struct floating_constant {
      using param_type = std::remove_cvref_t<decltype(V)>;
      using value_type = param_type::float_t;
      using type       = floating_constant<V>;

      constexpr static inline auto value = V;
      constexpr inline operator value_type() { return value; }
      constexpr inline value_type operator()() { return value; }
   };
} // namespace astro::types