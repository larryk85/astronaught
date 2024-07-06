#pragma once

#if 0
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <cstdint>
#include <signal.h>

#include "process_base.hpp"
#include "../../utils.hpp"

namespace astro::async {
   class process : public process_base<process> {
      public:
         inline process(std::string p) 
            : process_base<process>(p), _pid(fork()) { 
            util::check(_pid >= 0, "Failed to fork process");
         }

         inline void start_impl() {
            if (_pid == 0) {
               //::exec();
            }
         }

         inline void stop_impl() {
            ::kill(_pid, SIGSTOP);
         }

         inline void pause_impl() {
            ::kill(_pid, SIGSTOP);
         }

         inline void resume_impl() {
            ::kill(_pid, SIGCONT);
         }

         inline void kill_impl() {
            ::kill(_pid, SIGKILL);
         }

         inline void wait_impl() {
            int status;
            //waitpid(pid, &status, 0);
         }

         inline void wait_for(std::size_t ms) {
            int status;
            //waitpid(pid, &status, WNOHANG);
         }

         inline void wait_until(std::size_t ms) {
            int status;
            //waitpid(pid, &status, WNOHANG);
         }

      private:
         pid_t _pid = 0;
   };

} // namespace astro::async

#endif