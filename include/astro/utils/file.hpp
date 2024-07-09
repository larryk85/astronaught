#pragma once

#include <cstdlib>
#include <cstdio>

#include <filesystem>
#include <string_view>

#if ASTRO_OS == ASTRO_WINDOWS_BUILD
   #include "win/file_impl.hpp"
#else
   #include "unix/file_impl.hpp"
#endif

namespace astro::util {

}