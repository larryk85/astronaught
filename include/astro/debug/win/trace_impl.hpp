#pragma once

#define NOMINMAX
#include <windows.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
#undef NOMINMAX

#include <cstdint>

#include "../../utils/misc.hpp"


namespace astro::debug {
   constexpr static inline auto MAX_SYMBOL_NAME_LENGTH = 512;

   struct symbol_info {
      ULONG   SizeOfStruct;
      ULONG   TypeIndex;
      ULONG64 Reserved[2];
      ULONG   Index;
      ULONG   Size;
      ULONG64 ModBase;
      ULONG   Flags;
      ULONG64 Value;
      ULONG64 Address;
      ULONG   Register;
      ULONG   Scope;
      ULONG   Tag;
      ULONG   NameLen;
      ULONG   MaxNameLen = 255;
      char    Name[256];
   };
   
   template <std::size_t MaxFrames = 128>
   class backtrace {
      public:
         backtrace() {
            _proc = GetCurrentProcess();
            SymInitialize(_proc, nullptr, true);
            _frames = CaptureStackBackTrace(0, MaxFrames, _stack.data(), nullptr);
         }

         void print() {
            symbol_info info;
            for (uint32_t i = 0; i < _frames; ++i) {
               SymFromAddr(_proc, (DWORD64(_stack[i])), 0, &info);
               std::cout << "[" << i << "] " << info.Name << std::endl;
            }
         }

         using stack_type = std::array<void*, MaxFrames>;

      private:
         stack_type _stack  = {};
         uint32_t   _frames = 0;
         HANDLE     _proc   = 0;
   };

} // namespace astro::debug