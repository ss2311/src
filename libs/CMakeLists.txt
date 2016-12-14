####
cmake_minimum_required(VERSION 3.3.2)

file(GLOB_RECURSE LIBSRC "*.cpp" )
add_library(common SHARED ${LIBSRC})

