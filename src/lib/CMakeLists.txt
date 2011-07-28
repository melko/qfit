configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/fitconfig.h.in
                ${CMAKE_CURRENT_SOURCE_DIR}/fitconfig.h)

add_library(fit libfile.cpp log.cpp fittools.cpp)
