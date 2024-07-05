include(../LibVersa.cmake)

# ##################################################################################################
# Define the main test executable.
# ##################################################################################################
define_property(
   TARGET 
   PROPERTY 
   VERSA_IS_TEST 
   BRIEF_DOCS "Is this target a test?" 
   FULL_DOCS "Is this target a test?"
)

macro( add_test name )
   add_executable( ${name} ${name}.cpp )
   target_link_libraries( ${name} PRIVATE versa Catch2::Catch2WithMain )
   set_property( TARGET ${name} PROPERTY VERSA_IS_TEST TRUE )
   catch_discover_tests( ${name} )
endmacro()

add_test(async_tests)
add_test(compile_time_tests)
add_test(cryptid_tests)
add_test(fixed_string_tests)
add_test(logger_tests)
add_test(map_tests)
add_test(memory_tests)
add_test(versa_tests)
add_test(interpolate_tests)
add_test(serialize_tests)
add_test(type_tests)
add_test(utils_tests)

add_test(enum_defs_1_tests)
add_test(enum_defs_2_tests)
add_test(enum_defs_3_tests)
add_test(enum_defs_4_tests)

target_compile_definitions( enum_defs_1_tests PRIVATE 
   -DVERSA_ENUM_MAX_ELEMS=50
   -DVERSA_ENUM_UPPER_BOUND=120
)

target_compile_definitions( enum_defs_2_tests PRIVATE 
   -DVERSA_ENUM_MAX_ELEMS=135
   -DVERSA_ENUM_LOWER_BOUND=-300
)

target_compile_definitions( enum_defs_3_tests PRIVATE 
   -DVERSA_ENUM_UPPER_BOUND=3600
   -DVERSA_ENUM_LOWER_BOUND=3000
)

target_compile_definitions( enum_defs_4_tests PRIVATE 
   -DVERSA_ENUM_UPPER_BOUND=1313
)