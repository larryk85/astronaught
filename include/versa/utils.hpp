#pragma once

#include <cstddef>

#include <array>
#include <concepts>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "constants.hpp"

/**
 * @namespace versa::util
 * @brief Contains utility functions for the Versa library.
 */
namespace versa::util {
   [[noreturn]] inline void unreachable() {
      #if VERSA_OS == VERSA_OS_WINDOWS
         __assume(false);
      #else
         __builtin_unreachable();
      #endif
   }

   /*
   * @brief A set of helpers for std::visit with lambdas
   */
   template <class... Ts>
   struct overloaded : Ts... {
      using Ts::operator()...;
   };
   template <class... Ts>
   overloaded(Ts...) -> overloaded<Ts...>;

   /*
   * @brief A helper for scope guarding using RAII
   */
   template <typename F>
   struct scope_guard {
      scope_guard(F&& f) : _f(static_cast<F&&>(f)) {}
      ~scope_guard() { _f(); }
      F _f;
   };

   /*
   * @brief A helper for testing for maybe being void typed
   */
   struct maybe_void_t {
      template <typename T>
      constexpr inline friend T&& operator, (T&& v, maybe_void_t) noexcept {
         return std::forward<T>(v);
      }
   };

   constexpr static inline maybe_void_t maybe_void;

   /**
    * @brief Checks a condition and throws a runtime_error if the condition is false.
    * @param condition The condition to check.
    * @param message The error message to include in the exception.
    */
   static inline bool check(const bool condition, std::string message) {
      if (!condition) [[unlikely]] {
         throw std::runtime_error(std::move(message));
      }
      return true;
   }

   /**
    * @brief Checks a condition and throws a runtime_error if the condition is false.
    * @param condition The condition to check.
    * @param message The error message to include in the exception.
    */
   constexpr static inline bool check(const bool condition, std::string_view message) {
      if (!condition) [[unlikely]] {
         throw std::runtime_error(std::string(message.data(), message.size()));
      }
      return true;
   }

   /**
    * @brief Checks a condition and throws a runtime_error if the condition is false.
    * @param condition The condition to check.
    * @param message The error message to include in the exception.
    */
   constexpr static inline bool check(const bool condition, const char* message) {
      if (!condition) [[unlikely]] {
         throw std::runtime_error(std::string(message));
      }
      return true;
   }

   /**
    * @brief Checks a condition and throws a runtime_error if the condition is false.
    * @param condition The condition to check.
    * @param message The error message to include in the exception.
    * @param size The size of the message.
    */
   constexpr static inline bool check(const bool condition, const char* message, std::size_t size) {
      if (!condition) [[unlikely]] {
         throw std::runtime_error(std::string(message, size));
      }
      return true;
   }

   /**
    * @brief Checks a condition and invokes a callable if the condition is false.
    * @tparam Func The type of the callable.
    * @param condition The condition to check.
    * @param func The callable to invoke if the condition is false.
    */
   template <typename Func>
   requires std::invocable<Func>
   constexpr static inline bool check(const bool condition, Func&& func) {
      if (!condition) [[unlikely]] func();
      return true;
   }

   template <typename T>
   consteval static inline void unused(T&& t) {
      do {
         (void)t;
      } while (false);
   }

#include <signal.h>

   struct closure {
      template <typename Ret, typename T>
      static constexpr inline Ret exec(int i, siginfo_t* s, void* data) {
         return (Ret) (*(T*)fn<T>())(i, s, data);
      }

      template <typename Ret = void, typename F = Ret(*)(int, siginfo_t*, void*), typename T>
      static inline F ptr(T& t) {
         fn<T>(&t);
         return (F) exec<Ret, T>;
      }

      template <typename T>
      static inline void* fn(void* new_fn = nullptr) {
         static void* fn;
         if (new_fn != nullptr)
            fn = new_fn;
         return fn;
      }
   };

} // namespace versa::util