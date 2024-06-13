#pragma once

#include <setjmp.h>

#include <cstdint>

#include <atomic>
#include <tuple>

#include "utils.hpp"

namespace versa::signals {

   enum class signal : std::uint8_t {
      access_violation,
      illegal_instruction,
      stack_overflow,
      bus_error,
      float_error,
      abort,
      terminate,
      unknown
   };

   struct signal_info {
      signal sig;
      std::uint32_t code;
   };
   
   template <class... Handlers>
   class executor {
      public:
         inline executor(Handlers... handlers) : handlers(std::forward<Handlers>(handlers)...) {}

         template <typename Func>
         requires std::invocable<Func>
         inline void operator()(Func&& func) {
            int32_t v = sigsetjmp(*util::get_jmp().get(), 1);
            util::check(v == 0, "signal handler failure");
            func();
         }

      private:
         std::tuple<Handlers...> handlers;
   };
} // namespace versa::signals

#define VERSA_TRY(ex /*executor with handlers set*/) if (sigsetjmp(*versa::util::get_jmp().get(), 1)==0) 
#define VERSA_CATCH   else 