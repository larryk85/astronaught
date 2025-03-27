#pragma once

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include <concepts>
#include <cstdint>
#include <functional>

#include "../../utils/jmp.hpp"
#include "../../utils/closure_wrapper.hpp"
#include "../signals.hpp"

namespace astro::async {
   constexpr static inline uint32_t signal_to_unix_signal(signal sig) {
      switch (sig) {
         case signal::access_violation:
            return SIGSEGV;
         case signal::illegal_instruction:
            return SIGILL;
         case signal::stack_overflow:
            return SIGSEGV;
         case signal::bus_error:
            return SIGBUS;
         case signal::float_error:
            return SIGFPE;
         case signal::abort:
            return SIGABRT;
         case signal::terminate:
            return SIGTERM;
         default:
            return SIGKILL;
      }
   }

   template <astro::async::signal Signal>
   struct handler {
      constexpr static inline astro::async::signal signal = Signal;
      constexpr static inline uint32_t signaln = signal_to_unix_signal(Signal);
      template <typename Func>
      inline handler(Func fn) {
         struct sigaction sa;
         sa.sa_flags = SA_SIGINFO;
         sigemptyset(&sa.sa_mask);
         auto cl = [&](int sig, siginfo_t* info, void* ucontext) -> void {
            util::unused(ucontext);
            fn(signal_info{Signal, static_cast<std::uint32_t>(sig)});
            util::get_jmp().jump();
         };
         void (*cb)(int, siginfo_t*, void*) = util::closure::ptr(cl);
         sa.sa_sigaction = cb;
         util::check(sigaction(signaln, &sa, nullptr) == 0, "Failed to set signal handler");
      }

      inline ~handler() {
         struct sigaction sa;
         sa.sa_flags = SA_SIGINFO | SA_NODEFER;
         sigemptyset(&sa.sa_mask);
         sa.sa_sigaction = SIG_DFL;
         util::check(sigaction(signaln, &sa, nullptr) == 0, "Failed to reset signal handler");
      }
   };
} // namespace astro::async