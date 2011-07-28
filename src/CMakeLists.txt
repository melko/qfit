include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(lib)
add_subdirectory(cli)

#option(USE_GUI "Build the Qt interface" ON)
if(USE_GUI)
add_subdirectory(gui)
endif(USE_GUI)
