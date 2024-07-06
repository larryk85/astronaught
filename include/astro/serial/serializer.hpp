#pragma once

#include "../utils.hpp"

namespace astro::serial {

   struct serializer_base {
      constexpr static inline bool is_serial = true;

      using byte_t           = std::uint8_t;
      using pos_t            = std::int64_t;
      using iterator_t       = byte_t*;
      using const_iterator_t = const byte_t*;

      serializer_base() = default;

      constexpr inline serializer_base(std::size_t sz) 
         : _pos(0), _data(sz) {}

      constexpr inline std::size_t size() const noexcept { return _data.size(); }
      constexpr inline pos_t pos() const noexcept { return _pos; }
      constexpr inline void pos(pos_t pos) noexcept { _pos = pos; }

      constexpr inline iterator_t begin() noexcept { return _data.data(); }
      constexpr inline const_iterator_t begin() const noexcept { return _data.data(); }
      constexpr inline const_iterator_t cbegin() const noexcept { return _data.data(); }
      constexpr inline iterator_t end() noexcept { return _data.data() + _data.size(); }
      constexpr inline const_iterator_t end() const noexcept { return _data.data() + _data.size(); }
      constexpr inline const_iterator_t cend() const noexcept { return _data.data() + _data.size(); }

      inline void write(pos_t pos, const void* val, std::size_t sz) {
         _data.resize(_data.size() + sz);
         std::memcpy(_data.data() + pos, val, sz);
      }

      inline void write(const void* val, std::size_t sz) {
         write(_pos, val, sz);
         _pos += sz;
      }

      inline void read(pos_t pos, void* val, std::size_t sz) const {
         std::memcpy(val, _data.data() + pos, sz);
      }

      inline void read(void* val, std::size_t sz) {
         read(_pos, val, sz);
         _pos += sz;
      }

      constexpr inline void fastforward(std::size_t sz) { _pos += sz; }
      constexpr inline void rewind(std::size_t sz) { _pos -= sz; }
      constexpr inline void reset() { _pos = 0; }

      std::size_t               _pos  = 0;
      std::vector<std::uint8_t> _data = {};
   };

   template <typename S>
   concept serial_type = requires {
      std::is_same_v<decltype(S::is_serial), bool>;
   };
} // namespace astro::serial