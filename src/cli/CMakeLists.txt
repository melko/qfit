set(cfit_SRCS main.cpp)
add_executable(cfit ${cfit_SRCS})

target_link_libraries(cfit qfit::fit)

install(TARGETS cfit DESTINATION bin)
