# FILEPATH: cmake/Utils.cmake

# Function to get all targets in the project
#
# This function retrieves all targets in the project by traversing the directory structure
# and collecting the targets from each directory. It stores the result in the specified variable.
#
# Arguments:
# - targets: The variable to store the list of targets
#
# Example usage:
# ```
# astronaught_get_all_targets(all_targets)
# message("All targets: ${all_targets}")
# ```
function(astronaught_get_all_targets targets)
   set(dir_queue ${PROJECT_SOURCE_DIR})
   set(result)

   while(dir_queue)
      list(GET dir_queue 0 current_dir)
      list(REMOVE_AT dir_queue 0)

      get_property(subdirs DIRECTORY ${current_dir} PROPERTY SUBDIRECTORIES)
      list(APPEND dir_queue ${subdirs})

      get_property(sub_targets DIRECTORY ${current_dir} PROPERTY BUILDSYSTEM_TARGETS)
      list(APPEND result ${sub_targets})
   endwhile()
   set(${targets} ${result} PARENT_SCOPE)
endfunction()

# Function to check if a target is a test target
#
# This function checks if the specified target is a test target by inspecting the
# `astronaught_IS_TEST` property of the target. It sets the result in the specified variable.
#
# Arguments:
# - target: The target to check
# - is_test: The variable to store the result (TRUE or FALSE)
#
# Example usage:
# ```
# astronaught_is_test(my_target is_test)
# if (is_test)
#     message("Target is a test")
# else()
#     message("Target is not a test")
# endif()
# ```
function(astronaught_is_test target is_test)
   get_property(is_test_val
      TARGET ${target}
      PROPERTY astronaught_IS_TEST)
   if (${is_test_val})
      set(${is_test} TRUE PARENT_SCOPE)
   else()
      set(${is_test} FALSE PARENT_SCOPE)
   endif()
endfunction()

# Function to check if a target is a library target
#
# This function checks if the specified target is a library target by inspecting the
# `TYPE` property of the target. It sets the result in the specified variable.
#
# Arguments:
# - target: The target to check
# - is_lib: The variable to store the result (TRUE or FALSE)
#
# Example usage:
# ```
# astronaught_is_lib(my_target is_lib)
# if (is_lib)
#     message("Target is a library")
# else()
#     message("Target is not a library")
# endif()
# ```
function(astronaught_is_lib target is_lib)
   get_target_property(type_prop ${target} TYPE)
   if (${type_prop} STREQUAL "INTERFACE_LIBRARY" OR
       ${type_prop} STREQUAL "STATIC_LIBRARY" OR
       ${type_prop} STREQUAL "SHARED_LIBRARY" OR
       ${type_prop} STREQUAL "MODULE_LIBRARY")
      set(${is_lib} TRUE PARENT_SCOPE)
   else()
      set(${is_lib} FALSE PARENT_SCOPE)
   endif()
endfunction()

# Function to retrieve include directories for a target
#
# This function retrieves the include directories for the specified target. It distinguishes
# between INTERFACE_LIBRARY targets and other targets, and stores the result in the specified variable.
#
# Arguments:
# - target: The target to retrieve include directories for
# - include_dirs: The variable to store the include directories
#
# Example usage:
# ```
# astronaught_include_dirs(my_target include_dirs)
# message("Include directories: ${include_dirs}")
# ```
function(astronaught_include_dirs target include_dirs)
   get_property(type_prop TARGET ${target} PROPERTY TYPE)
   if (${type_prop} STREQUAL "INTERFACE_LIBRARY")
     set(INCLUDE_DIRS INTERFACE_INCLUDE_DIRECTORIES)
   else()
     set(INCLUDE_DIRS INCLUDE_DIRECTORIES)
   endif()

   get_target_property(include_dirs ${target} ${INCLUDE_DIRS})
   if(include_dirs)
      list(REMOVE_DUPLICATES include_dirs)
      set(build_interface_dirs "")

      foreach(dir ${include_dirs})
         string(REGEX MATCH "\\$<BUILD_INTERFACE:[^>]*>" build_interface ${dir})
         string(REGEX MATCH "\\$<INSTALL_INTERFACE:[^>]*>" install_interface ${dir})
         if(build_interface)
            string(REGEX REPLACE "^\\$<BUILD_INTERFACE:(.*)>$" "\\1" build_interface ${build_interface})
            list(APPEND build_interface_dirs ${build_interface})
         elseif(NOT install_interface)
            list(APPEND build_interface_dirs ${dir})
         endif()
      endforeach()
   else()
      set(build_interface_dirs "")
   endif()
   set(${ARGV1} ${build_interface_dirs} PARENT_SCOPE)
endfunction()

# Function to retrieve project-wide include directories
#
# This function retrieves the project-wide include directories by traversing the directory structure
# and collecting the include directories from each library target. It excludes include directories
# that belong to dependencies. It stores the result in the specified variable.
#
# Arguments:
# - include_dirs: The variable to store the project-wide include directories
#
# Example usage:
# ```
# astronaught_project_include_dirs(project_include_dirs)
# message("Project include directories: ${project_include_dirs}")
# ```
function(astronaught_project_include_dirs include_dirs)
   set(dir_queue ${PROJECT_SOURCE_DIR})
   set(result)

   while(dir_queue)
      list(GET dir_queue 0 current_dir)
      list(REMOVE_AT dir_queue 0)

      get_property(subdirs DIRECTORY ${current_dir} PROPERTY SUBDIRECTORIES)
      list(APPEND dir_queue ${subdirs})

      get_property(sub_targets DIRECTORY ${current_dir} PROPERTY BUILDSYSTEM_TARGETS)
      list(REMOVE_DUPLICATES sub_targets)
      foreach(target ${sub_targets})
         astronaught_is_test(${target} is_test)
         astronaught_is_lib(${target} is_lib)
         if(NOT is_test AND is_lib)
            astronaught_include_dirs(${target} inc_dirs)
            if(inc_dirs)
               string(REGEX MATCH ".*\/_deps\/.*" is_dep ${inc_dirs})
            endif()
            if (NOT is_dep)
               list(APPEND result ${inc_dirs})
            endif()
         endif()
      endforeach()
   endwhile()
   list(REMOVE_DUPLICATES result)
   set(${include_dirs} ${result} PARENT_SCOPE)
endfunction()