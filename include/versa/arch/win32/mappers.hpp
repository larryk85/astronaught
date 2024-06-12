#pragma once

#include <cstddef>
#include <cstdint>

#include <windows.h>

#include <limits>

#include "../../mapper.hpp"

namespace versa::memory {
   constexpr static inline uint8_t access_mode_to_win_mode(access_mode mode) {
      switch (mode) {
         case access_mode::read:
            return PAGE_READONLY;
         case access_mode::write:
            return PAGE_READWRITE;
         case access_mode::execute:
            return PAGE_EXECUTE;
         case access_mode::read_write:
            return PAGE_READWRITE;
         case access_mode::read_execute:
            return PAGE_EXECUTE_READ;
         case access_mode::write_execute:
            return PAGE_EXECUTE_READWRITE;
         case access_mode::read_write_execute:
            return PAGE_EXECUTE_READWRITE;
         case access_mode::none:
            return PAGE_NOACCESS;
         default:
            return PAGE_NOACCESS;
      }
   }

   constexpr static inline access_mode win_mode_to_access_mode(int32_t mode) {
      switch (mode) {
         case PAGE_READONLY:
            return access_mode::read;
         case PAGE_READWRITE:
            return access_mode::write;
         case PAGE_EXECUTE:
            return access_mode::execute;
         case PAGE_EXECUTE_READ:
            return access_mode::read_execute;
         case PAGE_EXECUTE_READWRITE:
            return access_mode::read_write_execute;
         case PAGE_NOACCESS:
            return access_mode::none;
         default:
            return access_mode::none;
      }
   }

   constexpr static inline std::size_t base_page_size = 4096;

   template <std::size_t PageSize=base_page_size>
   class memory_mapper : public mapper_base<memory_mapper<PageSize>> {
      public:

         inline void* map_impl(std::size_t sz, access_mode mode) {
            void* addr = VirtualAlloc(nullptr, sz, MEM_COMMIT | MEM_RESERVE, access_mode_to_win_mode(mode));
            util::check(addr != nullptr, "Failed to map memory");
            return addr;
         }

         inline access_mode protect_impl(void* ptr, std::size_t sz, access_mode mode) {
            DWORD old_protect;
            util::check(VirtualProtect(ptr, sz, access_mode_to_win_mode(mode), &old_protect), "Failed to protect memory");
            return win_mode_to_access_mode(old_protect);
         }

         inline int32_t unmap_impl(void* ptr) {
            util::check(VirtualFree(ptr, 0, MEM_RELEASE), "Failed to unmap memory");
            return 1;
         }

         inline std::size_t page_size_impl() const noexcept {
            return get_page_size();
         }

      private:
         inline auto get_system_info() const noexcept {
            SYSTEM_INFO sys_info;
            GetSystemInfo(&sys_info);
            return sys_info;
         }

         inline auto get_page_size() const noexcept {
            static const auto info = get_system_info();
            return info.dwPageSize;
         }

         std::size_t mapped = 0;
   };

} // namespace versa::memory