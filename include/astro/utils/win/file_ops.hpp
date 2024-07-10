/**
 * @file file_ops.hpp
 * @brief File operation utilities for Windows systems.
 */

#pragma once

#include <cstdlib>
#include <cstdio>

#include <filesystem>
#include <string_view>

#include "../../utils/misc.hpp"

#define NOMINMAX
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#undef NOMINMAX

namespace astro::util {
   using file_type = HANDLE;
   /**
    * @brief File mode constants.
    */
   constexpr static inline int32_t fmode_read      = GENERIC_READ;  ///< Read mode.
   constexpr static inline int32_t fmode_write     = GENERIC_WRITE; ///< Write mode.
   constexpr static inline int32_t fmode_append    = 0x8000;        ///< Append mode.
   constexpr static inline int32_t fmode_overwrite = 0x4000;        ///< Overwrite mode.

   /**
    * @brief Helper macro to set flags based on mode.
    * @param MODE The mode to check against.
    * @param VAR The variable to set the flag on.
    * @param FLAG The flag to set.
    */
   #define set_if(MODE, VAR, FLAG) if ((mode & MODE) == MODE) VAR |= FLAG

   /**
    * @brief Opens a file.
    * @tparam Mode The mode to open the file with.
    * @param path The path to the file.
    * @return The file handle.
    * @throws None.
    */
   static inline file_type fopen(std::string_view path, std::size_t mode) noexcept {
      DWORD access_mode = mode & ~(fmode_append | fmode_overwrite);
      DWORD shared_mode = 0;
      DWORD disposition = OPEN_EXISTING;
      set_if(fmode_read, shared_mode, FILE_SHARE_READ);
      set_if(fmode_write, shared_mode, FILE_SHARE_WRITE);
      set_if(fmode_append, disposition, OPEN_ALWAYS);
      set_if(fmode_overwrite, disposition, CREATE_ALWAYS);

      return ::CreateFileA(path.data(), access_mode, shared_mode, nullptr, disposition, FILE_ATTRIBUTE_NORMAL, nullptr);
   }

   /**
    * @brief Duplicates a file handle.
    * @param handle The handle to duplicate.
    * @return The duplicated handle.
    * @throws None.
    */
   static inline file_type fduplicate(file_type handle) noexcept {
      file_type dup = 0;
      bool res = ::DuplicateHandle(GetCurrentProcess(), handle, GetCurrentProcess(), &dup, 0, FALSE, DUPLICATE_SAME_ACCESS);
      check(res, "Failed to duplicate handle");
      return dup;
   }

   /**
    * @brief Closes a file handle.
    * @param handle The handle to close.
    * @return True if the handle was successfully closed, false otherwise.
    * @throws None.
    */
   static inline bool fclose(file_type handle) noexcept {
      return ::CloseHandle(handle) != 0;
   }

   /**
    * @brief Writes to a file.
    * @param handle The handle to write to.
    * @param data The data to write.
    * @param size The size of the data to write.
    * @return The number of bytes written.
    * @throws None.
    */
   static inline std::size_t fwrite(file_type handle, const void* data, DWORD size) noexcept {
      DWORD written = 0;
      ::WriteFile(handle, data, size, &written, nullptr);
      return written;
   }

   /**
    * @brief Read from a file.
    * @param handle The handle to read from.
    * @param data The data to read into.
    * @param size The size of the data to read.
    * @return The number of bytes read.
    * @throws None.
    */
   static inline std::size_t fread(file_type handle, void* data, DWORD size) noexcept {
      DWORD read = 0;
      ::ReadFile(handle, data, size, &read, nullptr);
      return read;
   }

   /**
    * @brief Creates a C-style FILE* from a file handle.
    *
    * @param handle The file handle to convert.
    * @param mode The mode to open the file with.
    * @return The C-style FILE* or nullptr if an error occurred.
    */
   static inline FILE* to_cfile(file_type handle, DWORD mode) noexcept {
      char file_mode_str[4];
      file_mode_str[0] = (mode & fmode_read) ? 'r' : 'w';
      file_mode_str[1] = 'b';
      file_mode_str[2] = (mode & fmode_read) && (mode & fmode_write) ? '+' : '\0';
      file_mode_str[3] = '\0';

      int os_file_descriptor = _open_osfhandle((intptr_t)handle, mode);
      if (os_file_descriptor == -1) {
         return nullptr;
      }

      return _fdopen(os_file_descriptor, file_mode_str);
   }

   #undef set_if
} // namespace astro::util
