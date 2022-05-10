cmake_minimum_required(VERSION 3.0)

project(ops VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set (LIB ${CMAKE_SOURCE_DIR}/libinfer.dylib)
else()
  if (CMAKE_C_COMPILER MATCHES "aarch64")
    set (LIB ${CMAKE_SOURCE_DIR}/libinfer.so)
  else()
    set (LIB ${CMAKE_SOURCE_DIR}/libmops.so)
    set (LIB2 ${CMAKE_SOURCE_DIR}/libinfer.so)
  endif()
endif()

# add_library(
#   infer SHARED
#   infer.hpp 
#   infer.cpp
# )

add_executable(main main.cpp)
# target_link_libraries(main PUBLIC infer)
target_link_libraries(main PUBLIC ${LIB2})
# the more element one should be placed in the bottom
target_link_libraries(main PUBLIC ${LIB})

