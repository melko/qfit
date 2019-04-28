configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/fitconfig.h.in
                ${CMAKE_CURRENT_SOURCE_DIR}/fitconfig.h)

add_library(fit libfile.cpp log.cpp fittools.cpp)
add_library(qfit::fit ALIAS fit)
target_include_directories(fit INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
