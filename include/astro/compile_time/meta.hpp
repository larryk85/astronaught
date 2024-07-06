#pragma once

#include <algorithm>
#include <source_location>
#include <type_traits>
#include <utility>
#include "traits.hpp"
#include "string.hpp"

namespace astro::ct {
   template <typename T>
   struct meta {
   };

   constexpr std::string_view function_name(std::string_view fn = std::source_location::current().function_name()) { return fn; }
   constexpr std::string_view file_name(std::string_view fn = std::source_location::current().file_name()) { return fn; }
   constexpr std::size_t line_number(std::size_t ln = std::source_location::current().line()) { return ln; }
   constexpr std::size_t column_number(std::size_t cn = std::source_location::current().column()) { return cn; }

   namespace detail {
      template <auto V>
      struct frozen_wrapper {
         static constexpr auto value = V;
      };

      template <typename T>
      concept wrapper_type = requires {
         { std::is_base_of_v<frozen_wrapper<T::value>, T> };
      };

      template <typename T>
      concept enum_type = requires {
         { std::is_enum_v<T> };
      };

      template <typename T>
      consteval static inline std::string_view start() {
         constexpr bool msvc = astro::info::build_info_v.comp == astro::info::compiler::msvc;
         if constexpr (msvc) {
            if constexpr (std::is_enum_v<T>) {
               return "name<enum ";
            } else if constexpr (std::is_union_v<T>) {
               return "name<union ";
            } else if constexpr (std::is_class_v<T>) {
               if constexpr (std::is_standard_layout_v<T>)
                  return "name<struct ";
               else
                  return "name<class ";
            } else {
               return "name<";
            }
         } else {
            return "T = ";
         }
      }

      template <auto V> 
      consteval static inline std::string_view start() {
         constexpr bool msvc = astro::info::build_info_v.comp == astro::info::compiler::msvc;
         return (msvc) ? "name<" : "V = ";
      }

      consteval static inline std::string_view end() {
         constexpr bool msvc = astro::info::build_info_v.comp == astro::info::compiler::msvc;
         constexpr bool clang = astro::info::build_info_v.comp == astro::info::compiler::clang;
         constexpr bool icc = astro::info::build_info_v.comp == astro::info::compiler::intel;
         if constexpr (msvc)
            return ">(void)";
         else if constexpr (clang || icc)
            return "]";
         else
            return ";";
      }

      template <typename T>
      consteval std::string_view name() {
         constexpr auto fn = std::string_view{ASTRO_PRETTY_FUNCTION};
         constexpr auto ss = start<T>();
         constexpr auto es = end();
         constexpr auto sidx = fn.find(ss);
         constexpr auto eidx = std::min(fn.rfind(es), fn.find('<', sidx+ss.size()));
         return fn.substr(sidx+ss.size(), eidx-sidx-ss.size());
      }

      template <auto V>
      consteval std::string_view name() {
         constexpr auto fn = std::string_view{ASTRO_PRETTY_FUNCTION};
         constexpr auto ss = start<V>();
         constexpr auto es = end();
         constexpr auto sidx = fn.find(ss);
         constexpr auto eidx = std::min(fn.rfind(es), fn.find('<', sidx+ss.size()));
         return fn.substr(sidx+ss.size(), eidx-sidx-ss.size());
      }

      constexpr bool is_valid_nm(std::string_view sv) {
         for (auto c : sv) {
            if (c == ')' || c == '(' || c == '{' || c == '}' || c == '<' || c == '>')
               return false;
         }
         return true;
      }

      constexpr auto valid_vnameof(std::string_view sv) { 
         return is_valid_nm(sv) ? sv : "";
      }
   } // namespace detail

   template <typename T>
   constexpr static inline std::string_view nameof() {
         return detail::name<T>();
   }

   template <auto X>
   constexpr static inline std::string_view valueof() {
      return detail::name<X>();
   }

   template <detail::enum_type E, E v>
   constexpr static inline std::string_view enameof() {
      constexpr auto sv = detail::name<v>();
      constexpr std::size_t n  = sv.rfind(':');
      if constexpr (n != std::string_view::npos) {
         return sv.substr(n+1);
      } else {
         return sv;
      }
   }

   template <typename T>
   constexpr static inline std::string_view type_name_v = nameof<T>();

   template <auto X>
   constexpr static inline std::string_view value_of_v = valueof<X>();

   template <detail::enum_type E, E v>
   constexpr static inline std::string_view enum_name_v = enameof<E, v>();

   constexpr int32_t enum_lb_v    = ASTRO_ENUM_LOWER_BOUND;
   constexpr int32_t enum_ub_v    = ASTRO_ENUM_UPPER_BOUND;
   constexpr auto    enum_range_v = ASTRO_ENUM_MAX_ELEMS;

   using enum_idx_mapping_t = std::pair<std::string_view, int64_t>;
   using enum_name_mapping_t = std::pair<int64_t, std::string_view>;
} // namespace astro::ct