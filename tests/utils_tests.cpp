#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <astro/utils.hpp>

using namespace astro::util;

std::string foo(int, float);

template <typename F, std::size_t N>
struct bar {
   constexpr bar() : b(N) {}
   std::size_t b;
};

TEST_CASE("Misc Tests", "[misc_tests]") {
}