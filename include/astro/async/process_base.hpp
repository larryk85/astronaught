#pragma once

#include <cstddef>
#include <cstdint>

#include <string>
#include <string_view>

namespace astro::async {
   template <class Derived>
   class process_base {
      public:
         inline explicit process_base(std::string p) : _path(std::move(p)) {}

         constexpr process_base(const process_base&) = default;
         constexpr process_base(process_base&&) = default;
         constexpr process_base& operator=(const process_base&) = default;
         constexpr process_base& operator=(process_base&&) = default;
         ~process_base() = default;

         constexpr const std::string& path() const noexcept { return _path; }
         constexpr std::string& path() noexcept { return _path; }

         constexpr void start() { ref().start_impl(); }
         constexpr void stop() { ref().stop_impl(); }
         constexpr void pause() { ref().pause_impl(); }
         constexpr void resume() { ref().resume_impl();}
         constexpr void kill() { ref().kill_impl(); }
         constexpr void wait() { ref().wait_impl();}
         constexpr void wait_for(std::size_t ms) { ref().wait_for(ms); }
         constexpr void wait_until(std::size_t ms) { ref().wait_until(ms); }
      private:
         constexpr inline Derived& ref() noexcept {
            return static_cast<Derived&>(*this);
         }
         constexpr inline const Derived& ref() const noexcept {
            return static_cast<Derived&>(*this);
         }

         std::string _path;
   };
} // namespace astro::util