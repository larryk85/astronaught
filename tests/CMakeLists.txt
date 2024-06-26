include(../LibVersa.cmake)

# ##################################################################################################
# Define the main test executable.
# ##################################################################################################
add_executable( 
   versa_unit_tests 
   async_tests.cpp
   fixed_string_tests.cpp
   logger_tests.cpp
   map_tests.cpp
   memory_tests.cpp
   versa_tests.cpp
   #handler_tests.cpp
   #frozen_tests.cpp
   #process_tests.cpp
)

add_executable(
   versa_enum_1_tests
   enum_defs_1_tests.cpp
)

add_executable(
   versa_enum_2_tests
   enum_defs_2_tests.cpp
)

add_executable(
   versa_enum_3_tests
   enum_defs_3_tests.cpp
)

add_executable(
   versa_enum_4_tests
   enum_defs_4_tests.cpp
)

target_link_libraries( versa_unit_tests PRIVATE versa Catch2::Catch2WithMain )
target_link_libraries( versa_enum_1_tests PRIVATE versa Catch2::Catch2WithMain )
target_compile_definitions( versa_enum_1_tests PRIVATE 
   -DVERSA_ENUM_MAX_ELEMS=50
   -DVERSA_ENUM_UPPER_BOUND=120
)

target_link_libraries( versa_enum_2_tests PRIVATE versa Catch2::Catch2WithMain )
target_compile_definitions( versa_enum_2_tests PRIVATE 
   -DVERSA_ENUM_MAX_ELEMS=135
   -DVERSA_ENUM_LOWER_BOUND=-300
)

target_link_libraries( versa_enum_3_tests PRIVATE versa Catch2::Catch2WithMain )
target_compile_definitions( versa_enum_3_tests PRIVATE 
   -DVERSA_ENUM_UPPER_BOUND=3600
   -DVERSA_ENUM_LOWER_BOUND=3000
)

target_link_libraries( versa_enum_4_tests PRIVATE versa Catch2::Catch2WithMain )
target_compile_definitions( versa_enum_4_tests PRIVATE 
   -DVERSA_ENUM_UPPER_BOUND=1313
)

catch_discover_tests( versa_unit_tests )
catch_discover_tests( versa_enum_1_tests )
catch_discover_tests( versa_enum_2_tests )
catch_discover_tests( versa_enum_3_tests )
catch_discover_tests( versa_enum_4_tests )