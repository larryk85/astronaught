#pragma once

#include <cstddef>

#include <array>
#include <compare>
#include <concepts>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "constants.hpp"
#include "meta.hpp"

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

   template <typename T, typename U, std::size_t N>
   constexpr static inline std::strong_ordering memcmp(const T (&lhs)[N], const U (&rhs)[N]) {
      for (std::size_t i = 0; i < N; ++i) {
         if (lhs[i] < rhs[i]) {
            return std::strong_ordering::less;
         } else if (lhs[i] > rhs[i]) {
            return std::strong_ordering::greater;
         }
      }
      return std::strong_ordering::equal; 
   }

   namespace detail {
      struct closure_wrapper {
         template <typename R, typename FP, typename... Args>
         static constexpr inline R exec(Args&&... args) {
            return (R) (*(FP*)fn<FP>)(args...);
         }

         template <typename R, typename FT, class Params, std::size_t... Is>
         static constexpr inline R exec(std::index_sequence<Is...>) {
            return exec<R, FT, std::tuple_element_t<Is, Params>...>;
         }

         template <auto CB>
         static inline auto ptr() {
            using func_type = meta::function_type_t<&decltype(CB)::operator()>;
            using ret_type  = meta::return_type_t<&decltype(CB)::operator()>;
            using param_type = meta::param_type_t<&decltype(CB)::operator()>;
            fn<func_type>(CB);
            return (func_type) exec<ret_type, func_type, param_type>(std::make_index_sequence<std::tuple_size<param_type>::value>());
         }

         template <typename Fn, class CB>
         static inline decltype(auto) fn(CB& cb) { return fn<Fn>(&cb); }

         template <typename FT>
         static inline void* fn(void* new_fn = nullptr) {
            static void* fn;
            if (new_fn != nullptr)
               fn = new_fn;
            return fn;
         }
      };
   }  // namespace versa::util::detail

   template <auto CB>
   static inline decltype(auto) closure() {
      return detail::closure_wrapper::ptr<CB>();
   }

   template <class CB>
   static inline decltype(auto) closure(CB&& cb) { return closure<cb>();}

} // namespace versa::util