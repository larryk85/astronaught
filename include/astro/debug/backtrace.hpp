#pragma once

#include "../info/build_info.hpp"

#if ASTRO_OS == ASTRO_WINDOWS_BUILD
   #include "win/trace_impl.hpp"
#else
   #include "unix/trace_impl.hpp"
#endif