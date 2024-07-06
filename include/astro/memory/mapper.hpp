#pragma once

#include "../info.hpp"

#if ASTRO_OS == ASTRO_WINDOWS_BUILD
   #include "win/mapper_impl.hpp"
#else
   #include "unix/mapper_impl.hpp"
#endif