#pragma once

#include <cstddef>
#include <cstdint>

#include <limits>

#include "utils.hpp"

namespace versa::memory {
   enum class access_mode : std::uint8_t {
      none = 0,
      read = 1,
      write = 2,
      execute = 4,
      read_write = read | write,
      read_execute = read | execute,
      write_execute = write | execute,
      read_write_execute = read | write | execute
   };

   enum class protection_mode : std::uint8_t {

   };

   template <class Derived>
   class mapper_base {
      public:
         using self_t = Derived;

         template <std::size_t N>
         [[nodiscard]] constexpr inline decltype(auto) map(access_mode mode=access_mode::none) { return dref().map_impl(N, mode); }
         [[nodiscard]] constexpr inline decltype(auto) map(std::size_t n, access_mode mode=access_mode::none) { return dref().map_impl(n, mode); }
         template <std::size_t N>
         [[nodiscard]] constexpr inline decltype(auto) protect(void* ptr, access_mode mode=access_mode::none) { return dref().protect_impl(ptr, N, mode); }
         [[nodiscard]] constexpr inline decltype(auto) protect(void* ptr, std::size_t n, access_mode mode=access_mode::none) { return dref().protect_impl(ptr, n, mode); }
         [[nodiscard]] constexpr inline decltype(auto) unmap(void* ptr) { return dref().unmap_impl(ptr); }

         

         constexpr inline std::size_t page_size() const noexcept { return dref().page_size_impl(); }

      private:
         constexpr inline self_t& dref() noexcept {
            return static_cast<self_t&>(*this);
         }

         constexpr inline const self_t& dref() const noexcept {
            return static_cast<const self_t&>(*this);
         }
   };

} // namespace versa::memory