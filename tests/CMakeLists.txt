include(../cmake/Astro.cmake)

# ##################################################################################################
# Define the main test executable.
# ##################################################################################################
define_property(
   TARGET
   PROPERTY
   ASTRO_IS_TEST
   BRIEF_DOCS "Is this target a test?"
   FULL_DOCS "Is this target a test?"
)

macro( add_test name )
   add_executable( ${name} ${name}.cpp )
   target_link_libraries( ${name} PRIVATE astro::naught Catch2::Catch2WithMain )
   set_property( TARGET ${name} PROPERTY ASTRO_IS_TEST TRUE )
   catch_discover_tests( ${name} )
endmacro()

add_test(astro_tests)
#add_test(async_tests)
add_test(compile_time_tests)
add_test(cryptid_tests)
add_test(debug_tests)
add_test(file_tests)
add_test(fixed_string_tests)
add_test(frozen_tests)
add_test(io_tests)
#add_test(logger_tests)
add_test(map_tests)
add_test(memory_tests)
#add_test(interpolate_tests)
add_test(serialize_tests)
add_test(types_tests)
add_test(utils_tests)
add_test(testing_tests)

add_test(enum_defs_1_tests)
add_test(enum_defs_2_tests)
add_test(enum_defs_3_tests)
add_test(enum_defs_4_tests)

target_compile_definitions( enum_defs_1_tests PRIVATE
   -DASTRO_ENUM_MAX_ELEMS=50
   -DASTRO_ENUM_UPPER_BOUND=120
)

target_compile_definitions( enum_defs_2_tests PRIVATE
   -DASTRO_ENUM_MAX_ELEMS=135
   -DASTRO_ENUM_LOWER_BOUND=-300
)

target_compile_definitions( enum_defs_3_tests PRIVATE
   -DASTRO_ENUM_UPPER_BOUND=3600
   -DASTRO_ENUM_LOWER_BOUND=3000
)

target_compile_definitions( enum_defs_4_tests PRIVATE
   -DASTRO_ENUM_UPPER_BOUND=1313
)