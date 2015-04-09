cmake_minimum_required(VERSION 3.0)
project("SpaceGame")

# General settings

set("CMAKE_RUNTIME_OUTPUT_DIRECTORY" "${PROJECT_BINARY_DIR}/out")

include("cmake/sfml.cmake")
include("cmake/nucleus.cmake")

# SpaceGame

include_directories("src")

file(GLOB_RECURSE "SOURCE_FILES" "src/*.cpp" "src/*.h")

add_executable("SpaceGame" WIN32 ${SOURCE_FILES})
target_link_libraries("SpaceGame" "sfml-main" "sfml-graphics" "nucleus")

# tools/model_convert

add_executable("ModelConvert"
  "tools/model_convert/model_convert.cpp"
  "tools/model_convert/parser.cpp"
  "tools/model_convert/parser.h"
)
target_link_libraries("ModelConvert" "nucleus")
