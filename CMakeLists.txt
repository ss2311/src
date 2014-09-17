####
cmake_minimum_required(VERSION 2.8)

project(SS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O0 -pg -g -std=c++11 -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
add_subdirectory(libs)
add_subdirectory(tests)

# UTs	
#enable_testing()
#add_test(mytest tests/test1 ${ARGS})

add_custom_target(ut 
		COMMAND ${SS_BINARY_DIR}/tests/test1 \${ARGS}
		DEPENDS test1
#		WORKING_DIRECTORY 
		COMMENT "Running unit tests...")

add_custom_target(gdb 
		COMMAND gdb --args ${SS_BINARY_DIR}/tests/test1 \${ARGS}
		DEPENDS test1
#		WORKING_DIRECTORY 
		COMMENT "Running unit tests inside gdb ...")
