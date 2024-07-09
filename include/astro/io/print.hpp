#pragma once

#include <cstdio>
#include <cstdint>

#include <array>
#include <memory>
#include <vector>

#include "../utils/misc.hpp"

namespace astro::io {
   constexpr static inline std::size_t default_buffer_size = 2048;


   template <auto Sink, std::size_t BufferSize=default_buffer_size>
   struct write_buffer {
      using ptr_t = std::unique_ptr<char[]>;
      ptr_t data = std::make_unique<char[]>(BufferSize);
      std::size_t             size = 0;

      write_buffer() = default;
      write_buffer(const write_buffer&) = default;
      write_buffer(write_buffer&&) = default;
      write_buffer& operator=(const write_buffer&) = default;
      write_buffer& operator=(write_buffer&&) = default;

      constexpr inline std::size_t capacity() const noexcept { return BufferSize; }
      constexpr inline std::size_t remaining() const noexcept { return BufferSize - size; }
      constexpr inline bool empty() const noexcept { return size == 0; }
      constexpr inline bool full() const noexcept { return size == BufferSize; }
      constexpr inline void clear() noexcept { size = 0; }
      constexpr inline void reset() noexcept { size = 0; data = std::make_unique<char[]>(BufferSize); }

      inline void sync() noexcept {
         fputs(data.get(), Sink);
      }

      inline int32_t put(std::string_view value) noexcept {
         if (size + value.size() > BufferSize) {
            return -1;
         }

         std::memcpy(data.get() + size, value.data(), value.size());
         size += value.size();
         return 0;
      }
   };

   template <typename T>
   inline int32_t put(char *ptr, std::size_t sz, T&& value) noexcept {
   }

} // namespace astro::io