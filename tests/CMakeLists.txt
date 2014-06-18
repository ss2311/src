####
cmake_minimum_required(VERSION 2.8)

# where all includes are
include_directories(${SS_SOURCE_DIR}/libs)

# where all libraries are
link_directories(${SS_BINARY_DIR}/libs)

#BOOST libs
set(libs_boost boost_chrono boost_system boost_program_options boost_unit_test_framework boost_regex)	

# list of executables to be built
file(GLOB TESTSRC "*.cpp")
add_executable(test1 ${TESTSRC})# src files it needs
target_link_libraries(test1 common ${libs_boost})		# what libs it needs

