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
   SECTION("Check demangle tests") {
      const auto demangled = astro::util::demangle<decltype(foo)>();
      std::cout << "Demangled: " << demangled << std::endl;
      const auto demangled2 = astro::util::demangle<bar<decltype(foo), 2>>();
      std::cout << "Demangeld: " << demangled2 << std::endl;
   }
}