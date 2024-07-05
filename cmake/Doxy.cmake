function(versa_generate_doxygen_docs)
   # Define argument types
   set(options)
   set(oneValueArgs NAME EXTRA_FILES CONFIG_NAME DOX_DIR DOX_OUTPUT_DIR)
   set(multiValueArgs SOURCE_DIRS)
   # Parse function arguments
   cmake_parse_arguments(ARGS "" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

   # Ensure Doxygen is available
   find_package(Doxygen REQUIRED)

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
   string(REPLACE ";" " " DOX_SOURCE_DIRS "${SOURCE_DIRS}")

   # Create necessary directories
   file(MAKE_DIRECTORY ${DOX_OUTPUT_DIR})
   file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/html)

   # Determine the number of processors
   include(ProcessorCount)
   ProcessorCount(N)
   if(NOT N OR N EQUAL 0)
      set(DOX_NUM_PROC 1)
   else()
      set(DOX_NUM_PROC ${N})
   endif()

   # Configure Doxygen file and variables
   set(DOX_FILE ${DOX_OUTPUT_DIR}/Doxyfile)
   set(DOX_INDEX_FILE ${PROJECT_BINARY_DIR}/html/index.html)
   set(DOX_PROJECT_VERSION ${PROJECT_VERSION})
   set(DOX_PROJECT_BRIEF ${PROJECT_DESCRIPTION})

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
