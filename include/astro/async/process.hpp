#pragma once

#include "../info.hpp"

#if ASTRO_OS == ASTRO_WINDOWS_BUILD
   #include "win/process_impl.hpp"
#else
   #include "unix/process_impl.hpp"
#endif