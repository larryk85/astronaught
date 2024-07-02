#pragma once

#include <cstdint>

namespace versa::memory {

   class tagged_ptr {
      public:
         constexpr inline tagged_ptr(std::uint64_t ptr, std::uint16_t tag) 
            : _ptr(ptr), _tag(tag) {}

         constexpr inline tagged_ptr(void* ptr, std::uint16_t tag) 
            : _ptr(reinterpret_cast<std::uint64_t>(ptr)), _tag(tag) {}

         constexpr tagged_ptr(const tagged_ptr&) = default;
         constexpr tagged_ptr(tagged_ptr&&) = default;
         constexpr tagged_ptr& operator=(const tagged_ptr&) = default;
         constexpr tagged_ptr& operator=(tagged_ptr&&) = default;
         constexpr ~tagged_ptr() = default;

         constexpr inline const void* ptr() const noexcept { return reinterpret_cast<void*>(_ptr); }
         constexpr inline void* ptr() noexcept { return reinterpret_cast<void*>(_ptr); }
         constexpr inline void ptr(void* ptr) noexcept { _ptr = reinterpret_cast<std::uint64_t>(ptr); }

         constexpr inline std::uint64_t tag() const noexcept { return _tag; }
         constexpr inline void tag(std::uint16_t tag) noexcept { _tag = tag; }

      private:
         std::uint64_t _ptr : 47;
         std::uint64_t _tag : 17;
   };

   template <typename T>
   class discriminant : public tagged_ptr {
      public:
         using tagged_ptr::tagged_ptr;
         using tagged_ptr::tag;

         constexpr inline discriminant(T* ptr, std::uint16_t tag) 
            : tagged_ptr(ptr, tag) {}

         constexpr inline const T* ptr() const noexcept { return tagged_ptr::ptr(); }
         constexpr inline T* ptr() noexcept { return tagged_ptr::ptr(); }
         constexpr inline void ptr(T* ptr) noexcept { tagged_ptr(ptr); }
   };

}