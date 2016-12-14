####
cmake_minimum_required(VERSION 3.3.2)

# where all includes are
include_directories(${SS_SOURCE_DIR}/libs)
include_directories(SYSTEM /usr/local/include)

# where all libraries are
link_directories(${SS_BINARY_DIR}/libs)
link_directories(/usr/local/lib)

#BOOST libs
#set(libs_boost boost_system boost_program_options boost_regex)	
#set(libs_others pthread)

# list of executables to be built
file(GLOB TRSYS "tradingsystem/*.cpp")
add_executable(trsys ${TRSYS})# src files it needs
target_link_libraries(trsys common ${libs_others} ${libs_boost})		# what libs it needs

