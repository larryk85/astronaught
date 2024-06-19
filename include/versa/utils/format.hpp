#pragma once

#include <cstdint>

#include "misc.hpp"
#include "../fixed_string.hpp"

namespace versa::util {
   /**
    * @brief A simple class for formatting strings.
    */
   template <std::size_t N>
   class format {
      public:
         /**
          * @brief Construct a new format object.
          * 
          * @param fmt The format string.
          */
         format(fixed_string<N> fmt) : _fmt(fmt) {}

         /**
          * @brief Format the string with the given arguments.
          * 
          * @tparam Args The argument types.
          * @param args The arguments.
          * @return std::string The formatted string.
          */
         template <typename... Args>
         std::string operator()(Args&&... args) const {
            return _fmt;
         }

      private:
         std::string _fmt;
   };
} // namespace versa::util