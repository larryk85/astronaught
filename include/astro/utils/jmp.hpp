#pragma once

#include <setjmp.h>

#include <atomic>

#include "defs.hpp"

namespace astro::util {
   class jmp {
      public:
         jmp() = default;
         jmp(const jmp&) = delete;
         jmp(jmp&&) = delete;
         jmp& operator=(const jmp&) = delete;
         jmp& operator=(jmp&&) = delete;

         inline bool set() { 
            #pragma warning(suppress: 4611)
            int32_t v = setjmp(env);
            #pragma warning(default: 4611)
            return v == 0; 
         }
         inline jmp_buf* get() { return &env; }
         inline void jump(int32_t s=0) { longjmp(env, s); }

      private:
         jmp_buf env;
   };

   static inline jmp& get_jmp() {
      static thread_local jmp jmp_env;
      return jmp_env;
   }
} // namespace astro::jmp