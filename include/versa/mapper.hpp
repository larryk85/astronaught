#pragma once

#include "constants.hpp"

#ifdef VERSA_PLATFORM_WINDOWS
   #include "arch/win32/mapper_impl.hpp"
#else
   #include "arch/unix/mapper_impl.hpp"
#endif