#pragma once

#include "../compile_time/traits.hpp"
#include "misc.hpp"

namespace astro::util {

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
         using func_type = ct::function_type<&decltype(CB)::operator()>;
         using ret_type  = ct::return_type<&decltype(CB)::operator()>;
         using param_type = ct::param_type<&decltype(CB)::operator()>;
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

} // namespace astro::util