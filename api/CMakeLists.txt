cmake_minimum_required(VERSION 3.0)

project(ops VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set (LIB ${CMAKE_SOURCE_DIR}/libinfer.dylib)
else()
  if (CMAKE_C_COMPILER MATCHES "aarch64")
    set (LIB ${CMAKE_SOURCE_DIR}/libinfer.so)
  else()
    # x86_64
    set (LIB ${CMAKE_SOURCE_DIR}/libmops.so)
    set (LIB2 ${CMAKE_SOURCE_DIR}/libinfer.so)
    set (PINC /usr/include/python3.6)
  endif()
endif()

add_executable(main main.cpp)
target_include_directories(main PUBLIC ${PINC})
target_link_libraries(main PUBLIC ${LIB2})
target_link_libraries(main PUBLIC ${LIB})

