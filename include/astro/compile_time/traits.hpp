#pragma once

#if __has_include(<cxxabi.h>)
   #include <cxxabi.h>
#endif

#include <type_traits>
#include <string_view>

#include "../info/build_info.hpp"
#include "../utils/defs.hpp"
#include "../utils/concepts.hpp"

namespace astro::ct {
   namespace detail::return_ty {
      template <typename T>
      struct return_type;

      template <typename R, typename... Args>
      struct return_type<R(Args...)> {
         using type = R;
      };

      template <typename R, typename... Args>
      struct return_type<R(*)(Args...)> {
         using type = R;
      };

      template <typename R, typename... Args>
      struct return_type<R(&)(Args...)> {
         using type = R;
      };

      template <typename R, typename... Args>
      struct return_type<R(&&)(Args...)> {
         using type = R;
      };

      template <typename C, typename R, typename... Args>
      struct return_type<R(C::*)(Args...)> {
         using type = R;
      };

      template <typename C, typename R, typename... Args>
      struct return_type<R(C::*)(Args...) const> {
         using type = R;
      };

      template <typename C, typename R, typename... Args>
      struct return_type<R(C::*)(Args...) const noexcept > {
         using type = R;
      };

      template <typename C, typename R, typename... Args>
      struct return_type<R(C::*)(Args...) noexcept > {
         using type = R;
      };

   } // namespace detail::return_ty

   template <typename F>
   using return_type = typename detail::return_ty::return_type<F>::type;

   template <auto F>
   using return_type_t = return_type<decltype(F)>;

   namespace detail::param_ty {
      template <typename T>
      struct param_type;

      template <typename R, typename... Args>
      struct param_type<R(Args...)> {
         using type = std::tuple<Args...>;
      };

      template <typename R, typename... Args>
      struct param_type<R(*)(Args...)> {
         using type = std::tuple<Args...>;
      };

      template <typename R, typename... Args>
      struct param_type<R(&)(Args...)> {
         using type = std::tuple<Args...>;
      };

      template <typename R, typename... Args>
      struct param_type<R(&&)(Args...)> {
         using type = std::tuple<Args...>;
      };

      template <typename C, typename R, typename... Args>
      struct param_type<R(C::*)(Args...)> {
         using type = std::tuple<Args...>;
      };

      template <typename C, typename R, typename... Args>
      struct param_type<R(C::*)(Args...) const> {
         using type = std::tuple<Args...>;
      };

      template <typename C, typename R, typename... Args>
      struct param_type<R(C::*)(Args...) const noexcept > {
         using type = std::tuple<Args...>;
      };

      template <typename C, typename R, typename... Args>
      struct param_type<R(C::*)(Args...) noexcept > {
         using type = std::tuple<Args...>;
      };
   } // namespace detail::param_ty

   template <class F>
   using param_type = typename detail::param_ty::param_type<F>::type;

   template <auto F>
   using param_type_t = param_type<decltype(F)>;


   namespace detail::func_ty {
      template <typename T>
      struct function_type;

      template <typename R, typename... Args>
      struct function_type<R(Args...)> {
         using type = R(Args...);
         using return_type = R;
         using params_type = std::tuple<Args...>;
         constexpr static inline std::size_t arity = sizeof...(Args);
      };

      template <typename R, typename... Args>
      struct function_type<R(*)(Args...)> {
         using type = R(Args...);
         using return_type = R;
         using params_type = std::tuple<Args...>;
         constexpr static inline std::size_t arity = sizeof...(Args);
      };

      template <typename R, typename... Args>
      struct function_type<R(&)(Args...)> {
         using type = R(Args...);
         using return_type = R;
         using params_type = std::tuple<Args...>;
         constexpr static inline std::size_t arity = sizeof...(Args);
      };

      template <typename R, typename... Args>
      struct function_type<R(&&)(Args...)> {
         using type = R(Args...);
         using return_type = R;
         using params_type = std::tuple<Args...>;
         constexpr static inline std::size_t arity = sizeof...(Args);
      };

      template <typename C, typename R, typename... Args>
      struct function_type<R(C::*)(Args...)> {
         using type = R(Args...);
         using return_type = R;
         using params_type = std::tuple<Args...>;
         constexpr static inline std::size_t arity = sizeof...(Args);
      };

      template <typename C, typename R, typename... Args>
      struct function_type<R(C::*)(Args...) const> {
         using type = R(Args...);
         using return_type = R;
         using params_type = std::tuple<Args...>;
         constexpr static inline std::size_t arity = sizeof...(Args);
      };

      template <typename C, typename R, typename... Args>
      struct function_type<R(C::*)(Args...) const noexcept > {
         using type = R(Args...);
         using return_type = R;
         using params_type = std::tuple<Args...>;
         constexpr static inline std::size_t arity = sizeof...(Args);
      };

      template <typename C, typename R, typename... Args>
      struct function_type<R(C::*)(Args...) noexcept > {
         using type = R(Args...);
         using return_type = R;
         using params_type = std::tuple<Args...>;
         constexpr static inline std::size_t arity = sizeof...(Args);
      };

      template <typename R, typename... Args>
      struct function_ptr;

      template <typename R, typename... Args>
      struct function_ptr<R(Args...)> {
         using type = R(*)(Args...);
      };
   } // namespace detail::func_ty

   template <class T, class U>
   constexpr inline U&& make_dependent(U&& u) { return std::forward<U>(u); }
   #define astro_WORKAROUND(x) make_dependent<decltype(x)>(x)

   template <typename F>
   using function_type = typename detail::func_ty::function_type<F>::type;

   template <typename F>
   using function_return_type = typename detail::func_ty::function_type<F>::return_type;

   template <typename F>
   using function_params_type = typename detail::func_ty::function_type<F>::params_type;

   template <typename F>
   constexpr static inline auto function_arity = function_type<F>::arity;

   //template <auto F>
   //using function_type_t = function_type<decltype(F)>;

   template <typename F>
   using function_ptr = typename detail::func_ty::function_ptr<F>::type;

   //template <auto F>
   //using function_ptr_t = function_ptr<decltype(F)>;

   namespace detail {
      template <typename T>
      struct is_templated {
         constexpr static inline bool value = false;
      };

      template <template <typename...> class T, typename... Args>
      struct is_templated<T<Args...>> {
         constexpr static inline bool value = true;
      };

      template <template <auto...> class T, auto... Args>
      struct is_templated<T<Args...>> {
         constexpr static inline bool value = true;
      };

      template <typename T>
      struct is_reflectable {
         constexpr static inline bool value = false;
      };

      template <astro::util::reflectable_type T>
      struct is_reflectable<T> {
         constexpr static inline bool value = true;
      };
   } // namespace detail

   template <typename T>
   constexpr static inline bool is_templated_v = detail::is_templated<T>::value;

   template <typename T>
   constexpr static inline bool is_reflectable_v = detail::is_reflectable<T>::value;
} // namespace astro::ct
