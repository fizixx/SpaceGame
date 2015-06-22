
set(CANVAS_ROOT_DIR "" CACHE PATH "Canvas source root directory")
if(NOT CANVAS_ROOT_DIR)
  message(FATAL_ERROR "Canvas not found. Please set CANVAS_ROOT_DIR.")
endif()

add_subdirectory("${CANVAS_ROOT_DIR}" "canvas")
target_include_directories("canvas" PUBLIC "${CANVAS_SOURCE_DIR}/include")
