#pragma once

#include <cmath>

#include <concepts>
#include <numbers>

#include "concepts.hpp"
#include "misc.hpp"

namespace astro::util {
   template <numeric_type NT>
   struct easing {
      using underlying_type = NT;

      /// @brief linear y=x
      constexpr static inline NT in_out_linear(NT x) noexcept { return x; }
      /// @brief quadratic y=x^2
      constexpr static inline NT in_quad(NT x) noexcept { return x * x; }
      /**
       * @brief quadratic y=-x^2+2x
       *
       * This function calculates the quadratic easing out equation for a given
       * value 'x'. The quadratic easing out equation is defined as:
       *
       *     y = -(x^2 + 2x)
       *
       * This function returns the value of 'y' which represents the easing
       * out value for the given input 'x'.
       *
       * @param x The input value for which the easing out value is to be calculated.
       *
       * @return The easing out value calculated for the given input 'x'.
       */
      constexpr static inline NT out_quad(NT x) noexcept { return -(x * (x-2)); }
      /// @brief quadratic y=2*(x^2)       ; [0, 0.5)
      ///        quadratic y=-(2*x^2)+4x-1 ; [0.5, 1]
      constexpr static inline NT in_out_quad(NT x) noexcept { 
         NT x2 = x * x;
         return (x<0.5) ? 2*x2 : -2*x2 + 4*x - 1; 
      }

      /// @brief sine y=1-cos((xpi)/2)
      constexpr static inline NT in_sine(NT x) noexcept { return 1 - std::cos((x*std::numbers::pi)/2); }
      /// @brief sine y=sin((xpi)/2)
      constexpr static inline NT out_sine(NT x) noexcept { return std::sin((x*std::numbers::pi)/2); }
   };
} // namespace astro::util