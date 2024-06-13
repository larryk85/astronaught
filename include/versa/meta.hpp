#pragma once

namespace versa::meta {
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

   template <auto F>
   using return_type_t = typename detail::return_ty::return_type<decltype(F)>::type;

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

   template <auto F>
   using param_type_t = typename detail::param_ty::param_type<decltype(F)>::type;

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
   } // namespace detail::func_ty

   template <auto F>
   using function_type_t = typename detail::func_ty::function_type<decltype(F)>::type;

   template <typename T>
   constexpr inline std::string_view type_name() {
      constexpr std::string_view full_name = 
   #ifdef __clang__
         __PRETTY_FUNCTION__;
   #elif __GNUC__
         __PRETTY_FUNCTION__;
   #elif _MSC_VER
         __FUNCSIG__;
   #else
   #error "Currently only supporting Clang, GCC, and MSVC compilers"
   #endif

      constexpr auto start = 
   #ifdef _MSC_VER
         full_name.find("type_name<") + 10;
   #else
         full_name.find("T = ") + 4;
   #endif

      constexpr auto end = 
   #ifdef __clang__
         full_name.find("]");
   #elif __GNUC__
         full_name.find(";");
   #elif _MSC_VER
         full_name.rfind(">");
   #endif

      return full_name.substr(start, end - start);
   }

   template <typename T>
   constexpr static inline std::string_view type_name_v = type_name<T>();

} // namespace versa::meta