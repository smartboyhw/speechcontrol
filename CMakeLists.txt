project(SpeechControl)
cmake_minimum_required(VERSION 2.8)

## Let's use our own modules as well.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(SpeechControlDefaults)

## Options to toggle components of SpeechControl.
option(WITH_DOCS               "Generate Doxygen-made documentation?"       ON)
option(WITH_KDE_SUPPORT        "Build KDE specific features?"               ON)
option(WITH_PYTHON_BINDINGS    "Generate Python support for SpeechControl?" ON)
option(WITH_PLUGINS            "Build the included plug-ins?"               ON)
option(WITH_TESTS              "Generate the test build system?"            ON)

## required packages
find_package(PkgConfig REQUIRED)
find_package(Xdo REQUIRED)
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
set(PROJECT_VERSION "0.1")
set(SPCHCNTRL_CMAKE_SDIR "${PROJECT_SOURCE_DIR}/cmake")
set(SPCHCNTRL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/SpeechControl")
set(SPCHCNTRL_CMAKE_INSTALL_DIR "${CMAKE_MODULE_PATH}")
set(SPCHCNTRL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(SPCHCNTRL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(SPCHCNTRL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/speechcontrol")
set(SPCHCNTRL_INCLUDE_DIRS ${QT_INCLUDE_DIR}
    ${SPCHCNTRL_INCLUDE_DIR}
    ${XDO_INCLUDE_DIRS}
    ${POCKETSPHINX_INCLUDE_DIRS}
    ${QTGSTREAMER_INCLUDE_DIR})
set(SPCHCNTRL_LIBRARIES ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QTGLIB_LIBRARIES}
    ${XDO_LIBRARIES}
    ${QTGSTREAMER_LIBRARIES}
    ${QTGSTREAMER_UI_LIBRARIES}
    ${QTGSTREAMER_UTILS_LIBRARIES}
    ${POCKETSPHINX_LIBRARIES})
set(SPCHCNTRL_COMPILE_FLAGS "${QTGSTREAMER_FLAGS} -fexceptions -feliminate-unused-debug-symbols")
set(SPCHCNTRL_COMPILE_FLAGS_DEBUG "-Wall -Wextra -Wabi -Wctor-dtor-privacy -Wformat -Wunused")
set(SPCHCNTRL_COMPILE_DEFINITIONS ${QT_DEFINITIONS})
set(SPCHCNTRL_QRESC "${CMAKE_SOURCE_DIR}/res/res.qrc")
file(GLOB SPCHCNTRL_UI "res/ui/*.ui")


## Now we'll add in the Qt stuff
include("${QT_USE_FILE}")

## The subdirectories, order's vital here.
add_subdirectory(res)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(doc)
add_subdirectory(cmake)