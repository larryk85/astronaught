#include <string>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <fstream>
#include <source_location>

#include <astro/info.hpp>
#include <astro/utils.hpp>
#include <astro/compile_time/traits.hpp>
#include <astro/compile_time/map.hpp>

using namespace astro;
using namespace astro::ct;

TEST_CASE("Map Tests", "[map_tests]") {
   SECTION("Testing Constructors") {
      auto el = std::pair<int,float>{1l,2.4f};
      auto m0  = map{el};

      CHECK(m0.size() == 1);

      const auto beg = m0.begin();
      auto end = m0.end();

      CHECK(beg->first == 1l);
      CHECK(beg->second == 2.4f);
      CHECK(end == beg+1);

      auto beg2    = m0.begin();
      beg2->first  = 34l;
      beg2->second = 42.24f;

      CHECK(beg->first  == 34l);
      CHECK(beg->second == 42.24f);

      CHECK(beg2->first  == 34l);
      CHECK(beg2->second == 42.24f);

      const auto f = m0[1l];
      CHECK(!f.has_value());

      auto f2 = m0[34l];
      CHECK(f2.has_value());
      CHECK(f2.value() == 42.24f);

      f2.value().get() = 1234.1234f;
      CHECK(f2.value() == 1234.1234f);

      CHECK(beg->second == 1234.1234f);

      auto f3 = m0.at(1234.1234f, m0.b_tag_v);

      CHECK(f3.has_value());
      CHECK(f3.value() == 34);
   }

   SECTION("Testing pair_generator") {
      using genny = astro::util::pair_generator<std::string_view, double>;
      constexpr auto g = genny{};
      
      auto p1 = g("hello", 13.4);
      auto p2 = g("foo", 42.42);
      auto p3 = g("bar", 1234.2345);

      CHECK(p1 == std::pair<std::string_view,double>("hello", 13.4));
      CHECK(p2 == std::pair<std::string_view,double>("foo", 42.42));
      CHECK(p3 == std::pair<std::string_view,double>("bar", 1234.2345));

      CHECK(p1.first == "hello");
      CHECK(p1.second == 13.4);

      CHECK(p2.first == "foo");
      CHECK(p2.second == 42.42);

      CHECK(p3.first == "bar");
      CHECK(p3.second == 1234.2345);
   }

   SECTION("Testing Maps") {
      using genny = astro::util::pair_generator<std::string_view, double>;
      constexpr auto g = genny{};

      auto m0 = map(g("foo", 12.12), g("bar", 23.23), g("baz", 34.34), 
                    g("qux", 45.45), g("quux", 56.56));

      CHECK(m0.size() == 5);
      CHECK(m0["foo"].value() == 12.12);
      CHECK(m0["bar"].value() == 23.23);
      CHECK(m0["baz"].value() == 34.34);
      CHECK(m0["qux"].value() == 45.45);
      CHECK(m0["quux"].value() == 56.56);
      CHECK(m0["fuzz"].has_value() == false);
      m0["foo"].value().get() = 123.123;
      CHECK(m0["foo"].value() == 123.123);
   }      
}