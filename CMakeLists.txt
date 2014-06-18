####
cmake_minimum_required(VERSION 2.8)

project(SS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O0 -g -std=c++11 -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
add_subdirectory(libs)
add_subdirectory(tests)

