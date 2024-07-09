#define CATCH_CONFIG_WINDOWS_SEH
#include <catch2/catch_all.hpp>

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

      CHECK(rs.size() == 22);
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

      const auto is_custom_valid = [&](const auto& tmpl, const auto& s) {
         CHECK(tmpl.size() == s.size());
         CHECK((std::string_view)tmpl != s);
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

TEST_CASE("Random File Tests", "[utils][random_string][temporary_file]") {
   SECTION("Check file name generation") {
      auto tmp_dir = std::filesystem::temp_directory_path();
      auto fn = generate_temp_file_name("");

      CHECK(tmp_dir.string() == fn);

      fn = generate_temp_file_name("tmp-%%%-%%%");
      is_valid((tmp_dir / "tmp-%%%-%%%").string(), fn);
      CHECK(fn.starts_with((tmp_dir/"tmp-").string()));
   }
}