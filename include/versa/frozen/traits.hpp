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
      consteval static inline std::string_view munch(std::string_view sv) {
         using namespace versa::info;
         if constexpr (info::build_info_v.comp == compiler::msvc) {
            return sv.substr(sv.find("type_name<") + 10, sv.rfind(">"));
         } else if constexpr (info::build_info_v.comp == compiler::clang) {
            return sv.substr(sv.find("T = ") + 4, sv.find("]"));
         } else {
            return sv.substr(sv.find("T = ") + 4, sv.find(";"));
         }
      }
   } // namespace detail::frozen

   consteval static inline std::string_view function_name(std::string_view fn=__builtin_FUNCTION()) { return fn; }
   consteval static inline std::string_view file_name(std::string_view fn=__builtin_FILE()) { return fn; }
   consteval static inline std::size_t line_number(std::size_t ln=__builtin_LINE()) { return ln; }

   template <typename T>
   consteval static inline std::string_view nameof() {
      return detail::frozen::munch(VERSA_PRETTY_FUNCTION);
   }

   template <auto X>
   consteval static inline std::string_view nameof() {
      return detail::frozen::munch(VERSA_PRETTY_FUNCTION);
   }

   template <typename T>
   constexpr static inline std::string_view type_name_v = nameof<T>();

   template <typename Enum>
   concept enum_type = std::is_enum_v<Enum>;

} // namespace versa::frozen