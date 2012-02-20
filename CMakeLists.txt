cmake_minimum_required(VERSION 2.8)
project(SpeechControl)

set(PROJECT_VERSION "0.1")

## Let's use our own modules as well.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

## Options to toggle components of SpeechControl.
option(WITH_PYTHON_BINDINGS    "Generate Python support for SpeechControl?" ON)
option(WITH_DBUS               "Add D-Bus support to SpeechControl?"        ON)
option(WITH_API_DOCS           "Generate Doxygen-made API documentation?"   ON)
option(WITH_KDE                "Build this application for KDE?"            OFF)
option(WITH_TESTS              "Generate the test build system?"            OFF)

## required packages
find_package(PkgConfig REQUIRED)
find_package(Qt4 4.7 COMPONENTS
  QtCore
  QtGui
  QtXml
  REQUIRED)
find_package(QtGStreamer REQUIRED)
pkg_search_module(POCKETSPHINX REQUIRED pocketsphinx sphinxbase)

execute_process(COMMAND ${PKGCONFIG_EXECUTABLE} pocketsphinx --variable=modeldir RESULT_VARIABLE POCKETSPHINX_MODELDIR)

include("${QT_USE_FILE}")

## variables
set(SPCHCNTRL_INCLUDE_DIRS ${QT_INCLUDE_DIR}
    ${POCKETSPHINX_INCLUDE_DIRS}
    ${QTGSTREAMER_INCLUDE_DIR})
set(SPCHCNTRL_LIBRARIES ${QT_LIBRARIES}
    ${QTGLIB_LIBRARIES}
    ${QTGSTREAMER_LIBRARIES}
    ${QTGSTREAMER_UI_LIBRARIES}
    ${QTGSTREAMER_UTILS_LIBRARIES}
    ${POCKETSPHINX_LIBRARIES})
set(SPCHCNTRL_CMAKE_SDIR "${PROJECT_SOURCE_DIR}/cmake")
set(SPCHCNTRL_CMAKE_DIR "${CMAKE_INSTALL_PREFIX}/share/cmake/SpeechControl")
set(SPCHCNTRL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(SPCHCNTRL_BIN_DIR "${CMAKE_INSTALL_PREFIX}/bin")
set(SPCHCNTRL_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include/speechcontrol")
set(SPCHCNTRL_COMPILE_FLAGS "-Wall"
    "-std=c++0x"
    ${QTGSTREAMER_FLAGS})
set(SPCHCNTRL_COMPILE_DEFINITIONS )

## The subdirectories :)
add_subdirectory(interfaces)
add_subdirectory(cmake)
add_subdirectory(ui)
add_subdirectory(src)
add_subdirectory(tests)

## Got docs? :D
if (WITH_API_DOCS)
    set(DOXYGEN_SKIP_DOT NO)
    find_package(Doxygen REQUIRED)
    if(DOXYGEN_FOUND)
        find_program(HAVE_DOT dot)

        if(HAVE_DOT)
            message(STATUS "Found 'dot' program, Doxygen will use it to generate graphs for documentation.")
            set(HAVE_DOT YES)
        else(HAVE_DOT)
            set(HAVE_DOT NO)
        endif(HAVE_DOT)

        configure_file("${PROJECT_SOURCE_DIR}/Doxyfile.in" "${PROJECT_BINARY_DIR}/Doxyfile")
        add_custom_target(doc-api ALL
            ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
            COMMENT "Generating API documentation with Doxygen (output at ${PROJECT_BINARY_DIR}/Doxygen.output)...")

        install(DIRECTORY "${PROJECT_SOURCE_DIR}/doc/"
                DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/libspeechcontrol"
                COMPONENT "doc")
        message(STATUS "API documentation will be generated.")
    endif(DOXYGEN_FOUND)
else(WITH_API_DOCS)
    message(STATUS "No documentation will be generated.")
endif(WITH_API_DOCS)