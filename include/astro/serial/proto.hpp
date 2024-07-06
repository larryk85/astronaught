#pragma once

#include "../utils.hpp"

namespace astro::serial {
   class proto {
      public:
         proto() = default;
         ~proto() = default;

         template <typename T>
         static void serialize(T const& value, std::ostream& stream) {
            stream << value;
         }

         template <typename T>
         static void deserialize(T& value, std::istream& stream) {
            stream >> value;
         }
      private:
   };
};