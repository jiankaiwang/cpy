cmake_minimum_required(VERSION 3.0)

project(ops VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)

add_library(
  mops SHARED
  mops.hpp 
  mops.cpp
)

add_executable(libs libs.cpp)
target_link_libraries(libs PUBLIC mops)

