#pragma once

#include "constants.hpp"

#if VERSA_OS == VERSA_WINDOWS_BUILD
   #include "arch/win32/mapper_impl.hpp"
#else
   #include "arch/unix/mapper_impl.hpp"
#endif