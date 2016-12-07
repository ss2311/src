####
cmake_minimum_required(VERSION 2.8)

## To make cmake use custom GCC on MAC OSX
set(CMAKE_CXX_COMPILER /usr/local/Cellar/gcc5/5.4.0/bin/g++-5)
set(CMAKE_C_COMPILER /usr/local/Cellar/gcc5/5.4.0/bin/gcc-5)

project(SS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O0 -pg -g -std=c++11 -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O3 -std=c++11 -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
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
