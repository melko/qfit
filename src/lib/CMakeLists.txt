configure_file(fitconfig.h.in
               fitconfig.h)

add_library(fit libfile.cpp log.cpp fittools.cpp)
add_library(qfit::fit ALIAS fit)
target_include_directories(fit PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(fit INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
