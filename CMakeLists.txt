# The name of our project is "HELLO". CMakeLists files in this project can 
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and 
# to the root binary directory of the project as ${HELLO_BINARY_DIR}. 
cmake_minimum_required (VERSION 2.6) 
project (siptricks) 

set(IncludeDir ${CMAKE_CURRENT_SOURCE_DIR}/../resiprocate .)
include_directories(${IncludeDir})

# Recurse into the "Hello" and "Demo" subdirectories. This does not actually 
# cause another cmake executable to run. The same process will walk through 
# the project's entire directory structure. 
add_subdirectory (helper)
add_subdirectory (siplayer)
add_subdirectory (b2bua)
add_subdirectory (bridge)
add_subdirectory (cnode)
add_subdirectory (softphone)
