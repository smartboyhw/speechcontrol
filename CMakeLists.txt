project(SpeechControl)
cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

## Options to toggle components of SpeechControl.
## By default, they're all off, just to speed up build times :)
option(WITH_PYTHON_BINDINGS    "Generate Python support for SpeechControl?" OFF)
option(WITH_PLUGINS            "Build the included plug-ins?"               OFF)
option(WITH_DOCS               "Generate Doxygen-made documentation?"       OFF)
option(WITH_TESTS              "Generate the test build system?"            OFF)

## Required packages
find_package(PkgConfig 0.2 REQUIRED)
find_package(Qt4 4.7 COMPONENTS
  QtCore
  QtGui
  QtDBus
  QtXml
  QtDeclarative
  REQUIRED)
find_package(QtGStreamer REQUIRED)
find_package(SphinxBase 0.7 REQUIRED)
find_package(PocketSphinx 0.7 REQUIRED)

include(SpeechControlDefaults)
include(SpeechControlMacros)

## variables
set(PROJECT_VERSION "0.1")
set(SPCHCNTRL_BUILDING ON)
set(SPCHCNTRL_CMAKE_SDIR "${PROJECT_SOURCE_DIR}/cmake")
set(SPCHCNTRL_INCLUDE_DIRS ${QT_INCLUDE_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTXML_INCLUDE_DIR}
    ${SPCHCNTRL_INCLUDE_DIR}
    ${POCKETSPHINX_INCLUDE_DIRS}
    ${QTGSTREAMER_INCLUDE_DIR}
    "${QTGSTREAMER_INCLUDE_DIR}/QGst"
    ${QTGSTREAMER_INCLUDES})
set(SPCHCNTRL_BUILD_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_BINARY_DIR}/src")
set(SPCHCNTRL_LIBRARIES ${QT_QTCORE_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${QTGLIB_LIBRARIES}
    ${QTGSTREAMER_LIBRARIES}
    ${QTGSTREAMER_UI_LIBRARIES}
    ${QTGSTREAMER_UTILS_LIBRARIES}
    ${POCKETSPHINX_LIBRARIES})

## The subdirectories, order's vital here.
add_subdirectory(cmake)
add_subdirectory(src)
add_subdirectory(res)

if(WITH_TESTS)
    add_subdirectory(tests)
endif()

if(WITH_DOCS)
    add_subdirectory(doc)
endif()
