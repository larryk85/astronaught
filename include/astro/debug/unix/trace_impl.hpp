#pragma once

#include <execinfo.h>
#include <cstdint>
#include <iostream>
#include <string_view>

#include "../../utils/misc.hpp"

namespace astro::debug {
   constexpr static inline auto MAX_SYMBOL_NAME_LENGTH = 512;
   constexpr static inline auto MAX_PATH__LENGTH = 4096;
   
   template <std::size_t MaxFrames = 128>
   class backtracer {
      public:
         /**
          * Function that performs a backtrace by capturing the stack frames.
          *
          * @constructor
          */
         backtracer() {
            _frames = backtrace(_stack.data(), MaxFrames);
         }

         /**
          * Dumps the stack trace of the current process by querying the symbol information
          * for each frame in the stack.
          */
         void dump() noexcept {
            char** symbols = backtrace_symbols(_stack.data(), _frames);
            astro::util::check(symbols != nullptr, "Failed to get backtrace symbols");
            for (uint32_t i = 0; i < _frames; ++i) {
               std::cout << "[" << i << "] " << std::string_view{symbols[i]} << std::endl;
            }

            if(symbols) free(symbols);
         }

         using stack_type = std::array<void*, MaxFrames>;

      private:
         stack_type _stack  = {};
         uint32_t   _frames = 0;
   };

} // namespace astro::debug