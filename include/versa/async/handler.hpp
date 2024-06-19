#pragma once

#if VERSA_OS == VERSA_WINDOWS_BUILD
   #include "win/handler_impl.hpp"
#else
   #include "unix/handler_impl.hpp"
#endif