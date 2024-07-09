#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <astro/debug.hpp>

using namespace astro::debug;

int barfun(int i) { return i; }

int foofun() { 
   return barfun(34); 
}

int bazfun() { 
   backtracer bt;
   bt.dump();
   return foofun(); 
}

std::string foo(int, float);

template <typename F, std::size_t N>
struct bar {
   constexpr bar() : b(N) {}
   std::size_t b;
};

TEST_CASE("Misc Tests", "[misc_tests]") {
   SECTION("Check demangle tests") {
      const auto demangled = demangle<decltype(foo)>();
      std::cout << "Demangled: " << demangled << std::endl;
      const auto demangled2 = demangle<bar<decltype(foo), 2>>();
      std::cout << "Demangeld: " << demangled2 << std::endl;
   }
}

TEST_CASE("Debug Tests", "[debug]") {
   SECTION("Testing backtracing") {
      backtracer bt;
      bt.dump();

      CHECK(bazfun() == 34);
   }
}