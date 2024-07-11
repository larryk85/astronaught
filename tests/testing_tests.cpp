#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include "util.hpp"

#include <astro/testing/assertion.hpp>
#include <astro/utils.hpp>

using namespace astro;
using namespace astro::testing;
using namespace astro::test;

float test_func() {
   return 42;
}

TEST_CASE("Testing Testing Framework", "[test][testing]") {
   auto equal = [](auto x, auto y) { return x == y; };
   attest<"{x} {op} {y}">(42, equal, test_func);
}