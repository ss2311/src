####
cmake_minimum_required(VERSION 2.8)

file(GLOB LIBSRC "*.cpp" "logger/*.cpp")
add_library(common SHARED ${LIBSRC})

