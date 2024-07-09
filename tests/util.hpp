#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#ifdef _WIN32
#define NOMINMAX
#include <io.h>
#undef NOMINMAX
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#else
#include <unistd.h>
#endif

#include <astro/io/sink.hpp>

namespace astro::test {
   template <astro::io::stdio Output, typename Func>
   static inline std::string capture(Func&& func) noexcept {
      /*
      FILE* stream = nullptr;
      if (Output == astro::io::stdio::out) {
         stream = stdout;
      } else if (Output == astro::io::stdio::err || Output == astro::io::stdio::log) {
         stream = stderr;
      }

      fflush(stream);
      int32_t orig_fd = dup(fileno(stream));

      std::string tmpl = std::filesystem::temp_directory_path().string() + "/capture_test_XXXXXX";
      char temp_filename[tmpl.size() + 1];
      std::memcpy(temp_filename, tmpl.data(), tmpl.size());
      int32_t tmp_fd = ::mkstemp(temp_filename);
      if (tmp_fd == -1) {
         close(orig_fd);
         return "";
      }

      std::string temp_file = temp_filename;
      close(tmp_fd);

      if (freopen(temp_file.c_str(), "w", stream) == nullptr) {
         close(orig_fd);
         return "";
      }

      func();

      fflush(stream);
      freopen(temp_file.c_str(), "r", stream);  

      std::ifstream f(temp_file);
      if (!f.is_open()) {
         freopen("/dev/tty", "w", stream); // Restore stdout/stderr to the terminal
         close(orig_fd);
         return "";
      }

      std::string buff;
      auto fs = std::filesystem::file_size(temp_file);
      buff.resize(fs);
      f.read(buff.data(), fs);
      f.close();
      freopen("/dev/tty", "w", stream);
      fflush(stream);
      dup2(orig_fd, fileno(stream));
      close(orig_fd);

      std::filesystem::remove(temp_file);
      
      return buff;
      */

      return "";
   }

   template <astro::io::stdio Output, typename Func>
   static inline bool capture_and_compare(Func&& func, std::string_view expected) noexcept {
      return capture<Output>(func) == expected;
   }
} // namespace astro::test