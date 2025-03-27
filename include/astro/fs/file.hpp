#pragma once

#include <cstdlib>
#include <cstdio>

#include <filesystem>
#include <fstream>
#include <string_view>
#include <utility>

#include "file_ops.hpp"
#include "file_mode.hpp"

namespace astro::fs {

   class file {
      public:
         using path_t = std::filesystem::path;

         file() = default;
         inline file(const file& other)
            : _file(fduplicate(other._file)) {
         }

         file(file&& other) = default;

         inline file(const path_t& path, file_mode mode = file_mode::error)
            : _file(fopen(path.string(), mode)), _mode(mode), _path(path) {
         }

         inline file(const file_type& f)
            : _file(f) {
         }

         inline file& operator=(const file& other) {
            _file = fduplicate(other._file);
            return *this;
         }

         file& operator=(file&& other) = default;

         ~file() { if (_file != invalid_file) close(); }

         inline bool close() noexcept { return fclose(_file); }

         inline bool open(const path_t& path, file_mode mode = file_mode::error) noexcept {
            if (close()) {
               _file = fopen(path.string(), mode);
               _mode = mode;
               _path = path;
               return is_fd_invalid(_file);
            } else {
               return false;
            }
         }

         inline bool is_open() const noexcept {
            return is_fd_open(_file);
         }

         inline operator bool() const noexcept {
            return is_fd_open(_file);
         }

         inline operator FILE*() {
            auto fd = to_cfile(_file, _mode);
            util::check(fd, "Failed to create cfile from file: " + _path.string());
            return fd;
         }

         inline operator std::fstream() {
            auto fd = to_cfile(_file, _mode);
            util::check(fd, "Failed to create std::fstream from file: " + _path.string());
            return std::fstream(); //fd);
         }

      private:
         file_type _file = invalid_file;
         file_mode _mode = file_mode::error;
         path_t    _path = "";
   };

} // namespace astro::fs