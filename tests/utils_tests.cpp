#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

#include <errno.h>

#include <astro/fs.hpp>
#include <astro/utils.hpp>
#include <astro/compile_time.hpp>

using namespace astro::util;
using namespace astro::literals;

static inline void is_valid(char tc, char c) {
   if (tc == '%')
      CHECK(((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')));
}

template <typename S>
static inline void is_valid(S&& tmpl, const std::string& rs) {
   for (std::size_t i=0; i < rs.size(); i++) {
      is_valid(tmpl[i], rs[i]);
   }
}

TEST_CASE("Random String Tests", "[utils][random_string]") {
   SECTION("Default charset tests") {
      auto rs = generate_random_string("");
      CHECK(rs.size() == 0);

      rs = generate_random_string("Hello, World!"_fs);
      CHECK(rs.size() == 13);
      rs = "Hello, World-%%-%%-%%"_rs;

      CHECK(rs.size() == 21);
      CHECK((std::string_view)rs != "Hello, World-%%-%%-%%");

      astro::ct::string tmpl = "Hello, World-%%-%%-%%"_fs;
      rs = generate_random_string(tmpl);
      CHECK((std::string_view)rs != tmpl);

      is_valid(tmpl, rs);
   }

   SECTION("Custom charset tests") {
      std::string_view charset = "abcdefg";
      auto rs = generate_random_string("", charset);
      CHECK(rs.size() == 0);

      rs = generate_random_string("Hello, World!"_fs, charset);
      CHECK(rs.size() == 13);

      astro::ct::string tmpl = "Hello, World-%%-%%-%%"_fs;
      rs = generate_random_string(tmpl, charset);

      const auto is_custom_valid = [&](const auto& tmp, const auto& s) {
         CHECK(tmpl.size() == s.size());
         CHECK((std::string_view)tmp != s);
         for (std::size_t i=0; i < s.size(); i++) {
            if (tmpl[i] == '%') {
               CHECK(charset.find(s[i]) != charset.npos);
            }
         }
      };

      is_custom_valid(tmpl, rs);

      for (std::size_t i=0; i < rs.size(); i++) {
         char c = rs[i];
         if (tmpl[i] == '%') {
            bool is_in_charset = (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g');
            CHECK(is_in_charset);
         }
      }
   }
}

TEST_CASE("File Tests", "[utils][file]") {
   SECTION("Check file generation") {
      auto f = astro::fs::fopen("./test.txt", astro::fs::file_mode::write);
      CHECK(!astro::fs::is_fd_invalid(f));
      auto df = astro::fs::fduplicate(f);
      CHECK(!astro::fs::is_fd_invalid(df));
      std::string_view str = "testing file tests";
      auto written = astro::fs::fwrite(f, str.data(), str.size());
      CHECK(written == str.size());
      CHECK(astro::fs::fclose(df));
      //auto f = astro::util::fopen("./util.hpp", astro::util::fmode_read);
      //CHECK(df != 0);
      //CHECK(astro::util::fclose(f));
   }
}

TEST_CASE("Random File Tests", "[utils][random_string][temporary_file]") {
   SECTION("Check file name generation") {
      auto tmp_dir = std::filesystem::temp_directory_path();
      auto fn = generate_temp_file_name("");

      CHECK(fn.starts_with(tmp_dir.string()));

      fn = generate_temp_file_name("tmp-%%%-%%%");
      is_valid((tmp_dir / "tmp-%%%-%%%").string(), fn);
      CHECK(fn.starts_with((tmp_dir/"tmp-").string()));
   }
}

#if 0
TEST_CASE("Byteswap Tests", "[utils][byteswap]") {
   SECTION("Check byteswap 1 byte") {
      CHECK(byteswap<uint8_t>(0x12) == 0x12);
      static_assert(std::is_same_v<decltype(byteswap<uint8_t>(0)), uint8_t>);
      uint8_t n = 34;
      CHECK(byteswap(n) == 34);
      int8_t n2 = 42;
      CHECK(byteswap(n2) == 42);
   }

   SECTION("Check byteswap 2 bytes" ) {
      CHECK(byteswap<uint16_t>(0x1234) == 0x3412);
      static_assert(std::is_same_v<decltype(byteswap<uint16_t>(0)), uint16_t>);
      uint16_t n = 0x1234;
      CHECK(byteswap(n) == 0x3412);
      //int16_t n2 = 0x5678;
      //CHECK(byteswap(n2) == 0x7856);
      //static_assert(std::is_same_v<decltype(byteswap(n2)), int16_t>);
   }

   SECTION("Check byteswap 4 bytes") {
      CHECK(byteswap<uint32_t>(0x12345678) == 0x78563412);
      static_assert(std::is_same_v<decltype(byteswap<uint32_t>(0)), uint32_t>);
      uint32_t n = 0x12345678;
      CHECK(byteswap(n) == 0x78563412);
      int32_t n2 = 0xabcdef12;
      CHECK(byteswap(n2) == 0x12efcdab);
      static_assert(std::is_same_v<decltype(byteswap(n2)), int32_t>);
   }

   SECTION("Check byteswap 8 bytes") {
      CHECK(byteswap<uint64_t>(0x1234567890ABCDEF) == 0xEFCDAB9078563412ul);
      static_assert(std::is_same_v<decltype(byteswap<uint64_t>(0)), uint64_t>);
      uint64_t n = 0x1234567890ABCDEFul;
      CHECK(byteswap(n) == 0xEFCDAB9078563412ul);
      int64_t n2 = 0xabcdef1234567890ul;
      CHECK(byteswap(n2) == 0x9078563412EFCDABul);
      static_assert(std::is_same_v<decltype(byteswap(n2)), int64_t>);
   }

   SECTION("Check byteswap 16 bytes") {
      using namespace astro::cryptid;
      CHECK(byteswap<uint128_t>(0x1234567890ABCDEF1234567890ABCDEF_ui128) == 0xEFCDAB9078563412EFCDAB9078563412_ui128);
      static_assert(std::is_same_v<decltype(byteswap<uint128_t>(0_ui128)), uint128_t>);
      uint128_t n = 0x1234567890ABCDEF1234567890ABCDEF_ui128;
      CHECK(byteswap(n) == 0xEFCDAB9078563412EFCDAB9078563412_ui128);
      //int128_t n2 = 0xabcdef1234567890abcdef1234567890abcdef_ui128;
      //CHECK(byteswap(n2) == 0x9078563412efcdab9078563412efcdab_ui128);
      //static_assert(std::is_same_v<decltype(byteswap(n2)), int128_t>);
   }
}
#endif