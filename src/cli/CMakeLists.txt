add_executable(cfit main.cpp)

target_link_libraries(cfit qfit::fit)

install(TARGETS cfit DESTINATION bin)
