#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <versa/utils.hpp>

using namespace versa::util;

std::string foo(int, float);

template <typename F, std::size_t N>
struct bar {
   constexpr bar() : b(N) {}
   std::size_t b;
};

TEST_CASE("Misc Tests", "[misc_tests]") {
   SECTION("Check demangle tests") {
      const auto demangled = versa::util::demangle<decltype(foo)>();
      std::cout << "Demangled: " << demangled << std::endl;
      const auto demangled2 = versa::util::demangle<bar<decltype(foo), 2>>();
      std::cout << "Demangeld: " << demangled2 << std::endl;
   }
}