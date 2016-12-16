####
cmake_minimum_required(VERSION 3.3.2)

## To make cmake use custom GCC on MAC OSX
if(APPLE)
	message(STATUS "Compiling for MacOS")
	set(CMAKE_CXX_COMPILER /usr/local/Cellar/gcc5/5.4.0/bin/g++-5)
	set(CMAKE_C_COMPILER /usr/local/Cellar/gcc5/5.4.0/bin/gcc-5)
ENDIF()

project(SS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O0 -pg -g -std=c++11 -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O3 -std=c++11 -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/,,$(abspath $<))\"'")

# where all includes are
include_directories(${SS_SOURCE_DIR}/libs)
include_directories(SYSTEM /usr/local/include)

add_subdirectory(libs)
add_subdirectory(tests)
add_subdirectory(apps)

# UTs	
#enable_testing()
#add_test(mytest tests/test1 ${ARGS})
#$ make ut ARGS="--run_test=test_perf"
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

add_custom_target(paper 
		COMMAND ${CMAKE_COMMAND} -E create_symlink ${SS_SOURCE_DIR}/tests/testData ${SS_BINARY_DIR}/testData
		COMMAND ${SS_BINARY_DIR}/apps/trsys testData/paper.cfg \${ARGS}
		DEPENDS trsys
#		WORKING_DIRECTORY 
		COMMENT "Running paper trading... ${SS_BINARY_DIR}/apps/trsys testData/paper.cfg")