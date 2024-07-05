function(versa_generate_doxygen_docs)
   set(options)
   set(oneValueArgs NAME EXTRA_FILES CONFIG_NAME SOURCE_DIR DOX_DIR DOX_OUTPUT_DIR)
   set(multiValueArgs)
   cmake_parse_arguments( ARGS "${options}" 
                               "${oneValueArgs}"
                               "${multiValueArgs}" 
                               ${ARGN} )

   find_package(Doxygen REQUIRED)
   if (NOT DOXYGEN_FOUND)
      message(FATAL_ERROR "Doxygen is needed to build the documentation.")
   endif()

   if (NOT ARGS_NAME)
      set(DOX_NAME ${PROJECT_NAME})
   else()
      set(DOX_NAME ${ARGS_NAME})
   endif()

   if (NOT ARGS_EXTRA_FILES)
      set(DOX_EXTRA_FILES "")
   else()
      set(DOX_EXTRA_FILES ${ARGS_EXTRA_FILES})
   endif()

   if (NOT ARGS_DOX_DIR)
      set(DOX_DIR ${PROJECT_SOURCE_DIR})
   else()
      set(DOX_DIR ${ARGS_DOX_DIR})
   endif()

   if (NOT ARGS_DOX_OUTPUT_DIR)
      set(DOX_OUTPUT_DIR ${PROJECT_BINARY_DIR}/docs/doxygen)
   else()
      set(DOX_OUTPUT_DIR ${ARGS_DOX_OUTPUT_DIR})
   endif()

   if (NOT ARGS_CONFIG_NAME)
      set(DOX_CONFIG ${DOX_DIR}/docs/Doxyfile.in)
   else()
      set(DOX_CONFIG ${DOX_DIR}/docs/${ARGS_CONFIG_NAME})
   endif()

   file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/docs)
   file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/docs/doxygen)
   file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/html)

   include(ProcessorCount)
   ProcessorCount(N)
   if (NOT N EQUAL 0)
      set(DOX_NUM_PROC ${N})
   else()
      set(DOX_NUM_PROC 1)
   endif()

   set(DOX_FILE ${DOX_OUTPUT_DIR}/Doxyfile)
   set(DOX_INDEX_FILE ${PROJECT_BINARY_DIR}/html/index.html)
   set(DOX_PROJECT_VERSION ${PROJECT_VERSION})
   set(DOX_PROJECT_BRIEF ${PROJECT_DESCRIPTION})

   configure_file(${DOX_CONFIG} ${DOX_FILE} @ONLY)

   add_custom_command(
      OUTPUT ${DOX_INDEX_FILE}
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOX_FILE}
      MAIN_DEPENDENCY ${DOX_FILE} ${DOX_CONFIG}
      DEPENDS ${DOX_EXTRA_FILES}
      COMMENT "Generating Doxygen documentation for ${DOX_NAME}"
   )

   add_custom_target( ${PROJECT_NAME}_doxygen_docs ALL DEPENDS ${DOX_INDEX_FILE})

   install( DIRECTORY ${PROJECT_BINARY_DIR}/html DESTINATION share/doc )

endfunction(versa_generate_doxygen_docs)