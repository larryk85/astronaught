#pragma once

#include <type_traits>

#include "traits.hpp"
#include "string.hpp"

namespace versa::frozen {
   template <typename T>
   struct meta {
   };

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

      constexpr static inline auto wrapper_s = string{"frozen_wrapper<"};
      constexpr static inline auto nameof_s  = string{"nameof<"};
      constexpr static inline auto t_eq_s    = string{"T = "};
      constexpr static inline auto e_eq_s    = string{"E = "};
      constexpr static inline auto enum_s    = string{"enum "};
      constexpr static inline auto void_s    = string{">(void)"};
      constexpr static inline auto angle_s   = string{">"};
      constexpr static inline auto sq_s      = string{"]"};
      constexpr static inline auto nil_s     = string{""};

      template <typename T>
      consteval static inline auto prefix() { 
         if constexpr (wrapper_type<T>)
            return wrapper_s; 
         else
            return nil_s;
      }

      template <typename T>
      consteval static inline auto enum_prefix() {
         if constexpr (enum_type<T>)
            return enum_s;
         else
            return nil_s;
      }

      template <typename T>
      consteval static inline auto start_str() {
         constexpr bool msvc = versa::info::build_info_v.comp == versa::info::compiler::msvc;

         if constexpr (msvc)
            return nameof_s + prefix<T>() + enum_prefix<T>();
         else
            return t_eq_s + prefix<T>() + enum_prefix<T>();
      }

      template <typename T>
      consteval static inline auto wrapper_suffix() {
         if constexpr (wrapper_type<T>)
            return angle_s;
         else
            return nil_s;
      }

      template <typename T>
      consteval static inline auto suffix() {
         constexpr bool msvc = versa::info::build_info_v.comp == versa::info::compiler::msvc;

         if constexpr (msvc)
            return void_s;
         else
            return sq_s;
      }

      template <typename T>
      consteval static inline auto end_str() {
         return wrapper_suffix<T>() + suffix<T>();
      }

      template <typename T>
      consteval static inline auto nameof() {
         using namespace versa::info;

         static_assert(info::build_info_v.comp == compiler::msvc  ||
                       info::build_info_v.comp == compiler::clang ||
                       info::build_info_v.comp == compiler::gcc,
                       "Compiler not supported");

         constexpr auto full_name = std::string_view{VERSA_PRETTY_FUNCTION};
         constexpr auto start     = full_name.find(start_str<T>().to_string_view());
         constexpr auto offset    = start_str<T>().size();
         constexpr auto end       = full_name.find(end_str<T>().to_string_view());

         return full_name.substr(start+offset, end-start-offset);
      }

      template <typename T>
      consteval static inline std::string_view nameof_only() {
         constexpr auto sv    = nameof<T>();
         constexpr auto start = sv.rfind("::");
         return sv.substr(start+2, sv.size()-start-2);
      }

      template <enum_type E, E v>
      consteval static inline auto vnameof() {
         constexpr auto full_name = std::string_view{VERSA_PRETTY_FUNCTION};
         constexpr auto start     = full_name.find(nameof<E>());
         constexpr auto offset    = nameof<E>().size()+1;
         constexpr auto end       = full_name.find(end_str<E>().to_string_view());
         return full_name.substr(start+offset, end-start-offset);
      }

      //template <typename E, auto V>
      //consteval static inline auto vnameof() {
      //   return vnameof<E, static_cast<E>(V)>();
      //}

      template <enum_type E, E v>
      consteval static inline auto vnameof_only() {
         constexpr auto sv    = vnameof<E,v>();
         constexpr auto start = sv.rfind("::");
         return sv.substr(start+2, sv.size()-start-2);
      }
   } // namespace detail::frozen

   consteval static inline std::string_view function_name(std::string_view fn=__builtin_FUNCTION()) { return fn; }
   consteval static inline std::string_view file_name(std::string_view fn=__builtin_FILE()) { return fn; }
   consteval static inline std::size_t line_number(std::size_t ln=__builtin_LINE()) { return ln; }

   template <typename T, bool FullName=true>
   consteval static inline std::string_view nameof() {
      if constexpr (FullName) {
         return detail::nameof<T>();
      } else {
         return detail::nameof_only<T>();
      }
   }

   template <auto X, bool FullName=true>
   consteval static inline std::string_view nameof() {
      return nameof<detail::frozen_wrapper<X>, FullName>();
   }

   template <detail::enum_type E, E v, bool FullName=true>
   consteval static inline std::string_view vnameof() {
      if constexpr (FullName) {
         return detail::vnameof<E,v>();
      } else {
         return detail::vnameof_only<E,v>();
      }
   }

   template <typename T, bool FullName=true>
   constexpr static inline std::string_view type_name_v = nameof<T, FullName>();

   template <auto V, bool FullName=true>
   constexpr static inline std::string_view enum_name_v = vnameof<std::decay_t<decltype(V)>, V, FullName>();

} // namespace versa::frozen::meta