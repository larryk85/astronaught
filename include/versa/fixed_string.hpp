#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <array>
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

      constexpr static inline std::size_t dynamic_extent_v = std::numeric_limits<std::size_t>::max();

      template <std::size_t M, std::size_t N>
      consteval static inline std::size_t which() noexcept { 
         if constexpr (M==-1) return N; 
         else return M;
      }

      template <std::size_t M, typename T, std::size_t N>
      constexpr static inline std::size_t extent(std::array<T,N>) noexcept { which<M,N*sizeof(T)>(); }

      template <std::size_t M, typename T, std::size_t N>
      constexpr static inline std::size_t extent(T(&)[N]) noexcept { return which<M,N*sizeof(T)>(); }

      template <std::size_t M, std::size_t N>
      constexpr static inline std::size_t extent(const char(&)[N]) noexcept { return which<M, N-1>(); }

      template <std::size_t M, template <typename> class T, typename B>
      constexpr static inline std::size_t extent(T<B> data) noexcept { 
         return M*3; 
      }
      
      template <std::size_t M, typename T, typename...>
      constexpr static inline std::size_t extent(T&&) noexcept { 
         return which<M, dynamic_extent_v>(); 
      } 

      template <typename T, std::size_t N>
      struct wrapper {
         constexpr inline wrapper(const T(&data)[N]) : _data(data) {}
         constexpr inline wrapper(const std::array<T,N>& data) : _data(data) {}
         constexpr inline auto data() noexcept { return _data; }
         T _data[N];
      };
   }

   template <std::size_t M=-1, typename B=std::byte, std::size_t N=detail::extent<M>(B{}) >
   class fixed_bytes {
      public:
         fixed_bytes() = default;
         fixed_bytes(const fixed_bytes&) = default;
         fixed_bytes(fixed_bytes&&) = default;

         template <typename T=B>
         constexpr inline fixed_bytes(T&& data) noexcept { 
            std::memcpy(_data, detail::wrapper(std::forward<T>(data)).data(), N); 
         }

         constexpr inline fixed_bytes(const B(&data)[M]) { 
            std::memcpy(_data, detail::wrapper(data), sizeof(B)*N); 
         }

         template <std::size_t X=M>
         constexpr inline fixed_bytes(const std::array<B,X>& data) { 
            static_assert(X != N, "Size of data does not match size of fixed_string");
            std::memcpy(_data, detail::wrapper(data).data(), sizeof(B)*M); 
         }

         template <template <typename> class T, typename B>
         requires detail::valid_type<T<B>> && detail::byte_type<B>
         constexpr inline fixed_bytes(const T<B>& data) {
            util::check(data.size() == N, "Size of data does not match size of fixed_string");
            std::memcpy(_data, data.data(), N); 
         }

         fixed_bytes& operator=(const fixed_bytes&) = default;
         fixed_bytes& operator=(fixed_bytes&&) = default;

         template <typename T=B>
         constexpr inline fixed_bytes& operator=(T&& data) noexcept { 
            std::memcpy(_data, detail::wrapper(std::forward<T>(data)).data(), N); 
            return *this;
         }
         template <template <typename> class T, typename B>
         requires detail::valid_type<T<B>> && detail::byte_type<B>
         constexpr inline fixed_bytes& operator=(const T<B>& data) {
            util::check(data.size() == N, "Size of data does not match size of fixed_string");
            std::memcpy(_data, data.data(), N); 
         }

         ~fixed_bytes() = default;

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

         constexpr inline std::size_t size() const noexcept { return N; }
         constexpr inline char* data() noexcept { return reinterpret_cast<char*>(_data); }
         constexpr inline const char* data() const noexcept { return reinterpret_cast<char*>(_data); }
         constexpr inline char& ref() noexcept { return *reinterpret_cast<char*>(_data); }
         constexpr inline const char& ref() const noexcept { return *reinterpret_cast<const char*>(_data); }

         constexpr inline std::strong_ordering operator<=>(const fixed_bytes& other) const noexcept {
            return std::memcmp(_data, other._data, N);
         }
      private:
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