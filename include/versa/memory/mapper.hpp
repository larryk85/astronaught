#pragma once

#include "../info.hpp"

#if VERSA_OS == VERSA_WINDOWS_BUILD
   #include "win/mapper_impl.hpp"
#else
   #include "unix/mapper_impl.hpp"
#endif