include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(qfit_SRCS about.cpp qfit.cpp main.cpp)
qt4_automoc(${qfit_SRCS})

if(WITH_QWT AND Qwt_VERSION VERSION_LESS 6.0.0)
	MESSAGE(FATAL_ERROR "Your Qwt version is too old (you have ${Qwt_VERSION} but at least 6.0.0 is needed), please upgrade or set the WITH_QWT flag to false")
endif(WITH_QWT AND Qwt_VERSION VERSION_LESS 6.0.0)

if(WITH_QWT)
	include_directories(${Qwt6_INCLUDE_DIR})
	set(qfit_ui maindialog.ui)
else(WITH_QWT)
	set(qfit_ui maindialog_noqwt.ui)
	message(WARNING "QFit will be built without the plot widget")
endif(WITH_QWT)

qt4_wrap_ui(qfit_SRCS ${qfit_ui})

set(qfit_resources resources.qrc)
qt4_add_resources(qfit_SRCS ${qfit_resources})

add_executable(qfit ${qfit_SRCS})

target_link_libraries(qfit ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${Qwt6_Qt4_LIBRARY} fit)

install(TARGETS qfit DESTINATION bin)
install(FILES qfit.desktop DESTINATION share/applications)
install(FILES qfit.png DESTINATION share/pixmaps)
