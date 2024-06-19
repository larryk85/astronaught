include(../LibVersa.cmake)

# ##################################################################################################
# Define the main test executable.
# ##################################################################################################
add_executable( 
   libversa_unit_tests 
   async_tests.cpp
   #fixed_string_tests.cpp
   logger_tests.cpp
   memory_tests.cpp
   versa_tests.cpp
   #handler_tests.cpp
   #meta_tests.cpp
   process_tests.cpp
)

target_link_libraries( libversa_unit_tests PRIVATE versa Catch2::Catch2WithMain )
catch_discover_tests(libversa_unit_tests)