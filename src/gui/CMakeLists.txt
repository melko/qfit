set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(WITH_QWT AND Qwt_VERSION_STRING VERSION_LESS 6.0.0)
  MESSAGE(FATAL_ERROR "Your Qwt version is too old (you have ${Qwt_VERSION_STRING} but at least 6.0.0 is needed), please upgrade or set the WITH_QWT flag to false")
endif(WITH_QWT AND Qwt_VERSION_STRING VERSION_LESS 6.0.0)

if(WITH_QWT)
  set(qfit_ui maindialog.ui)
else(WITH_QWT)
  set(qfit_ui maindialog_noqwt.ui)
  message(WARNING "QFit will be built without the plot widget")
endif(WITH_QWT)

set(qfit_resources resources.qrc)

add_executable(qfit
               about.cpp qfit.cpp main.cpp
               ${qfit_ui} ${qfit_resources})
target_link_libraries(qfit Qt5::Widgets qfit::fit)

if(WITH_QWT)
  target_compile_definitions(qfit PRIVATE WITH_QWT)
  target_link_libraries(qfit qwt::qwt)
endif(WITH_QWT)

install(TARGETS qfit DESTINATION bin)
install(FILES qfit.desktop DESTINATION share/applications)
install(FILES qfit.png DESTINATION share/pixmaps)
