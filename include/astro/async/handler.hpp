#pragma once

#include "../info/build_info.hpp"

#if ASTRO_OS == ASTRO_WINDOWS_BUILD
   #include "win/handler_impl.hpp"
#else
   #include "unix/handler_impl.hpp"
#endif
