#pragma once

#define NOMINMAX
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#undef NOMINMAX

#include <cstdint>
#include <iostream>
#include <string_view>

#include "../../utils/misc.hpp"

namespace astro::debug {
   constexpr static inline auto MAX_SYMBOL_NAME_LENGTH = 512;
   
   template <std::size_t MaxFrames = 128>
   class backtracer {
      public:
         /**
          * Function that performs a backtrace by capturing the stack frames.
          *
          * @constructor
          */
         backtracer() {
            _proc = GetCurrentProcess();
            SymInitialize(_proc, nullptr, true);
            _frames = CaptureStackBackTrace(0, MaxFrames, _stack.data(), nullptr);
         }

         /**
          * Dumps the stack trace of the current process by querying the symbol information
          * for each frame in the stack.
          */
         void dump() noexcept {
            SYMBOL_INFO* sym = (SYMBOL_INFO*)malloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char));
            sym->MaxNameLen = 255;
            sym->SizeOfStruct = sizeof(SYMBOL_INFO);
 
            for (uint32_t i = 0; i < _frames; ++i) {
               SymFromAddr(_proc, (DWORD64(_stack[i])), 0, (PSYMBOL_INFO)sym);
               std::cout << "[" << i << "] " << std::string_view{sym->Name, sym->NameLen};
               std::cout << " @ " << std::hex << "0x" << sym->Address << std::endl;
            }

            free(sym);
         }

         using stack_type = std::array<void*, MaxFrames>;

      private:
         stack_type _stack  = {};
         uint32_t   _frames = 0;
         HANDLE     _proc   = 0;
   };

} // namespace astro::debug