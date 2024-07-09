#include <string>
#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>
#include <iostream>
#include <type_traits>

#include <astro/utils/format.hpp>
#include <astro/utils/misc.hpp>
#include <astro/compile_time.hpp>
#include <astro/io.hpp>

using namespace astro::io;

/* If you add new tests or any code add them after line 42 */
astro::io::call_info test_func(int x, const std::string& y) {
   (void)x;
   (void)y;
   #line 15
   auto ci = call_info{};
   return ci;
}

TEST_CASE("Logger Tests", "[logger_tests]") {
   SECTION("Check call_info") {
      #line 21
      auto info = call_info{};

      CHECK(info.file == "logger_tests.cpp");
      CHECK(info.func == "CATCH2_INTERNAL_TEST_0");
      CHECK(info.line == 21);

      auto info2 = test_func(3, "Hello, World!");

      CHECK(info2.file == "logger_tests.cpp");
      CHECK(info2.func == "test_func");
      CHECK(info2.line == 15);

      auto info3 = []() {
         auto ci = call_info{};
         return ci;
      }();

      CHECK(info3.file == "logger_tests.cpp");
      CHECK((info3.func == "operator()" || info3.func == "operator ()"));
      CHECK(info3.line == 34);
   }
}

TEST_CASE("Format Tests", "[format_tests]") {
   using namespace astro::literals;
   SECTION("Check message_wrapper") {
      #line 47
      auto msg = message_wrapper{"Hello, World!"};
      CHECK(msg.value == "Hello, World!");
      CHECK(msg.info.file == "logger_tests.cpp");
      CHECK(msg.info.func == "CATCH2_INTERNAL_TEST_3");
      CHECK(msg.info.line == 47);

      auto msg2 = message_wrapper{"Different message"};
      CHECK(msg2.value == "Different message");
      CHECK(msg2.info.file == "logger_tests.cpp");
      CHECK(msg2.info.func == "CATCH2_INTERNAL_TEST_3");
      CHECK(msg2.info.line == 53);

      const auto func = [](message_wrapper msg) {
         CHECK(msg.value == "Hello, World!");
         CHECK(msg.info.file == "logger_tests.cpp");
         CHECK(msg.info.func == "CATCH2_INTERNAL_TEST_3");
         CHECK(msg.info.line == 66);
      };

      func("Hello, World!");
   }

   SECTION("Check console logger") {
      console_logger logger(stdout_v);
      const auto msg1 = logger.format("Hello, World!");
      CHECK(msg1 == "Hello, World!");
      CHECK(logger.format("Hello, {}", "World!") == "Hello, World!");
      CHECK(logger.format("{}, {}", "Hello", "World!") == "Hello, World!");
      CHECK(logger.format("{1}, {0}", "World!", "Hello") == "Hello, World!");
      CHECK(logger.format("{},{},{},{}", 0, 42.4f, "foo", 'c') == "0,42.4,foo,c");
   }
}

/* Add new code after this point */

///< TODO Add to 'utils' tests
TEST_CASE("UtilFormat Tests", "[utilformat_tests]") {
   using namespace astro::literals;
   SECTION("Check format") {
      CHECK(astro::util::format("Hello, World!"_fs) == "Hello, World!");
      CHECK(astro::util::format(std::string("Hello, {}"), "World!") == "Hello, World!");
      CHECK(astro::util::format(std::string_view{"{}, {}"}, "Hello", "World!") == "Hello, World!");
      CHECK(astro::util::format(astro::ct::create_string("{1}, {0}"), "World!", "Hello") == "Hello, World!");
      CHECK(astro::util::format("{},{},{},{}", 0, 42.4f, "foo", 'c') == "0,42.4,foo,c");
   }
}

TEST_CASE("Color Tests", "[color_tests]") {
   SECTION("Check color") {
      {
         auto c = term_color::red;
         auto fgc = fg{term_color::red};
         auto bgc = bg{term_color::red};
         auto bfgc = bright{fg{term_color::red}};
         auto bbgc = bright{bg{term_color::red}};

         CHECK(1 == static_cast<uint8_t>(c));
         CHECK(fgc.code() == 31);
         CHECK(bgc.code() == 41);
         CHECK(bfgc.code() == 91);
         CHECK(bbgc.code() == 101);
      }

      {
         auto c = term_color::white;
         auto fgc = fg{c};
         auto bgc = bg{c};
         auto bfgc = bright{fgc};
         auto bbgc = bright{bgc};

         CHECK(7 == static_cast<uint8_t>(c));
         CHECK(fgc.code() == 37);
         CHECK(bgc.code() == 47);
         CHECK(bfgc.code() == 97);
         CHECK(bbgc.code() == 107);
      }

      {
         auto c = default_scheme::rgb_of<fg_red_v.code()>();
         std::cout << "Color: " << c.r << std::endl;
         std::cout << "F " << fg_red_v.code() << std::endl;
      #if 0
   enum class term_color_mode : uint8_t {
      fg = 38,
      bg = 48,
   };

   enum class term_color_reset : uint8_t {
      fg = 39,
      bg = 49,
   };

   enum class term_color_mode_type : uint8_t {
      rgb    = 2,
      normal = 5,
   };

   enum class term_color_reset_type : uint8_t {
      fg = 0,
      bg = 0,
   };
   #endif
         std::cout << default_scheme::rgb_of<fg_red_v>().format() << std::endl;
         std::cout << "\x1b[38;2;" << default_scheme::rgb_of<fg_red_v>().format() << "m" << "Hello, World!" << "\x1b[0m" << std::endl;
         std::cout << "\x1b[38;2;160;128;87m" << "Hello, World!" << "\x1b[0m" << std::endl;
      }
   }
}