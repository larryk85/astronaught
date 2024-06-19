#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>
#include <iostream>
#include <fstream>

#include <versa/utils/fixed_string.hpp>

using namespace versa::util;

TEST_CASE("Compile Time String Tests", "[ct_string_tests]") {
   SECTION("Check compile time string construction") {
      fixed_string str("Hello, World!");

      CHECK(str.size() == 13);
      CHECK(str[0] == 'H');
      CHECK(str[1] == 'e');
      CHECK(str[2] == 'l');
      CHECK(str[3] == 'l');
      CHECK(str[4] == 'o');
      CHECK(str[5] == ',');
      CHECK(str[6] == ' ');
      CHECK(str[7] == 'W');
      CHECK(str[8] == 'o');
      CHECK(str[9] == 'r');
      CHECK(str[10] == 'l');
      CHECK(str[11] == 'd');
      CHECK(str[12] == '!');

      fixed_string str2 = str;
      CHECK(str2.size() == 13);

      str2[0] = 'h';
      CHECK(str2[0] == 'h');
      CHECK(str[0] == 'H');

      fixed_string str3 = std::move(str2);
      CHECK(str3.size() == 13);
      CHECK(str3[0] == 'h');

      str3[0] = 'J';

      CHECK(str3[0] == 'J');
   }

   SECTION("Check compile time string comparison") {
      constexpr fixed_string str("Hello, World!");
      constexpr fixed_string str2("Hello, World!");
      constexpr fixed_string str3("Hello, World");
      constexpr fixed_string str4("hello, World");

      CHECK(str == str2);
      CHECK(str <= str2);
      CHECK(str >= str2);
      CHECK(str != str3);
      CHECK(str > str3);
      CHECK(str3 < str4);
      CHECK(str4 >= str3);
      CHECK(!(str4 <= str3));
   }

   SECTION("Check compile time string range") {
      constexpr range<0, 10> r;
      CHECK(r.lower_bound == 0);
      CHECK(r.upper_bound == 10);
      CHECK(r.delta() == 10);

      constexpr range<5, 11> r2;
      CHECK(r2.lower_bound == 5);
      CHECK(r2.upper_bound == 11);
      CHECK(r2.delta() == 11-5);

      constexpr fixed_string str("Hello, World!");
      constexpr auto sub = str.substr(r2);
      CHECK(sub.size() == 6);
      CHECK(sub[0] == ',');
      CHECK(sub[1] == ' ');
      CHECK(sub[2] == 'W');
      CHECK(sub[3] == 'o');
      CHECK(sub[4] == 'r');
      CHECK(sub[5] == 'l');

      CHECK(sub.begin()[0] == ',');
      CHECK(sub.end() == sub.data() + sub.size());

      const char test_str[] = "Hello, World!";
      const char* test_str_ptr = test_str;

      for (auto c : str) {
         CHECK(c == *test_str_ptr);
         ++test_str_ptr;
      }
   }

   SECTION("Check compile time strings operations") {
      using namespace versa::literals;

      constexpr fixed_string hello = "hello"_fs;
      constexpr fixed_string world = "world"_fs;

      const char hello_world_s[] = "helloworld";

      CHECK(hello.size() == 5);
      CHECK(world.size() == 5);

      for (std::size_t i = 0; i < hello.size(); ++i) {
         CHECK(hello[i] == hello_world_s[i]);
      }

      for (std::size_t i = 0; i < world.size(); ++i) {
         CHECK(world[i] == hello_world_s[i+5]);
      }

      constexpr fixed_string hello_world = hello + world;

      for (std::size_t i = 0; i < hello_world.size(); ++i) {
         CHECK(hello_world[i] == hello_world_s[i]);
      }

      constexpr auto hey = fixed_string("hey");

      CHECK(hello.starts_with("hell"_fs));
      CHECK(!hello.starts_with(hey));
      CHECK(hello_world.starts_with(hello));
      CHECK(hello.ends_with("lo"_fs));
      CHECK(!hello.ends_with(hey));
      CHECK(hello_world.ends_with(world));

      constexpr auto r = range<5>{};
      constexpr auto world2 = hello_world.substr(r);

      CHECK(world2.size() == 5);
      CHECK(world2 == "world"_fs);

      auto found = find<hello_world, hello>();

      CHECK(found.lower_bound == 0);
      CHECK(found.upper_bound == 5);

      auto not_found = find<hello_world, "hey"_fs>();
      CHECK(not_found == error_range_v);

      auto found2 = find<hello_world, "low"_fs>();
      CHECK(found2.lower_bound == 3);
      CHECK(found2.upper_bound == 6);

      auto found3 = rfind<hello_world, "ello"_fs>();
      CHECK(found3.lower_bound == 0);
      CHECK(found3.upper_bound == 4);

      constexpr auto rev_hello = hello.reverse();
      CHECK(rev_hello == "olleh"_fs);
   }

   SECTION("Check compile time string conversions and printing") {
      using namespace versa::literals;
      using namespace versa::util;

      constexpr fixed_string hello = "hello"_fs;
      constexpr fixed_string world = "world"_fs;

      auto hello_world_str = to_string(hello + world);
      CHECK(hello_world_str == "helloworld");

      constexpr auto helloworldworld = hello+world+world;
      CHECK(helloworldworld.to_string_view() == "helloworldworld");

      constexpr auto one = "1"_fs;
      int one_int = to_integral<int, one>();
      CHECK(one_int == 1);

      constexpr auto num = "123456789"_fs;
      int num_int = to_integral<int, num>();
      CHECK(num_int == 123456789);

      constexpr auto num2 = "-12345678"_fs;
      constexpr int num2_int = to_integral<int, num2>();
      CHECK(num2_int == -12345678);

      constexpr auto byte = "255"_fs;
      unsigned char byte_char = to_integral_v<unsigned char, byte>;
      CHECK(byte_char == 255);

      // This should not compile successfully and should throw a static_assert
      // constexpr auto byte2 = "256"_fs;
      // byte_char = to_integral_v<unsigned char, byte2>;
      // CHECK(byte_char == 256);
   }
}