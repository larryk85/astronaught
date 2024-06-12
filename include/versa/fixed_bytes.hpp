#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <array>
#include <bit>
#include <concepts>
#include <limits>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "utils.hpp"

namespace versa::util {
   namespace detail {
      template <typename T>
      concept byte_type = std::is_same_v<T, std::byte> || 
                          std::is_same_v<T, uint8_t>   || 
                          std::is_same_v<T, int8_t>    ||
                          std::is_same_v<T, char>      ||
                          std::is_same_v<T, unsigned char>;

      template <template<typename> class T, typename B>
      concept valid_type = std::is_same_v< T<B>, std::vector<B> > ||
                           std::is_same_v< T<B>, std::basic_string<B> > ||
                           std::is_same_v< T<B>, std::basic_string_view<B> >;
      template <std::size_t N, bool dynamic=false>
      struct extent {
         constexpr static inline std::size_t value = N;
         constexpr static inline bool        is_dynamic = dynamic;
      };

      template <typename T, typename U, std::size_t N>
      constexpr static inline void memcpy(T* dest, const U(&src)[N]) noexcept {
         for (std::size_t i = 0; i < N; ++i) {
            dest[i] = std::bit_cast<T>(src[i]);
         }
      }

      template <typename T, typename U, std::size_t N>
      constexpr static inline void memcpy(T* dest, const std::array<U,N>& src) noexcept {
         for (std::size_t i = 0; i < N; ++i) {
            dest[i] = std::bit_cast<T>(src[i]);
         }
      }

      template <std::size_t N>
      struct wrapper {
         constexpr inline wrapper(const char(&data)[N]) noexcept : data{data} {}

         const char data[N];
         constexpr static inline std::size_t size = N;
      };
   } // namespace detail

   template <std::size_t N>
   struct fixed_bytes {
      fixed_bytes() = default;
      fixed_bytes(const fixed_bytes&) = default;
      fixed_bytes(fixed_bytes&&) = default;

      template <typename T, std::enable_if_t<!std::is_same_v<T,char>, int> = 0>
      constexpr inline fixed_bytes(const T(&data)[N]) noexcept {
         detail::memcpy(_data, data);
      }

      //template <std::size_t M, std::enable_if_t<M==N+1, int> = 0>
      constexpr inline fixed_bytes(const char(&data)[N+1]) noexcept { 
         detail::memcpy(_data, data); 
      }

      template <typename T>
      constexpr inline fixed_bytes(const std::array<T,N>& data) noexcept { 
         detail::memcpy(_data, data); 
      }

      template <template <typename> class U, typename T>
      requires detail::valid_type<U<T>> && detail::byte_type<T>
      constexpr inline fixed_bytes(const U<T>& data) {
         util::check(data.size() <= N, "Size of data is not less than or equal to size of fixed_string");
         copy(data);
      }

      ~fixed_bytes() = default;

      fixed_bytes& operator=(const fixed_bytes&) = default;
      fixed_bytes& operator=(fixed_bytes&&) = default;

      template <typename T>
      constexpr inline fixed_bytes& operator=(const T(&data)[N]) noexcept {
         std::memcpy(_data, data, sizeof(T)*N);
         return *this;
      }

      template <std::size_t M, std::enable_if_t<M==N+1, int> = 0>
      constexpr inline fixed_bytes& operator=(const char(&data)[M]) noexcept { std::memcpy(_data, data, N); return *this; }

      template <typename T>
      constexpr inline fixed_bytes& operator=(const std::array<T,N>& data) noexcept { std::memcpy(_data, data.data(), sizeof(T)*N); return *this; }

      template <template <typename> class U, typename T>
      requires detail::valid_type<U<T>> && detail::byte_type<T>
      constexpr inline fixed_bytes& operator=(const U<T>& data) {
         util::check(data.size() <= N, "Size of data is not less than or equal to size of fixed_string");
         copy(data);
      }

      constexpr inline char& operator[](std::size_t index) noexcept { return reinterpret_cast<char*>(_data)[index]; }

      constexpr inline const char& operator[](std::size_t index) const noexcept { return reinterpret_cast<const char*>(data)[index]; }

      constexpr inline char& at(std::size_t index) { 
         util::check(index < N, "Index out of range");
         return reinterpret_cast<char*>(_data)[index];
      }

      constexpr inline const char& at(std::size_t index) const { 
         util::check(index < N, "Index out of range");
         return reinterpret_cast<const char*>(_data)[index];
      }

      constexpr inline char* begin() noexcept { return reinterpret_cast<char*>(_data); }
      constexpr inline const char* begin() const noexcept { return reinterpret_cast<const char*>(_data); }
      constexpr inline char* end() noexcept { return reinterpret_cast<char*>(_data) + N; }
      constexpr inline const char* end() const noexcept { return reinterpret_cast<const char*>(_data) + N; }

      consteval inline bool empty() const noexcept { return N == 0; }
      constexpr inline void fill(char value) noexcept { std::fill_n(begin(), N, value);}
      constexpr inline void swap(fixed_bytes& other) noexcept { std::swap_ranges(begin(), end(), other.begin()); }
      constexpr inline void copy(const fixed_bytes& other) noexcept { std::copy(other.begin(), other.end(), begin()); }

      template <typename T>
      constexpr inline void copy(const T(&data)[N]) noexcept { std::copy(data, data+N, begin()); }
      constexpr inline void copy(const char(&data)[N]) noexcept { std::copy(data, data+N, begin()); }

      template <typename T>
      constexpr inline void copy(const std::array<T,N>& data) noexcept { std::copy(data.begin(), data.end(), begin()); }

      template <typename T>
      constexpr inline void copy(const T* data, std::size_t n) noexcept { 
         util::check(n == N, "Size of data does not match size of fixed_string");
         std::copy(data, data+N, begin()); 
      }

      template <typename T>
      constexpr inline void copy(const std::vector<T>& data) noexcept { 
         util::check(data.size() == N, "Size of data does not match size of fixed_string");
         std::copy(data.begin(), data.end(), begin()); 
      }

      consteval inline std::size_t size() const noexcept { return N; }
      constexpr inline char* data() noexcept { return reinterpret_cast<char*>(_data); }
      constexpr inline const char* data() const noexcept { return reinterpret_cast<char*>(_data); }
      constexpr inline char& ref() noexcept { return *reinterpret_cast<char*>(_data); }
      constexpr inline const char& ref() const noexcept { return *reinterpret_cast<const char*>(_data); }

      constexpr inline std::strong_ordering operator<=>(const fixed_bytes& other) const noexcept {
         return std::memcmp(_data, other._data, N);
      }


      alignas(uint64_t) std::byte _data[N];
   };

   template <std::size_t N>
   constexpr static inline std::string to_string(const fixed_bytes<N>& data) {
      return std::string(data.data(), N);
   }
   
   template <std::size_t N>
   constexpr static inline std::string_view to_string_view(const fixed_bytes<N>& data) {
      return std::string_view(data.data(), N);
   }

} // namespace versa::util