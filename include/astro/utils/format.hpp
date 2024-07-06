#pragma once

#include <cstdint>
#include <concepts>
#include <format>
#include <iostream>

#include "misc.hpp"
#include "../compile_time/string.hpp"

namespace astro::util {
   template <typename String>
   concept string_like = requires(String s) {
      { s.data() } -> std::convertible_to<const char*>;
      { s.size() } -> std::convertible_to<std::size_t>;
   };

   template <string_like S, typename... Args>
   inline std::string format(S&& fmt, Args&&... args) {
      return std::vformat(std::string_view{fmt.data(), fmt.size()}, std::make_format_args(args...));
   }

   template <std::size_t N, typename... Args>
   inline std::string format(const char(&fmt)[N], Args&&... args) {
      return std::vformat(fmt, std::make_format_args(args...));
   }

   template <string_like S, typename... Args>
   inline void print(S&& fmt, Args&&... args) {
      std::cout << format(fmt, std::forward<Args>(args)...) << std::endl;
   }

   template <std::size_t N, typename... Args>
   inline void print(const char(&fmt)[N], Args&&... args) {
      std::cout << format(std::string_view{fmt,N}, std::forward<Args>(args)...) << std::endl;
   }
} // namespace astro::util