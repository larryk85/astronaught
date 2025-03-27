#pragma once

#include <concepts>
#include <limits>

#include "floating_bytes.hpp"
#include "../utils.hpp"

/**
 * @namespace astro::types
 * Contains utilities and type traits for floating-point operations.
 */
namespace astro::types {

  /**
   * @namespace astro::types::detail
   * Internal namespace for implementation details. Not intended for direct use.
   */
  namespace detail {
      /**
       * @struct astro::types::detail::identity
       * A type trait used to deduce the underlying floating-point type.
       * 
       * @tparam T The type to deduce.
       */
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

      /**
       * @typedef astro::types::detail::float_t
       * A type alias that resolves to the underlying floating-point type of T.
       * 
       * @tparam T The type to deduce.
       */

      template <typename T>
      using float_t = typename identity<T>::type;
   }

   /**
    * @fn approx_eq
    * Checks if two floating-point numbers are approximately equal within a given epsilon.
    * 
    * @tparam A The type of the first floating-point number.
    * @tparam B The type of the second floating-point number.
    * @param a The first floating-point number.
    * @param b The second floating-point number.
    * @param epsilon The tolerance for comparison (default is 0.00001).
    * @return True if the absolute difference between a and b is less than epsilon, false otherwise.
    * @note This function is constexpr and noexcept.
    */
   template <float_type A, float_type B>
   constexpr static inline bool approx_eq(A a, B b, double epsilon = 0.00001) noexcept {
      return std::abs((detail::float_t<A>)a - (detail::float_t<B>)b) < epsilon;
   }

   /**
    * @fn fuzzy_eq
    * Checks if two floating-point numbers are approximately equal using the next representable values.
    * 
    * @tparam A The type of the first floating-point number.
    * @tparam B The type of the second floating-point number.
    * @param a The first floating-point number.
    * @param b The second floating-point number.
    * @return True if the second number lies between the next representable values of the first number, false otherwise.
    * @note This function is constexpr and noexcept.
    */
   template <float_type A, float_type B>
   constexpr static inline bool fuzzy_eq(A a, B b) noexcept {
      auto av = (detail::float_t<A>)a;
      auto bv = (detail::float_t<B>)b;
      return std::nextafter(av, std::numeric_limits<double>::lowest()) <= bv &&
             std::nextafter(av, std::numeric_limits<double>::max()) >= bv;
   }
}