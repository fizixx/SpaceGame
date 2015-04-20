cmake_minimum_required(VERSION 3.0)
project("SpaceGame")

# General settings

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/out")

# Compiler settings

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# nucleus
include("cmake/nucleus.cmake")

# SFML
include("cmake/sfml.cmake")

# SpaceGame

include_directories("src")

file(GLOB_RECURSE "SOURCE_FILES" "src/*.cpp" "src/*.h")

add_executable("SpaceGame" WIN32 MACOSX_BUNDLE ${SOURCE_FILES})
target_link_libraries("SpaceGame" "sfml-graphics" "nucleus")
if(WIN32)
  target_link_libraries("SpaceGame" "sfml-main")
endif()

# tools/model_convert

add_executable("ModelConvert"
  "tools/model_convert/model_convert.cpp"
  "tools/model_convert/parser.cpp"
  "tools/model_convert/parser.h"
)
target_link_libraries("ModelConvert" "nucleus")
