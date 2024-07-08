#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>
#include <iostream>
#include <type_traits>

#include <astro/async.hpp>

using namespace astro::async;

TEST_CASE("Async Tests", "[async_tests]") {
   SECTION("Check executor") {
      double d = 3.14;
      uint32_t ui = 340;

      executor exec{[ui, &d](int x, const std::string& y) -> std::size_t {
         d += 3.5;
         return int64_t(x+ui)+y.size();
      }};

      auto v1 = exec.exec(42, "Call from exec(...)");
      auto v2 = exec(24, "Call from operator()(...)");
      auto v3 = exec.exec_sync(12, "Call from exec_sync(...)");

      CHECK(std::is_same_v<decltype(v1), std::future<uint32_t>>);
      CHECK(std::is_same_v<decltype(v2), std::future<uint32_t>>);
      CHECK(std::is_same_v<decltype(v3), uint32_t>);

      try {
         CHECK(v1.get() == 401);
         CHECK(v2.get() == 389);
         CHECK(v3 == 376);
         CHECK(d == 13.64);
         CHECK(ui == 340);
      } catch (const std::exception& e) {
         std::cerr << e.what() << std::endl;
         REQUIRE(false);
      }

      std::cout << "D: " << d << std::endl;
   }
}