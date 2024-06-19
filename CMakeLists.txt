cmake_minimum_required(VERSION 3.14)

project(libversa VERSION 1.0.0.0)

message( STATUS "Building libversa v${libversa_VERSION}..." )

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads)
link_libraries(Threads::Threads)

include(CMakeDependentOption)
option(LIBVERSA_ENABLE_TESTS "enable building of unit tests" ON)

if (MSVC)
   if (CMAKE_SIZEOF_VOID_P EQUAL 8)
      add_compile_definitions(_AMD64_)
   else()
      add_compile_definitions(_X86_)
   endif()
endif()

include(LibVersa.cmake)

   set(oneValueArgs NAMESPACE MAJOR MINOR PATCH TWEAK SUFFIX INCLUDE_DIR)

versa_create_version_info(
   NAMESPACE "versa"
   SUFFIX "rc1"
   GIT_HASH
)

add_library(versa INTERFACE)
target_include_directories(versa INTERFACE
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
   $<INSTALL_INTERFACE:include>)

#target_compile_definitions(versa INTERFACE
#   $<$<CONFIG:Debug>:VERSALIB_DEBUG>
#   $<$<CONFIG:Release>:VERSALIB_RELEASE>
#   $<$<CONFIG:RelWithDebInfo>:VERSALIB_RELWITHDEBINFO>
#   $<$<CONFIG:MinSizeRel>:VERSALIB_MINSIZEREL>)

add_library(versa::versa ALIAS versa)

include(FetchContent)
if(LIBVERSA_ENABLE_TESTS)
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