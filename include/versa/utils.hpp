#pragma once

#include <cstddef>

#include <array>
#include <concepts>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

/**
 * @namespace versa::util
 * @brief Contains utility functions for the Versa library.
 */
namespace versa::util {
   /**
    * @brief Checks a condition and throws a runtime_error if the condition is false.
    * @param condition The condition to check.
    * @param message The error message to include in the exception.
    */
   constexpr static inline void check(const bool condition, std::string message) {
      if (!condition) [[unlikely]] {
         throw std::runtime_error(std::move(message));
      }
   }

   /**
    * @brief Checks a condition and throws a runtime_error if the condition is false.
    * @param condition The condition to check.
    * @param message The error message to include in the exception.
    */
   constexpr static inline void check(const bool condition, std::string_view message) {
      if (!condition) [[unlikely]] {
         throw std::runtime_error(std::string(message.data(), message.size()));
      }
   }

   /**
    * @brief Checks a condition and throws a runtime_error if the condition is false.
    * @param condition The condition to check.
    * @param message The error message to include in the exception.
    */
   constexpr static inline void check(const bool condition, const char* message) {
      if (!condition) [[unlikely]] {
         throw std::runtime_error(std::string(message));
      }
   }

   /**
    * @brief Checks a condition and throws a runtime_error if the condition is false.
    * @param condition The condition to check.
    * @param message The error message to include in the exception.
    * @param size The size of the message.
    */
   constexpr static inline void check(const bool condition, const char* message, std::size_t size) {
      if (!condition) [[unlikely]] {
         throw std::runtime_error(std::string(message, size));
      }
   }

   /**
    * @brief Checks a condition and invokes a callable if the condition is false.
    * @tparam Func The type of the callable.
    * @param condition The condition to check.
    * @param func The callable to invoke if the condition is false.
    */
   template <typename Func>
   requires std::invocable<Func>
   constexpr static inline void check(const bool condition, Func&& func) {
      if (!condition) [[unlikely]] func();
   }

} // namespace versa::util