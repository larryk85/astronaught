#pragma once

#include <concepts>
#include <type_traits>
#include <string>
#include <string_view>

namespace astro::util {
   namespace detail {
      template <std::size_t N>
      constexpr static inline bool cstring_type(const char (&)[N]) { return true; }
      template <typename S>
      constexpr static inline bool cstring_type(S) { return false; }
   } // namespace detail

   template <typename T>
   concept byte_type = std::integral<T> && sizeof(T) == 1;

   template <typename T>
   concept word_type = std::integral<T> && sizeof(T) == 2;

   template <typename T>
   concept dword_type = std::integral<T> && sizeof(T) == 4;

   template <typename T>
   concept qword_type = std::integral<T> && sizeof(T) == 8;

   template <typename T>
   concept reflectable_type = requires {
      T::is_reflectable;
   };

   template <typename N>
   concept numeric_type = std::integral<N> || std::floating_point<N>;

   template <typename CS>
   concept cstring_type = requires(CS cs) {
      { detail::cstring_type(cs) };
   };

   template <typename S>
   concept string_type = std::is_same_v<S, std::string> || std::is_same_v<S, std::string_view>;
}