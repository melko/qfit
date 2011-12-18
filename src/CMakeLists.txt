include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(lib)
add_subdirectory(cli)

if(WITH_GUI AND QT4_FOUND)
add_subdirectory(gui)
endif(WITH_GUI AND QT4_FOUND)
