# FILEPATH: cmake/Doxy.cmake

# This CMake script defines a function `astronaught_generate_doxygen_docs` that generates Doxygen documentation for a project.
# The function takes several arguments, including options and values, to customize the documentation generation process.
# It first parses the function arguments using `cmake_parse_arguments` and sets default values for the arguments if not provided.
# Then, it ensures that Doxygen is available by using `find_package(Doxygen REQUIRED)`.
# Next, it sets default values for the Doxygen configuration file, output directory, and other variables if not provided.
# It determines the source directories for documentation generation and appends the project include directories if the `ONLY_PROVIDED_DIRS` option is not set.
# After that, it creates the necessary directories for Doxygen output and determines the number of processors available.
# The Doxygen file and variables are configured, and the Doxyfile is generated using `configure_file`.
# A custom command is added to generate the documentation using the Doxygen executable and the generated Doxyfile.
# Finally, a custom target is added to trigger the documentation generation, and the generated documentation is installed.
# This script is typically included in a CMake project to enable Doxygen documentation generation.

include(cmake/Utils.cmake)

function(astronaught_generate_doxygen_docs)
   # Define argument types
   set(options ONLY_PROVIDED_DIRS VERBOSE)
   set(oneValueArgs NAME EXTRA_FILES CONFIG_NAME DOX_DIR DOX_OUTPUT_DIR)
   set(multiValueArgs SOURCE_DIRS EXCLUDE_DIRS)

   # TODO Add support for EXCLUDE_DIRS

   # Parse function arguments
   cmake_parse_arguments(ARGS "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

   set(VERBOSE ${ARGS_VERBOSE})
   if(NOT VERBOSE)
      set(VERBOSE FALSE)
   endif()

   astronaught_project_include_dirs(include_dirs)
   if (VERBOSE)
      message(STATUS "Found Project Include Directories: ${include_dirs}")
   endif()

   # Ensure Doxygen is available
   find_package(Doxygen REQUIRED ${VERBOSE})

   # Set default values if not provided
   set(DOX_NAME ${ARGS_NAME})
   if(NOT DOX_NAME)
      set(DOX_NAME ${PROJECT_NAME})
   endif()
   
   set(DOX_EXTRA_FILES ${ARGS_EXTRA_FILES})
   if(NOT DOX_EXTRA_FILES)
      set(DOX_EXTRA_FILES "")
   endif()
   
   set(DOX_DIR ${ARGS_DOX_DIR})
   if(NOT DOX_DIR)
      set(DOX_DIR ${PROJECT_SOURCE_DIR})
   endif()
   
   set(DOX_OUTPUT_DIR ${ARGS_DOX_OUTPUT_DIR})
   if(NOT DOX_OUTPUT_DIR)
      set(DOX_OUTPUT_DIR ${PROJECT_BINARY_DIR}/docs/doxygen)
   endif()
   
   set(DOX_CONFIG ${ARGS_CONFIG_NAME})
   if(NOT DOX_CONFIG)
      set(DOX_CONFIG ${DOX_DIR}/docs/Doxyfile.in)
   endif()

   set(SOURCE_DIRS ${ARGS_SOURCE_DIRS})
   if(NOT SOURCE_DIRS)
      set(SOURCE_DIRS ${PROJECT_SOURCE_DIRS})
   endif()

   if (NOT ARGS_ONLY_PROVIDED_DIRS)
      list(APPEND SOURCE_DIRS ${include_dirs})
   endif()

   if (VERBOSE)
      message(STATUS "Source Directories Recursed for Documentation: ${SOURCE_DIRS}")
   endif()

   string(REPLACE ";" " " DOX_SOURCE_DIRS "${SOURCE_DIRS}")

   # Create necessary directories
   file(MAKE_DIRECTORY ${DOX_OUTPUT_DIR})

   # Determine the number of processors
   include(ProcessorCount)
   ProcessorCount(N)
   set(DOX_NUM_PROC ${N})
   if(${DOX_NUM_PROC} EQUAL 0)
      set(DOX_NUM_PROC 1)
   elseif(${DOX_NUM_PROC} GREATER 32)
      set(DOX_NUM_PROC 32)
   endif()

   if(VERBOSE)
      message(STATUS "Number of Processing Threads for Doxygen: ${DOX_NUM_PROC}")
   endif()

   # Configure Doxygen file and variables
   set(DOX_FILE            ${DOX_OUTPUT_DIR}/Doxyfile)
   set(DOX_INDEX_FILE      ${DOX_OUTPUT_DIR}/html/index.html)
   set(DOX_PROJECT_VERSION ${PROJECT_VERSION})
   set(DOX_PROJECT_BRIEF   ${PROJECT_DESCRIPTION})

   # Configure the Doxyfile with current settings
   configure_file(${DOX_CONFIG} ${DOX_FILE} @ONLY)

   # Add custom command to generate documentation
   add_custom_command(
      OUTPUT ${DOX_INDEX_FILE}
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOX_FILE}
      MAIN_DEPENDENCY ${DOX_FILE} ${DOX_CONFIG}
      DEPENDS ${DOX_EXTRA_FILES}
      COMMENT "Generating Doxygen documentation for ${DOX_NAME}"
   )

   # Add custom target to trigger documentation generation
   add_custom_target(${PROJECT_NAME}_doxygen_docs ALL DEPENDS ${DOX_INDEX_FILE})

   # Install the generated documentation
   install(DIRECTORY ${PROJECT_BINARY_DIR}/html DESTINATION share/doc)
endfunction()