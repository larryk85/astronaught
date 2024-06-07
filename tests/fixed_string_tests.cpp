#include <string>
#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>

#include <versa/constants.hpp>
#include <versa/utils.hpp>
#include <versa/fixed_string.hpp>

using namespace versa::util;

TEST_CASE("Fixed_String Tests", "[fixed_string_tests]") {
   SECTION("Check Constructors") {
      CHECK(fixed_bytes<5>().size() == 5);
      CHECK(fixed_bytes("test").size() == 4);
      CHECK(fixed_bytes(std::array<char, 4>{'t', 'e', 's', 't'}).size() == 4);
      CHECK(fixed_bytes<4>(std::string("test")).size() == 4);
      CHECK(fixed_bytes<4>(std::string_view("test")).size() == 4);
      CHECK(fixed_bytes{"test"}.size() == 4);
   }

   SECTION("Check Constructors with Incorrect Sizes") {
      //auto fs = fixed_string<3>("test");
      //CHECK(fixed_string<5>(std::array<char, 4>{'t', 'e', 's', 't'}).size() == 4);
      //CHECK(fixed_string<5>(std::string("test")).size() == 4);
      //CHECK(fixed_string<5>(std::string_view("test")).size() == 4);
   }

}