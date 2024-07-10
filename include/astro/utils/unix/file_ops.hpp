/**
 * @file file_ops.hpp
 * @brief File operation utilities for Unix systems.
 */

#pragma once

#include <cstdlib>
#include <cstdio>

#include <filesystem>
#include <string_view>

#include "../../utils/misc.hpp"

#include <unistd.h>
#include <fcntl.h>


namespace astro::util {
   using file_type = int32_t;
   /**
    * @brief File mode constants.
    */
   constexpr static inline std::size_t fmode_read      = 0x1000; ///< Read mode.
   constexpr static inline std::size_t fmode_write     = 0x2000; ///< Write mode.
   constexpr static inline std::size_t fmode_append    = 0x8000; ///< Append mode.
   constexpr static inline std::size_t fmode_overwrite = 0x4000; ///< Overwrite mode.

   /**
    * @brief Helper macro to set flags based on mode.
    * @param mode The mode to check against.
    * @param var The variable to set the flag on.
    * @param flag The flag to set.
    */
   #define set_if(mode, var, flag) if constexpr ((Mode & mode) == mode) var |= flag

   /**
    * @brief Opens a file.
    * @tparam Mode The mode to open the file with.
    * @param path The path to the file.
    * @return The file handle.
    * @throws None.
    */
   template <std::size_t Mode>
   static inline file_type fopen(std::string_view path) noexcept {
      mode_t access_mode = 0;
      set_if(fmode_read, access_mode, O_RDONLY);
      set_if(fmode_write, access_mode, O_WRONLY);
      set_if(fmode_read | fmode_write, access_mode, O_RDWR);
      set_if(fmode_append, access_mode, O_APPEND);
      set_if(fmode_overwrite, access_mode, O_CREAT);

      return ::open(path.data(), access_mode, 0666);
   }

   /**
    * @brief Closes a file handle.
    * @param handle The handle to close.
    * @return True if the handle was successfully closed, false otherwise.
    */
   static inline bool fclose(file_type handle) noexcept {
      return ::close(handle) == 0;
   }

   /**
    * @brief Duplicates a file handle.
    * @param handle The handle to duplicate.
    * @return The duplicated handle.
    */
   static inline file_type fduplicate(file_type handle) noexcept {
      file_type dup = ::dup(handle);
      check(dup != -1, "Failed to duplicate handle");
      return dup;
   }

   #undef set_if

} // namespace astro::util
