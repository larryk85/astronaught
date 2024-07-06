#pragma once

#include <cstdint>

#include <compare>

#include "../compile_time/string.hpp"

namespace astro::info {
   struct version_t {
      version_t(const version_t&) = default;
      version_t(version_t&&) = default;
      constexpr inline version_t(std::uint16_t major=0, std::uint16_t minor=0, std::uint16_t patch=0, std::uint16_t tweak=0)
         : major(major), minor(minor), patch(patch), tweak(tweak) {}
      uint64_t major : 16 = 0;
      uint64_t minor : 16 = 0;
      uint64_t patch : 16 = 0;
      uint64_t tweak : 16 = 0; 
   };

   constexpr static inline std::size_t version_info_dynamic = std::numeric_limits<std::size_t>::max();

   template <std::size_t N=version_info_dynamic, std::size_t M=version_info_dynamic>
   struct version_info : public version_t {
      enum class parts : uint8_t {
         major,   /**< Major version */
         minor,   /**< Minor version */
         patch,   /**< Patch version */
         tweak,   /**< Tweak version */
         suffix,  /**< Version suffix */
         git_hash /**< Git hash */
      }; 

      using suffix_t   = std::conditional_t<N != version_info_dynamic, ct::string<N>, std::string>;
      using git_hash_t = std::conditional_t<M != version_info_dynamic, ct::string<M>, std::string>;

      suffix_t   suffix   = {};
      git_hash_t git_hash = {};

      version_info() = default;
      version_info(const version_info&) = default;
      version_info(version_info&&) = default;

      constexpr inline version_info(std::uint16_t major, std::uint16_t minor, 
                                    std::uint16_t patch, std::uint16_t tweak)
         : version_t(major, minor, patch, tweak) {}

      consteval inline version_info(std::uint16_t major, std::uint16_t minor, 
                                    std::uint16_t patch, std::uint16_t tweak, 
                                    suffix_t suffix)
         : version_t(major, minor, patch, tweak), suffix(suffix) {}

      constexpr inline version_info(std::uint16_t major, std::uint16_t minor, 
                                    std::uint16_t patch, std::uint16_t tweak, 
                                    suffix_t suffix, git_hash_t git_hash)
         : version_t(major, minor, patch, tweak), suffix(suffix), git_hash(git_hash) {}

      constexpr inline std::strong_ordering operator<=>(const version_info& other) const noexcept {
         return number() <=> other.number();
      }

      constexpr inline uint64_t number() const {
         return (major * 1000000) + (minor * 10000) + (patch * 100) + tweak;
      }

      template <parts part>
      constexpr inline auto get() const noexcept {
         if constexpr (part == parts::major) {
            return major;
         } else if constexpr (part == parts::minor) {
            return minor;
         } else if constexpr (part == parts::patch) {
            return patch;
         } else if constexpr (part == parts::tweak) {
            return tweak;
         } else if constexpr (part == parts::suffix) {
            return suffix;
         } else if constexpr (part == parts::git_hash) {
            return git_hash;
         } else {
            throw std::runtime_error("Invalid part requested");
         }
      }

      template <parts part>
      inline std::string to_string() const {
         if constexpr (part == parts::major) {
            return std::to_string(major);
         } else if constexpr (part == parts::minor) {
            return std::to_string(minor);
         } else if constexpr (part == parts::patch) {
            return std::to_string(patch);
         } else if constexpr (part == parts::tweak) {
            return std::to_string(tweak);
         } else if constexpr (part == parts::suffix) {
            return std::string(suffix.data(), suffix.size());
         } else if constexpr (part == parts::git_hash) {
            return std::string(git_hash.data(), git_hash.size());
         } else {
            return std::string();
         }
      }

      inline std::string to_string() const {
         const auto suffix = to_string<parts::suffix>().empty() ? "" : "-" + to_string<parts::suffix>();
         const auto git_hash = to_string<parts::git_hash>().empty() ? "" : " (" + to_string<parts::git_hash>() + ")";
         return to_string<parts::major>() + "." + 
                to_string<parts::minor>() + "." + 
                to_string<parts::patch>() + "." + 
                to_string<parts::tweak>() +
                suffix + git_hash; 
      }
   };
} // namespace astro::info