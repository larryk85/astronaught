#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>
#include <iostream>
#include <type_traits>

#include <versa/utils.hpp>

using namespace versa;
using namespace versa::util;

TEST_CASE("Enum Defs Tests 1", "[enum_defs_1_tests]") {
   constexpr std::size_t max_enum_elems = VERSA_ENUM_MAX_ELEMS; 
   constexpr int64_t enum_upper_bound = VERSA_ENUM_UPPER_BOUND;
   constexpr int64_t enum_lower_bound = VERSA_ENUM_LOWER_BOUND;

   CHECK(max_enum_elems   == 50);
   CHECK(enum_upper_bound == 120);
   CHECK(enum_lower_bound == 70);
}