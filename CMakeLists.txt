cmake_minimum_required(VERSION 3.0)
project("SpaceGame")

# Dependencies

include("cmake/nucleus.cmake")
include("cmake/canvas.cmake")
include("cmake/elastic.cmake")
include("cmake/junctions.cmake")

# Options

if(NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
endif()

# SpaceGame

include_directories("src")

file(GLOB_RECURSE "SOURCE_FILES" "src/*.cpp" "src/*.h")

add_executable("SpaceGame" WIN32 MACOSX_BUNDLE ${SOURCE_FILES})
target_link_libraries("SpaceGame" "canvas" "junctions" "elastic" "nucleus")

if(APPLE)
  find_library(OSX_COREFOUNDATION_LIBRARY CoreFoundation)
  find_library(OSX_COREGRAPHICS_LIBRARY CoreGraphics)
  find_library(OSX_COREVIDEO_LIBRARY CoreVideo)
  find_library(OSX_IOKIT_LIBRARY IOKit)
  find_library(OSX_APPKIT_LIBRARY AppKit)
  target_link_libraries("SpaceGame"
    ${OSX_COREFOUNDATION_LIBRARY}
    ${OSX_COREGRAPHICS_LIBRARY}
    ${OSX_COREVIDEO_LIBRARY}
    ${OSX_IOKIT_LIBRARY}
    ${OSX_APPKIT_LIBRARY}
  )
endif()
