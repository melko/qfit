# Find Qwt
# ~~~~~~~~
# Copyright (c) 2010, Tim Sutton <tim at linfiniti.com>
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
# Once run this will define:
#
# Qwt_FOUND          = system has Qwt lib
# Qwt_LIBRARY        = full path to the Qwt library
# Qwt_INCLUDE_DIR    = where to find headers
# Qwt_VERSION_STRING = version string from Qwt header file
# Qwt_VERSION        = version string (for compatibility with meson)
#


set(Qwt_LIBRARY_NAMES qwt-qt5 qwt6-qt5 qwt qwt6)

find_library(Qwt_LIBRARY
  NAMES ${Qwt_LIBRARY_NAMES}
  PATHS
    /usr/lib
    /usr/local/lib
    /usr/local/lib/qt5
    "$ENV{LIB_DIR}/lib"
    "$ENV{LIB}"
)

set(_qwt_fw)
if(Qwt_LIBRARY MATCHES "/qwt.*\\.framework")
  string(REGEX REPLACE "^(.*/qwt.*\\.framework).*$" "\\1" _qwt_fw "${Qwt_LIBRARY}")
endif()

FIND_PATH(Qwt_INCLUDE_DIR NAMES qwt.h PATHS
  "${_qwt_fw}/Headers"
  /usr/include
  /usr/include/qt5
  /usr/local/include
  /usr/local/include/qt5
  "$ENV{LIB_DIR}/include"
  "$ENV{INCLUDE}"
  PATH_SUFFIXES qwt-qt5 qwt qwt6
)

IF (Qwt_INCLUDE_DIR)
  FILE(READ ${Qwt_INCLUDE_DIR}/qwt_global.h qwt_header)
  STRING(REGEX REPLACE "^.*QWT_VERSION_STR +\"([^\"]+)\".*$" "\\1" Qwt_VERSION_STRING "${qwt_header}")
  SET(Qwt_VERSION "${Qwt_VERSION_STRING}")
ENDIF (Qwt_INCLUDE_DIR)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Qwt
  REQUIRED_VARS Qwt_INCLUDE_DIR Qwt_LIBRARY
  VERSION_VAR Qwt_VERSION_STRING
  )

IF (Qwt_FOUND AND NOT TARGET qwt::qwt)
    ADD_LIBRARY(qwt::qwt UNKNOWN IMPORTED)
    SET_TARGET_PROPERTIES(qwt::qwt PROPERTIES
      INTERFACE_INCLUDE_DIRECTORIES "${Qwt_INCLUDE_DIR}"
      IMPORTED_LOCATION "${Qwt_LIBRARY}"
      )
ENDIF (Qwt_FOUND AND NOT TARGET qwt::qwt)
