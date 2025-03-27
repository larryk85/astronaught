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
#include "../file_mode.hpp"

#define NOMINMAX
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#undef NOMINMAX

namespace astro::util::detail {
   using file_type = HANDLE;
   constexpr static inline file_type invalid_file = INVALID_HANDLE_VALUE;

   static inline file_type fopen_impl(std::string_view path, file_mode mode) noexcept {
      DWORD access_mode = static_cast<DWORD>(mode) & ~(static_cast<DWORD>(file_mode::append));
      DWORD shared_mode = 0;
      DWORD disposition = CREATE_ALWAYS;
      shared_mode |= set_if<DWORD>(file_mode::read, mode, FILE_SHARE_READ);
      disposition |= set_if<DWORD>(file_mode::read, mode, OPEN_EXISTING);
      shared_mode |= set_if<DWORD>(file_mode::write, mode, FILE_SHARE_WRITE);
      disposition |= set_if<DWORD>(file_mode::append, mode, OPEN_ALWAYS);

      return ::CreateFileA(path.data(), access_mode, shared_mode, nullptr, disposition, FILE_ATTRIBUTE_NORMAL, nullptr);
   }

   /**
    * @brief Checks if a file handle is still open.
    * @param handle The file handle to check.
    * @return True if the file handle is open, false otherwise.
    */
   static inline bool is_fd_open_impl(file_type handle) noexcept {
       return GetFileType(handle) != FILE_TYPE_UNKNOWN || GetLastError() != ERROR_INVALID_HANDLE;
   }

   static inline bool is_fd_invalid_impl(file_type handle) noexcept {
      return handle == invalid_file;
   }

   /**
    * @brief Duplicates a file handle.
    * @param handle The handle to duplicate.
    * @return The duplicated handle.
    * @throws None.
    */
   static inline file_type fduplicate_impl(file_type handle) noexcept {
      file_type dup = 0;
      bool res = ::DuplicateHandle(GetCurrentProcess(), handle, GetCurrentProcess(), &dup, 0, FALSE, DUPLICATE_SAME_ACCESS);
      util::check(res, "Failed to duplicate handle");
      return dup;
   }

   /**
    * @brief Closes a file handle.
    * @param handle The handle to close.
    * @return True if the handle was successfully closed, false otherwise.
    * @throws None.
    */
   static inline bool fclose_impl(file_type handle) noexcept {
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
   static inline std::int64_t fwrite_impl(file_type handle, const void* data, std::size_t size) noexcept {
      DWORD written = 0;
      ::WriteFile(handle, data, static_cast<int32_t>(size), &written, nullptr);
      return static_cast<std::int64_t>(written);
   }

   /**
    * @brief Read from a file.
    * @param handle The handle to read from.
    * @param data The data to read into.
    * @param size The size of the data to read.
    * @return The number of bytes read.
    * @throws None.
    */
   static inline std::int64_t fread_impl(file_type handle, void* data, std::size_t size) noexcept {
      DWORD read = 0;
      ::ReadFile(handle, data, static_cast<DWORD>(size), &read, nullptr);
      return static_cast<std::int64_t>(read);
   }

   /**
    * @brief Creates a C-style FILE* from a file handle.
    *
    * @param handle The file handle to convert.
    * @param mode The mode to open the file with.
    * @return The C-style FILE* or nullptr if an error occurred.
    */
   static inline FILE* to_cfile_impl(file_type handle, file_mode fmode, std::string_view mode) noexcept {
      int os_file_descriptor = _open_osfhandle((intptr_t)handle, static_cast<int32_t>(fmode));
      if (os_file_descriptor == -1) {
         return nullptr;
      }

      return _fdopen(os_file_descriptor, mode.data());
   }

} // namespace astro::util
