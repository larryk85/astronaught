#pragma once

#include <setjmp.h>

#include <atomic>

namespace versa::util {
   class jmp {
      public:
         jmp() = default;
         jmp(const jmp&) = delete;
         jmp(jmp&&) = delete;
         jmp& operator=(const jmp&) = delete;
         jmp& operator=(jmp&&) = delete;
         ~jmp() = default;

         __attribute__((always_inline))
         inline bool set() { 
            int32_t v = sigsetjmp(env, 1);
            return v == 0; 
         }
         inline sigjmp_buf* get() { return &env; }
         inline void jump(int32_t s=0) { siglongjmp(env, s); }

      private:
         sigjmp_buf env;
   };

   static inline jmp& get_jmp() {
      static thread_local jmp jmp_env;
      return jmp_env;
   }
} // namespace versa::jmp