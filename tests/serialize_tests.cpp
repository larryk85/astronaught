#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <astro/serial.hpp>

using namespace astro::util;
using namespace astro::serial;

TEST_CASE("Serial Tests", "[serial_tests]") {
   SECTION("Check basics") {
      alpha a{10};

      int i = 10;
      int j = 20;
      float f = 3.14f;
      double d = 3.14159;

      a.push(i);
      CHECK(a.pos() == sizeof(int));
      a.reset();
      int k = a.pop<int>();

      CHECK(k == 10);
      CHECK(a.pos() == 4);

      a.push(j);

      CHECK(a.pos() == 8);

      a.reset();

      CHECK(a.pop<int>() == 10);
      CHECK(a.pop<int>() == 20);

      a.reset();

      a.push(f);
      a.push(d);

      a.reset();

      CHECK(a.pop<float>() == 3.14f);
      CHECK(a.pop<double>() == 3.14159);

      std::string str = "Hello, World!";
      a.push(str);

      CHECK(static_cast<std::size_t>(a.pos()) == sizeof(f)+sizeof(d)+str.size());
   }
}