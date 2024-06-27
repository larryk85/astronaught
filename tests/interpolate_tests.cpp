#include <string>
#include <type_traits>

#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <versa/utils/interpolate.hpp>

using namespace versa;
using namespace versa::util;

TEST_CASE("Interpolate Tests", "[interpolate_tests]") {
   SECTION("Check interpolate linear") {
      const auto linear_check = [](auto n) { 
         using inter_t = interpolate<std::decay_t<decltype(n)>>;
         CHECK( inter_t::linear(n) == n ); 
      };

      for (std::size_t i=0; i < 1024; i++) {
         linear_check((int)i);
      }
   }
}