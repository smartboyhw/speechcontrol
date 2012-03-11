project(SpeechControl-DesktopControl-MPRIS)
cmake_minimum_required(VERSION 2.8)

find_package(SpeechControlApplication REQUIRED QUIET)
include(SpeechControlMacros)

set(SPDSKMPRIS_PLUGIN_MINOR_VERSION 0)
set(SPDSKMPRIS_PLUGIN_MAJOR_VERSION 1)
set(SPDSKMPRIS_PLUGIN_UUID "d5585928-6b93-11e1-852d-325a03d6df59")
set(SPDSKMPRIS_PLUGIN_VERSION "${SPDSKMPRIS_PLUGIN_MAJOR_VERSION}.${SPDSKMPRIS_PLUGIN_MINOR_VERSION}")
set(SPDSKMPRIS_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_INCLUDE_DIRS})
set(SPDSKMPRIS_PLUGIN_LIBRARIES ${SPCHCNTRL_LIBRARIES})

file(GLOB SPDSKMPRIS_SRCS "src/*.cpp")
file(GLOB SPDSKMPRIS_HDRS "src/*.hpp")

QT4_AUTOMOC(${SPDSKMPRIS_SRCS})
configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)
include_directories(${SPDSKMPRIS_PLUGIN_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/src"
    "${SPCHCNTRL_PLUGIN_BUILD_INCLUDE_DIRS}")

add_library(spchcntrl-mpris SHARED ${SPDSKMPRIS_SRCS})
target_link_libraries(spchcntrl-mpris ${SPDSKMPRIS_PLUGIN_LIBRARIES})
set_target_properties(spchcntrl-mpris PROPERTIES
    DEFINE_SYMBOL "SPCH_EXPORT_SYMBOLS"
    VERSION "${SPDSKMPRIS_PLUGIN_VERSION}"
    SOVERSION "${SPDSKMPRIS_PLUGIN_VERSION}")
speechcontrol_install_plugin(spchcntrl-mpris)
add_dependencies(plugins spchcntrl-mpris)

install(FILES "${PROJECT_SOURCE_DIR}/data/d5585928-6b93-11e1-852d-325a03d6df59.spec"
        DESTINATION ${SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR})