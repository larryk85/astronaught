#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <iostream>
#include <string>

#include <astro/types.hpp>

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


template <float_bytes F>
struct float_test {
   constexpr static inline auto value = F;
};

template <typename FC>
struct check_float_constant {
   using type = FC;
   constexpr static inline auto value = FC::value;
};

TEST_CASE("Floating Bytes Tests", "[floating_bytes_tests]") {
   SECTION("Check float_bytes") {
      float_bytes fb = 3.14f;

      CHECK(fb == 3.14f);

      fb = 0.0f;
      CHECK(fb == 0.0f);

      fb += 6.28f;
      CHECK(fb == 6.28f);

      fb -= 3.14f;
      CHECK(fb == 3.14f);

      fb = 3.14f - 6.28f;
      CHECK(fb == -3.14f);

      CHECK(-fb == 3.14f);

      float_bytes fb2 = 42.42f;

      fb += fb2;

      CHECK(approx_eq(fb, 39.28f));
      CHECK(fuzzy_eq(fb, 39.28f));
   }

   SECTION("Check double_bytes") {
      double_bytes fb = 3.14;

      CHECK(fb == 3.14);

      fb = 0.0;
      CHECK(fb == 0.0);

      fb += 6.28;
      CHECK(fb == 6.28);

      fb -= 3.14;
      CHECK(fb == 3.14);

      fb = 3.14 - 6.28;
      CHECK(fb == -3.14);

      CHECK(-fb == 3.14);

      double_bytes fb2 = 42.42;

      fb += fb2;

      CHECK(approx_eq(fb, 39.28));
      CHECK(fuzzy_eq(fb, 39.28));
   }

   SECTION("Check templates") {
      auto fval = float_test<3.154f>();
      CHECK(approx_eq(fval.value, 3.154f));

      using namespace astro::literals;
      using fc = floating_constant<3.154_fb>;
      using cfc = check_float_constant<fc>;

      fc fcval = {};
      cfc cfcval = {};

      CHECK(approx_eq(fcval(), 3.154f));
      CHECK(approx_eq(cfcval.value, 3.154f));
   }
}