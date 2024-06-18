#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>

#include <versa/arch/unix/process_impl.hpp>

using namespace versa::util;

TEST_CASE("Process Tests", "[process_tests]") {
   SECTION("Check process_base") {
      process proc = process("ls");
   }
}