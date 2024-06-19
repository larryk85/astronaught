#pragma once

#include "../info.hpp"

#if VERSA_OS == VERSA_WINDOWS_BUILD
   #include "win/process_impl.hpp"
#else
   #include "unix/process_impl.hpp"
#endif