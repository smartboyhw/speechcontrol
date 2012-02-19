project(SpeechControl)
cmake_minimum_required(VERSION 2.8)

## required packages
find_package(Doxygen)
find_package(PkgConfig REQUIRED)
find_package(Boost 1.4.6 COMPONENTS
  python
  REQUIRED)
find_package(PythonLibs 2.7 REQUIRED)
find_package(Qt4 4.7 COMPONENTS
  QtCore
  QtGui
  QtXml
  REQUIRED)
find_package(QtGStreamer REQUIRED)
pkg_search_module(POCKETSPHINX REQUIRED pocketsphinx sphinxbase)

EXEC_PROGRAM(${PKGCONFIG_EXECUTABLE} ARGS ${_package} --variable=modeldir
        OUTPUT_VARIABLE POCKETSPHINX_MODELDIR)

include("${QT_USE_FILE}")

## variables
set(SPCHCNTRL_INCLUDE_DIRS "${QT_INCLUDE_DIR}"
    ${POCKETSPHINX_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${QTGSTREAMER_INCLUDES})
set(SPCHCNTRL_LIBRARIES "${QT_LIBRARIES}"
    ${Boost_LIBRARIES}
    ${PYTHON_LIBRARIES}
    ${PHONON_LIBRARIES}
    ${QTGLIB_LIBRARIES}
    ${QTGSTREAMER_LIBRARIES}
    ${QTGSTREAMER_UI_LIBRARIES}
    ${QTGSTREAMER_UTILS_LIBRARIES}
    ${POCKETSPHINX_LIBRARIES})
set(SPCHCNTRL_CMAKE_SDIR "${PROJECT_SOURCE_DIR}/cmake")
set(SPCHCNTRL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/SpeechControl")
set(SPCHCNTRL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(SPCHCNTRL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(SPCHCNTRL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/spchcntrl")
set(PROJECT_VERSION "0.1")
add_definitions("-Wall -std=c++0x")

if(CMAKE_BUILD_TYPE EQUAL "Debug")
    set(Boost_USE_DEBUG_PYTHON ON)
endif()

## includes
include_directories(${SPCHCNTRL_INCLUDE_DIRS}
  "${PROJECT_SOURCE_DIR}/src")

add_subdirectory(src)
add_subdirectory(ui)

if(DOXYGEN_FOUND)
    find_program(HAVE_DOT dot)
    if(HAVE_DOT)
        message("Found 'dot' program, Doxygen will use it to generate graphs for documentation.")
        set(HAVE_DOT YES)
    else(HAVE_DOT)
        set(HAVE_DOT NO)
    endif(HAVE_DOT)
    configure_file("${PROJECT_SOURCE_DIR}/Doxyfile.in" "${PROJECT_BINARY_DIR}/Doxyfile")
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} Doxyfile
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
        COMMENT "Generating API documentation with Doxygen...")

    install(DIRECTORY "${PROJECT_SOURCE_DIR}/doc/"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libspeechcontrol"
            COMPONENT "doc")
endif()