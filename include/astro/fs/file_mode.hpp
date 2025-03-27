#pragma once

#include <cstdint>
#include <cstddef>

namespace astro::fs {

   /**
    * @brief File mode constants.
    * These are arbitrary, but chose the values for read and write to align with Windows GENERIC_READ and GENERIC_WRITE.
    */
   enum class file_mode : uint64_t {
      error  = 0x02000000L,
      read   = 0x80000000L, ///< Read mode.
      write  = 0x40000000L, ///< Write mode.
      append = 0x20000000L  ///< Append mode.
   };

   /**
    * @brief A bitwise OR operator for combining two file_mode values.
    * @param a The first file_mode value to be combined.
    * @param b The second file_mode value to be combined.
    *
    * @return The result of combining the two file_mode values using bitwise OR.
    */
   constexpr static file_mode operator |(file_mode a, file_mode b) noexcept {
      return static_cast<file_mode>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
   }

   /**
    * @brief A bitwise AND operator for combining two file_mode values.
    * @param a The first file_mode value to be combined.
    * @param b The second file_mode value to be combined.
    *
    * @return The result of combining the two file_mode values using bitwise AND.
    */
   constexpr static file_mode operator &(file_mode a, file_mode b) noexcept {
      return static_cast<file_mode>(static_cast<uint64_t>(a) & static_cast<uint64_t>(b));
   }

   /**
    * @brief Returns the value of `flags` if `to_test` and `against` have a non-zero intersection, otherwise returns 0.
    * @tparam T The type of the return value.
    * @param to_test The file mode to test against.
    * @param against The file mode to test against.
    * @param flags The value to return if the intersection is non-zero.
    *
    * @return The value of `flags` if `to_test` and `against` have a non-zero intersection, otherwise returns 0.
    *
    * @throws None.
    */
   template <typename T>
   constexpr static T set_if(file_mode to_test, file_mode against, std::size_t flags) noexcept {
      return ((to_test & against) == to_test) ? flags : 0;
   }

} // namespace astro::fs