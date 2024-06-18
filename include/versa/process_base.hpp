#pragma once

#include <cstddef>
#include <cstdint>

#include <string>

namespace versa::util {
   template <class Derived>
   class process_base {
      public:
         inline process_base(std::string p) : _path(p) {}

         constexpr inline process_base(const process_base&) = default;
         constexpr inline process_base(process_base&&) = default;
         constexpr inline process_base& operator=(const process_base&) = default;
         constexpr inline process_base& operator=(process_base&&) = default;
         constexpr inline ~process_base() = default;

         constexpr inline const std::string& path() const noexcept { return _path; }
         constexpr inline std::string& path() noexcept { return _path; }

         constexpr inline void start() { ref().start_impl(); }
         constexpr inline void stop() { ref().stop_impl(); }
         constexpr inline void pause() { ref().pause_impl(); }
         constexpr inline void resume() { ref().resume_impl();}
         constexpr inline void kill() { ref().kill_impl(); }
         constexpr inline void wait() { ref().wait_impl();}
         constexpr inline void wait_for(std::size_t ms) { ref().wait_for(ms); }
         constexpr inline void wait_until(std::size_t ms) { ref().wait_until(ms); }

      private:
         constexpr inline Derived& ref() noexcept {
            return static_cast<Derived&>(*this);
         }
         constexpr inline const Derived& ref() const noexcept {
            return static_cast<Derived&>(*this);
         }

         std::string _path;
   };
} // namespace versa::util