project(SpeechControl)
cmake_minimum_required(VERSION 2.8)

## Let's use our own modules to find some softwares not packed with the system.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
include(SpeechControlDefaults)
include(SpeechControlMacros)

## Options to toggle components of SpeechControl.
option(WITH_KDE_SUPPORT        "Build KDE specific features?"               ON)
option(WITH_PYTHON_BINDINGS    "Generate Python support for SpeechControl?" ON)
option(WITH_PLUGINS            "Build the included plug-ins?"               ON)
option(WITH_DOCS               "Generate Doxygen-made documentation?"       OFF)
option(WITH_TESTS              "Generate the test build system?"            OFF)

## Required packages
find_package(PkgConfig 0.2 REQUIRED)
find_package(Qt4 4.7 COMPONENTS
  QtCore
  QtGui
  QtDBus
  QtXml
  REQUIRED)
find_package(QtGStreamer REQUIRED)
find_package(SphinxBase 0.7 REQUIRED)
find_package(PocketSphinx 0.7 REQUIRED)

## Now we'll add in the Qt stuff
include("${QT_USE_FILE}")

## variables
set(PROJECT_VERSION "0.1")
set(SPCHCNTRL_BUILDING ON)
set(SPCHCNTRL_CMAKE_SDIR "${PROJECT_SOURCE_DIR}/cmake")
set(SPCHCNTRL_INCLUDE_DIRS ${QT_INCLUDE_DIR}
    ${SPCHCNTRL_INCLUDE_DIR}
    ${POCKETSPHINX_INCLUDE_DIRS}
    ${QTGSTREAMER_INCLUDE_DIR}
    "${QTGSTREAMER_INCLUDE_DIR}/QGst"
    ${QTGSTREAMER_INCLUDES})
set(SPCHCNTRL_LIBRARIES ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QTGLIB_LIBRARIES}
    ${QTGSTREAMER_LIBRARIES}
    ${QTGSTREAMER_UI_LIBRARIES}
    ${QTGSTREAMER_UTILS_LIBRARIES}
    ${POCKETSPHINX_LIBRARIES})
set(SPCHCNTRL_QRESC "${CMAKE_SOURCE_DIR}/res/res.qrc")
file(GLOB SPCHCNTRL_UI "res/ui/*.ui")

## The subdirectories, order's vital here.
add_subdirectory(res)
add_subdirectory(src)
add_subdirectory(cmake)

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

if(WITH_DOCS)
    add_subdirectory(doc)
endif()