#pragma once

#include <cstddef>
#include <cstdint>

#include <limits>

#include "../utils.hpp"
#include "modes.hpp"

namespace astro::memory {
   struct memory_info {
      std::size_t size;
      access_mode mode;
   };

   template <class Derived>
   class mapper_base {
      public:
         using self_t = Derived;

         inline mapper_base() { mapping.reserve(256); }

         [[nodiscard]] constexpr inline void* map(std::size_t n, access_mode mode=access_mode::none) { 
            void* ptr = dref().map_impl(n, mode);
            mapping[ptr] = {n, mode};
            return dref().map_impl(n, mode); 
         }

         template <std::size_t N>
         [[nodiscard]] constexpr inline void* map(access_mode mode=access_mode::none) { return map(N, mode); }

         [[nodiscard]] constexpr inline decltype(auto) protect(void* ptr, std::size_t n, access_mode mode=access_mode::none) { 
            auto& curr_mode = mapping[ptr].mode;
            auto prev_mode = curr_mode;
            curr_mode = mode;
            dref().protect_impl(ptr, n, curr_mode); 
            return prev_mode;
         }

         template <std::size_t N>
         [[nodiscard]] constexpr inline decltype(auto) protect(void* ptr, access_mode mode=access_mode::none) { return protect(ptr, N, mode); }

         [[nodiscard]] constexpr inline decltype(auto) unmap(void* ptr, std::size_t n=0) { 
            mapping.erase(ptr);
            return dref().unmap_impl(ptr, n); 
         }

         [[nodiscard]] constexpr inline access_mode mode(void* ptr) const { return mapping.at(ptr).mode; }

         [[nodiscard]] constexpr inline std::size_t size(void* ptr) const { return mapping.at(ptr).size; }

         [[nodiscard]] constexpr inline std::size_t page_size() const noexcept { return dref().page_size_impl(); }

      private:
         constexpr inline self_t& dref() noexcept {
            return static_cast<self_t&>(*this);
         }

         constexpr inline const self_t& dref() const noexcept {
            return static_cast<const self_t&>(*this);
         }

         std::unordered_map<void*, memory_info> mapping;
   };
} // namespace astro::memory