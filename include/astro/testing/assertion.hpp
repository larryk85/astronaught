#pragma once

#include <functional>
#include <string_view>
#include <tuple>
#include <utility>

#include "../utils.hpp"
#include "../compile_time.hpp"
#include "../info.hpp"

namespace astro::testing {

   template <ct::string Name, typename T>
   struct named_object {
      named_object(T&& object)
         : object(std::forward<T>(object)) {}
      constexpr static inline ct::string name = Name;
      T object;
   };

   namespace detail {
      template <ct::string S>
      constexpr static inline auto extract() noexcept {
         constexpr auto npos = std::string_view::npos;
         constexpr auto start = ct::find<S, '{'>();
         constexpr auto end = ct::find<S, '}', start+1>();
         return ct::range<start, end>{};
      }

      template <ct::string S, typename Arg, typename... Args>
      constexpr static inline auto created_named_objects(Arg arg, Args&&... args) noexcept {
         constexpr auto bnds = extract<S>();
         constexpr auto name = S.substr(ct::range<bnds.lower_bound+1, bnds.upper_bound - bnds.lower_bound>{});
         static_assert(name.size() > 0, "placeholder name cannot be empty.");
         auto obj = std::tuple{named_object<name, Arg>{std::forward<Arg>(arg)}};
         if constexpr (sizeof...(Args) > 0) {
            return std::tuple_cat(obj, created_named_objects<S.substr(ct::range<bnds.upper_bound + 2>{})>(std::forward<Args>(args)...));
         } else {
            return obj;
         }
      }

      template <typename T>
      constexpr static inline decltype(auto) invoke_or_fwd(T&& t) noexcept {
         if constexpr (std::is_invocable_v<T>) {
            return std::invoke(std::forward<T>(t));
         } else {
            return std::forward<T>(t);
         }
      }

      template <typename Op, typename T, typename U>
      constexpr static inline auto evaluate(Op&& op, T&& t, U&& u) noexcept {
         return std::invoke(op, invoke_or_fwd(std::forward<T>(t)), invoke_or_fwd(std::forward<U>(u)));
      }

      template <ct::string Nm, typename Tuple, std::size_t I = 0>
      constexpr static inline std::size_t find_index() noexcept {
         if constexpr (I < std::tuple_size_v<std::decay_t<Tuple>>) {
            if constexpr (std::tuple_element_t<I, std::decay_t<Tuple>>::name == Nm) {
               return I;   
            } else {
               return find_index<Nm, Tuple, I + 1>();
            }
         }
         return std::numeric_limits<std::size_t>::max();
      }

   } // namespace detail

   template <ct::string Scenario, typename... Args>
   static inline void attest(Args&&... args) {
      auto placeholders_args = detail::created_named_objects<Scenario>(std::forward<Args>(args)...);
      using placeholders_t   = decltype(placeholders_args);
      std::cout << "S " << std::tuple_size_v<placeholders_t> << std::endl;
      std::cout << "p " << std::get<0>(placeholders_args).name.to_string() << std::endl;
      std::cout << "p " << std::get<0>(placeholders_args).object << std::endl;
      std::cout << "p " << std::get<1>(placeholders_args).name.to_string() << std::endl;
      std::cout << "p " << std::get<2>(placeholders_args).name.to_string() << std::endl;
      std::cout << "I " << detail::find_index<ct::string{"x"}, placeholders_t>() << std::endl;
      auto x = std::get<detail::find_index<ct::string{"x"}, placeholders_t>()>(placeholders_args).object;
      std::cout << "X " << x << std::endl;

      //auto op = detail::find<0>("op", placeholders_args);
      //auto x = std::get<0>(placeholders_args).object;
      //auto y = std::get<2>(placeholders_args).object;
      //auto result = detail::evaluate(op, x, y);
      //std::cout << scenario << " " << detail::invoke_or_fwd(x) << " " << detail::invoke_or_fwd(y) << " " << std::boolalpha <<  result << std::endl;
   }

} // namespace astro::testing