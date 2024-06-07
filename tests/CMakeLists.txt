include(../LibVersa.cmake)

# ##################################################################################################
# Define the main test executable.
# ##################################################################################################
add_executable( 
   libversa_unit_tests 
   versa_tests.cpp
   fixed_string_tests.cpp
)

versa_setup_target( libversa_unit_tests
   NAMESPACE test_version_0
   MAJOR 1
   MINOR 0
   PATCH 0
   TWEAK 0
   SUFFIX alpha
)

target_link_libraries( libversa_unit_tests PRIVATE versa Catch2::Catch2WithMain )
catch_discover_tests(libversa_unit_tests)