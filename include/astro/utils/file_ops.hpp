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

#if ASTRO_OS == ASTRO_WINDOWS_BUILD
   #include "win/file_ops.hpp"
#else
   #include "unix/file_ops.hpp"
#endif

#include "file_mode.hpp"
#include "misc.hpp"

namespace astro::util {
   using file_type = detail::file_type; // OS defined

   /**
    * @brief Opens a file.
    * @tparam Mode The mode to open the file with.
    * @param path The path to the file.
    * @return The file handle.
    * @throws None.
    */
   static inline file_type fopen(std::string_view path, file_mode mode) noexcept {
      return detail::fopen_impl(path, mode);
   }

   /**
    * @brief Checks if a file descriptor is still open.
    * @param fd The file descriptor to check.
    * @return True if the file descriptor is open, false otherwise.
    */
   static inline bool is_fd_open(file_type fd) noexcept { return detail::is_fd_open_impl(fd); }

   /**
    * @brief  Checks if a file descriptor / handle is invalid.
    * @param handle
    * @return true
    * @return false
    */
   static inline bool is_fd_invalid(file_type handle) noexcept { return detail::is_fd_invalid_impl(handle); }

   /**
    * @brief Closes a file handle.
    * @param handle The handle to close.
    * @return True if the handle was successfully closed, false otherwise.
    */
   static inline bool fclose(file_type handle) noexcept { return detail::fclose_impl(handle) == 0; }


   /**
    * @brief Duplicates a file handle.
    * @param handle The handle to duplicate.
    * @return The duplicated handle.
    */
   static inline file_type fduplicate(file_type handle) noexcept { return detail::fduplicate_impl(handle); }

   /**
    * @brief Writes to a file.
    * @param handle The handle to write to.
    * @param data The data to write.
    * @param size The size of the data to write.
    * @return The number of bytes written.
    * @throws None.
    */
   static inline std::int64_t fwrite(file_type handle, const void* data, std::size_t size) noexcept {
      return detail::fwrite_impl(handle, data, size);
   }

   /**
    * @brief Read from a file.
    * @param handle The handle to read from.
    * @param data The data to read into.
    * @param size The size of the data to read.
    * @return The number of bytes read.
    * @throws None.
    */
   static inline std::int64_t fread(file_type handle, void* data, std::size_t size) noexcept {
      return detail::fread_impl(handle, data, size);
   }

   /**
    * @brief Creates a C-style FILE* from a file handle.
    * @param handle The file handle to convert.
    * @param mode The mode to open the file with.
    * @return The C-style FILE* or nullptr if an error occurred.
    */
   static inline FILE* to_cfile(file_type handle, file_mode mode) noexcept {
      char file_mode_str[4] = {0};
      file_mode_str[0] = ((mode & file_mode::read) == file_mode::read) ? 'r' : 'w';
      if (((mode & file_mode::read) == file_mode::read) && ((mode & file_mode::write) == file_mode::write)) {
         file_mode_str[1] = '+';
         file_mode_str[2] = 'b';
      } else {
         file_mode_str[1] = 'b';
      }

      return detail::to_cfile_impl(handle, mode, std::string_view{file_mode_str, 4});
   }

} // namespace astro::util