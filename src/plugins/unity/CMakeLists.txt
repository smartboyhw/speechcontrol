project(SpeechControl-Unity)
cmake_minimum_required(VERSION 2.8)

find_package(SpeechControlApplication REQUIRED QUIET)
include(SpeechControlMacros)

set(SPUNITY_PLUGIN_MINOR_VERSION 1)
set(SPUNITY_PLUGIN_MAJOR_VERSION 0)
set(SPUNITY_PLUGIN_ID "508eb64e")
set(SPUNITY_PLUGIN_VERSION "${SPUNITY_PLUGIN_MAJOR_VERSION}.${SPUNITY_PLUGIN_MINOR_VERSION}")
set(SPUNITY_PLUGIN_INCLUDE_DIRS ${SPCHCNTRL_INCLUDE_DIRS})
set(SPUNITY_PLUGIN_LIBRARIES ${SPCHCNTRL_LIBRARIES})

file(GLOB SPUNITY_SRCS "src/*.cpp")
file(GLOB SPUNITY_HDRS "src/*.hpp")

QT4_AUTOMOC(${SPUNITY_SRCS})

configure_file("${PROJECT_SOURCE_DIR}/config.hpp.in"
               "${PROJECT_SOURCE_DIR}/src/config.hpp" @ONLY)

include_directories(${SPUNITY_PLUGIN_INCLUDE_DIRS}
    "${PROJECT_SOURCE_DIR}/src"
    ${SPCHCNTRL_PLUGIN_BUILD_INCLUDE_DIRS})

add_library(spchcntrl-unity SHARED ${SPUNITY_SRCS})
target_link_libraries(spchcntrl-unity ${SPUNITY_PLUGIN_LIBRARIES})
set_target_properties(spchcntrl-unity PROPERTIES
    DEFINE_SYMBOL "SPCH_EXPORT_SYMBOLS"
    VERSION "${SPUNITY_PLUGIN_VERSION}"
    SOVERSION "${SPUNITY_PLUGIN_VERSION}")
speechcontrol_install_plugin(spchcntrl-unity ${SPUNITY_PLUGIN_ID})
add_dependencies(plugins spchcntrl-unity)