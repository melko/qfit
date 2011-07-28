include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(cfit_SRCS main.cpp)
add_executable(cfit ${cfit_SRCS})

target_link_libraries(cfit fit)

install(TARGETS cfit DESTINATION bin)
