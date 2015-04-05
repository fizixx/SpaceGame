cmake_minimum_required(VERSION 3.0)
project("SpaceGame")

# General settings

set("CMAKE_RUNTIME_OUTPUT_DIRECTORY" "${PROJECT_BINARY_DIR}/out")

include("cmake/sfml.cmake")
include("cmake/nucleus.cmake")

# SpaceGame

file(GLOB_RECURSE "SOURCE_FILES" "src/*.cpp" "src/*.h")

add_executable("SpaceGame" WIN32 ${SOURCE_FILES})
target_link_libraries("SpaceGame" "sfml-main" "sfml-graphics" "base")
