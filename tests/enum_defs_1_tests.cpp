#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <astro/utils.hpp>

using namespace astro;
using namespace astro::util;

TEST_CASE("Enum Defs Tests 1", "[enum_defs_1_tests]") {
   constexpr std::size_t  max_enum_elems   = ASTRO_ENUM_MAX_ELEMS; 
   constexpr std::int64_t enum_upper_bound = ASTRO_ENUM_UPPER_BOUND;
   constexpr std::int64_t enum_lower_bound = ASTRO_ENUM_LOWER_BOUND;

   CHECK(max_enum_elems   == 50);
   CHECK(enum_upper_bound == 120);
   CHECK(enum_lower_bound == 70);
}