include_directories(${QT_INCLUDES} ${Qwt5_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(qfit_SRCS about.cpp qfit.cpp main.cpp)
qt4_automoc(${qfit_SRCS})

set(qfit_ui maindialog.ui)
qt4_wrap_ui(qfit_SRCS ${qfit_ui})

set(qfit_resources resources.qrc)
qt4_add_resources(qfit_SRCS ${qfit_resources})

add_executable(qfit ${qfit_SRCS})

target_link_libraries(qfit ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${Qwt6_Qt4_LIBRARY} fit)

install(TARGETS qfit DESTINATION bin)
