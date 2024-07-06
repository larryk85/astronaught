#pragma once

#include <functional>
#include <string_view>

#include "../utils/misc.hpp"
#include "../compile_time/string.hpp"

namespace versa::app {

   template <typename T, auto S>
   class command {
      public:
         using function_t = std::function<T(std::string_view)>;
         constexpr static inline auto cmd = S;
         constexpr inline command(T&& def) 
            : _default_value(def), _description("") {}

         constexpr inline command(T&& def, std::string_view desc) 
            : _default_value(def), _description(desc.data(), desc.size()) {}

      private:
         T           _default_value;
         std::string _description;
         function_t  _callback;
   };

} // namespace versa::app