include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

find_package(Qt4 REQUIRED)

set(QFIT_VERSION_MAJOR 0)
set(QFIT_VERSION_MINOR 0)

set(qfit_SRCS qfit.cpp main.cpp)
qt4_automoc(${qfit_SRCS})
add_executable(qfit ${qfit_SRCS})

target_link_libraries(qfit ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} fit)
