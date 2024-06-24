#pragma once

#if __has_include(<cxxabi.h>)
   #include <cxxabi.h>
#endif

#include <type_traits>
#include <string_view>

#include "../info/build_info.hpp"
#include "../utils/defs.hpp"

namespace versa::frozen {
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
      };

      template <typename R, typename... Args>
      struct function_type<R(*)(Args...)> {
         using type = R(Args...);
      };

      template <typename R, typename... Args>
      struct function_type<R(&)(Args...)> {
         using type = R(Args...);
      };

      template <typename R, typename... Args>
      struct function_type<R(&&)(Args...)> {
         using type = R(Args...);
      };

      template <typename C, typename R, typename... Args>
      struct function_type<R(C::*)(Args...)> {
         using type = R(Args...);
      };

      template <typename C, typename R, typename... Args>
      struct function_type<R(C::*)(Args...) const> {
         using type = R(Args...);
      };

      template <typename C, typename R, typename... Args>
      struct function_type<R(C::*)(Args...) const noexcept > {
         using type = R(Args...);
      };

      template <typename C, typename R, typename... Args>
      struct function_type<R(C::*)(Args...) noexcept > {
         using type = R(Args...);
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
   #define VERSA_WORKAROUND(x) make_dependent<decltype(x)>(x)

   template <typename F>
   using function_type = typename detail::func_ty::function_type<F>::type;

   //template <auto F>
   //using function_type_t = function_type<decltype(F)>;

   template <typename F>
   using function_ptr = typename detail::func_ty::function_ptr<F>::type;

   //template <auto F>
   //using function_ptr_t = function_ptr<decltype(F)>;

   namespace detail::frozen {
      template <auto V>
      struct frozen_wrapper {
         static constexpr auto value = V;
      };

      template <typename T>
      concept wrapper_type = requires {
         { std::is_base_of_v<frozen_wrapper<T::value>, T> };
      };

      consteval bool msvc_() {
         return versa::info::build_info_v.comp == versa::info::compiler::msvc;
      }

      template <typename T>
      consteval static inline std::string_view start_str() {
         if constexpr (msvc_()) {
            return "type_name<";
         } else {
            if constexpr (wrapper_type<T>) {
               return "frozen_wrapper<";
            } else {
               return "T = ";
            }
         }
      }

      template <typename T>
      consteval static inline std::string_view end_str() {
         if constexpr (msvc_()) {
            return ">";
         } else {
            if constexpr (wrapper_type<T>) {
               return ">";
            } else {
               return "]";
            }
         }
      }

      template <typename T>
      consteval static inline std::string_view nameof() {
         using namespace versa::info;

         static_assert(info::build_info_v.comp == compiler::msvc  ||
                       info::build_info_v.comp == compiler::clang ||
                       info::build_info_v.comp == compiler::gcc,
                       "Compiler not supported");

         constexpr auto full_name = std::string_view{VERSA_PRETTY_FUNCTION};
         constexpr auto start     = full_name.find(start_str<T>());
         constexpr auto offset    = start_str<T>().size();
         constexpr auto end       = full_name.find(end_str<T>());

         return full_name.substr(start+offset, end-start-offset);
      }

      template <typename T>
      consteval static inline std::string_view nameof_only() {
         constexpr auto sv = nameof<T>();
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
         return detail::frozen::nameof<T>();
      } else {
         return detail::frozen::nameof_only<T>();
      }
   }

   template <auto X, bool FullName=true>
   consteval static inline std::string_view nameof() {
      return nameof<detail::frozen::frozen_wrapper<X>, FullName>();
   }

   template <typename T, bool FullName=true>
   constexpr static inline std::string_view type_name_v = nameof<T, FullName>();

   template <typename E, bool FullName=true>
   requires std::is_enum_v<E>
   constexpr static inline std::string_view enum_name_v = nameof<E, FullName>();

   template <auto V, bool FullName=true>
   requires std::is_enum_v<decltype(V)>
   constexpr static inline std::string_view enum_value_name_v = nameof<V, FullName>();

} // namespace versa::frozen