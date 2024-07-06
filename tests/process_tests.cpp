#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>

#include <astro/async.hpp>

using namespace astro::util;

TEST_CASE("Process Tests", "[process_tests]") {
   SECTION("Check process_base") {
      process proc = process("ls");
   }
}