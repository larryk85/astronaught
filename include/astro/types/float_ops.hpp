#pragma once

#include <concepts>
#include <limits>

#include "floating_bytes.hpp"
#include "../utils.hpp"

namespace astro::types {
   namespace detail {
      template <typename T>
      struct identity;

      template <typename F>
      struct identity<floating_bytes<F>> {
         using type = F;
      };

      template <std::floating_point F>
      struct identity<F> {
         using type = F;
      };

      template <typename T>
      using float_t = typename identity<T>::type;
   }

   template <float_type A, float_type B>
   constexpr static inline bool approx_eq(A a, B b, double epsilon = 0.00001) noexcept {
      return std::abs((detail::float_t<A>)a - (detail::float_t<B>)b) < epsilon;
   }

   template <float_type A, float_type B>
   constexpr static inline bool fuzzy_eq(A a, B b) noexcept {
      auto av = (detail::float_t<A>)a;
      auto bv = (detail::float_t<B>)b;
      return std::nextafter(av, std::numeric_limits<double>::lowest()) <= bv &&
             std::nextafter(av, std::numeric_limits<double>::max()) >= bv;
   }
}