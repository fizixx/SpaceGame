cmake_minimum_required(VERSION 3.0)
project("SpaceGame")

# Dependencies

include("cmake/nucleus.cmake")
include("cmake/canvas.cmake")
include("cmake/elastic.cmake")
include("cmake/junctions.cmake")

# SpaceGame

include_directories("src")

file(GLOB_RECURSE "SOURCE_FILES" "src/*.cpp" "src/*.h")

add_executable("SpaceGame" WIN32 MACOSX_BUNDLE ${SOURCE_FILES})
target_link_libraries("SpaceGame" "canvas" "junctions" "elastic" "nucleus")

# tools/model_convert

add_executable("ModelConvert"
  "tools/model_convert/model_convert.cpp"
  "tools/model_convert/parser.cpp"
  "tools/model_convert/parser.h"
)
target_link_libraries("ModelConvert" "nucleus")
