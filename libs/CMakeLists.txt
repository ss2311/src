####
cmake_minimum_required(VERSION 2.8)

file(GLOB LIBSRC "*.cpp" "logger" "misc" "fx")
add_library(common SHARED ${LIBSRC})

