#pragma once

#include <source_location>
#include <type_traits>
#include <utility>

#include "traits.hpp"
#include "string.hpp"

namespace versa::frozen {
   template <typename T>
   struct meta {
   };

   consteval static inline std::string_view function_name(std::string_view fn=std::source_location::current().function_name()) { return fn; }
   consteval static inline std::string_view file_name(std::string_view fn=std::source_location::current().file_name()) { return fn; }
   consteval static inline std::size_t line_number(std::size_t ln=std::source_location::current().line()) { return ln; }
   consteval static inline std::size_t column_number(std::size_t cn=std::source_location::current().column()) { return cn; }

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

      template <enum_type E, E v>
      consteval static inline auto vnameof_only() {
         constexpr auto sv    = vnameof<E,v>();
         constexpr auto start = sv.rfind("::");
         return sv.substr(start+2, sv.size()-start-2);
      }

      constexpr static inline bool is_valid_nm(std::string_view sv) {
         for (auto c : sv) {
            if (c == ')' || c == '(' || c == '{' || c == '}' || c == '<' || c == '>')
               return false;
         }
         return true;
      }

      constexpr static inline auto valid_vnameof(std::string_view sv) { 
         return is_valid_nm(sv) ? sv : "";
      }
   } // namespace detail::frozen

   template <typename T, bool Full=true>
   consteval static inline std::string_view nameof() {
      if constexpr (Full) {
         return detail::nameof<T>();
      } else {
         return detail::nameof_only<T>();
      }
   }

   template <auto X, bool Full=true>
   consteval static inline std::string_view nameof() {
      return nameof<detail::frozen_wrapper<X>, Full>();
   }

   template <detail::enum_type E, E v, bool Full=true>
   consteval static inline std::string_view vnameof() {
      if constexpr (Full) {
         return detail::vnameof<E,v>();
      } else {
         return detail::vnameof_only<E,v>();
      }
   }

   template <typename T, bool Full=true>
   constexpr static inline std::string_view type_name_v = nameof<T, Full>();

   template <auto V, bool Full=true>
   constexpr static inline std::string_view enum_name_v = detail::valid_vnameof(
                                                            vnameof<std::decay_t<decltype(V)>, V, Full>());

   constexpr static inline int32_t enum_lb_v    = VERSA_ENUM_LOWER_BOUND;
   constexpr static inline int32_t enum_ub_v    = VERSA_ENUM_UPPER_BOUND;
   constexpr static inline auto    enum_range_v = VERSA_ENUM_MAX_ELEMS;

   using enum_idx_mapping_t = std::pair<std::string_view, int64_t>;
   using enum_name_mapping_t = std::pair<int64_t, std::string_view>;

   namespace detail::enums {
      template <typename E, auto V>
      consteval static inline decltype(auto) name() noexcept {
         return versa::frozen::enum_name_v<static_cast<E>(V), false>;
      }

      template <typename P, std::size_t N>
      constexpr static inline auto filter(std::array<P,N> ms) noexcept {
         std::size_t i = 0;
         for ( const auto& e : ms ) {
            if ( !e.first.empty() ) {
               ms[i++] = e;
            }
         }
         return std::make_pair(ms,i);
      }

      template <typename E, std::size_t... Is>
       static inline auto values(std::index_sequence<Is...>) noexcept {
         constexpr auto mappings = filter(std::array{ std::make_pair(name<E, enum_lb_v+Is>(), enum_lb_v+Is)... });
         for (std::size_t i=0; i < mappings.second; i++) {
            std::cout << "Mappings : " << mappings.first[i].first << " @ " << mappings.first[i].second << std::endl;
         }
         //for (std::int32_t i=0; i < enum_range_v; i++) {
         //   if (![i].empty()) {
         //      std::cout << "Valid @" << (enum_lb_v+i) << " " << names[i] << std::endl;
         //   }
         //   //std::cout << "Valid["<<i<<"] = " << (int32_t)valid[i+128] << std::endl;
         //}
      }
   } // namespace versa::frozen::detail::enums

} // namespace versa::frozen