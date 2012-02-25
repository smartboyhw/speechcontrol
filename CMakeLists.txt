cmake_minimum_required(VERSION 2.8)
project(SpeechControl)

set(PROJECT_VERSION "0.1")

## Let's use our own modules as well.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

## Options to toggle components of SpeechControl.
option(WITH_PYTHON_BINDINGS    "Generate Python support for SpeechControl?" ON)
option(WITH_API_DOCS           "Generate Doxygen-made API documentation?"   ON)
option(WITH_PLUGINS            "Build the included plug-ins?"               ON)
option(WITH_TESTS              "Generate the test build system?"            OFF)
option(WITH_KDE_SUPPORT        "Build another application for KDE?"         OFF)

## required packages
find_package(PkgConfig REQUIRED)
find_package(Qt4 4.7 COMPONENTS
  QtCore
  QtGui
  QtDBus
  QtXml
  REQUIRED)
find_package(QtGStreamer REQUIRED)
pkg_check_modules(POCKETSPHINX REQUIRED pocketsphinx sphinxbase)

execute_process(COMMAND ${PKGCONFIG_EXECUTABLE} pocketsphinx --variable=modeldir RESULT_VARIABLE POCKETSPHINX_MODELDIR)

## variables
set(SPCHCNTRL_CMAKE_SDIR "${PROJECT_SOURCE_DIR}/cmake")
set(SPCHCNTRL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/SpeechControl")
set(SPCHCNTRL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(SPCHCNTRL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(SPCHCNTRL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/speechcontrol")
set(SPCHCNTRL_INCLUDE_DIRS ${QT_INCLUDE_DIR}
    ${SPCHCNTRL_INCLUDE_DIR}
    ${POCKETSPHINX_INCLUDE_DIRS}
    ${QTGSTREAMER_INCLUDE_DIR})
set(SPCHCNTRL_LIBRARIES ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QTGLIB_LIBRARIES}
    ${QTGSTREAMER_LIBRARIES}
    ${QTGSTREAMER_UI_LIBRARIES}
    ${QTGSTREAMER_UTILS_LIBRARIES}
    ${POCKETSPHINX_LIBRARIES})
set(SPCHCNTRL_COMPILE_FLAGS "-Wall -std=c++0x ${QTGSTREAMER_FLAGS} -fexceptions")
set(SPCHCNTRL_COMPILE_DEFINITIONS ${QT_DEFINITIONS})
set(SPCHCNTRL_QRESC )
file(GLOB SPCHCNTRL_UI "res/ui/*.ui")

## Now we'll add in the Qt stuff
include("${QT_USE_FILE}")

## The subdirectories, order's vital here.
add_subdirectory(doc)
add_subdirectory(res)
add_subdirectory(src)
add_subdirectory(tests)