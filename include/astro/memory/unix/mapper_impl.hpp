#pragma once


#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <cstddef>
#include <cstdint>

#include <limits>
#include <unordered_map>

#include "../mapper_base.hpp"
#include "../modes.hpp"

namespace astro::memory {
   constexpr static inline uint8_t access_mode_to_unix_mode(access_mode mode) {
      switch (mode) {
         case access_mode::read:
            return PROT_READ;
         case access_mode::write:
            return PROT_WRITE;
         case access_mode::execute:
            return PROT_EXEC;
         case access_mode::read_write:
            return PROT_READ | PROT_WRITE;
         case access_mode::read_execute:
            return PROT_READ | PROT_EXEC;
         case access_mode::write_execute:
            return PROT_WRITE | PROT_EXEC;
         case access_mode::read_write_execute:
            return PROT_READ | PROT_WRITE | PROT_EXEC;
         case access_mode::none:
            return PROT_NONE;
         default:
            return PROT_NONE;
      }
   }

   constexpr static inline std::size_t base_page_size = 4096;

   class memory_mapper : public mapper_base<memory_mapper> {
      public:
         inline void* map_impl(std::size_t sz, access_mode mode) {
            int32_t prot = access_mode_to_unix_mode(mode);
            void* addr = mmap(nullptr, sz, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            util::check(addr != MAP_FAILED, "Failed to map memory");
            return addr;
         }

         inline void protect_impl(void* ptr, std::size_t sz, access_mode mode) {
            int32_t prot = access_mode_to_unix_mode(mode);
            int32_t result = mprotect(ptr, sz, prot);
            util::check(result == 0, "Failed to protect memory");
         }

         inline int32_t unmap_impl(void* ptr, std::size_t sz) {
            int32_t result = munmap(ptr, sz);
            util::check(result == 0, "Failed to unmap memory");
            return result;
         }

         inline std::size_t page_size_impl() const noexcept {
            return get_page_size();
         }

      private:
         inline std::size_t get_page_size() const noexcept {
            return sysconf(_SC_PAGESIZE);
         }
   };
} // namespace astro::memory