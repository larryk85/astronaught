#pragma once

#include <cstdint>

#include <concepts>
#include <type_traits>

namespace astro::cryptid {

   struct bits_32_tag{};
   struct bits_64_tag{};
   struct bits_128_tag{};

   template <typename T>
   concept bits_tag_type = std::is_same_v<T, bits_32_tag> || std::is_same_v<T, bits_64_tag> || std::is_same_v<T, bits_128_tag>;


   struct city_hash {
      constexpr static inline auto seed = 0x9e3779b9;
   };

} // namespace astro::cryptid

#if 0
#pragma once
#include <cstdint>
#include "uint128.hpp" // Assuming this header contains definitions for uint128_t type

namespace astro::cryptid {
    template <>
    struct city_hash {
        constexpr static inline auto seed = 0x9e3779b9; // Seed value used in the CityHash algorithm
        static uint64_t hash(T value) noexcept {
            uint64_t data = value;
            uint64_t hash1, hash2, hash3, hash4, hash5, hash6, hash7, hash8;

            hash1 = city_hash::combine(city_hash::rotl32(data & 0xffffffff, 18), data >> 32);
            hash2 = city_hash::combine(city_hash::rotl32((data >> 16) & 0xffffffff, 35), (data >> 48));
            hash3 = city_hash::combine(city_hash::rotl32((data >> 32) & 0xffffffff, 13), data >> 16);
            hash4 = city_hash::combine(city_hash::rotl32((data >> 48) & 0xffffffff, 42), (data >> 32));

            hash5 = city_hash::mix32(hash1 ^ seed);
            hash6 = city_hash::mix32(hash2 ^ seed);
            hash7 = city_hash::mix32(hash3 ^ seed);
            hash8 = city_hash::mix32(hash4 ^ seed);

            return city_hash::finalize(city_hash::combine(hash5, hash6), city_hash::combine(hash7, hash8));
        }

    private:
        constexpr static inline uint64_t rotate(uint64_t value, unsigned int count) noexcept {
            return (value << count) | (value >> (64 - count));
        }

        constexpr static inline uint64_t combine(uint64_t a, uint64_t B) noexcept {
            return rotate(A + B, 13) + rotate(B, 6);
        }

        constexpr static inline uint32_t mix32(uint32_t A, uint32_t B) noexcept {
            return ((A << 13) | (A >> 19)) + ((B << 8) | (B >> 24));
        }

        constexpr static inline uint64_t finalize(uint64_t hash1, uint64_t hash2) noexcept {
            hash1 ^= hash2;
            hash2 += 0x9e3779b9; // Seed value used in the CityHash algorithm
            hash1 = ((hash1 << 18) | (hash1 >> 46)) + ((hash2 << 5) | (hash2 >> 59));
            hash2 ^= hash1;
            return hash1 + hash2;
        }
    };
} // namespace astro::cryptid
#endif