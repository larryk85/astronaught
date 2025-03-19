

# FILEPATH: cmake/Astro.cmake

# This CMake script defines a macro called `astro_create_version_info` that is used to generate version information for a project.
# The macro takes several arguments, including `NAMESPACE`, `MAJOR`, `MINOR`, `PATCH`, `TWEAK`, and `SUFFIX`, which can be provided by the user.
# If any of these arguments are not provided, the macro will use default values based on the project's version information.
# The macro also supports an optional `GIT_LOG` flag, which, when enabled, retrieves the latest commit hash from the Git repository.
# The macro uses the `configure_file` command to generate `version.h` and `version.hpp` files based on template files.
# These generated files will contain the version information and can be included in the project's source code.

# Usage:
# astro_create_version_info(NAMESPACE <namespace> [MAJOR <major>] [MINOR <minor>] [PATCH <patch>] [TWEAK <tweak>] [SUFFIX <suffix>] [GIT_LOG])

# Arguments:
# - `NAMESPACE`: The namespace for the version information. If not provided, it defaults to the project's namespace.
# - `MAJOR`: The major version number. If not provided, it defaults to the project's major version number.
# - `MINOR`: The minor version number. If not provided, it defaults to the project's minor version number.
# - `PATCH`: The patch version number. If not provided, it defaults to the project's patch version number.
# - `TWEAK`: The tweak version number. If not provided, it defaults to the project's tweak version number.
# - `SUFFIX`: The version suffix. If not provided, it defaults to the project's version suffix, if available.
# - `GIT_HASH`: Optional flag to include the latest commit hash in the version information. If provided, it retrieves the commit hash using the `git log` command.
# - `INCLUDE_DIR`: This is the location of the astronaught/version.h.in, version.hpp.in, etc.
# Example:
# astro_create_version_info(NAMESPACE MyProject MAJOR 1 MINOR 2 PATCH 3 TWEAK 4 SUFFIX "alpha" GIT_LOG)

cmake_minimum_required(VERSION 3.18)

include(ExternalProject)
include(GNUInstallDirs)

function(astro_create_version_info)
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
endfunction(astro_create_version_info)

function(astro_color_map token output)
   string(ASCII 27 Esc)
   macro(set_color_code code)
      foreach(var ${ARGN})
         set(Color_${var} "${code}")
      endforeach()
   endmacro()

   set_color_code("${Esc}[0m"    reset           R  0)
   set_color_code("${Esc}[1m"    bold             B  1)
   set_color_code("${Esc}[2m"    Dim              Dm 16)
   set_color_code("${Esc}[3m"    italic           It 17)
   set_color_code("${Esc}[4m"    underline        Ul 18)
   set_color_code("${Esc}[5m"    blink            Bl 19)
   set_color_code("${Esc}[9m"    strikethrough    St 20)
   set_color_code("${Esc}[30m"   black            bk 2)
   set_color_code("${Esc}[31m"   red              r  2)
   set_color_code("${Esc}[32m"   green            g  3)
   set_color_code("${Esc}[33m"   yellow           y  4)
   set_color_code("${Esc}[34m"   blue             b  5)
   set_color_code("${Esc}[35m"   magenta          m  6)
   set_color_code("${Esc}[36m"   cyan             c  7)
   set_color_code("${Esc}[37m"   white            w  8)
   set_color_code("${Esc}[40m"   bg_black         bgbk 21)
   set_color_code("${Esc}[41m"   bg_red           bgr  22)
   set_color_code("${Esc}[42m"   bg_green         bgg  23)
   set_color_code("${Esc}[43m"   bg_yellow        bgy  24)
   set_color_code("${Esc}[44m"   bg_blue          bgb  25)
   set_color_code("${Esc}[45m"   bg_magenta       bgm  26)
   set_color_code("${Esc}[46m"   bg_cyan          bgc  27)
   set_color_code("${Esc}[47m"   bg_white         bgw  28)

   set(${output} "${Color_${token}}" PARENT_SCOPE)
endfunction(astro_color_map)

# Function to check if output is to a terminal with color support
function(astro_detect_color_support result_var)
   # Default to no color support
   set(color_support FALSE)

   # Check for common UNIX-like terminal environment variables
   if(DEFINED ENV{TERM})
      string(FIND "$ENV{TERM}" "xterm" xterm_found)
      string(FIND "$ENV{TERM}" "color" color_found)
      string(FIND "$ENV{TERM}" "ansi" ansi_found)
      string(FIND "$ENV{TERM}" "screen" screen_found)
      string(FIND "$ENV{TERM}" "tmux" tmux_found)
      if (xterm_found GREATER -1 OR color_found GREATER -1 OR ansi_found GREATER -1 OR screen_found GREATER -1 OR tmux_found GREATER -1)
         set(color_support TRUE)
      endif()
   endif()

   # Check for COLORTERM environment variable
   if(DEFINED ENV{COLORTERM})
      set(color_support TRUE)
   endif()

   # Check for Windows Terminal
   if(DEFINED ENV{WT_SESSION})
      set(color_support TRUE)
   endif()

   set(${result_var} ${color_support} PARENT_SCOPE)
endfunction(astro_detect_color_support)

macro(astro_call func)
   cmake_language(CALL ${func} ${ARGN})
endmacro(astro_call)

# Function to remove ANSI color escape codes from a string
function(astro_remove_ansi_escapes output_msg msg)
   string(ASCII 27 Esc)
   string(REGEX REPLACE "${Esc}\[(1;)?[0-9]*m" "" clean_msg "${msg}")
   set(${output_msg} "${clean_msg}" PARENT_SCOPE)
endfunction(astro_remove_ansi_escapes)

function(astro_fmt output msg)
   set(options STYLE)
   set(options)
   set(oneValueArgs STYLE)
   set(multiValueArgs)
   cmake_parse_arguments(ARGS "${options}"
                              "${oneValueArgs}"
                              "${multiValueArgs}"
                              ${ARGN})

   if (ARGS_STYLE)
      if (ARGS_STYLE   STREQUAL "PERCENT")
         set(delin "%")
         set(re_delin "%")
      elseif (ARGS_STYLE STREQUAL "CARET")
         set(delin "^")
         set(re_delin "\\^")
      elseif (ARGS_STYLE STREQUAL "EXCLAMATION")
         set(delin "!")
         set(re_delin "\\!")
      elseif (ARG_STYLE  STREQUAL "AMPERSAND")
         set(delin "@")
         set(re_delin "@")
      else()
         message(FATAL_ERROR "Unknown style: ${ARGS_STYLE} <::> Valid styles: [PERCENT, CARET, EXCLAMATION, AMPERSAND]")
      endif()
   else()
      set(delin "^")
      set(re_delin "\\^")
   endif()

   set(full_msg "${msg}${delin}reset${delin}")

   macro(replace pattern0 pattern1 pattern2)
      astro_color_map(${pattern0} color_code)
      string(REGEX REPLACE "${re_delin}(${pattern0}|${pattern1}|${pattern2})${re_delin}[ ]?" "${color_code}" full_msg "${full_msg}")
   endmacro()

   replace("reset"           "R"    "0")
   replace("bold"            "B"    "1")
   replace("red"             "r"    "2")
   replace("green"           "g"    "3")
   replace("yellow"          "y"    "4")
   replace("blue"            "b"    "5")
   replace("magenta"         "m"    "6")
   replace("cyan"            "c"    "7")
   replace("white"           "w"    "8")
   replace("bold_red"        "Br"   "9")
   replace("bold_green"      "Bg"   "10")
   replace("bold_yellow"     "By"   "11")
   replace("bold_blue"       "Bb"   "12")
   replace("bold_magenta"    "Bm"   "13")
   replace("bold_cyan"       "Bc"   "14")
   replace("bold_white"      "Bw"   "15")
   replace("dim"             "Dm"   "16")
   replace("italic"          "It"   "17")
   replace("underline"       "Ul"   "18")
   replace("blink"           "Bl"   "19")
   replace("strikethrough"   "St"   "20")
   replace("bg_black"        "bgbk" "21")
   replace("bg_red"          "bgr"  "22")
   replace("bg_green"        "bgg"  "23")
   replace("bg_yellow"       "bgy"  "24")
   replace("bg_blue"         "bgb"  "25")
   replace("bg_magenta"      "bgm"  "26")
   replace("bg_cyan"         "bgc"  "27")
   replace("bg_white"        "bgw"  "28")
   replace("bg_bold_red"     "bgBr" "29")
   replace("bg_bold_green"   "bgBg" "30")
   replace("bg_bold_yellow"  "bgBy" "31")
   replace("bg_bold_blue"    "bgBb" "32")
   replace("bg_bold_magenta" "bgBm" "33")
   replace("bg_bold_cyan"    "bgBc" "34")
   replace("bg_bold_white"   "bgBw" "35")

   astro_detect_color_support(color_support)

   if (NOT color_support)
      string(ASCII 27 Esc)
      string(REGEX REPLACE "${Esc}\[(1;)?[0-9]*m" "" full_msg "${full_msg}")
   endif()

   set(${output} "${full_msg}" PARENT_SCOPE)
endfunction(astro_fmt)

function(astro_print message)
   set(options)
   set(oneValueArgs TYPE STYLE)
   set(multiValueArgs)
   cmake_parse_arguments(ARGS "${options}"
                              "${oneValueArgs}"
                              "${multiValueArgs}"
                              ${ARGN})

   astro_fmt(output "${message}" STYLE ${ARGS_STYLE})
   if (ARGS_TYPE)
      message(${ARGS_TYPE} "${output}")
   else()
      message(STATUS "${output}")
   endif()
endfunction(astro_print)

function(astro_center_text output msg length)
   string(LENGTH "${msg}" msg_len)
   math(EXPR total_padding "${length} - ${msg_len}")
   math(EXPR left_padding "${total_padding} / 2")
   math(EXPR right_padding "${total_padding} - ${left_padding}")

   string(REPEAT " " ${left_padding} left_padding_str)
   string(REPEAT " " ${right_padding} right_padding_str)
   set(${output} "${left_padding_str}${msg}${right_padding_str}" PARENT_SCOPE)
endfunction(astro_center_text)


function(astro_options_print)
   set(options)
   set(oneValueArgs)
   set(multiValueArgs OPTIONS)
   cmake_parse_arguments(ARGS "${options}"
                              "${oneValueArgs}"
                              "${multiValueArgs}"
                              ${ARGN})

   set(orig_version_str "^bold^^blue^^italic^${PROJECT_NAME} ^reset^^yellow^v${${PROJECT_NAME}_VERSION}^reset^")
   astro_fmt(version_str "${orig_version_str}")
   astro_remove_ansi_escapes(clean_version_str "${version_str}")
   string(LENGTH "${clean_version_str}" version_str_len)

   set(longest_opt_len 0)
   foreach(option IN LISTS ARGS_OPTIONS)
      string(LENGTH "${option}" opt_len)
      if (opt_len GREATER longest_opt_len)
         set(longest_opt_len ${opt_len})
      endif()
   endforeach()

   math(EXPR full_length "${longest_opt_len} + 20")
   math(EXPR text_length "${full_length} - 11")

   string(REPEAT "─" ${version_str_len} version_border)
   string(REPEAT "─" 6 status_border)
   set(version_title "╭┤${version_str}╞")
   math(EXPR version_title_len "${version_str_len}+3")
   set(status_title "╡^bold_cyan^status^reset^├╮")
   astro_fmt(status_title "${status_title}")
   set(status_title_len 8)

   math(EXPR partial_sum "${version_str_len} + 3 + ${status_title_len}")
   math(EXPR diff_len "${full_length} - ${partial_sum}")

   string(REPEAT "═" ${diff_len} diff_str)
   set(main_title "${version_title}${diff_str}${status_title}")

   string(REPEAT " " ${diff_len} diff_str)
   message(STATUS " ╭${version_border}╮${diff_str}╭${status_border}╮")
   message(STATUS "${main_title}")
   message(STATUS "│╰${version_border}╯${diff_str}╰${status_border}╯│")

   string(REPEAT "═" ${text_length} left_bar)
   string(REPEAT "═" 9 right_bar)
   message(STATUS "╞${left_bar}╦${right_bar}╡")

   macro(print_option option)
      astro_center_text(option_str "${option}" ${text_length})
      if (${${option}})
         set(status_value "✅")
         astro_center_text(status_value "${status_value}" 10)
         astro_fmt(status_value "^bg_bold_green^ ${status_value}^reset^")
         astro_fmt(option_str "^green^^dim^${option_str} ^reset^")
         message(STATUS "│${option_str}║${status_value}│")
      else()
         set(status_value "❌")
         astro_center_text(status_value "${status_value}" 10)
         astro_fmt(status_value "^dim^^bg_bold_red^ ${status_value}^reset^")
         astro_fmt(option_str "^red^^dim^^strikethrough^${option_str} ^reset^")
         message(STATUS "│${option_str}║${status_value}│")
      endif()
   endmacro(print_option)

   list(LENGTH ARGS_OPTIONS options_length)
   math(EXPR last_options_index "${options_length} - 2")
   foreach(index RANGE 0 ${last_options_index})
      list(GET ARGS_OPTIONS ${index} option)
      print_option(${option})
      message(STATUS "╞${left_bar}╬${right_bar}╡")
   endforeach()

   math(EXPR last_options_index "${options_length} - 1")
   list(GET ARGS_OPTIONS ${last_options_index} option)
   print_option(${option})
   message(STATUS "╞${left_bar}╩${right_bar}╡")

   string(REPEAT "─" ${text_length} left_bar)
   string(REPEAT "─" 9 right_bar)
   message(STATUS "╰${left_bar}─${right_bar}╯")
endfunction(astro_options_print)