# FILEPATH: libversa/LibVersa.cmake

# This CMake script defines a macro called `versa_create_version_info` that is used to generate version information for a project.
# The macro takes several arguments, including `NAMESPACE`, `MAJOR`, `MINOR`, `PATCH`, `TWEAK`, and `SUFFIX`, which can be provided by the user.
# If any of these arguments are not provided, the macro will use default values based on the project's version information.
# The macro also supports an optional `GIT_LOG` flag, which, when enabled, retrieves the latest commit hash from the Git repository.
# The macro uses the `configure_file` command to generate `version.h` and `version.hpp` files based on template files.
# These generated files will contain the version information and can be included in the project's source code.

# Usage:
# versa_create_version_info(NAMESPACE <namespace> [MAJOR <major>] [MINOR <minor>] [PATCH <patch>] [TWEAK <tweak>] [SUFFIX <suffix>] [GIT_LOG])

# Arguments:
# - `NAMESPACE`: The namespace for the version information. If not provided, it defaults to the project's namespace.
# - `MAJOR`: The major version number. If not provided, it defaults to the project's major version number.
# - `MINOR`: The minor version number. If not provided, it defaults to the project's minor version number.
# - `PATCH`: The patch version number. If not provided, it defaults to the project's patch version number.
# - `TWEAK`: The tweak version number. If not provided, it defaults to the project's tweak version number.
# - `SUFFIX`: The version suffix. If not provided, it defaults to the project's version suffix, if available.
# - `GIT_HASH`: Optional flag to include the latest commit hash in the version information. If provided, it retrieves the commit hash using the `git log` command.
# - `INCLUDE_DIR`: This is the location of the versa/version.h.in, version.hpp.in, etc.
# - `LANG`: The language to use for the version information. If not provided, it defaults to the language of your project. Currently, only C and C++ are supported.
#           If set to C, the generated files will use C-style structs, functions and macros in version.h. 
#           If set to C++/CXX/CPP, the generated files will use C++-style classes, functions and namespaces in version.hpp.in.

# Example:
# versa_create_version_info(NAMESPACE MyProject MAJOR 1 MINOR 2 PATCH 3 TWEAK 4 SUFFIX "alpha" GIT_LOG)

set(_VERSA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

function(versa_create_version_info)
   set(options GIT_HASH)
   set(oneValueArgs NAMESPACE MAJOR MINOR PATCH TWEAK SUFFIX INCLUDE_DIR)
   set(multiValueArgs)
   cmake_parse_arguments( LV_ARGS "${options}" 
                                  "${oneValueArgs}"
                                  "${multiValueArgs}" 
                                  ${ARGN} )

   message(STATUS "Generating version information for ${LV_TARGETS}")

   if (NOT LV_ARGS_NAMESPACE) 
      set(NAMESPACE ${PROJECT_NAME})
   else()
      set(NAMESPACE ${LV_ARGS_NAMESPACE})
   endif()

   if (NOT LV_ARGS_MAJOR)
      set(MAJOR ${PROJECT_VERSA_MAJOR})
   else()
      set(MAJOR ${LV_ARGS_MAJOR})
   endif()

   if (NOT LV_ARGS_MINOR)
      set(MINOR ${PROJECT_VERSA_MINOR})
   else()
      set(MINOR ${LV_ARGS_MINOR})
   endif()

   if (NOT LV_ARGS_PATCH)
      set(PATCH ${PROJECT_VERSA_PATCH})
   else()
      set(PATCH ${LV_ARGS_PATCH})
   endif()

   if (NOT LV_ARGS_TWEAK)
      set(TWEAK ${PROJECT_VERSA_TWEAK})
   else()
      set(TWEAK ${LV_ARGS_TWEAK})
   endif()

   if (NOT LV_ARGS_SUFFIX)
      if (PROJECT_VERSA_SUFFIX)
         set(SUFFIX "-${PROJECT_VERSA_SUFFIX}")
      endif()
   else()
      set(SUFFIX ${LV_ARGS_SUFFIX})
   endif()
   
   if (LV_ARGS_GIT_HASH)
      message(STATUS "Retrieving latest commit hash from Git repository")
      execute_process(
         COMMAND git log -1 --format=%H
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
         OUTPUT_VARIABLE GIT_HASH
         OUTPUT_STRIP_TRAILING_WHITESPACE
      )
   endif()
   
   set(LV_MSG "Creating version information for ${NAMESPACE} ${MAJOR}.${MINOR}.${PATCH}.${TWEAK}")

   if (SUFFIX)
      set(LV_MSG "${LV_MSG}-${SUFFIX}")
      set(LV_USE_SUFFIX 1)
   endif()

   if (GIT_HASH)
      set(LV_MSG "${LV_MSG} (${GIT_HASH})")
      set(LV_USE_GIT_HASH 1)
   endif()

   message(STATUS ${LV_MSG})

   if (NOT LV_ARGS_INCLUDE_DIR)
      set(LV_ARGS_INCLUDE_DIR ${_VERSA_INCLUDE_DIR}/include)
   endif()

   configure_file(${LV_ARGS_INCLUDE_DIR}/versa/info/version.pp.in
                  ${CMAKE_CURRENT_BINARY_DIR}/include/${NAMESPACE}/info/version.pp @ONLY)

   configure_file(${LV_ARGS_INCLUDE_DIR}/versa/info/version.hpp.in
                  ${CMAKE_CURRENT_BINARY_DIR}/include/${NAMESPACE}/info/version.hpp @ONLY)
endfunction(versa_create_version_info)
 

#macro(versa_setup_target _target)
#   set(options GIT_HASH)
#   set(oneValueArgs NAMESPACE MAJOR MINOR PATCH TWEAK SUFFIX)
#   set(multiValueArgs)
#   cmake_parse_arguments( LV_ARGS "${options}" 
#                                  "${oneValueArgs}"
#                                  "${multiValueArgs}" 
#                                  ${ARGN} )
#
#   message(STATUS "Generating version information for ${LV_TARGETS}")
#
#   if (NOT LV_ARGS_NAMESPACE) 
#      set(NAMESPACE ${PROJECT_NAME})
#   else()
#      set(NAMESPACE ${LV_ARGS_NAMESPACE})
#   endif()
#
#   if (NOT LV_ARGS_MAJOR)
#      set(MAJOR ${PROJECT_VERSA_MAJOR})
#   else()
#      set(MAJOR ${LV_ARGS_MAJOR})
#   endif()
#
#   if (NOT LV_ARGS_MINOR)
#      set(MINOR ${PROJECT_VERSA_MINOR})
#   else()
#      set(MINOR ${LV_ARGS_MINOR})
#   endif()
#
#   if (NOT LV_ARGS_PATCH)
#      set(PATCH ${PROJECT_VERSA_PATCH})
#   else()
#      set(PATCH ${LV_ARGS_PATCH})
#   endif()
#
#   if (NOT LV_ARGS_TWEAK)
#      set(TWEAK ${PROJECT_VERSA_TWEAK})
#   else()
#      set(TWEAK ${LV_ARGS_TWEAK})
#   endif()
#
#   if (NOT LV_ARGS_SUFFIX)
#      if (PROJECT_VERSA_SUFFIX)
#         set(SUFFIX "-${PROJECT_VERSA_SUFFIX}")
#      endif()
#   else()
#      set(SUFFIX ${LV_ARGS_SUFFIX})
#   endif()
#   
#   if (LV_ARGS_GIT_HASH)
#      message(STATUS "Retrieving latest commit hash from Git repository")
#      execute_process(
#         COMMAND git log -1 --format=%H
#         WORKING_DIRECTORY ${${PROJECT_NAME}_LIBVERSA_PROJECT_DIR}
#         OUTPUT_VARIABLE GIT_HASH
#         OUTPUT_STRIP_TRAILING_WHITESPACE
#      )
#   endif()
#   
#   set(LV_MSG "Creating version information for ${NAMESPACE} ${MAJOR}.${MINOR}.${PATCH}.${TWEAK}")
#
#   if (SUFFIX)
#      set(LV_MSG "${LV_MSG}-${SUFFIX}")
#      set(LV_USE_SUFFIX 1)
#   endif()
#
#   if (GIT_HASH)
#      set(LV_MSG "${LV_MSG} (${GIT_HASH})")
#      set(LV_USE_GIT_HASH 1)
#   endif()
#
#   message(STATUS ${LV_MSG})
#
#   target_compile_definitions(${_target} PRIVATE 
#      _VERSA_PROJECT_NAMESPACE=${LV_NAMESPACE}
#      _VERSA_PROJECT_MAJOR_VERSION=${LV_MAJOR}
#      _VERSA_PROJECT_MINOR_VERSION=${LV_MINOR}
#      _VERSA_PROJECT_PATCH_VERSION=${LV_PATCH}
#      _VERSA_PROJECT_TWEAK_VERSION=${LV_TWEAK}
#      _VERSA_PROJECT_SUFFIX=${LV_SUFFIX}
#      _VERSA_PROJECT_GIT_HASH=${LV_GIT_HASH}
#      _VERSA_PROJECT_USE_SUFFIX=${LV_USE_SUFFIX}
#      _VERSA_PROJECT_USE_GIT_HASH=${LV_USE_GIT_HASH}
#      NSS="foo"
#   )
#
#   #get_target_property(LV_INCLUDES versa INCLUDE_DIRECTORIES)
#   #list(APPEND LV_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/include)
#   #set_target_properties(versa PROPERTIES INCLUDE_DIRECTORIES LV_INCLUDES)
#endmacro()

# version stuff, command line parsing, function traits, reflection, logging, allocators, static and dynamic modules