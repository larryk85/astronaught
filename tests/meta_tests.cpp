#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>

#include <versa/meta.hpp>

using namespace versa;
using namespace versa::meta;

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

TEST_CASE("Meta Tests", "[meta_tests]") {
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
      std::cout << type_name_v<function_type_t<test_func_0>> << std::endl;
      CHECK(std::is_same_v<function_type_t<test_func_0>, int(int,float,char)>);
      CHECK(std::is_same_v<function_type_t<test_func_1>, float(float,std::string,std::ofstream)>);
      CHECK(std::is_same_v<function_type_t<test_func_2>, void(bool,decltype(std::cout),double)>);


   }
}