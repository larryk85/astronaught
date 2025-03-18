#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>

#include <astro/compile_time.hpp>
#include <astro/utils.hpp>

using namespace astro;
using namespace astro::ct;

int test_func_0(int, float, char) { return 0; }
float test_func_1(float, std::string, std::ofstream) { return 0; }
void test_func_2(bool, decltype(std::cout), double) {}

class test_class {
   public:
      int test_method_0() { return 0; }
      float test_method_1(bool, int, float) const { return 0; }
      void test_method_2(std::string, std::string, std::tuple<int, float>) const noexcept { return; }
      std::string test_method_3(double, double, float) noexcept { return "hello"; }
};

void (*fp)(int, int, char) = nullptr;

void call_cb(void (*cb)(int, int, char)) {
   cb(1, 2, 'a');
}

struct closure {
   template <auto CB, typename R, typename FP, typename... Args>
   static constexpr inline R exec(Args&&... args) {
      return (R) (*(FP*)fn<CB>())(std::forward<Args>(args)...);
   }

   template <auto CB, typename R, typename FT, class Params, std::size_t... Is>
   static constexpr inline FT exec0(std::index_sequence<Is...>) {
      return (FT) exec<CB, R, FT, std::tuple_element_t<Is, Params>...>;
   }

   //template <auto CB>
   //static inline auto ptr() {
   //   using func_type = ct::function_type_t<&decltype(CB)::operator()>;
   //   using ret_type  = ct::return_type_t<&decltype(CB)::operator()>;
   //   using param_type = ct::param_type_t<&decltype(CB)::operator()>;
   //   fn<func_type>(CB);
   //   return (func_type) exec<ret_type, func_type, param_type>(std::make_index_sequence<std::tuple_size<param_type>::value>());
   //}
   //template <typename Ret = void, typename F = Ret(*)(int, int, char), typename T>
   template <auto Fn, typename F>
   static inline decltype(auto) ptr(F& f) {
      using ty = decltype(&decltype(Fn)::operator());
      using r = return_type<ty>;
      using p = param_type<ty>;
      using ft = function_type<ty>;
      using fp = function_ptr<ft>;
      std::cout << "FP " << nameof<fp>() << std::endl;

      fn<Fn>(&f);
      return (fp) exec0<Fn, r, fp, p>(std::make_index_sequence<std::tuple_size<p>::value>());
   }

   template <auto Fn>
   static inline void* fn(void* new_fn = nullptr) {
      static void* fn;
      if (new_fn != nullptr)
         fn = new_fn;
      return fn;
   }
};

void foo(int aa, float bb, std::string cc) {
   std::cout << aa << " " << bb << " " << cc << std::endl;
}

TEST_CASE("ct Tests", "[frozen_tests]") {
   SECTION("Check return types") {
      CHECK(std::is_same_v<return_type_t<test_func_0>, int>);
      CHECK(std::is_same_v<return_type_t<test_func_1>, float>);
      CHECK(std::is_same_v<return_type_t<test_func_2>, void>);

      CHECK(!std::is_same_v<return_type_t<test_func_2>, int>);

      CHECK(std::is_same_v<return_type_t<&test_class::test_method_0>, int>);
      CHECK(std::is_same_v<return_type_t<&test_class::test_method_1>, float>);
      CHECK(std::is_same_v<return_type_t<&test_class::test_method_2>, void>);
      CHECK(std::is_same_v<return_type_t<&test_class::test_method_3>, std::string>);
   }

   SECTION("Check param types") {
      CHECK(std::is_same_v<param_type_t<test_func_0>,
                                        std::tuple<int,float,char>>);
      CHECK(std::is_same_v<param_type_t<test_func_1>,
                                        std::tuple<float,std::string,std::ofstream>>);
      CHECK(std::is_same_v<param_type_t<test_func_2>,
                                        std::tuple<bool,decltype(std::cout),double>>);
      CHECK(std::is_same_v<param_type_t<&test_class::test_method_0>,
                                        std::tuple<>>);
      CHECK(std::is_same_v<param_type_t<&test_class::test_method_1>,
                                        std::tuple<bool,int,float>>);
      CHECK(std::is_same_v<param_type_t<&test_class::test_method_2>,
                                        std::tuple<std::string,std::string,std::tuple<int,float>>>);
      CHECK(std::is_same_v<param_type_t<&test_class::test_method_3>,
                                        std::tuple<double,double,float>>);
   }

   SECTION("Check function_type_t") {
      CHECK(std::is_same_v<function_type<decltype(test_func_0)>, int(int,float,char)>);
      CHECK(std::is_same_v<function_type<decltype(test_func_1)>, float(float,std::string,std::ofstream)>);
      CHECK(std::is_same_v<function_type<decltype(test_func_2)>, void(bool,decltype(std::cout),double)>);
   }

#if 0
   auto cl = [&](int sig, float* info, void* ucontext) -> void {
   };

   auto cl2 = [&](int s, int i, char c) -> void {
      std::cout << "Hello" << std::endl;
   };

   //call_cb(astro::util::detail::closure_wrapper::ptr(cl));
   //auto p = astro::util::closure<cl>();
   //std::cout << "func " << type_name_v<function_type<
      //&decltype(cl2)::operator()>> << std::endl;
   auto cb = closure::ptr<cl2>(cl2);
   //call_cb(cb);

   std::cout << "Func " << type_name_v<function_type<decltype(cb)>> << std::endl;
#endif
}