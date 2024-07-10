#pragma once

#include <cstdlib>
#include <cstdio>

#include <filesystem>
#include <fstream>
#include <string_view>

#if ASTRO_OS == ASTRO_WINDOWS_BUILD
   #include "win/file_ops.hpp"
#else
   #include "unix/file_ops.hpp"
#endif

namespace astro::util {

   class file {
      public:
         using path_t = std::filesystem::path;

         file() = default;
         inline file(const file& other) 
            : _file(fduplicate(other._file)) {
         }

         file(file&& other) = default;

         inline file(const path_t& path, int32_t mode = 0) 
            : _file(fopen(path.string(), mode)), _mode(mode), _path(path) {
         }

         inline file& operator=(const file& other) {
            _file = fduplicate(other._file);
            return *this;   
         }

         file& operator=(file&& other) = default;

         ~file() {
            if (_file) {
               fclose(_file);
            }
         }

         inline operator FILE*() {
            auto fd = to_cfile(_file, _mode);
            util::check(fd, "Failed to create cfile from file: " + _path.string());
            return fd;
         }

         inline operator std::fstream() {
            auto fd = to_cfile(_file, _mode);
            util::check(fd, "Failed to create std::fstream from file: " + _path.string());
            return std::fstream(fd);
         }

      private:
         file_type _file = 0;
         int32_t   _mode = 0;
         path_t    _path = "";
   };

}