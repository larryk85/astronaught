cmake_minimum_required(VERSION 3.18)

project(astronaught VERSION 1.0.0.0)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)
link_libraries(Threads::Threads)

include(CMakeDependentOption)
option(ASTRONAUGHT_ENABLE_TESTS    "Enable building of unit tests" ON)
option(ASTRONAUGHT_ENABLE_DOCS     "Enable creation of the documentation" ON)
option(ASTRONAUGHT_ENABLE_INSTALL  "Enable installation" ON)
option(ASTRONAUGHT_ENABLE_PEDANTIC "Enable pedantic warnings" OFF)

message( STATUS "Building astronaught v${astronaught_VERSION}..." )
if (MSVC)
   if (CMAKE_SIZEOF_VOID_P EQUAL 8)
      add_compile_definitions(_AMD64_)
   else()
      add_compile_definitions(_X86_)
   endif()
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#add_executable(test_cpp ${PROJECT_SOURCE_DIR}/scripts/test.cpp)

include(cmake/Astro.cmake)
include(cmake/Doxy.cmake)
include(cmake/Utils.cmake)

astro_options_print(
   OPTIONS ASTRONAUGHT_ENABLE_TESTS
           ASTRONAUGHT_ENABLE_DOCS
           ASTRONAUGHT_ENABLE_INSTALL
           ASTRONAUGHT_ENABLE_PEDANTIC
)

astro_create_version_info(
   NAMESPACE "astro"
   SUFFIX    "rc2"
   GIT_HASH
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(astronaught INTERFACE)

target_include_directories(astronaught INTERFACE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
   $<INSTALL_INTERFACE:include>)

if(ASTRONAUGHT_ENABLE_PEDANTIC)
   target_compile_options(astronaught INTERFACE
      $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Werror>
      $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Werror>)
endif()

target_compile_options(astronaught INTERFACE
   $<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>)

target_compile_definitions(astronaught INTERFACE
   $<$<CONFIG:Debug>:ASTRONAUGHT_DEBUG>
   $<$<CONFIG:Release>:ASTRONAUGHT_RELEASE>
   $<$<CONFIG:RelWithDebInfo>:ASTRONAUGHT_RELWITHDEBINFO>
   $<$<CONFIG:MinSizeRel>:ASTRONAUGHT_MINSIZEREL>)

add_library(astro::naught ALIAS astronaught)

include(FetchContent)
if(ASTRONAUGHT_ENABLE_TESTS)
   FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2
      GIT_TAG v3.6.0
   )

   FetchContent_GetProperties(Catch2)
   if (NOT Catch2_POPULATED)
      FetchContent_MakeAvailable(Catch2)
      list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/contrib")
   endif()

   include(CTest)
   include(Catch)

   enable_testing()
   add_subdirectory(tests)
endif()

if(ASTRONAUGHT_ENABLE_DOCS)
   astronaught_generate_doxygen_docs()
endif()

include(GNUInstallDirs)

# ##################################################################################################
# Configure version info.
# ##################################################################################################
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/astronaught-config-version.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/astronaught-config-version.cmake @ONLY)

# ##################################################################################################
# Installation.
# ##################################################################################################
if(ASTRONAUGHT_ENABLE_INSTALL)
   include(CMakePackageConfigHelpers)
   include(GNUInstallDirs)

   message(STATUS "Installing astronaught ...")
   set(astronaught_INCLUDE_DIRS ${CMAKE_INSTALL_INCLUDEDIR})
   configure_package_config_file(cmake/astronaught-config.cmake.in
      ${CMAKE_CURRENT_BINARY_DIR}/astronaught-config.cmake
      INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/astronaught
      PATH_VARS astronaught_INCLUDE_DIRS
   )
   write_basic_package_version_file(
      ${CMAKE_CURRENT_BINARY_DIR}/astronaught-config-version.cmake
      VERSION ${astronaught_VERSION}
      COMPATIBILITY SameMajorVersion
   )

   install(TARGETS astronaught
           EXPORT astronaught-targets
           LIBRARY
           PUBLIC_HEADER
              DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
              COMPONENT Headers
   )
   install(EXPORT astronaught-targets
      NAMESPACE astronaught::
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/astronaught
   )
   install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
           DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
   )
   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/astronaught-config.cmake
                 ${CMAKE_CURRENT_BINARY_DIR}/astronaught-config-version.cmake
                 DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/astronaught
   )
endif()