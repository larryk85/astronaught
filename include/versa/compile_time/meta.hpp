#pragma once

#include <source_location>
#include <type_traits>
#include <utility>
#include "traits.hpp"
#include "string.hpp"

namespace versa::ct {
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
         constexpr bool msvc = versa::info::build_info_v.comp == versa::info::compiler::msvc;
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
         constexpr bool msvc = versa::info::build_info_v.comp == versa::info::compiler::msvc;
         return (msvc) ? "name<" : "V = ";
      }

      consteval static inline std::string_view end() {
         constexpr bool msvc = versa::info::build_info_v.comp == versa::info::compiler::msvc;
         constexpr bool clang = versa::info::build_info_v.comp == versa::info::compiler::clang;
         constexpr bool icc = versa::info::build_info_v.comp == versa::info::compiler::intel;
         if constexpr (msvc)
            return ">(void)";
         else if constexpr (clang || icc)
            return "]";
         else
            return ";";
      }

      template <typename T>
      consteval std::string_view name() {
         constexpr auto fn = std::string_view{VERSA_PRETTY_FUNCTION};
         constexpr auto ss = start<T>();
         constexpr auto es = end();
         constexpr auto sidx = fn.find(ss);
         constexpr auto eidx = std::min(fn.rfind(es), fn.find('<', sidx+ss.size()));
         return fn.substr(sidx+ss.size(), eidx-sidx-ss.size());
      }

      template <auto V>
      consteval std::string_view name() {
         constexpr auto fn = std::string_view{VERSA_PRETTY_FUNCTION};
         constexpr auto ss = start<V>();
         constexpr auto es = end();
         constexpr auto sidx = fn.find(ss);
         constexpr auto eidx = std::min(fn.rfind(es), fn.find('<', sidx+ss.size()));
         return fn.substr(sidx+ss.size(), eidx-sidx-ss.size());
      }

#if 0
      constexpr auto wrapper_s = string{"frozen_wrapper<"};
      constexpr auto nameof_s  = string{"nameof<"};
      constexpr auto t_eq_s    = string{"T = "};
      constexpr auto e_eq_s    = string{"E = "};
      constexpr auto enum_s    = string{"enum "};
      constexpr auto void_s    = string{">(void)"};
      constexpr auto angle_s   = string{">"};
      constexpr auto sq_s      = string{"]"};
      constexpr auto nil_s     = string{""};

      template <typename T>
      constexpr auto prefix() { 
         if constexpr (wrapper_type<T>)
            return wrapper_s; 
         else
            return nil_s;
      }

      template <typename T>
      constexpr auto enum_prefix() {
         if constexpr (enum_type<T>)
            return enum_s;
         else
            return nil_s;
      }

      template <typename T>
      constexpr auto start_str() {
         constexpr bool msvc = versa::info::build_info_v.comp == versa::info::compiler::msvc;

         if constexpr (msvc)
            return nameof_s + prefix<T>() + enum_prefix<T>();
         else
            return t_eq_s + prefix<T>() + enum_prefix<T>();
      }

      template <typename T>
      constexpr auto wrapper_suffix() {
         if constexpr (wrapper_type<T>)
            return angle_s;
         else
            return nil_s;
      }

      template <typename T>
      constexpr auto suffix() {
         constexpr bool msvc = versa::info::build_info_v.comp == versa::info::compiler::msvc;

         if constexpr (msvc)
            return void_s;
         else
            return sq_s;
      }

      template <typename T>
      constexpr auto end_str() {
         return wrapper_suffix<T>() + suffix<T>();
      }

      template <typename T>
      constexpr auto nameof() {
         constexpr auto full_name = std::string_view{VERSA_PRETTY_FUNCTION};
         constexpr auto start     = full_name.find(start_str<T>().to_string_view());
         constexpr auto offset    = start_str<T>().size();
         constexpr auto end       = full_name.find(end_str<T>().to_string_view(), start + offset);
         return full_name.substr(start + offset, end - start - offset);
      }

      template <typename T>
      constexpr std::string_view nameof_only() {
         constexpr auto sv = nameof<T>();
         constexpr auto start = [sv]() constexpr {
            for (size_t i = sv.size(); i > 0; --i) {
               if (sv[i - 1] == ':') return i;
            }
            return size_t(0);
         }();
         return sv.substr(start);
      }

      template <enum_type E, E v>
      constexpr auto vnameof() {
         constexpr auto full_name = std::string_view{VERSA_PRETTY_FUNCTION};
         constexpr auto start     = full_name.find(nameof<E>());
         constexpr auto offset    = nameof<E>().size() + 1;
         constexpr auto end       = full_name.find(end_str<E>().to_string_view(), start + offset);
         return full_name.substr(start + offset, end - start - offset);
      }

      template <enum_type E, E v>
      constexpr std::string_view vnameof_only() {
         constexpr auto sv = vnameof<E, v>();
         constexpr auto start = [sv]() constexpr {
            for (size_t i = sv.size(); i > 0; --i) {
               if (sv[i - 1] == ':') return i;
            }
            return size_t(0);
         }();
         return sv.substr(start);
      }
      #endif

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

   constexpr int32_t enum_lb_v    = VERSA_ENUM_LOWER_BOUND;
   constexpr int32_t enum_ub_v    = VERSA_ENUM_UPPER_BOUND;
   constexpr auto    enum_range_v = VERSA_ENUM_MAX_ELEMS;

   using enum_idx_mapping_t = std::pair<std::string_view, int64_t>;
   using enum_name_mapping_t = std::pair<int64_t, std::string_view>;

/*
   namespace detail::enums {
      template <typename E, auto V>
      constexpr decltype(auto) name() noexcept {
         return versa::ct::enum_name_v<static_cast<E>(V), false>;
      }

      template <typename P, std::size_t N>
      constexpr auto filter(std::array<P, N> ms) noexcept {
         std::size_t i = 0;
         for (const auto& e : ms) {
            if (!e.first.empty()) {
               ms[i++] = e;
            }
         }
         return std::make_pair(ms, i);
      }

      template <typename E, std::size_t... Is>
      constexpr auto mappings(std::index_sequence<Is...>) noexcept {
         return filter(std::array{ std::make_pair(name<E, enum_lb_v + Is>(), enum_lb_v + Is)... });
      }

      template <typename E>
      constexpr decltype(auto) mappings() noexcept {
         return mappings<E>(std::make_index_sequence<VERSA_ENUM_MAX_ELEMS>{});
      }
   } // namespace detail::enums

   template <typename T>
   constexpr static inline std::string_view start() {
      constexpr bool msvc = versa::info::build_info_v.comp == versa::info::compiler::msvc;
      constexpr bool isenum = std::is_enum_v<T>;
      if constexpr (msvc) {
         if constexpr (std::is_enum_v<T>) {
            return "name<enum ";
         } else if constexpr (std::is_union_v<T>) {
            return "name<union ";
         } else if constexpr (std::is_class_v<T>) {
            if constexpr (std::is_pod_v<T>) {
               return "name<struct ";
            } else {
               return "name<class ";
            }
         } else {
            return "name< ";
         }
      } else {
         return "T = ";
      }
   }

   template <auto V> 
   constexpr static inline std::string_view start() {
      constexpr bool msvc = versa::info::build_info_v.comp == versa::info::compiler::msvc;
      if constexpr (msvc) {
         return "name<";
      } else {
         return "V = ";
      }
   }

   constexpr static inline std::string_view end() {
      constexpr bool msvc  = versa::info::build_info_v.comp == versa::info::compiler::msvc;
      constexpr bool clang = versa::info::build_info_v.comp == versa::info::compiler::clang;
      constexpr bool icc   = versa::info::build_info_v.comp == versa::info::compiler::intel;
      if constexpr (msvc) {
         return ">(void)";
      } else if constexpr (clang || icc) {
         return "]";
      } else {
         return ";";
      }
   }

   template <auto V>
   constexpr static inline std::string_view name() {
      constexpr std::string_view fn = {VERSA_PRETTY_FUNCTION};
      constexpr std::string_view ss = start<V>();
      constexpr std::string_view es = end();
      constexpr auto sidx = fn.find(ss);
      constexpr auto eidx = fn.rfind(es);
      return fn.substr(sidx+ss.size(), eidx-sidx-ss.size());
   }

   template <typename T>
   constexpr static inline std::string_view name() {
      constexpr std::string_view fn = {VERSA_PRETTY_FUNCTION};
      constexpr std::string_view ss = start<T>();
      constexpr std::string_view es = end();
      constexpr auto sidx = fn.find(ss);
      constexpr auto eidx = fn.rfind(es);
      return fn.substr(sidx+ss.size(), eidx-sidx-ss.size());
   }
   */

} // namespace versa::ct