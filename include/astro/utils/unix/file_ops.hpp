/**
 * @file file_ops.hpp
 * @brief File operation utilities for Unix systems.
 */

#pragma once

#include <cstdlib>
#include <cstdio>

#include <filesystem>
#include <string_view>
#include <iostream>

#include "../misc.hpp"
#include "../file_mode.hpp"

#include <unistd.h>
#include <fcntl.h>

namespace astro::util::detail {
   using file_type = int32_t;

   static inline file_type fopen_impl(std::string_view path, file_mode mode) noexcept {
      uint32_t access_mode = 0;
      access_mode |= set_if<uint32_t>(file_mode::read, mode, O_RDONLY);
      access_mode |= set_if<uint32_t>(file_mode::write, mode, O_WRONLY | O_CREAT);
      if (access_mode & O_WRONLY == O_WRONLY && access_mode & O_RDONLY == O_RDONLY)
         access_mode = O_RDWR | O_CREAT;
      access_mode |= set_if<uint32_t>(file_mode::append, mode, O_APPEND);
      return ::open(path.data(), access_mode, 0666);
   }

   static inline bool is_fd_open_impl(file_type fd) noexcept {
       return ::fcntl(fd, F_GETFL) != -1 || errno != EBADF;
   }

   static inline bool fclose_impl(file_type handle) noexcept {
      return ::close(handle) == 0;
   }

   static inline bool is_fd_invalid_impl(file_type handle) noexcept {
      return handle == -1;
   }

   static inline file_type fduplicate_impl(file_type handle) noexcept {
      file_type dup = ::dup(handle);
      check(dup != -1, "Failed to duplicate handle");
      return dup;
   }

   static inline std::int64_t fwrite_impl(file_type handle, const void* data, std::size_t size) noexcept {
      return ::write(handle, data, size);
   }

   static inline std::int64_t fread_impl(file_type handle, void* data, std::size_t size) noexcept {
      return ::read(handle, data, size);
   }

   /**
    * @brief Creates a C-style FILE* from a file handle.
    *
    * @param handle The file handle to convert.
    * @param mode The mode to open the file with.
    * @return The C-style FILE* or nullptr if an error occurred.
    */
   static inline FILE* to_cfile_impl(file_type handle, file_mode fmode, std::string_view mode) noexcept { return fdopen(handle, mode.data()); }

} // namespace astro::util

