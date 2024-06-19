#pragma once

#include <setjmp.h>

#include <atomic>

#include "defs.hpp"

namespace versa::util {
   class jmp {
      public:
         jmp() = default;
         jmp(const jmp&) = delete;
         jmp(jmp&&) = delete;
         jmp& operator=(const jmp&) = delete;
         jmp& operator=(jmp&&) = delete;
         ~jmp() = default;

         //VERSA_ALWAYS_INLINE
         inline bool set() { 
            int32_t v = setjmp(env);
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
} // namespace versa::jmp