project(SpeechControlLibrary)
cmake_minimum_required(VERSION 2.8)

## @todo Considering dropping QtGui support in this library.

## @todo Obtain the directory of the models from pkg-config
# PkgConfig to get model dir `pkg-config --variable=modeldir pocketsphinx`

## variables
set(SPCHCNTRL_LIB_INCLUDE_DIR "${SPCHCNTRL_INCLUDE_DIR}/lib")
set(SPCHCNTRL_LIB_INCLUDE_DIRS "${SPCHNCTRL_INCLUDE_DIRS}")
set(SPCHCNTRL_LIB_LIBRARIES "${SPCHCNTRL_LIBRARIES}")
set(SPCHCNTRL_LIB_MAJOR_VERSION 0)
set(SPCHCNTRL_LIB_MINOR_VERSION 1)
set(SPCHCNTRL_LIB_VERSION "${SPCHCNTRL_LIB_MAJOR_VERSION}.${SPCHCNTRL_LIB_MINOR_VERSION}")

## configures
configure_file("${SPCHCNTRL_CMAKE_SDIR}/SpeechControlLibraryConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfig.cmake" @ONLY)

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
    "${PROJECT_SOURCE_DIR}/config.hpp" @ONLY)

configure_file("${SPCHCNTRL_CMAKE_SDIR}/SpeechControlLibraryConfigVersion.cmake.in"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfigVersion.cmake" @ONLY)

## setting up sources
file(GLOB SPCHCNTRL_LIB_SOURCES "*.cpp")
file(GLOB SPCHCNTRL_LIB_HEADERS "*.hpp")
QT4_WRAP_CPP(SPCHCNTRL_LIB_SOURCES ${SPCHCNTRL_LIB_HEADERS} OPTIONS
    -DBOOST_TT_HAS_OPERATOR_HPP_INCLUDED)

add_library(spchcntrl SHARED ${SPCHCNTRL_LIB_SOURCES})

set_target_properties(spchcntrl PROPERTIES
    PROJECT_LABEL "libspchcntrl"
    VERSION ${SPCHCNTRL_LIB_VERSION}
    SOVERSION ${SPCHCNTRL_LIB_VERSION})

target_link_libraries(spchcntrl ${SPCHCNTRL_LIB_LIBRARIES})

install(TARGETS spchcntrl
    EXPORT SpeechControlLibraryDepends
    LIBRARY DESTINATION "${SPCHCNTRL_LIB_DIR}")

install(EXPORT SpeechControlLibraryDepends
    DESTINATION "${SPCHCNTRL_CMAKE_DIR}"
    COMPONENT "dev")

install(FILES ${SPCHCNTRL_LIB_HEADERS}
    DESTINATION "${SPCHCNTRL_LIB_INCLUDE_DIR}"
    COMPONENT "dev")

install(FILES "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfig.cmake"
    "${PROJECT_BINARY_DIR}/InstallFiles/SpeechControlLibraryConfigVersion.cmake"
    DESTINATION "${SPCHCNTRL_CMAKE_DIR}"
    COMPONENT "dev")