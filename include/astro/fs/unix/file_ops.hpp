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

#include "../../utils/misc.hpp"
#include "../file_mode.hpp"

#include <unistd.h>
#include <fcntl.h>

/**
 * @namespace astro::util::detail
 * @brief Contains low-level utility functions for file operations on Unix-like systems.
 *
 * This namespace provides a set of inline functions for performing file operations
 * such as opening, closing, reading, writing, and duplicating file descriptors.
 * These functions are designed to work with Unix file descriptors and provide
 * a thin wrapper around system calls.
 */

/**
 * @typedef file_type
 * @brief Alias for the type representing a file descriptor.
 */

/**
 * @brief Opens a file and returns its file descriptor.
 *
 * @param path The path to the file.
 * @param mode The mode in which to open the file (e.g., read, write, append).
 * @return The file descriptor, or -1 if an error occurred.
 */

/**
 * @brief Checks if a file descriptor is open.
 *
 * @param fd The file descriptor to check.
 * @return True if the file descriptor is open, false otherwise.
 */

/**
 * @brief Closes a file descriptor.
 *
 * @param handle The file descriptor to close.
 * @return True if the file descriptor was successfully closed, false otherwise.
 */

/**
 * @brief Checks if a file descriptor is invalid.
 *
 * @param handle The file descriptor to check.
 * @return True if the file descriptor is invalid, false otherwise.
 */

/**
 * @brief Duplicates a file descriptor.
 *
 * @param handle The file descriptor to duplicate.
 * @return The duplicated file descriptor, or -1 if an error occurred.
 * @throws std::runtime_error if the duplication fails.
 */

/**
 * @brief Writes data to a file descriptor.
 *
 * @param handle The file descriptor to write to.
 * @param data A pointer to the data to write.
 * @param size The number of bytes to write.
 * @return The number of bytes written, or -1 if an error occurred.
 */

/**
 * @brief Reads data from a file descriptor.
 *
 * @param handle The file descriptor to read from.
 * @param data A pointer to the buffer to store the read data.
 * @param size The number of bytes to read.
 * @return The number of bytes read, or -1 if an error occurred.
 */

/**
 * @brief Converts a file descriptor to a C-style FILE*.
 *
 * @param handle The file descriptor to convert.
 * @param fmode The file mode to use for the conversion.
 * @param mode The mode string to pass to fdopen (e.g., "r", "w").
 * @return A pointer to the FILE structure, or nullptr if an error occurred.
 */
namespace astro::fs {
   using file_type = int32_t;
   constexpr static inline file_type invalid_file = -1;

   static inline file_type fopen_impl(std::string_view path, file_mode mode) noexcept {
      uint32_t access_mode = 0;
      access_mode |= set_if<uint32_t>(file_mode::read, mode, O_RDONLY);
      access_mode |= set_if<uint32_t>(file_mode::write, mode, O_WRONLY | O_CREAT);
      if ((access_mode & O_WRONLY) == O_WRONLY && (access_mode & O_RDONLY) == O_RDONLY)
         access_mode = O_RDWR | O_CREAT;
      access_mode |= set_if<uint32_t>(file_mode::append, mode, O_APPEND);
      return ::open(path.data(), access_mode, 0666);
   }

   static inline bool is_fd_open_impl(file_type fd) noexcept {
       return ::fcntl(fd, F_GETFL) != invalid_file || errno != EBADF;
   }

   static inline bool fclose_impl(file_type handle) noexcept {
      return ::close(handle) == 0;
   }

   static inline bool is_fd_invalid_impl(file_type handle) noexcept {
      return handle == invalid_file;
   }

   static inline file_type fduplicate_impl(file_type handle) noexcept {
      file_type dup = ::dup(handle);
      util::check(dup != -1, "Failed to duplicate handle");
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

