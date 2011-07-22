project(qfit)
cmake_minimum_required(VERSION 2.6)

set(QFIT_VERSION_MAJOR 0)
set(QFIT_VERSION_MINOR 0)

option(USE_GUI "Build the Qt interface" ON)

add_subdirectory(src)
