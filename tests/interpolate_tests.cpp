#include <string>
#include <type_traits>

#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <astro/utils/easing.hpp>

using namespace astro;
using namespace astro::util;

#define INTERP_CHECK(interp_type, type, exp)             \
   for (std::size_t i=0; i < 32; i++) {                  \
      type n = (type)i;                                  \
      CHECK( easing<type>::interp_type((type)n)     \
           == exp((type)n) );                            \
      if constexpr (std::is_signed_v<type>) {            \
         CHECK( easing<type>::interp_type(-(type)n) \
              == exp(-(type)n) );                        \
      }                                                  \
   }

#pragma warning(disable:4146)

TEST_CASE("Interpolate Tests", "[interpolate_tests]") {
   using float32_t = float;
   using float64_t = double;

   SECTION("Check linear") {
      const auto y = [](auto x) { return x; };
      INTERP_CHECK(in_out_linear, int16_t,   y);
      INTERP_CHECK(in_out_linear, int32_t,   y);
      INTERP_CHECK(in_out_linear, int64_t,   y);
      INTERP_CHECK(in_out_linear, uint16_t,  y);
      INTERP_CHECK(in_out_linear, uint32_t,  y);
      INTERP_CHECK(in_out_linear, uint64_t,  y);
      INTERP_CHECK(in_out_linear, float32_t, y);
      INTERP_CHECK(in_out_linear, float64_t, y);
   }

   SECTION("Check quadratic in") {
      const auto y = []<typename T>(T x) -> T { return x*x; };
      INTERP_CHECK(in_quad, int16_t,   y);
      INTERP_CHECK(in_quad, int32_t,   y);
      INTERP_CHECK(in_quad, int64_t,   y);
      INTERP_CHECK(in_quad, uint16_t,  y);
      INTERP_CHECK(in_quad, uint32_t,  y);
      INTERP_CHECK(in_quad, uint64_t,  y);
      INTERP_CHECK(in_quad, float32_t, y);
      INTERP_CHECK(in_quad, float64_t, y);
   }

   SECTION("Check quadratic out") {
      const auto y = []<typename T>(T x) -> T { return -(x*(x-2)); };
      INTERP_CHECK(out_quad, int16_t,   y);
      INTERP_CHECK(out_quad, int32_t,   y);
      INTERP_CHECK(out_quad, int64_t,   y);
      INTERP_CHECK(out_quad, uint16_t,  y);
      INTERP_CHECK(out_quad, uint32_t,  y);
      INTERP_CHECK(out_quad, uint64_t,  y);
      INTERP_CHECK(out_quad, float32_t, y);
      INTERP_CHECK(out_quad, float64_t, y);
   }

   SECTION("Check quadratic in out") {
      const auto y = []<typename T>(T x) -> T { 
         if (x < 0.5)
            return 2*x*x; 
         else {
            return 1 - ((-2*x+2)*(-2*x+2))/2;
         }
      };

      INTERP_CHECK(in_out_quad, int16_t,   y);
      INTERP_CHECK(in_out_quad, int32_t,   y);
      INTERP_CHECK(in_out_quad, int64_t,   y);
      INTERP_CHECK(in_out_quad, uint16_t,  y);
      INTERP_CHECK(in_out_quad, uint32_t,  y);
      INTERP_CHECK(in_out_quad, uint64_t,  y);
      INTERP_CHECK(in_out_quad, float32_t, y);
      INTERP_CHECK(in_out_quad, float64_t, y);
   }

   SECTION("Check sine in") {
      const auto y = []<typename T>(T x) -> T { 
         return 1 - std::cos((x*std::numbers::pi/2));
      };

      INTERP_CHECK(in_sine, int16_t,   y);
      INTERP_CHECK(in_sine, int32_t,   y);
      INTERP_CHECK(in_sine, int64_t,   y);
      INTERP_CHECK(in_sine, uint16_t,  y);
      INTERP_CHECK(in_sine, uint32_t,  y);
      INTERP_CHECK(in_sine, uint64_t,  y);
      INTERP_CHECK(in_sine, float32_t, y);
      INTERP_CHECK(in_sine, float64_t, y);
   }
}