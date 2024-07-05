versa_generate_doxygen_docs()

# find_package(Doxygen REQUIRED)

# #This will be the main output of our command
# set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
# set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
# set(DOXYGEN_INDEX_FILE ${CMAKE_CURRENT_BINARY_DIR}/html/index.html)
# set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
# set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) #Doxygen won't create this for us
# file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}) #Doxygen won't create this for us
# add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
#                    DEPENDS versa::versa 
#                    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
#                    MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
#                    COMMENT "Generating docs"
#                    VERBATIM
# )

# add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

# find_package(Sphinx REQUIRED)
# set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
# set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
# set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# add_custom_target(Sphinx ALL
#                   COMMAND
#                   ${SPHINX_EXECUTABLE} -b html
#                   -Dbreathe_projects.versa=${DOXYGEN_OUTPUT_DIR}/xml
#                   ${SPHINX_SOURCE} ${SPHINX_BUILD}
#                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#                   DEPENDS
#                   ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
#                   ${DOXYGEN_INDEX_FILE}
#                   MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
#                   COMMENT "Generating documentation with Sphinx"
# )