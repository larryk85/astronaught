#pragma once


#include <cstddef>

#include <array>
#include <compare>
#include <concepts>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "../info/build_info.hpp"
#include "../compile_time/traits.hpp"
#include "../cryptid/uint128.hpp"

#include "defs.hpp"

// forward declaration for memcmp
namespace astro::ct {
   template <std::size_t N>
   struct string;
} // namespace astro::ct

/**
 * @namespace astro::util
 * @brief Contains utility functions for the astro library.
 */
namespace astro::util {
   [[noreturn]] inline void unreachable() {
      #if ASTRO_OS == ASTRO_WINDOWS_BUILD
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

   template <typename... Ts>
   consteval static inline void unused(Ts&&...) {}

   // helper where memcmp is not constexpr
   template <typename T, std::size_t N>
   concept memcmpable_type = std::is_same_v<T, std::array<char, N>> || std::is_same_v<T, astro::ct::string<N>>;

   template <typename T, typename U, std::size_t M, std::size_t N>
   consteval static inline std::strong_ordering memcmp(const T (&lhs)[M], const U (&rhs)[N]) {
      constexpr std::size_t I = M < N ? M : N;
      for (std::size_t i = 0; i < I; ++i) {
         if (auto cmp = lhs[i] <=> rhs[i]; cmp != 0){
            return cmp;
         }
      }

      if constexpr (M < N) return std::strong_ordering::less;
      else if constexpr (M > N) return std::strong_ordering::greater;
      else return std::strong_ordering::equal;
   }

   template <template <std::size_t> class T, template <std::size_t> class U, std::size_t M, std::size_t N>
   requires (memcmpable_type<T<M>, M> && memcmpable_type<U<N>, N>)
   consteval static inline std::strong_ordering memcmp(const T<M>& lhs, const U<N>& rhs) {
      constexpr std::size_t I = M < N ? M : N;
      for (std::size_t i = 0; i < I; ++i) {
         if (auto cmp = lhs[i] <=> rhs[i]; cmp != 0){
            return cmp;
         }
      }

      if constexpr (M < N) return std::strong_ordering::less;
      else if constexpr (M > N) return std::strong_ordering::greater;
      else return std::strong_ordering::equal;
   }

   #if 0
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
            using func_type = ct::function_type_t<&decltype(CB)::operator()>;
            using ret_type  = ct::return_type_t<&decltype(CB)::operator()>;
            using param_type = ct::param_type_t<&decltype(CB)::operator()>;
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
   }  // namespace astro::util::detail

   template <auto CB>
   static inline decltype(auto) closure() {
      return detail::closure_wrapper::ptr<CB>();
   }

   template <class CB>
   static inline decltype(auto) closure(CB&& cb) { return closure<cb>();}
   #endif

   template <typename F, typename S>
   struct pair_generator {
      using first_t  = F;
      using second_t = S;
      using pair_t   = std::pair<F,S>;

      constexpr inline auto operator()(const first_t& f, const second_t& s) const noexcept { 
         return std::make_pair(f,s); 
      }
   };


   #ifdef __has_builtin

      #if __has_builtin(__builtin_bswap128)
         constexpr static inline cryptid::uint128_t bswap128(cryptid::uint128_t x) noexcept { return std::bit_cast<cryptid::uint128_t>(__builtin_bswap128(std::bit_cast<__int128 unsigned>(x))); }
      #else
         constexpr static inline cryptid::uint128_t bswap128(cryptid::uint128_t x) noexcept { 
            #if defined(_MSC_VER)
                return { _byteswap_uint64(x.high()), _byteswap_uint64(x.low()) };
            #elif defined(__GNUC__) || defined(__clang__)
                return { __builtin_bswap64(x.high()), __builtin_bswap64(x.low()) };
            #else
                return { ((x.high() & 0xFF) << 56) | ((x.high() & 0xFF00) << 40) | ((x.high() & 0xFF0000) << 24) | ((x.high() & 0xFF000000) << 8) |
                         ((x.high() >> 8) & 0xFF000000) | ((x.high() >> 24) & 0xFF0000) | ((x.high() >> 40) & 0xFF00) | ((x.high() >> 56) & 0xFF),
                         ((x.low() & 0xFF) << 56) | ((x.low() & 0xFF00) << 40) | ((x.low() & 0xFF0000) << 24) | ((x.low() & 0xFF000000) << 8) |
                         ((x.low() >> 8) & 0xFF000000) | ((x.low() >> 24) & 0xFF0000) | ((x.low() >> 40) & 0xFF00) | ((x.low() >> 56) & 0xFF) };
            #endif
         }
      #endif

      #if __has_builtin(__builtin_bswap64)
         constexpr static inline uint64_t bswap64(uint64_t x) noexcept { return __builtin_bswap64(x); }
      #else
         constexpr static inline uint64_t bswap64(uint64_t x) noexcept { return _byteswap_uint64(x); }
      #endif

      #if __has_builtin(__builtin_bswap32)
         constexpr static inline uint32_t bswap32(uint32_t x) noexcept { return __builtin_bswap32(x); }
      #else
         constexpr static inline uint32_t bswap32(uint32_t x) noexcept { return _byteswap_ulong(x); }
      #endif

      #if __has_builtin(__builtin_bswap16)
         constexpr static inline uint16_t bswap16(uint16_t x) noexcept { return __builtin_bswap16(x); }
      #else
         constexpr static inline uint16_t bswap16(uint16_t x) noexcept { return _byteswap_ushort(x); }
      #endif

   #endif

   template <typename T>
   concept integral_type = std::is_integral_v<T> || std::is_same_v<T, cryptid::uint128_t>;

#if 0
   template <integral_type T>
   constexpr static inline auto byteswap(T v) noexcept {
      if constexpr (sizeof(T) == 1) return v;
      else if constexpr (sizeof(T) == 2)  return static_cast<T>(bswap16(static_cast<uint16_t>(v)));
      else if constexpr (sizeof(T) == 4)  return static_cast<T>(bswap32(static_cast<uint32_t>(v)));
      else if constexpr (sizeof(T) == 8)  return static_cast<T>(bswap64(static_cast<uint64_t>(v)));
      else if constexpr (sizeof(T) == 16) return static_cast<T>(bswap128(static_cast<cryptid::uint128_t>(v)));
   }
#endif

   /*
   template <typename T>
   constexpr static inline pearson_hash(T h, std::string_view s) {
      for (auto c = s.begin(); c != s.end(); ++c) {
         h = pearson_hash(h, *c);
      }
      for (auto c : s) {
         h = pearson_hash(h, c);
      }
      return h;
   }
   */
} // namespace astro::util