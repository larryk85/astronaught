#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <span>

#include "serializer.hpp"

namespace astro::serial {

   class alpha : public serializer_base {
      public:
         using serializer_base::serializer_base;
         alpha() = default;
         ~alpha() = default;

         alpha(const alpha&) = default;
         alpha(alpha&&) = default;
         alpha& operator=(const alpha&) = default;
         alpha& operator=(alpha&&) = default;

         template <typename T>
         constexpr inline void pack(T&& v) {
           serialize(*this, std::forward<T>(v));
         }

         template <typename T>
         constexpr inline void push(T&& v) {
            serialize(*this, std::forward<T>(v));
         }

         template <typename T>
         constexpr inline T pop() {
            return deserialize<T>(*this);
         }

         template <typename T>
         constexpr inline std::span<T> view() {
            return std::span<T>(reinterpret_cast<T*>(begin()), size()/sizeof(T));
         }
   };

   template <util::numeric_type N>
   constexpr static inline void serialize(alpha& a, N val) noexcept {
      a.write(&val, sizeof(N));
   }

   template <util::numeric_type N>
   constexpr static inline N deserialize(alpha& a) noexcept {
      N val;
      a.read(&val, sizeof(N));
      return val;
   }

   template <util::string_type S>
   constexpr static inline void serialize(alpha& a, const S& val) noexcept {
      a.write(val.data(), val.size());
   }

   template <util::string_type S>
   constexpr static inline S deserialize(alpha& a) noexcept {
      S val;
      val.resize(a.size());
      a.read(val.data(), val.size());
      return val;
   }
} // namespace astro::serial