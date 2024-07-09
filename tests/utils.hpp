#pragma once

#include <cstdio>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string_view>
#include <mutex>
#include <thread>

#include <astro/io/sink.hpp>

#include <unistd.h>

namespace astro::test {

   struct io_capturer {
      io_capturer(std::FILE** f) noexcept
         : lock(mtx) {
         curr_out = f;
         fflush(*curr_out);
         int32_t flush_dup = dup(fileno(*curr_out));
         orig_fd = dup(fileno(*curr_out));

         std::string tmpl = "capture_test_XXXXXX";
         int32_t tmp_fd = ::mkstemp(tmpl.data());
         temp_file = std::filesystem::temp_directory_path() / tmpl;
         close(tmp_fd);
         close(flush_dup);
         freopen(temp_file.c_str(), "w", *curr_out);
         fflush(*curr_out);
         freopen(temp_file.c_str(), "w", *curr_out);
      }

      bool is_same(std::string_view s) noexcept {
         std::string buff;
         buff.resize(std::filesystem::file_size(temp_file));
         fflush(*curr_out);
         std::ifstream f(temp_file);
         f.read(buff.data(), buff.size());
         f.close();
         return buff == s;
      }

      void reset() noexcept {
         fflush(*curr_out);
         freopen(temp_file.c_str(), "w", *curr_out);
      }

      ~io_capturer() noexcept {
         fflush(*curr_out);
         dup2(orig_fd, fileno(*curr_out));
         close(orig_fd);
         *curr_out = fdopen(fileno(*curr_out), "w");
         std::filesystem::remove(temp_file);
      }

      FILE** curr_out       = nullptr;
      int32_t orig_fd       = -1;
      std::string temp_file = "";
      std::lock_guard<std::mutex> lock;
      std::mutex                  mtx;
   };

   template <astro::io::stdio Output, typename Func>
   static inline bool capture_and_compare(Func&& func, std::string_view expected) noexcept {
      FILE* stream = nullptr;
      if (Output == astro::io::stdio::stdout) {
         stream = stdout;
      } else if (Output == astro::io::stdio::stderr) {
         stream = stderr;
      } else if (Output == astro::io::stdio::stdlog) {
         stream = stderr;
      } else {
         return false;
      }


      fflush(stream);
      int32_t dp      = dup(fileno(stream));
      int32_t orig_fd = dup(fileno(stream));

      if (dp == -1 || orig_fd == -1) {
         return false;
      }

      std::string tmpl = "capture_test_XXXXXX";
      int32_t tmp_fd = ::mkstemp(tmpl.data());
      std::string temp_file = std::filesystem::temp_directory_path() / tmpl;
      close(tmp_fd);
      close(dp);

      if (freopen(temp_file.c_str(), "w", stream) == nullptr) {
         return false;
      }

      func();

      std::string buff;
      auto fs = std::filesystem::file_size(temp_file);
      buff.resize(fs);

      std::ifstream f(temp_file);
      if (!f.is_open()) {
         return false;
      }

      f.read(buff.data(), fs);
      f.close();

      bool equals = buff == expected;

      fflush(stream);
      dup2(orig_fd, fileno(stream));
      close(orig_fd);

      if (stream != nullptr) {
         stream = fdopen(fileno(stream), "w");
      }

      //std::filesystem::remove(temp_file);
      std::cout << temp_file << std::endl;

      return buff.size() == expected.size() && std::memcmp(buff.data(), expected.data(), buff.size()) == 0;
   }
 
} // namespace astro::test

