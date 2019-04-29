cmake_minimum_required(VERSION 3.0)

project(qfit VERSION 0.3.1)

add_compile_options(-Wall -Wextra)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include(FeatureSummary)
option(WITH_GUI "Build the Qt interface" ON)
option(WITH_QWT "Build GUI with the plot Widget" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules")

find_package(Qt5 COMPONENTS Widgets)
set_package_properties(Qt5 PROPERTIES
	TYPE RECOMMENDED
	PURPOSE "Required for the GUI")
if(NOT TARGET Qt5::Widgets)
	set(WITH_GUI OFF)
endif(NOT TARGET Qt5::Widgets)

find_package(Qwt)
set_package_properties(Qwt PROPERTIES
	TYPE OPTIONAL
	PURPOSE "Required for the plotting widget")
if(NOT TARGET qwt::qwt)
	set(WITH_QWT OFF)
endif(NOT TARGET qwt::qwt)

add_subdirectory(src)

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)


# CPack vars
set(CPACK_PACKAGE_VENDOR "qfit development team")
set(CPACK_GENERATOR STGZ TBZ2)
set(CPACK_SOURCE_GENERATOR TBZ2)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "qfit-${PROJECT_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "qfit-${PROJECT_VERSION}-x86_64-linux")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README)

set(CPACK_SOURCE_IGNORE_FILES
	".git*"
	".kdev4"
	"qfit.kdev4"
	"${CMAKE_CURRENT_SOURCE_DIR}/build")

include(CPack)
