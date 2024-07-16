# FILEPATH: cmake/Astro.cmake

# This CMake script defines a macro called `astronaught_create_version_info` that is used to generate version information for a project.
# The macro takes several arguments, including `NAMESPACE`, `MAJOR`, `MINOR`, `PATCH`, `TWEAK`, and `SUFFIX`, which can be provided by the user.
# If any of these arguments are not provided, the macro will use default values based on the project's version information.
# The macro also supports an optional `GIT_LOG` flag, which, when enabled, retrieves the latest commit hash from the Git repository.
# The macro uses the `configure_file` command to generate `version.h` and `version.hpp` files based on template files.
# These generated files will contain the version information and can be included in the project's source code.

# Usage:
# astronaught_create_version_info(NAMESPACE <namespace> [MAJOR <major>] [MINOR <minor>] [PATCH <patch>] [TWEAK <tweak>] [SUFFIX <suffix>] [GIT_LOG])

# Arguments:
# - `NAMESPACE`: The namespace for the version information. If not provided, it defaults to the project's namespace.
# - `MAJOR`: The major version number. If not provided, it defaults to the project's major version number.
# - `MINOR`: The minor version number. If not provided, it defaults to the project's minor version number.
# - `PATCH`: The patch version number. If not provided, it defaults to the project's patch version number.
# - `TWEAK`: The tweak version number. If not provided, it defaults to the project's tweak version number.
# - `SUFFIX`: The version suffix. If not provided, it defaults to the project's version suffix, if available.
# - `GIT_HASH`: Optional flag to include the latest commit hash in the version information. If provided, it retrieves the commit hash using the `git log` command.
# - `INCLUDE_DIR`: This is the location of the astronaught/version.h.in, version.hpp.in, etc.
# - `LANG`: The language to use for the version information. If not provided, it defaults to the language of your project. Currently, only C and C++ are supported.
#           If set to C, the generated files will use C-style structs, functions and macros in version.h.
#           If set to C++/CXX/CPP, the generated files will use C++-style classes, functions and namespaces in version.hpp.in.

# Example:
# astronaught_create_version_info(NAMESPACE MyProject MAJOR 1 MINOR 2 PATCH 3 TWEAK 4 SUFFIX "alpha" GIT_LOG)

function(astronaught_create_version_info)
   set(options GIT_HASH)
   set(oneValueArgs NAMESPACE MAJOR MINOR PATCH TWEAK SUFFIX INCLUDE_DIR)
   set(multiValueArgs)
   cmake_parse_arguments( ARGS "${options}"
                               "${oneValueArgs}"
                               "${multiValueArgs}"
                               ${ARGN} )

   message(STATUS "Generating version information for ${PROJECT_NAME}")

   set(NAMESPACE ${ARGS_NAMESPACE})
   if (NOT NAMESPACE)
      set(NAMESPACE ${PROJECT_NAME})
   endif()

   set(MAJOR ${ARGS_MAJOR})
   if (NOT MAJOR)
      set(MAJOR ${PROJECT_VERSION_MAJOR})
   endif()

   set(MINOR ${ARGS_MINOR})
   if (NOT MINOR)
      set(MINOR ${PROJECT_VERSION_MINOR})
   endif()

   set(PATCH ${ARGS_PATCH})
   if (NOT PATCH)
      set(PATCH ${PROJECT_VERSION_PATCH})
   endif()

   set(TWEAK ${ARGS_TWEAK})
   if (NOT TWEAK)
      set(TWEAK ${PROJECT_VERSION_TWEAK})
   endif()

   set(SUFFIX ${ARGS_SUFFIX})
   if (NOT SUFFIX)
      set(SUFFIX "-${SUFFIX}")
   endif()

   if (ARGS_GIT_HASH)
      message(STATUS "Retrieving latest commit hash from Git repository")
      execute_process(
         COMMAND git log -1 --format=%H
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
         OUTPUT_VARIABLE GIT_HASH
         OUTPUT_STRIP_TRAILING_WHITESPACE
      )
   endif()

   set(MSG "Creating version information for ${NAMESPACE} ${MAJOR}.${MINOR}.${PATCH}.${TWEAK}")

   if (SUFFIX)
      set(MSG "${MSG}-${SUFFIX}")
      set(USE_SUFFIX 1)
   endif()

   if (GIT_HASH)
      set(MSG "${MSG} (${GIT_HASH})")
      set(USE_GIT_HASH 1)
   endif()

   message(STATUS ${MSG})

   set(INCLUDE_DIR ${ARGS_INCLUDE_DIR})
   if (NOT INCLUDE_DIR)
      set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
   endif()

   configure_file(${INCLUDE_DIR}/astro/info/version.pp.in
                  ${PROJECT_BINARY_DIR}/include/${NAMESPACE}/info/version.pp @ONLY)

   configure_file(${INCLUDE_DIR}/astro/info/version.hpp.in
                  ${PROJECT_BINARY_DIR}/include/${NAMESPACE}/info/version.hpp @ONLY)
endfunction(astronaught_create_version_info)

function(astro_options_print)
   set(options)
   set(oneValueArgs)
   set(multiValueArgs OPTIONS)
   cmake_parse_arguments( ARGS "${options}"
                               "${oneValueArgs}"
                               "${multiValueArgs}"
                               ${ARGN} )

   string(LENGTH "${PROJECT_NAME}" NLEN)

   set(vers_str "${PROJECT_NAME} v${${PROJECT_NAME}_VERSION}")
   string(LENGTH "${vers_str}" VSLEN)

   set(ops_avail "options available")
   string(LENGTH "${ops_avail}" OALEN)

   string(REPEAT "─" ${VSLEN} vers_border)
   string(REPEAT "─" ${OALEN} ops_avail_border)
   set(sub_top_title "╭─┤${PROJECT_NAME} v${${PROJECT_NAME}_VERSION}╞══╡${ops_avail}")
   string(LENGTH ${sub_top_title} STLEN)
   math(EXPR TBLN "${VSLEN} + ${OALEN} + 10")
   math(EXPR TBLN_HALF "${TBLN} / 2")
   math(EXPR TBLN_QTR "${TBLN} / 4")
   math(EXPR TBLN_ "${TBLN} / 4")
   string(REPEAT "─" ${TBLN_HALF} bar)
   string(REPEAT " " ${TBLN_HALF} space)

   message(STATUS "  ╭${vers_border}╮  ╭${ops_avail_border}╮")
   message(STATUS "${sub_top_title}${sub_top_border}├───╮")
   message(STATUS "│ ╰${vers_border}╯  ╰${ops_avail_border}╯   │")
   message(STATUS "├${bar}╥${bar}┤")
   message(STATUS "│${space}║${space}│")
   message(STATUS "│${space}║${space}│")
   foreach(option IN ITEMS
      "│ ASTRONAUGHT_ENABLE_TESTS    : ${ASTRONAUGHT_ENABLE_TESTS} - Enable building of unit tests"
      "┊ ASTRONAUGHT_ENABLE_DOCS     : ${ASTRONAUGHT_ENABLE_DOCS} - Enable creation of the documentation"
      "┊ ASTRONAUGHT_ENABLE_INSTALL  : ${ASTRONAUGHT_ENABLE_INSTALL} - Enable installation"
      "│ ASTRONAUGHT_ENABLE_PEDANTIC : ${ASTRONAUGHT_ENABLE_PEDANTIC} - Enable pedantic warnings")
      message(STATUS "${option}")
   endforeach()
endfunction()
