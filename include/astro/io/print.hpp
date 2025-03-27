#pragma once

#include <cstdio>
#include <cstdint>

#include <array>
#include <memory>
#include <vector>

#include "../utils/misc.hpp"

namespace astro::io {
   constexpr static inline std::size_t default_buffer_size = 2048;

   template <auto Sink, std::size_t BufferSize=default_buffer_size, auto sinkWriter=std::fwrite>
   struct write_buffer {
      using ptr_t = std::unique_ptr<char[]>;
      ptr_t data = std::make_unique<char[]>(BufferSize);
      std::size_t index = 0;

      write_buffer() = default;
      write_buffer(const write_buffer&) = default;
      write_buffer(write_buffer&&) = default;
      write_buffer& operator=(const write_buffer&) = default;
      write_buffer& operator=(write_buffer&&) = default;

      constexpr inline std::size_t capacity() const noexcept { return BufferSize; }
      constexpr inline std::size_t remaining() const noexcept { return BufferSize - index; }
      constexpr inline bool empty() const noexcept { return index == 0; }
      constexpr inline bool full() const noexcept { return index == BufferSize; }
      constexpr inline void clear() noexcept { index = 0; }
      constexpr inline void reset() noexcept { index = 0; data = std::make_unique<char[]>(BufferSize); }
      inline void flush() noexcept { (data.get(), 1, index, Sink); index = 0; }

      inline int32_t put(std::string_view value) noexcept {
         if (index + value.size() > BufferSize) {
            return -1;
         }

         std::memcpy(data.get() + index, value.data(), value.size());
         index += value.size();
         return 0;
      }
   };

} // namespace astro::io