####
cmake_minimum_required(VERSION 3.3.2)

# where all libraries are
link_directories(${SS_BINARY_DIR}/libs)
link_directories(/usr/local/lib)

#BOOST libs
#set(libs_boost boost_chrono boost_system boost_program_options boost_unit_test_framework boost_regex)	
set(libs_boost boost_unit_test_framework boost_iostreams)	

set(libs_others pthread)

# list of executables to be built
file(GLOB TESTSRC "*.cpp")
add_executable(test1 ${TESTSRC})# src files it needs
target_link_libraries(test1 common ${libs_others} ${libs_boost})		# what libs it needs

