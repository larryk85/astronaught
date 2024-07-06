#pragma once

#include <cstdint>

namespace astro::memory {

   class tagged_ptr {
      public:
         constexpr inline tagged_ptr(std::uint64_t ptr, std::uint16_t tag) 
            : _ptr(ptr), _tag(tag) {}

         template <typename T>
         constexpr inline tagged_ptr(T* ptr, std::uint16_t tag) 
            : _ptr(reinterpret_cast<std::uint64_t>(ptr)), _tag(tag) {}

         inline tagged_ptr(void* ptr, std::uint16_t tag) 
            : _ptr(reinterpret_cast<std::uint64_t>(ptr)), _tag(tag) {}

         constexpr tagged_ptr(const tagged_ptr&) = default;
         constexpr tagged_ptr(tagged_ptr&&) = default;
         constexpr tagged_ptr& operator=(const tagged_ptr&) = default;
         constexpr tagged_ptr& operator=(tagged_ptr&&) = default;
         constexpr ~tagged_ptr() = default;

         inline const void* ptr() const noexcept { 
            return reinterpret_cast<const void*>(_ptr); 
         }
         inline void* ptr() noexcept { 
            return reinterpret_cast<void*>(_ptr); 
         }
         inline void ptr(void* ptr) noexcept { 
            _ptr = reinterpret_cast<std::uint64_t>(ptr); 
         }

         template <typename U>
         constexpr inline const U* as_ptr() const noexcept { 
            return reinterpret_cast<const U*>(_ptr); 
         }
         template <typename U>
         constexpr inline U* as_ptr() noexcept { 
            return reinterpret_cast<U*>(_ptr); 
         }

         template <typename U>
         constexpr inline const U& as() const noexcept { 
            return *as_ptr<U>(); 
         }
         template <typename U>
         constexpr inline U& as() noexcept { 
            return *as_ptr<U>(); 
         }


         constexpr inline std::uint64_t tag() const noexcept { return _tag; }
         constexpr inline void tag(std::uint16_t tag) noexcept { _tag = tag; }

         inline void* operator->() noexcept { return ptr(); }
         inline const void* operator->() const noexcept { return ptr(); }

      private:
         std::uint64_t _ptr : 47;
         std::uint64_t _tag : 17;
   };
}