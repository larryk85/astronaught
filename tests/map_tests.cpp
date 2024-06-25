#include <string>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <fstream>
#include <source_location>

#include <versa/info.hpp>
#include <versa/utils.hpp>
#include <versa/frozen/traits.hpp>
#include <versa/frozen/map.hpp>

using namespace versa;
using namespace versa::frozen;

TEST_CASE("Map Tests", "[map_tests]") {
   SECTION("Testing Constructors") {
      using le = map<int,float,1>::elem_t;
      auto m0  = map<int,float,1>{le{1l,2.4f}};

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

      auto f3 = m0[1234.1234f, m0.b_tag_v];

      CHECK(f3.has_value());
      CHECK(f3.value() == 34);
   }
}