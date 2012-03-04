project(SpeechControl-Transcriber)
cmake_minimum_required(VERSION 2.8)

find_package(SpeechControlApplication REQUIRED QUIET)
include(SpeechControlMacros)

set(SPTRSB_PLUGIN_MINOR_VERSION 0)
set(SPTRSB_PLUGIN_MAJOR_VERSION 1)
set(SPTRSB_PLUGIN_UUID "c6aae83e-6529-11e1-852d-bdd49ac4649b")
set(SPTRSB_PLUGIN_VERSION "${SPTRSB_PLUGIN_MAJOR_VERSION}.${SPTRSB_PLUGIN_MINOR_VERSION}")
set(SPTRSB_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_INCLUDE_DIRS})
set(SPTRSB_PLUGIN_LIBRARIES ${SPCHCNTRL_LIBRARIES})

file(GLOB SPTRSB_UI "ui/*.ui")
file(GLOB SPTRSB_QRES "ui/*.qrc")
file(GLOB SPTRSB_SRCS "src/*.cpp")
file(GLOB SPTRSB_HDRS "src/*.hpp")

QT4_AUTOMOC(${SPTRSB_SRCS})
QT4_WRAP_UI(SPTRSB_SRCS ${SPTRSB_UI})
QT4_ADD_RESOURCES(SPTRSB_SRCS ${SPTRSB_QRESC})

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)

include_directories(${SPTRSB_PLUGIN_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/../../"
    "${PROJECT_BINARY_DIR}")

add_library(spchcntrl-transcriber SHARED ${SPTRSB_SRCS})
target_link_libraries(spchcntrl-transcriber ${SPTRSB_PLUGIN_LIBRARIES})
speechcontrol_install_plugin(spchcntrl-transcriber)
add_dependencies(plugins spchcntrl-transcriber)

install(FILES "${PROJECT_SOURCE_DIR}/data/c6aae83e-6529-11e1-852d-bdd49ac4649b.spec"
        DESTINATION ${SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR})