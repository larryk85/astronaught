#pragma once

#include <cstdint>

#include <algorithm>
#include <array>
#include <compare>
#include <concepts>
#include <limits>
#include <string>
#include <string_view>

#include "misc.hpp"

namespace versa::util {
   namespace detail {
      template <std::size_t N, char... Cs>
      struct cstr_to_int;

      template <std::size_t N, char C, char... Next>
      struct cstr_to_int <N, C, Next...> {
         constexpr static inline std::size_t value = cstr_to_int<(N*10)+(C-'0'), Next...>::value;
      };

      template <std::size_t N>
      struct cstr_to_int <N> {
         constexpr static inline std::size_t value = N;
      };

      template <std::size_t A, std::size_t B>
      consteval static inline std::size_t min() {
         return (A < B) ? A : B;
      }
   } // namespace detail

   template <char... Cs>
   constexpr static inline std::size_t cstr_to_int_v = detail::cstr_to_int<0, Cs...>::value;

   constexpr static inline std::size_t range_end_v = std::numeric_limits<std::size_t>::max();

   template <std::size_t LB, std::size_t UB=range_end_v>
   struct range {
      static_assert(LB <= UB, "Lower bound must be less than upper bound");
      constexpr static inline std::size_t lower_bound = LB;
      constexpr static inline std::size_t upper_bound = UB;

      constexpr static inline std::size_t delta(std::size_t ub=UB) noexcept { return std::min(ub-LB, UB-LB); }
      
      template <std::size_t NUB>
      constexpr static inline std::size_t delta() noexcept { return detail::min<NUB-LB, UB-LB>(); }

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

   template <std::size_t N>
   struct fixed_string {
      using self_t = fixed_string<N>;

      template <std::size_t M>
      using templ_t = fixed_string<M>;

      constexpr static inline auto size_v = N-1;

      fixed_string() = delete;
      fixed_string(const fixed_string&) = default;
      fixed_string(fixed_string&&) = default;
      fixed_string& operator=(const fixed_string&) = default;
      fixed_string& operator=(fixed_string&&) = default;

      constexpr inline fixed_string(const char (&str)[N]) noexcept {
         std::copy_n(str, size_v, _data);
      }

      constexpr inline fixed_string(const std::array<char, N>& arr) noexcept {
         std::copy_n(arr.begin(), size_v, _data);
      }

      constexpr inline const char* data() const noexcept { return &_data[0]; }
      constexpr char* data() noexcept { return &_data[0]; }

      constexpr inline const char& operator[](std::size_t i) const noexcept { return _data[i]; }
      constexpr inline char& operator[](std::size_t i) noexcept { return _data[i]; }

      constexpr inline char& at(std::size_t i) {
         check(i < size_v, "Index out of range");
         return _data[i];
      }

      constexpr inline const char& at(std::size_t i) const { return at(i); }

      consteval static inline std::size_t size() { return size_v; }

      constexpr inline const char* begin() const noexcept { return _data; }
      constexpr inline char* begin() noexcept { return _data; }
      constexpr inline const char* rbegin() const noexcept { return _data+size_v-1; }
      constexpr inline char* rbegin() noexcept { return _data+size_v-1; }
      constexpr inline const char* end() const noexcept { return _data+size_v; }
      constexpr inline char* end() noexcept { return _data+size_v; }
      constexpr inline const char* rend() const noexcept { return _data-1; }
      constexpr inline char* rend() noexcept { return _data-1; }

      template <std::size_t M>
      VERSA_CT_CONST inline std::strong_ordering compare(fixed_string<M> other) const noexcept {
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
      VERSA_CT_CONST inline std::strong_ordering operator<=>(fixed_string<M> other) const noexcept {
         return compare(other);
      }

      template <std::size_t M>
      VERSA_CT_CONST inline bool operator==(fixed_string<M> other) const noexcept {
         if constexpr (size_v != decltype(other)::size_v) {
            return false;
         } else if constexpr (size_v == decltype(other)::size_v) {
            for (std::size_t i = 0; i < size_v; ++i) {
               if (_data[i] != other[i]) {
                  return false;
               }
            }
            return true;
         } else {
            return true;
         }
      }

      template <std::size_t M>
      VERSA_CT_CONST inline bool operator<(fixed_string<M> other) const noexcept {
         if constexpr (size_v < decltype(other)::size_v) {
            return true;
         } else if constexpr (size_v > decltype(other)::size_v) {
            return false;
         } else {
            for (std::size_t i = 0; i < size_v; ++i) {
               if (_data[i] < other[i]) {
                  return true;
               } else if (_data[i] > other[i]) {
                  return false;
               }
            }
            return false;
         }
      }

      template <range_t R>
      VERSA_CT_CONST inline auto substr(R r={}) const noexcept -> templ_t<R::delta(size_v)+1> {
         return {substr<R::lower_bound>(std::make_index_sequence<R::delta(size_v)>{})};
      }

      template <auto R>
      requires (range_t<decltype(R)>)
      VERSA_CT_CONST inline auto substr() const noexcept -> templ_t<decltype(R)::delta(size_v)+1> {
         return {substr<decltype(R)::lower_bound>(std::make_index_sequence<decltype(R)::delta(size_v)>{})};
      }


      template <std::size_t M>
      VERSA_CT_CONST inline bool starts_with(fixed_string<M> other) const noexcept {
         constexpr auto osz = decltype(other)::size_v;
         if constexpr (size_v < osz) {
            return false;
         } else {
            return substr(range<0, osz>{}) == other;
         }
      }

      template <std::size_t M>
      VERSA_CT_CONST inline bool ends_with(fixed_string<M> other) const noexcept {
         constexpr auto osz = decltype(other)::size_v;
         if constexpr (size_v < osz) {
            return false;
         } else {
            return substr(range<size_v-osz>{}) == other;
         }
      }

      VERSA_CT_CONST inline auto reverse() const noexcept -> templ_t<N> {
         return {reverse(std::make_index_sequence<size_v>{})};
      }

      template <std::size_t M>
      VERSA_CT_CONST inline auto concat(fixed_string<M> other) const noexcept {
         constexpr std::size_t sz = size_v + M;
         std::array<char, sz> arr;
         std::copy_n(begin(), size_v, arr.begin());
         std::copy_n(other.begin(), other.size_v, arr.begin()+size_v);
         return templ_t<sz>{arr};
      }

      template <std::size_t M>
      VERSA_CT_CONST inline decltype(auto) operator+(fixed_string<M> other) const noexcept {
         return concat(other);
      }

      VERSA_CT_CONST inline std::string_view to_string_view() const noexcept {
         return std::string_view(data(), size_v);
      }

      inline std::string to_string() const noexcept {
         return std::string(data(), size_v);
      }

      template <std::size_t... Is>
      VERSA_CT_CONST inline auto reverse(std::index_sequence<Is...>) const noexcept {
         return std::array{_data[size_v-Is-1]...,'\0'};
      }

      template <std::size_t LB, std::size_t... Is>
      VERSA_CT_CONST inline auto substr(std::index_sequence<Is...>) const noexcept {
         constexpr std::size_t Delta = sizeof...(Is);
         static_assert(LB <= size(), "Lower bound out of range");
         static_assert(Delta+LB <= size(), "Upper bound out of range");
         return std::array{_data[Is+LB]..., '\0'};
      }

      char _data[size_v];
   };

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

   template <class T>
   concept fixed_string_type  = requires {
      {T::size_v} -> std::convertible_to<std::size_t>;
      {T::_data} -> std::convertible_to<const char*>;
   };

   template <auto a, auto b, std::size_t i=0>
   requires (fixed_string_type<decltype(a)> && fixed_string_type<decltype(b)>)
   VERSA_CT_CONST static inline auto find() noexcept {
      constexpr auto a_sz = decltype(a)::size_v;
      constexpr auto b_sz = decltype(b)::size_v;

      if constexpr (a_sz < b_sz) {
         return error_range_v;
      } else {
         if constexpr (a.starts_with(b)) {
            return range<i, i+b_sz>{};
         } else {
            constexpr auto sub_a = a.template substr<range<i>>();
            return find<sub_a, b, i+1>();
         }
      }
   }

   template <auto a, auto b, std::size_t i=decltype(a)::size_v>
   requires (fixed_string_type<decltype(a)> && fixed_string_type<decltype(b)>)
   VERSA_CT_CONST static inline auto rfind() noexcept {
      constexpr auto a_sz = decltype(a)::size_v;
      constexpr auto b_sz = decltype(b)::size_v;

      if constexpr (a_sz < b_sz) {
         return error_range_v;
      } else {
         if constexpr (a.ends_with(b)) {
            return range<i-b_sz, i>{};
         } else {
            constexpr auto sub_a = a.template substr<range<0, i>>();
            return rfind<sub_a, b, i-1>();
         }
      }
   }

   template <std::size_t N>
   static inline std::string to_string(fixed_string<N> fs) noexcept {
      return std::string(fs.data(), fs.size());
   }

   template <auto V, bool B>
   struct checker_v {
      constexpr static inline bool value = B;
   };

   template <typename T, bool B>
   struct checker {
      constexpr static inline bool value = B;
   };

   template <auto T>
   struct $ {};

   template <typename CS, typename... Ts>
   [[deprecated]] constexpr static inline void ct_dump() noexcept {}
  
   #define CT_DUMP(...) ct_dump< __VA_ARGS__>()

   namespace detail {
      template <std::integral I, fixed_string FS>
      constexpr static inline I to_integral() noexcept {
         I result = 0;
         for (std::size_t i = 0; i < FS.size(); ++i) {
            result = (result * 10) + (FS[i] - '0');
         }
         return result;
      }
   } // namespace versa::util::detail

   template <std::integral I, fixed_string FS>
   constexpr static inline I to_integral() noexcept {
      constexpr auto digs = std::numeric_limits<I>::digits;
      if constexpr (FS[0] == '-') {
         constexpr auto res = detail::to_integral<std::size_t, FS.substr(range<1>{})>();
         static_assert(res < (1ul << digs), "Integral value out of range");
         return -static_cast<I>(res);
      } else {
         constexpr auto res = detail::to_integral<std::size_t, FS>();
         static_assert(res < (1ul << (sizeof(I)*8)), "Integral value out of range");
         return static_cast<I>(res);
      }
   }

   template <std::integral I, fixed_string FS>
   constexpr static inline I to_integral_v = to_integral<I, FS>();

} // namespace versa::util

namespace versa::literals {
   template <util::fixed_string S>
   consteval inline auto operator""_fs() { return S; }

   template <char... Cs>
   consteval inline auto operator""_int() {
      return util::integral<std::size_t, util::cstr_to_int_v<Cs...>>{};
   }

} // namespace versa::literals