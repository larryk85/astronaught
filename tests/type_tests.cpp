#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <iostream>
#include <string>

#include <astro/types/variant.hpp>

using namespace astro::util;
using namespace astro::types;

TEST_CASE("Variant Tests", "[variant_tests]") {
   SECTION("Check basics") {
      using var = astro::types::variant<int, float, double, std::string>;
      int i = 14;
      var v = &i;

      CHECK(v.tag() == 0);
      CHECK(v.as<int>() == 14);

      float f = 3.14f;
      v = var{&f};
      CHECK(v.tag() == 1);
      CHECK(v.as<float>() == 3.14f);
      std::cout << "Float: " << (uintptr_t)&f << std::endl;
      std::cout << "FloatPtr: " << (uintptr_t)v.as<float>() << std::endl;

      auto s = std::string{"Hello, World!"};
      v = var{&s};
      CHECK(v.tag() == 3);
      CHECK(v.as<std::string>() == "Hello, World!");
      std::cout << "String: " << (uintptr_t)&s << std::endl;
      std::cout << "StringPtr: " << (uintptr_t)v.ptr<std::string>() << std::endl;
   }
}