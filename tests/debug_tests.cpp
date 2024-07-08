#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <astro/debug.hpp>

using namespace astro::debug;

int bar(int i) { return i; }

int foo() { 
   return bar(34); 
}

int baz() { 
   backtrace bt;
   bt.print();
   return foo(); 
}

TEST_CASE("Debug Tests", "[debug]") {
   SECTION("Testing backtracing") {
      backtrace bt;
      bt.print();

      CHECK(baz() == 34);
   }
}