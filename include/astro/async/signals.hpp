#pragma once

#include <setjmp.h>

#include <cstdint>

#include <atomic>
#include <tuple>

#include "../utils.hpp"

namespace astro::async {

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

         template <typename Handler>
         requires std::invocable<Handler>
         inline void operator()(Handler&& handler) {
            int32_t v = setjmp(*util::get_jmp().get());
            util::check(v == 0, "signal handler failure");
            handler();
         }

      private:
         std::tuple<Handlers...> handlers;
   };
} // namespace astro::signals

#define ASTRO_TRY(ex /*executor with handlers set*/) if (sigsetjmp(*astro::util::get_jmp().get(), 1)==0)
#define ASTRO_CATCH   else