#pragma once

#include <concepts>

#include "misc.hpp"

namespace versa::util {
   template <typename T>
   concept numeric_type = std::integral<T> || std::floating_point<T>; 

   template <numeric_type NT>
   struct interpolate {
      using underlying_type = NT;

      /// @brief linear y=x
      constexpr static inline NT linear(NT n) { return n; }
      /// @brief quadratic y=x^2
      constexpr static inline NT quadratic_in(NT n) { return n * n; }
      /// @brief quadratic y=-x^2+2x
      constexpr static inline NT quadratic_out(NT n) { return -(n * (n-2)); }
      /// @brief quadratic y=(1/2)((2x)^2)           ; [0, 0.5)
      ///        quadratic y=-(1/2)((2x-1)*(2x-3)-1) ; [0.5, 1]
      constexpr static inline NT quadratic_in_out(NT n) { return (n<0.5) ? quadratic_in(n) : quadratic_out(n); }
   };
} // namespace versa::util