cmake_minimum_required(VERSION 2.6)

project(qfit)

set(QFIT_VERSION_MAJOR 0)
set(QFIT_VERSION_MINOR 0)

option(USE_GUI "Build the Qt interface" ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(lib)
#add_subdirectory(cli)

#option(USE_GUI "Build the Qt interface" ON)
if(USE_GUI)
add_subdirectory(gui)
endif(USE_GUI)
