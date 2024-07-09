#pragma once

#include <format>
#include <iostream>

#include "../compile_time/string.hpp"

#include "call_info.hpp"
#include "term_color.hpp"

namespace astro::io {
   struct message_wrapper {
      constexpr inline message_wrapper(std::string_view value, call_info info={}) noexcept
         : value(value), info(info) {}

      template <std::size_t N>
      constexpr inline message_wrapper(const char(&value)[N], call_info info={}) noexcept
         : value(value), info(info) {}

      std::string_view value;
      call_info        info;
   };

   template <typename Derived>
   class logger {
      public:
         using derived_type = Derived;

         template <typename... Args>
         void log(message_wrapper msg, Args&&... args) {
            static_cast<Derived*>(this)->log_msg(format(msg, std::forward<Args>(args)...));
         }

         template <typename... Args>
         decltype(auto) format(message_wrapper msg, Args&&... args) {
            return std::vformat(msg.value, std::make_format_args(args...));
         }
   };

   struct stdout_t{ constexpr inline auto& operator()() noexcept { return std::cout; } };
   struct stdlog_t{ constexpr inline auto& operator()() noexcept { return std::clog; } };
   struct stderr_t{ constexpr inline auto& operator()() noexcept { return std::cerr; } };
   constexpr static inline auto stdout_v = stdout_t{};
   constexpr static inline auto stdlog_v = stdlog_t{};
   constexpr static inline auto stderr_v = stderr_t{};

   template <typename Sink=stdout_t>
   class console_logger : public logger<console_logger<Sink>> {
      public:
         using sink_type = decltype(Sink{}());

         constexpr inline console_logger(Sink sink) noexcept {
            (void)sink;
         }

         constexpr inline sink_type sink() noexcept { return Sink{}(); }

         constexpr inline void log_msg(std::string&& msg) {
            sink() << std::move(msg) << std::endl;
         }
   };

} // namespace astro::log