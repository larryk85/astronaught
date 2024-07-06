#pragma once

#include <cstddef>
#include <cstdint>

#include <type_traits>

namespace astro::memory {
   template <class Derived>
   class allocator_base {
      public:
         using self_t = Derived;

         template <class T, std::size_t N>
         constexpr inline T* allocate() {
            return dref().template allocate_impl<T>(N);
         }

         template <class T>
         constexpr inline T* allocate(std::size_t n) {
            return dref().template allocate_impl<T>(n);
         }

         template <class T>
         constexpr inline void deallocate(T* ptr) {
            dref().template deallocate_impl(ptr);
         }
      
      private:
         constexpr inline self_t& dref() noexcept {
            return static_cast<self_t&>(*this);
         }
   };
} // namespace astro::memory