function(versa_generate_sphinx_docs)
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

   #versa_project_include_dirs(include_dirs)

   # Ensure Sphinx is available
   find_package(Sphinx REQUIRED ${VERBOSE})
   set(SPHINX_SOURCE ${PROJECT_SOURCE_DIR})
   set(SPHINX_BUILD ${PROJECT_BINARY_DIR}/docs/sphinx)

   add_custom_target(Sphinx ALL
                     COMMAND
                     ${SPHINX_EXECUTABLE} -b html
                     -Dbreathe_projects.versa=${DOXYGEN_OUTPUT_DIR}/xml
                     ${SPHINX_SOURCE} ${SPHINX_BUILD}
                     WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                     DEPENDS
                     ${PROJECT_SOURCE_DIR}/index.rst
                     ${DOXYGEN_INDEX_FILE}
                     MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
                     COMMENT "Generating documentation with Sphinx")
endfunction()