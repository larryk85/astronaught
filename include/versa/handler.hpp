#pragma once

#if defined(VERSA_PLATFORM_WINDOWS)
   #include "arch/win32/handler_impl.hpp"
#else
   #include "arch/unix/handler_impl.hpp"
#endif