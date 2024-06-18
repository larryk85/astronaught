#pragma once

#include "info.hpp"

#if VERSA_OS == VERSA_WINDOWS_BUILD
   #include "arch/win32/process_impl.hpp"
#else
   #include "arch/unix/process_impl.hpp"
#endif