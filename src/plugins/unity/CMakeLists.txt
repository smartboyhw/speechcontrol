project(SpeechControl-Python)
cmake_minimum_required(VERSION 2.8)

find_package(SpeechControlApplication REQUIRED QUIET)
include(SpeechControlMacros)

set(SPHUD_PLUGIN_MINOR_VERSION 0)
set(SPHUD_PLUGIN_MAJOR_VERSION 1)
set(SPHUD_PLUGIN_UUID "508eb64e-6bb4-11e1-852d-020054746872")
set(SPHUD_PLUGIN_VERSION "${SPHUD_PLUGIN_MAJOR_VERSION}.${SPHUD_PLUGIN_MINOR_VERSION}")
set(SPHUD_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_INCLUDE_DIRS})
set(SPHUD_PLUGIN_LIBRARIES ${SPCHCNTRL_LIBRARIES})

file(GLOB SPHUD_SRCS "src/*.cpp")
file(GLOB SPHUD_HDRS "src/*.hpp")

QT4_AUTOMOC(${SPHUD_SRCS})

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)

include_directories(${SPHUD_PLUGIN_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/src"
    "${SPCHCNTRL_PLUGIN_BUILD_INCLUDE_DIRS}")

add_library(spchcntrl-unity SHARED ${SPHUD_SRCS})
target_link_libraries(spchcntrl-unity ${SPHUD_PLUGIN_LIBRARIES})
set_target_properties(spchcntrl-unity PROPERTIES
    DEFINE_SYMBOL "SPCH_EXPORT_SYMBOLS"
    VERSION "${SPHUD_PLUGIN_VERSION}"
    SOVERSION "${SPHUD_PLUGIN_VERSION}")
speechcontrol_install_plugin(spchcntrl-unity)
add_dependencies(plugins spchcntrl-unity)

install(FILES "${PROJECT_SOURCE_DIR}/data/${SPHUD_PLUGIN_UUID}.spec"
        DESTINATION ${SPCHCNTRL_PLUGINS_SPEC_INSTALL_DIR})