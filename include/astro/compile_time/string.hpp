#pragma once

#include <cstdint>
#include <algorithm>
#include <array>
#include <compare>
#include <concepts>
#include <limits>
#include <string>
#include <string_view>

#include "../utils/misc.hpp"

/**
 * @brief This file contains the implementation of the `string` class template and related concepts and functions.
 * The `string` class template represents a compile-time string with a fixed size. It provides various operations
 * such as comparison, concatenation, substring extraction, and more. The file also defines the `range` struct template,
 * which represents a range of indices, and provides utility functions for working with compile-time strings.
 */
namespace astro::ct {
   namespace detail {
      template <std::size_t N, char... Cs>
      struct cstr_to_int;

      template <std::size_t N, char C, char... Next>
      struct cstr_to_int<N, C, Next...> {
         constexpr static inline std::size_t value = cstr_to_int<(N*10)+(C-'0'), Next...>::value;
      };

      template <std::size_t N>
      struct cstr_to_int<N> {
         constexpr static inline std::size_t value = N;
      };

      template <std::size_t A, std::size_t B>
      consteval static inline std::size_t min() {
         return (A < B) ? A : B;
      }

      template <std::size_t N>
      consteval static inline std::array<char, N-1> to_array(const char(&str)[N]) noexcept {
         std::array<char, N-1> arr;
         std::copy_n(str, N-1, arr.begin());
         return arr;
      }
   } // namespace detail

   template <char... Cs>
   constexpr static inline std::size_t cstr_to_int_v = detail::cstr_to_int<0, Cs...>::value;

   constexpr static inline std::size_t range_end_v = std::numeric_limits<std::size_t>::max();

   template <std::size_t LB, std::size_t UB = range_end_v>
   struct range {
      static_assert(LB <= UB, "Lower bound must be less than or equal to upper bound");
      constexpr static inline std::size_t lower_bound = LB;
      constexpr static inline std::size_t upper_bound = UB;

      constexpr static inline std::size_t delta(std::size_t ub = UB) noexcept { return std::min(ub - LB, UB - LB); }
      
      template <std::size_t NUB>
      constexpr static inline std::size_t delta() noexcept { return detail::min<NUB - LB, UB - LB>(); }

      constexpr static inline std::size_t size() noexcept { return delta(); }
      constexpr static inline bool empty() noexcept { return delta() == 0; }

      template <std::size_t LB2, std::size_t UB2>
      constexpr inline bool operator==(range<LB2, UB2> r) const noexcept {
         return lower_bound == r.lower_bound && upper_bound == r.upper_bound;
      }
   }; 

   constexpr static inline auto error_range_v = range<range_end_v, range_end_v>{};

   template <class R>
   concept range_t = requires {
      { R::lower_bound } -> std::convertible_to<std::size_t>;
      { R::upper_bound } -> std::convertible_to<std::size_t>;
   };

   template <class T>
   concept string_type = requires {
      { T::size_v } -> std::convertible_to<std::size_t>;
      { T::_data } -> std::convertible_to<const char*>;
   };

   template <std::size_t N>
   struct string {

      constexpr static inline auto size_v = N;
      consteval static inline std::size_t size() noexcept { return N; }

      string() = delete;
      string(const string&) = default;
      string(string&&) = default;
      string& operator=(const string&) = default;
      string& operator=(string&&) = default;

      template <std::size_t M>
      constexpr inline string(const char (&str)[M]) noexcept {
         std::copy_n(str, size_v, _data);
      }

      constexpr inline string(const std::array<char, N>& arr) noexcept {
         std::copy_n(arr.begin(), size_v, _data);
      }

      constexpr inline const char* data() const noexcept { return &_data[0]; }
      constexpr inline char* data() noexcept { return &_data[0]; }

      constexpr inline const char& operator[](std::size_t i) const noexcept { return _data[i]; }
      constexpr inline char& operator[](std::size_t i) noexcept { return _data[i]; }

      constexpr inline char& at(std::size_t i) {
         util::check(i < size_v, "Index out of range");
         return _data[i];
      }

      constexpr inline const char& at(std::size_t i) const { return at(i); }
      constexpr inline const char* begin() const noexcept { return _data; }
      constexpr inline char* begin() noexcept { return _data; }
      constexpr inline const char* rbegin() const noexcept { return _data + size_v - 1; }
      constexpr inline char* rbegin() noexcept { return _data + size_v - 1; }
      constexpr inline const char* end() const noexcept { return _data + size_v; }
      constexpr inline char* end() noexcept { return _data + size_v; }
      constexpr inline const char* rend() const noexcept { return _data - 1; }
      constexpr inline char* rend() noexcept { return _data - 1; }

      template <std::size_t M>
      ASTRO_CT_CONST inline std::strong_ordering compare(string<M> other) const noexcept {
         if constexpr (size_v < decltype(other)::size_v) {
            return std::strong_ordering::less;
         } else if constexpr (size_v > decltype(other)::size_v) {
            return std::strong_ordering::greater;
         }
         for (std::size_t i = 0; i < size_v; ++i) {
            if (_data[i] < other[i]) {
               return std::strong_ordering::less;
            } else if (_data[i] > other[i]) {
               return std::strong_ordering::greater;
            }
         }
         return std::strong_ordering::equal;
      }

      template <std::size_t M>
      ASTRO_CT_CONST inline std::strong_ordering operator<=>(string<M> other) const noexcept {
         return compare(other);
      }

      template <std::size_t M>
      ASTRO_CT_CONST inline bool operator==(string<M> other) const noexcept {
         return compare(other) == 0;
      }

      template <std::size_t M>
      ASTRO_CT_CONST inline bool operator<(string<M> other) const noexcept {
         return compare(other) < 0;
      }

      template <range_t R>
      consteval inline auto substr(R) const noexcept {
         return string<R::delta(size_v)>{substr<R::lower_bound>(std::make_index_sequence<R::delta(size_v)>{})};
      }

      template <auto R>
      requires (range_t<decltype(R)>)
      consteval inline auto substr() const noexcept { return substr(R); }

      template <std::size_t M>
      ASTRO_CT_CONST inline bool starts_with(const string<M>& other) const noexcept {
         if constexpr (size_v < M) {
            return false;
         } else {
            for (std::size_t i = 0; i < M; ++i) {
               if (_data[i] != other[i]) {
                  return false;
               }
            }
            return true;
         }
      }

      template <ct::string O>
      ASTRO_CT_CONST inline bool starts_with() const noexcept {
         if constexpr (size_v < O.size_v) {
            return false;
         } else {
            return eq_from<0, O>();
         }
      }

      template <std::size_t M>
      ASTRO_CT_CONST inline bool ends_with(const string<M>& other) const noexcept {
         if constexpr (size_v < M) {
            return false;
         } else {
            for (std::size_t i = size_v-M; i < M; ++i) {
               if (_data[(size_v-M) + i] != other[i]) {
                  return false;
               }
            }
            return true;
         }
      }

      template <auto O>
      requires (ct::string_type<decltype(O)>)
      ASTRO_CT_CONST inline bool ends_with() const noexcept {
         if constexpr (size_v < O.size_v) {
            return false;
         } else {

            return eq_from<size_v-O.size_v, O>();
         }
      }

      ASTRO_CT_CONST inline string<N> reverse() const noexcept {
         std::array<char, size_v> arr;
         std::reverse_copy(begin(), end(), arr.begin());
         return string<N>{arr};
      }

      template <std::size_t M>
      ASTRO_CT_CONST inline string<N - M> trim_back() const noexcept {
         constexpr std::size_t sz = N - M;
         std::array<char, sz> arr;
         std::copy_n(begin(), sz, arr.begin());
         return string<sz>{arr};
      }

      template <std::size_t M>
      ASTRO_CT_CONST inline string<N - M> trim_front() const noexcept {
         constexpr std::size_t sz = N - M;
         std::array<char, sz> arr;
         std::copy_n(begin()+M, sz, arr.begin());
         return string<sz>{arr};
      }


      template <std::size_t M>
      ASTRO_CT_CONST inline auto concat(string<M> other) const noexcept {
         constexpr std::size_t sz = size_v + M;
         std::array<char, sz> arr;
         std::copy_n(begin(), size_v, arr.begin());
         std::copy_n(other.begin(), other.size_v, arr.begin() + size_v);
         return string<sz>{arr};
      }

      template <std::size_t M>
      ASTRO_CT_CONST inline decltype(auto) operator+(string<M> other) const noexcept {
         return concat(other);
      }

      ASTRO_CT_CONST inline operator std::string_view() const noexcept {
         return to_string_view();
      }

      ASTRO_CT_CONST inline std::string_view to_string_view() const noexcept {
         return std::string_view(data(), size_v);
      }

      inline std::string to_string() const noexcept {
         return std::string(data(), size_v);
      }

      template <std::size_t... Is>
      consteval inline string<N> reverse(std::index_sequence<Is...>) const noexcept {
         std::array<char, size_v> arr;
         std::copy_backward(begin(), end(), arr.end());
         return {arr};
      }

      template <std::size_t I, std::size_t M>
      consteval inline bool eq_from(string<M> other) const noexcept {
         for (std::size_t i = 0; i < M; ++i) {
            if (_data[I + i] != other[i]) {
               return false;
            }
         }
         return true;
      }

      template <std::size_t I, auto O>
      consteval inline bool eq_from() const noexcept {
         for (std::size_t i = 0; i < O.size_v; ++i) {
            if (_data[I + i] != O[i]) {
               return false;
            }
         }
         return true;
      }

      template <std::size_t LB, std::size_t... Is>
      consteval inline auto substr(std::index_sequence<Is...>) const noexcept {
         constexpr std::size_t Delta = sizeof...(Is);
         static_assert(LB <= size(), "Lower bound out of range");
         static_assert(Delta + LB <= size(), "Upper bound out of range");
         return string<Delta>{std::array<char, Delta>{_data[Is + LB]...}};
      }

      constexpr inline decltype(auto) data2() const noexcept { return _data; }

      char _data[size_v];
   };

   /// deduction guide for string to account for null-terminator
   template <std::size_t M>
   string(const char(&str)[M]) -> string<M-1>;

   template <typename T>
   concept integral_type = std::is_integral_v<T>;

   template <integral_type T, T V>
   struct integral {
      constexpr static inline T value = V;
      using value_type = T;
      using type = integral<T, V>;
      constexpr inline operator value_type() const noexcept { return value; }
      constexpr inline value_type operator()() const noexcept { return value; }
   };

   template <auto A, int64_t I, char... Cs>
   requires (ct::string_type<decltype(A)>)
   ASTRO_CT_CONST static inline int64_t find_first_impl() noexcept {
      if constexpr (I >= decltype(A)::size_v) {
         return -1;
      } else {
         if constexpr (((A[I] == Cs) || ...)) {
            return I;
         } else {
            return find_first_impl<A, I+1, Cs...>();
         }
      }
   }

   template <auto A, char... Cs>
   requires (ct::string_type<decltype(A)>)
   ASTRO_CT_CONST static inline int64_t find_first() noexcept { return find_first_impl<A, 0, Cs...>(); }

   template <auto A, int64_t I, char... Cs>
   requires (ct::string_type<decltype(A)>)
   ASTRO_CT_CONST static inline int64_t find_first_not_impl() noexcept {
      if constexpr (I >= decltype(A)::size_v) {
         return -1;
      } else {
         if constexpr (((A[I] != Cs) && ...)) {
            return I;
         } else {
            return find_first_not_impl<A, I+1, Cs...>();
         }
      }
   }

   template <auto A, char... Cs>
   requires (ct::string_type<decltype(A)>)
   ASTRO_CT_CONST static inline int64_t find_first_not() noexcept { return find_first_not_impl<A, 0, Cs...>(); }

   template <auto A, auto B, std::size_t I = 0>
   requires (ct::string_type<decltype(A)> && ct::string_type<decltype(B)>)
   ASTRO_CT_CONST static inline auto find() noexcept {
      constexpr auto b_sz = decltype(B)::size_v;

      if constexpr (!A.template eq_from<I, B>()) {
         if constexpr (I + b_sz >= decltype(A)::size_v) {
            return error_range_v;
         } else {
            return find<A, B, I+1>();
         }
      } else {
         return range<I, I + b_sz>{};
      }
   }

   template <auto A, int64_t I, char... Cs>
   requires (ct::string_type<decltype(A)>)
   ASTRO_CT_CONST static inline int64_t rfind_first_impl() noexcept {
      if constexpr (I < 0) {
         return -1;
      } else {
         if constexpr (((A[I] == Cs) || ...)) {
            return I;
         } else {
            return rfind_first_impl<A, I-1, Cs...>();
         }
      }
   }

   template <auto A, char... Cs>
   requires (ct::string_type<decltype(A)>)
   ASTRO_CT_CONST static inline int64_t rfind_first() noexcept {
      return rfind_first_impl<A, decltype(A)::size_v - 1, Cs...>();
   }

   template <auto A, int64_t I, char... Cs>
   requires (ct::string_type<decltype(A)>)
   ASTRO_CT_CONST static inline int64_t rfind_first_not_impl() noexcept {
      if constexpr (I < 0) {
         return -1;
      } else {
         if constexpr (((A[I] != Cs) && ...)) {
            return I;
         } else {
            return rfind_first_not_impl<A, I-1, Cs...>();
         }
      }
   }

   template <auto A, char... Cs>
   requires (ct::string_type<decltype(A)>)
   ASTRO_CT_CONST static inline int64_t rfind_first_not() noexcept {
      return rfind_first_not_impl<A, decltype(A)::size_v - 1, Cs...>();
   }


   template <auto A, auto B, std::size_t I = decltype(A)::size_v>
   requires (ct::string_type<decltype(A)> && ct::string_type<decltype(B)>)
   ASTRO_CT_CONST static inline auto rfind() noexcept {
      constexpr auto b_sz = decltype(B)::size_v;

      if constexpr (!A.template eq_from<I - b_sz, B>()) {
         if constexpr (I - b_sz == 0) {
            return error_range_v;
         } else {
            return rfind<A, B, I - 1>();
         }
      } else {
         return range<I - b_sz, I>{};
      }
   }

   template <std::size_t N>
   static inline std::string to_string(string<N> fs) noexcept {
      return std::string(fs.data(), fs.size());
   }

   namespace detail {
      template <std::integral I, string FS>
      constexpr static inline I to_integral() noexcept {
         I result = 0;
         for (std::size_t i = 0; i < FS.size(); ++i) {
            result = (result * 10) + (FS[i] - '0');
         }
         return result;
      }
   } // namespace detail

   template <std::integral I, string FS>
   constexpr static inline I to_integral() noexcept {
      if constexpr (FS[0] == '-') {
         constexpr auto res = detail::to_integral<std::size_t, FS.substr(range<1>{})>();
         return -static_cast<I>(res);
      } else {
         constexpr auto res = detail::to_integral<std::size_t, FS>();
         return static_cast<I>(res);
      }
   }

   template <std::integral I, string FS>
   constexpr static inline I to_integral_v = to_integral<I, FS>();

} // namespace astro::ct

namespace astro::literals {
   template <ct::string S>
   consteval inline ct::string<S.size()> operator""_fs() noexcept { 
      return S; 
      //return S.template trim_back<1>(); // remove the null-terminator
   }

   template <char... Cs>
   consteval inline auto operator""_int() { return ct::integral<std::size_t, ct::cstr_to_int_v<Cs...>>{}; }

} // namespace astro::literals


