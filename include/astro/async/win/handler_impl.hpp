#pragma once

#include <windows.h>
#include <eh.h>

#include <concepts>
#include <cstdint>
#include <functional>

#include "../../utils.hpp"
#include "../../jmp.hpp"
#include "../../signals.hpp"

namespace astro::async {
    constexpr static inline DWORD signal_to_windows_exception(signal sig) {
        switch (sig) {
            case signal::access_violation:
                return EXCEPTION_ACCESS_VIOLATION;
            case signal::illegal_instruction:
                return EXCEPTION_ILLEGAL_INSTRUCTION;
            case signal::stack_overflow:
                return EXCEPTION_STACK_OVERFLOW;
            case signal::bus_error:
                return EXCEPTION_BUS_ERROR;
            case signal::float_error:
                return EXCEPTION_FLT_DIVIDE_BY_ZERO;
            case signal::abort:
                return EXCEPTION_NONCONTINUABLE_EXCEPTION;
            case signal::terminate:
                return EXCEPTION_NONCONTINUABLE_EXCEPTION;
            default:
                return EXCEPTION_NONCONTINUABLE_EXCEPTION;
        }
    }

    template <astro::signals::signal Signal>
    struct handler {
        constexpr static inline astro::signals::signal signal = Signal;
        constexpr static inline DWORD exception_code = signal_to_windows_exception(Signal);

        template <typename Func>
        inline handler(Func fn) {
            auto seh_handler = [&](unsigned int code, struct _EXCEPTION_POINTERS* ep) -> int {
                util::unused(ep);
                if (code == exception_code) {
                    fn(signal_info{Signal, static_cast<std::uint32_t>(code)});
                    util::get_jmp().jump();
                }
                return EXCEPTION_CONTINUE_SEARCH;
            };

            _set_se_translator([seh_handler](unsigned int code, struct _EXCEPTION_POINTERS* ep) {
                int result = seh_handler(code, ep);
                if (result == EXCEPTION_CONTINUE_SEARCH) {
                    throw std::runtime_error("Unhandled SEH exception");
                }
            });
        }

        inline ~handler() {
            _set_se_translator(nullptr);
        }
    };
} // namespace astro::async