cmake_minimum_required(VERSION 3.20)
project(unsafeterrain)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, defaulting to debug...")
  set(CMAKE_BUILD_TYPE "DEBUG")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# setup external libraries
add_subdirectory(external/glfw)
add_subdirectory(external/glm)

include_directories(external/glfw/include)
include_directories(external/glad/include)
include_directories(external/glm)
include_directories(external)

include_directories(src)

file(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.cpp")

file(GLOB_RECURSE EXTERNAL_FILES "external/glad/*.c" "external/glad/*.h"
     "external/imgui/*.cpp" "external/imgui/*.h")

# Shader files
file(
  GLOB_RECURSE
  SHADER_FILES
  "shaders/*.frag"
  "shaders/*.vert"
  "shaders/*.geom"
  "shaders/*.comp"
  "shaders/*.glsl")

add_executable(main ${SOURCE_FILES} ${EXTERNAL_FILES} ${SHADER_FILES})

target_compile_options(
  main
  PRIVATE "-Wall"
          "-Wextra"
          "-std=c++23"
          "-pedantic"
          $<$<CONFIG:RELEASE>:-Werror>
          $<$<CONFIG:RELEASE>:-O3>
          $<$<CONFIG:DEBUG>:-g>
          $<$<CONFIG:DEBUG>:-O0>
          $<$<CONFIG:DEBUG>:-fno-inline>
          $<$<CONFIG:DEBUG>:-fsanitize=address>)

set_target_properties(main PROPERTIES CXX_EXTENSIONS OFF CXX_STANDARD 20)
target_link_libraries(main PRIVATE glfw asan)
